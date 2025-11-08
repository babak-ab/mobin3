#include "VideoRecord.h"

/**
 * @brief Default constructor.
 * @param parent The QObject parent.
 * Initializes GStreamer and default properties.
 */
VideoRecord::
VideoRecord(QObject* parent)
    : QObject(parent)
{
    gst_init(Q_NULLPTR, Q_NULLPTR);
    m_elements.pipeline = Q_NULLPTR;
    m_elements.bus = Q_NULLPTR;
    m_needdata = false;
    m_recordStatus = false;
    m_bitrate = 1000;
    m_waitBeforeNoVideo = 5;
}

/**
 * @brief Constructor with output directory.
 * @param dirName Directory path for storing video recordings.
 * @param parent The QObject parent.
 */
VideoRecord::
VideoRecord(QString dirName, QObject* parent)
    : VideoRecord(parent)
{
    m_videoLocation = dirName;

    if (!QDir(m_videoLocation).exists())
    {
        QDir(m_videoLocation).mkpath(dirName);
    }
}

/**
 * @brief Destructor.
 * Stops and frees the GStreamer pipeline if active.
 */
VideoRecord::
~VideoRecord()
{
    if (m_elements.pipeline != Q_NULLPTR)
    {
        gst_element_set_state(m_elements.pipeline, GST_STATE_NULL);
        gst_object_unref(m_elements.pipeline);
        m_elements.pipeline = Q_NULLPTR;
    }
}

/**
 * @brief Gets the current video file name.
 * @return Current video filename.
 */
QString VideoRecord::videoFileName()
{
    return m_currentVideoFileName;
}

bool VideoRecord::isRecording() const
{
    return m_recordStatus;
}

/**
 * @brief Starts video recording by building and running a GStreamer pipeline.
 * Creates a UDP source pipeline that saves the stream into MKV files using
 * `splitmuxsink`. Chooses a port depending on the active camera.
 */
void VideoRecord::startRecord()
{
    if (!QDir(m_videoLocation).exists())
    {
        QDir(m_videoLocation).mkpath(m_videoLocation);
    }

    m_currentVideoFileName = m_videoLocation + videoName();

    QString pipe2Descr;

    pipe2Descr = QString("v4l2src device=/dev/video10 ! video/x-raw, format=YUY2, framerate=30/1 ! "
                         "queue ! nvvidconv ! video/x-raw(memory:NVMM), format=I420 ! nvv4l2h265enc maxperf-enable=true "
                         "insert-sps-pps=1 bitrate=6000000 ! h265parse ! splitmuxsink name=mux max-size-time=1000000000000 "
                         "muxer-factory=matroskamux muxer-properties=properties streamable=true "
                         "location=%1")
            .arg(m_currentVideoFileName);

    m_elements.pipeline = gst_parse_launch(pipe2Descr.toLatin1().data(), Q_NULLPTR);

    qCritical() << "pipe2Descr" << pipe2Descr << m_elements.pipeline;

    if (m_elements.pipeline == Q_NULLPTR)
    {
        m_elements.pipeline = Q_NULLPTR;
        return;
    }

    m_elements.bus = gst_element_get_bus(m_elements.pipeline);

    gst_bus_add_signal_watch(m_elements.bus);

    gst_bus_set_sync_handler(m_elements.bus, (GstBusSyncHandler)&VideoRecord::onBusMessageVideoPipeline, this, Q_NULLPTR);

    gst_object_unref(m_elements.bus);

    Q_EMIT sigRecordingStatusChanged();
    gst_element_set_state(m_elements.pipeline, GST_STATE_PLAYING);

    m_recordStatus = true;
}

/**
 * @brief Gets the directory where videos are stored.
 * @return Directory path as QString.
 */
QString VideoRecord::
videoLocation() const
{
    return m_videoLocation;
}

/**
 * @brief Sets the directory for saving videos.
 * @param videoLocation New directory path.
 */
void VideoRecord::
setVideoLocation(const QString &videoLocation)
{
    m_videoLocation = videoLocation;
}

/**
 * @brief Helper to wrap GStreamer objects with smart pointers.
 * @tparam T Type of GstMiniObject.
 * @param o Raw pointer to GstMiniObject.
 * @return std::shared_ptr with unref deleter.
 */
template <typename T>
std::shared_ptr<T>
VideoRecord::takeGstMiniObject(T* o)
{
    std::shared_ptr<T> ptr(o, [](T* d)
    {
        gst_mini_object_unref(reinterpret_cast<GstMiniObject*>(d));
    });
    return ptr;
}

/**
 * @brief Handles synchronous bus messages from the GStreamer pipeline.
 * @param bus The GStreamer bus.
 * @param msg The GStreamer message.
 * @param user_data Pointer to this VideoRecord instance.
 * @return Always GST_BUS_DROP (messages handled asynchronously).
 */
GstBusSyncReply VideoRecord::
onBusMessageVideoPipeline(GstBus* bus, GstMessage* msg, gpointer user_data)
{
    VideoRecord* thisClass = static_cast<VideoRecord*>(user_data);
    Q_UNUSED(bus);
    auto messagePtr = takeGstMiniObject(msg);
    qRegisterMetaType<std::shared_ptr<GstMessage>>("std::shared_ptr<GstMessage>");

    QMetaObject::invokeMethod(thisClass, "busMessage",
                              Qt::QueuedConnection, Q_ARG(std::shared_ptr<GstMessage>, messagePtr));
    return GST_BUS_DROP;
}

/**
 * @brief Asynchronous message handler for GStreamer bus messages.
 * @param message Smart pointer to GstMessage.
 * Handles EOS, ERROR, and STATE_CHANGED messages.
 */
void VideoRecord::
busMessage(std::shared_ptr<GstMessage> message)
{
    switch (GST_MESSAGE_TYPE(message.get()))
    {
    case GST_MESSAGE_EOS:
    {
        m_recordStatus = false;
        gst_element_set_state(m_elements.pipeline, GST_STATE_NULL);
        gst_object_unref(m_elements.pipeline);
        m_elements.pipeline = Q_NULLPTR;
        Q_EMIT sigEos();
        Q_EMIT sigRecordingStatusChanged();

        QDir dir(m_videoLocation);
        QStringList filters = {"*.mkv"};
        QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Time);

        if (!files.isEmpty())
        {
            const QString dateTimeString =
                    QDateTime::currentDateTime().
                    toString("yyyy/MM/dd - hh:mm:ss");

           const QString lastFilePath =
                   files.first().absoluteFilePath();

           const QString label = QString("%2").arg(dateTimeString);

            Q_EMIT sigRecordingSaved(lastFilePath, label);
        }
        break;
    }
    case GST_MESSAGE_ERROR:
    {
        GError* error;
        gchar* debug;
        m_recordStatus = false;
        gst_message_parse_error(message.get(), &error, &debug);
        Q_EMIT sigError(GST_OBJECT_NAME(message.get()->src), error->message, debug);
        gst_element_set_state(m_elements.pipeline, GST_STATE_NULL);
        gst_object_unref(m_elements.pipeline);
        m_elements.pipeline = Q_NULLPTR;
        Q_EMIT sigRecordingStatusChanged();
        break;
    }
    case GST_MESSAGE_STATE_CHANGED:
    {
        GstState new_state, pending_state;
        if (m_elements.appsrc != Q_NULLPTR)
        {
            gst_element_get_state(m_elements.appsrc, &new_state, &pending_state, 1000);
            if (new_state == GST_STATE_PLAYING)
            {
                m_recordStatus = true;
                Q_EMIT sigRecordingStatusChanged();
            }
        }
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Generates a new video file name with timestamp.
 * @return Formatted file name with prefix and datetime.
 * Example: `TV_Video_2025_09_23_(12_30_45)_00000001.mkv`
 */
QString VideoRecord::videoName()
{    
    const QString dateTimeString =
            QDateTime::currentDateTime().
            toString("yyyy_MM_dd_hh_mm_ss");

    QString name = dateTimeString + "_%08d.mkv";

    return name;
}

/**
 * @brief Stops recording gracefully by sending EOS to the pipeline.
 */
void VideoRecord::stopRecord()
{
    m_recordStatus = false;
    if (m_elements.pipeline)
    {
        gst_element_send_event(m_elements.pipeline, gst_event_new_eos());
    }
}

/**
 * @brief Pauses the recording pipeline.
 */
void VideoRecord::
pause()
{
    if (m_elements.pipeline)
    {
        m_recordStatus = false;
        gst_element_set_state(m_elements.pipeline, GST_STATE_PAUSED);
    }
}
