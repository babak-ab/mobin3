#include "videorecord.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>

#include <QStandardPaths>

void VideoRecord::start_feed(GstElement* appsrc, guint unused, gpointer user_data)
{
    VideoRecord* videoCapture = static_cast<VideoRecord*>(user_data);
    if (videoCapture)
        videoCapture->_need_data = true;
}

void VideoRecord::stop_feed(GstElement* appsrc, guint unused, gpointer user_data)
{
    VideoRecord* videoCapture = static_cast<VideoRecord*>(user_data);
    if (videoCapture)
        videoCapture->_need_data = false;
}

bool VideoRecord::bus_message(GstBus* bus, GstMessage* msg, gpointer user_data)
{
    VideoRecord* videoCapture = static_cast<VideoRecord*>(user_data);

    switch (GST_MESSAGE_TYPE(msg)) {

    case GST_MESSAGE_EOS:
        gst_element_set_state(videoCapture->_data.pipeline, GST_STATE_NULL);
        gst_object_unref(videoCapture->_data.pipeline);
        videoCapture->_data.pipeline = Q_NULLPTR;

        break;

    case GST_MESSAGE_ERROR: {
        gchar* debug;
        GError* error;

        gst_message_parse_error(msg, &error, &debug);

        g_printerr("Error1: %s %s\n", error->message, debug);
        g_free(debug);
        g_error_free(error);
        break;
    }
    }

    return true;
}

VideoRecord::VideoRecord(QSize resolution, QString location)
    : _location(location)
    , _resolution(resolution)

{

}

bool VideoRecord::pushFrame(QByteArray ba)
{

    auto size = ba.size();
    auto data = ba.data();

    if (_data.pipeline == nullptr)
        return false;

    if (size != _resolution.width() * _resolution.height() * 1.5) {
        return false;
    }

    if (_need_data == false)
        return false;

    _need_data = false;

    GstFlowReturn ret;

    auto buffer = gst_buffer_new_allocate(NULL, size, NULL);
    GstMapInfo map;
    gst_buffer_map(buffer, &map, GST_MAP_WRITE);
    memcpy(map.data, data, size);
    gst_buffer_unmap(buffer, &map);

    g_signal_emit_by_name(_data.src, "push-buffer", buffer, &ret);

    gst_buffer_unref(buffer);

    if (ret != GST_FLOW_OK)
        return false;
    return true;
}

void VideoRecord::initialize(const QString& location)
{
    _location = location;
    if (!QDir(_location).exists()) {
        QDir(_location).mkpath(location);
    }

    QString caps = QString("video/x-raw,format=I420,width=%1,height=%2,framerate=%3/1 ")
                       .arg(QString::number(_resolution.width()))
                       .arg(QString::number(_resolution.height()))
                       .arg(QString::number(30));

    m_currentVideoFileName = _location + videoName();

    QString pipestr;
    pipestr = QString("appsrc caps=%1 is-live=true name=src max-buffers=1 block=TRUE format=3 do-timestamp=true ! "
                      "queue ! nvvidconv ! video/x-raw(memory:NVMM), format=I420 ! nvv4l2h264enc maxperf-enable=1 insert-sps-pps=1 bitrate=5000000 ! "
                      "h264parse ! splitmuxsink muxer=mp4mux max-size-time=300000000000 sync=false location=%2")
            .arg(caps)
            .arg(m_currentVideoFileName);

    qDebug() << "Video record pipestr: " << pipestr;
    qDebug() << "----------------------------------------";

    _data.pipeline = gst_parse_launch(pipestr.toLatin1().data(), NULL);

    _data.src = gst_bin_get_by_name(GST_BIN(_data.pipeline), "src");

    g_object_set(G_OBJECT(_data.src), "emit-signals", TRUE, NULL);
    g_signal_connect(_data.src, "need-data", (GCallback)start_feed, this);
    //g_signal_connect(_data.src, "enough-data", (GCallback)stop_feed, this);

    _data.bus = gst_pipeline_get_bus(GST_PIPELINE(_data.pipeline));
    gst_bus_add_watch(_data.bus, (GstBusFunc)bus_message, this);
    gst_object_unref(_data.bus);

    gst_element_set_state(_data.pipeline, GST_STATE_PLAYING);
}

void VideoRecord::setResolution(QSize resolution)
{
    _resolution = resolution;
}

QString VideoRecord::videoName()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeToString = dateTime.toString("yyyy_MM_dd_(hh_mm_ss)");
    QString name = "/Video_" +
            dateTimeToString +
            "_%04d.mp4";

    return name;
}

void VideoRecord::start()
{
    if (_data.pipeline) {
        _data.timestamp = 0;
        gst_element_set_state(GST_ELEMENT(_data.pipeline), GST_STATE_PLAYING);
    }
}

void VideoRecord::stop()
{
    if (_data.pipeline) {
        _need_data = false;
        gst_element_send_event(_data.pipeline, gst_event_new_eos());
        QTimer::singleShot(500, [this] {
            if (_data.pipeline) {
                gst_element_set_state(_data.pipeline, GST_STATE_NULL);
                gst_object_unref(_data.pipeline);
                _data.pipeline = Q_NULLPTR;
            }
        });
    }
}
