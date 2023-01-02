#include "videocapture.h"
#include <QDebug>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

/* called when the appsink notifies us that there is a new buffer ready for
 * processing */
static GstFlowReturn
on_new_sample_from_sink(GstElement* sink, gpointer user_data)
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

        videoCapture->notify(_data, _size);

        gst_buffer_unmap(buffer, &info);
        gst_sample_unref(sample);
        return GST_FLOW_OK;
    }
    return GST_FLOW_ERROR;
}

gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer user_data)
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

VideoCapture::VideoCapture(QString name, QString device, QSize resolution)
    : ISubject(name)
    , _device(device)
    , _resolution(resolution)

{
    gst_init(NULL,NULL);
}

VideoCapture::~VideoCapture()
{
    detachAll();
}

void VideoCapture::initialize()
{

#ifdef Q_OS_WIN32
    QString caps = QString("video/x-raw,format=BGRA,width=%1,height=%2,framerate=%3/1 ")
                       .arg(QString::number(_resolution.width()))
                       .arg(QString::number(_resolution.height()))
                       .arg(QString::number(30));

    // Uncomment this for the main capture card
    QString pipestr = QString("ksvideosrc device-index=%1  do-stats=TRUE ! image/jpeg,width=%2,height=%3 ! jpegdec ! videoconvert ! video/x-raw,format=BGRA,width=%4,height=%5,framerate=%6/1 ! queue ! appsink name=sink caps=%7")
                          .arg(_device)
                          .arg(QString::number(_resolution.width()))
                          .arg(QString::number(_resolution.height()))
                          .arg(QString::number(_resolution.width()))
                          .arg(QString::number(_resolution.height()))
                          .arg(QString::number(30))
                          .arg(caps);

//        QString pipestr = QString("ksvideosrc device-index=%1  do-stats=TRUE ! videoconvert ! video/x-raw,format=BGRA,width=%4,height=%5,framerate=%6/1 ! queue ! appsink name=sink caps=%7")
//                              .arg(_device)
//                              .arg(QString::number(_resolution.width()))
//                              .arg(QString::number(_resolution.height()))
//                              .arg(QString::number(30))
//                              .arg(caps);

#endif
#ifdef Q_OS_LINUX
    QString caps = QString("video/x-raw,format=BGRA,width=%1,height=%2,framerate=%3/1 ")
                       .arg(QString::number(_resolution.width()))
                       .arg(QString::number(_resolution.height()))
                       .arg(QString::number(30));

//        QString pipestr = QString("videotestsrc ! videoconvert ! "
//                                  "video/x-raw,format=BGRA,width=%2,height=%3,framerate=%4/1 ! appsink name=sink caps=%5")
//                              .arg(QString::number(_resolution.width()))
//                              .arg(QString::number(_resolution.height()))
//                              .arg(QString::number(30))
//                              .arg(caps);

    QString pipestr = QString("v4l2src device=%1 is-live=true ! image/jpeg,width=%2,height=%3 ! jpegdec !"
                              " videoconvert ! video/x-raw,format=BGRA,width=%4,height=%5,framerate=%6/1 ! queue !"
                              " appsink name=sink caps=%7")
                          .arg(_device)
                          .arg(QString::number(_resolution.width()))
                          .arg(QString::number(_resolution.height()))
                          .arg(QString::number(_resolution.width()))
                          .arg(QString::number(_resolution.height()))
                          .arg(QString::number(30))
                          .arg(caps);

#endif

    qDebug() << "pipestr: " << pipestr;

    _data.pipeline = gst_parse_launch(pipestr.toLatin1().data(), NULL);

    _data.sink = gst_bin_get_by_name(GST_BIN(_data.pipeline), "sink");

    g_object_set(G_OBJECT(_data.sink), "emit-signals", TRUE, "sync", FALSE, NULL);
    _data.sink = gst_bin_get_by_name(GST_BIN(_data.pipeline), "sink");
    g_object_set(G_OBJECT(_data.sink), "emit-signals", TRUE, "sync", FALSE, NULL);
    g_signal_connect(_data.sink, "new-sample", G_CALLBACK(on_new_sample_from_sink), this);

    _data.bus = gst_pipeline_get_bus(GST_PIPELINE(_data.pipeline));
    gst_bus_add_watch(_data.bus, (GstBusFunc)bus_call, this);
    gst_object_unref(_data.bus);
}

void VideoCapture::start()
{
    if (_data.pipeline) {
        gst_element_set_state(GST_ELEMENT(_data.pipeline), GST_STATE_PLAYING);
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

void VideoCapture::detach(IObserver* observer)
{
    _list_observer.removeAll(observer);
}

void VideoCapture::attach(IObserver* observer)
{
    _list_observer.append(observer);
}

void VideoCapture::notify(uchar* data, long size)
{
    for (int i = 0; i < _list_observer.size(); i++) {
        _list_observer[i]->onReadyRead(_name, data, size);
    }
}

void VideoCapture::detachAll()
{
    _list_observer.clear();
}

QSize VideoCapture::resolution() const
{
    return _resolution;
}
