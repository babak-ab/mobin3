#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H
#include "global.h"
#include <QObject>
#include <QSize>
#include <gst/gst.h>

///
/// \brief The VideoCapture class
/// This class captures the video from the capture card based on the GStreamer library.
class VideoCapture : public QObject {

    Q_OBJECT
    struct Data {
        GstElement* pipeline = Q_NULLPTR;
        GstElement* sink = Q_NULLPTR;
        GstBus* bus = Q_NULLPTR;
    };

private:
    bool _reset = false;
    QString _device;
    QSize _resolution;
    Data _data;

private:
    static GstFlowReturn on_new_sample_from_sink(GstElement* sink, gpointer user_data);

    static gboolean bus_message(GstBus* bus, GstMessage* msg, gpointer user_data);

public:
    ///
    /// \brief VideoCapture
    /// The constructor.
    /// \param device
    /// The device index.
    /// \param resolution
    /// The capturing resolution.
    explicit VideoCapture(QString device, QSize resolution);
    ~VideoCapture();

    ///
    /// \brief initialize
    /// Initializes the pipes of GStreamer for capturing.
    void initialize();

    ///
    /// \brief finalize
    /// Finalizes the pipes of GStreamer.
    void finalize();

    ///
    /// \brief start
    /// Starts the capturing based on the GStreamer pipe.
    void start();

    ///
    /// \brief stop
    /// Stops the capturing pipe of the GStreamer.
    void stop();

    ///
    /// \brief resolution
    /// Returns the resolution of the capturing in pixel.
    /// \return
    /// The size.
    QSize resolution() const;

    void checkConnection();
public:
    QString device() const;
    void setDevice(const QString& device);
    bool reset() const;
    void setReset(bool reset);

Q_SIGNALS:
    void sigFrameReady(const QByteArray &ba);
};

#endif // VIDEOCAPTURE_H
