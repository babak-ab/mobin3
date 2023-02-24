#ifndef VIDEORECORD_H
#define VIDEORECORD_H
#include "global.h"
#include "iostream"
#include <QMetaObject>
#include <QObject>
#include <QSize>
#include <QTimer>
#include <gst/gst.h>
#include <memory>

///
/// \brief The VideoRecord class
/// This class provides the capability of the recording
/// the input video for the application.
class VideoRecord : public QObject {
    Q_OBJECT
    struct Data {
        GstElement* pipeline = Q_NULLPTR;
        GstElement* src = Q_NULLPTR;
        GstBus* bus = Q_NULLPTR;
        GstClockTime timestamp = 0;
    };

private:
    QSize _resolution;
    Data _data;

    QString _location;
    bool _need_data = false;
    bool _stop = false;

private:
    template <typename T>
    static std::shared_ptr<T> takeGstMiniObject(T* o);
    static void start_feed(GstElement* appsrc, guint unused, gpointer user_data);
    static void stop_feed(GstElement* appsrc, guint unused, gpointer user_data);
    static bool bus_message(GstBus* bus, GstMessage* msg, gpointer user_data);

public:

    ///
    /// \brief VideoRecord
    /// The construction.
    /// \param resolution
    /// The resolution of the recording in pixels.
    /// \param location
    /// The location of the recording.
    explicit VideoRecord(QSize resolution, QString location = "");

    ///
    /// \brief pushFrame
    /// Pushes new frame to the recording buffer.
    /// \param ba
    /// The data to be pushed.
    /// \return
    /// The result of pushing new frame.
    bool pushFrame(QByteArray ba);

    ///
    /// \brief initialize
    /// Initializes the recording procedure.
    /// \param location
    /// The location of the recorded file.
    void initialize(const QString& location);

    ///
    /// \brief finalize
    /// Finalizes the recording process.
    void finalize();

    ///
    /// \brief start
    /// Starts the recording.
    void start();

    ///
    /// \brief stop
    /// Stops the recording.
    void stop();


Q_SIGNALS:
    void sigFrameReady(QByteArray ba);
};

#endif // VIDEORECORD_H
