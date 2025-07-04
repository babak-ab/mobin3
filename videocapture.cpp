#include "videocapture.h"
#include <QDebug>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

/* called when the appsink notifies us that there is a new buffer ready for
 * processing */
GstFlowReturn
VideoCapture::on_new_sample_from_sink(GstElement* sink, gpointer user_data)
{
    VideoCapture* videoCapture = static_cast<VideoCapture*>(user_data);
    GstSample* sample;
    g_signal_emit_by_name(sink, "pull-sample", &sample);
    if (sample) {
        GstMapInfo info;
        GstBuffer* buffer = gst_sample_get_buffer(sample);
        gst_buffer_map(buffer, &info, (GstMapFlags)(GST_MAP_READ));

        auto _data = info.data;
        auto _size = info.size;

        //videoCapture->notify(_data, _size);

        QByteArray ba((char*)_data, _size);
        gst_buffer_unmap(buffer, &info);
        gst_sample_unref(sample);

        Q_EMIT videoCapture->sigFrameReady(ba);
        return GST_FLOW_OK;
    }
    return GST_FLOW_ERROR;
}

gboolean VideoCapture::bus_message(GstBus* bus, GstMessage* msg, gpointer user_data)
{
    VideoCapture* videoCapture = static_cast<VideoCapture*>(user_data);
    switch (GST_MESSAGE_TYPE(msg)) {

    case GST_MESSAGE_EOS:

        break;

    case GST_MESSAGE_ERROR: {
        gchar* debug;
        GError* error;

        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);
        g_printerr("Error: %s\n", error->message);
        g_error_free(error);
        break;
    }
    }

    return TRUE;
}

GstPadProbeReturn VideoCapture::cb_have_data(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{

    VideoCapture* videoCapture = static_cast<VideoCapture*>(user_data);
    gint x, y;
    GstMapInfo map;
    // guint16 *ptr, t;
    guint16 *t;
    guint8 *ptr=nullptr;
    GstBuffer *buffer;

    // g_print("In %s %d \n", __func__,cntr_val++);

    buffer = GST_PAD_PROBE_INFO_BUFFER (info);

    buffer = gst_buffer_make_writable (buffer);

    /* Making a buffer writable can fail (for example if it
       * cannot be copied and is used more than once)
       */
    if (buffer == NULL)
        return GST_PAD_PROBE_OK;

    /* Mapping a buffer can fail (non-writable) */
    if (gst_buffer_map (buffer, &map, GST_MAP_WRITE)) {

        auto _data = map.data;
        auto _size = info->size;

        QByteArray ba((char*)_data, _size);

        Q_EMIT videoCapture->sigI420_FrameReady(ba);

        gst_buffer_unmap (buffer, &map);
    }

    GST_PAD_PROBE_INFO_DATA (info) = buffer;

    return GST_PAD_PROBE_OK;
}

QString VideoCapture::device() const
{
    return _device;
}

void VideoCapture::setDevice(const QString& device)
{
    _device = device;
    if (_data.pipeline) {
        gst_element_set_state(_data.pipeline, GST_STATE_NULL);
        gst_object_unref(_data.pipeline);
    }

    initialize();
    start();
}

bool VideoCapture::reset() const
{
    return _reset;
}

void VideoCapture::setReset(bool reset)
{
    _reset = reset;
}

VideoCapture::VideoCapture(QString device, QSize resolution)
    : _device(device)
    , _resolution(resolution)

{

}

VideoCapture::~VideoCapture()
{

}

void VideoCapture::initialize()
{

#ifdef Q_OS_WIN32
    QString caps = QString("video/x-raw,format=BGRA,width=%1,height=%2,framerate=%3/1 ")
            .arg(QString::number(_resolution.width()))
            .arg(QString::number(_resolution.height()))
            .arg(QString::number(30));


    QString pipestr = QString("ksvideosrc device-index=%1  do-stats=TRUE "
                              " ! video/x-raw, format=(string)YUY2, width=%2, height=%3, framerate=(fraction)30/1, pixel-aspect-ratio=(fraction)1/1"
                              " ! videoconvert ! video/x-raw, format=BGRA, width=%4, height=%5, framerate=%6/1 ! queue ! appsink name=sink caps=%7")
            .arg(_device)
            .arg(QString::number(_resolution.width()))
            .arg(QString::number(_resolution.height()))
            .arg(QString::number(_resolution.width()))
            .arg(QString::number(_resolution.height()))
            .arg(QString::number(30))
            .arg(caps);

#endif

#ifdef Q_OS_LINUX
    QString caps = QString("video/x-raw,format=BGRA,width=%1,height=%2,framerate=%3/1 ")
            .arg(QString::number(_resolution.width()))
            .arg(QString::number(_resolution.height()))
            .arg(QString::number(30));

    QString pipestr = QString("v4l2src device=%1 ! video/x-raw, format=YUY2, width=%2, height=%3, framerate=30/1 ! "
//                              "videoconvert name=sourceI420 ! video/x-raw, format=I420, width=%2, height=%3, framerate=30/1  ! "
//                              "videoconvert ! video/x-raw,format=BGRA,width=%2,height=%3,framerate=%4/1 ! appsink name=sink caps=%5")
                              "queue ! nvvidconv ! video/x-raw(memory:NVMM), format=BGRx ! nvvidconv ! video/x-raw, width=%2, height=%3, format=BGRx, framerate=30/1 ! "
                              "videoconvert ! video/x-raw,format=BGRA,width=%2,height=%3,framerate=%4/1 ! appsink name=sink caps=%5")
            .arg(_device)
            .arg(QString::number(_resolution.width()))
            .arg(QString::number(_resolution.height()))
            .arg(QString::number(30))
            .arg(caps);

#endif

    qDebug() << "pipestr" << pipestr;

    _data.pipeline = gst_parse_launch(pipestr.toLatin1().data(), NULL);

//    _data.source = gst_bin_get_by_name(GST_BIN(_data.pipeline), "sourceI420");

    _data.sink = gst_bin_get_by_name(GST_BIN(_data.pipeline), "sink");

//    _data.pad = gst_element_get_static_pad(_data.source, "src");
//    gst_pad_add_probe(_data.pad, GST_PAD_PROBE_TYPE_BUFFER,
//                      (GstPadProbeCallback) cb_have_data, this, NULL);

//    gst_object_unref(_data.pad);

    g_object_set(G_OBJECT(_data.sink), "emit-signals", TRUE, "sync", FALSE, NULL);
    _data.sink = gst_bin_get_by_name(GST_BIN(_data.pipeline), "sink");
    g_signal_connect(_data.sink, "new-sample", G_CALLBACK(on_new_sample_from_sink), this);


    _data.bus = gst_pipeline_get_bus(GST_PIPELINE(_data.pipeline));
    gst_bus_add_watch(_data.bus, (GstBusFunc)bus_message, this);
    gst_object_unref(_data.bus);
}

void VideoCapture::start()
{
    if (_data.pipeline) {
        gst_element_set_state(GST_ELEMENT(_data.pipeline), GST_STATE_PLAYING);
    }
}

void VideoCapture::pause()
{
    if (_data.pipeline) {
        gst_element_set_state(GST_ELEMENT(_data.pipeline), GST_STATE_PAUSED);
    }
}

void VideoCapture::stop()
{
    if (_data.pipeline) {
        gst_element_set_state(_data.pipeline, GST_STATE_NULL);
        gst_object_unref(_data.pipeline);
        _data.pipeline = Q_NULLPTR;
    }
}

QSize VideoCapture::resolution() const
{
    return _resolution;
}

void VideoCapture::checkConnection()
{
    stop();
    initialize();
    start();
}
