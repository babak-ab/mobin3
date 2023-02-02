#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H
#include "global.h"
#include <QObject>
#include <QSize>
#include <gst/gst.h>

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
    explicit VideoCapture(QString device, QSize resolution);
    ~VideoCapture();
    void initialize();
    void finalize();
    void start();
    void stop();
    QSize resolution() const;

public:
    QString device() const;
    void setDevice(const QString& device);
    bool reset() const;
    void setReset(bool reset);

Q_SIGNALS:
    void sigFrameReady(QByteArray ba);
};

#endif // VIDEOCAPTURE_H
