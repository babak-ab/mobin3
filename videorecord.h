#ifndef VIDEORECORD_H
#define VIDEORECORD_H
#include "iostream"
#include <QMetaObject>
#include <QObject>
#include <QSize>
#include <QTimer>
#include <gst/gst.h>
#include <memory>
#include "global.h"

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

    QString m_currentVideoFileName;

private:
    template <typename T>
    static std::shared_ptr<T> takeGstMiniObject(T* o);
    static void start_feed(GstElement* appsrc, guint unused, gpointer user_data);
    static void stop_feed(GstElement* appsrc, guint unused, gpointer user_data);
    static bool bus_message(GstBus* bus, GstMessage* msg, gpointer user_data);

    QString videoName();
public:
    explicit VideoRecord(QSize resolution, QString location = "");
    bool pushFrame(QByteArray ba);
    void initialize(const QString& location);
    void setResolution(QSize resolution);
    void finalize();
    void start();
    void stop();


Q_SIGNALS:
    void sigFrameReady(QByteArray ba);
};

#endif // VIDEORECORD_H
