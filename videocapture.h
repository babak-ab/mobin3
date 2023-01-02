#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H
#include "global.h"
#include <QObject>
#include <QSize>
#include <gst/gst.h>

class VideoCapture : public ISubject {

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
    QList<IObserver*> _list_observer;

public:
    explicit VideoCapture(QString name, QString device, QSize resolution);
    ~VideoCapture();
    void initialize();
    void finalize();
    void start();
    void stop();
    QSize resolution() const;

public:
    void attach(IObserver* observer);
    void detach(IObserver* observer);
    void notify(uchar* data, long size);
    void detachAll();

    QString device() const;
    void setDevice(const QString& device);
    bool reset() const;
    void setReset(bool reset);
};

#endif // VIDEOCAPTURE_H
