#ifndef APPCONTROL_H
#define APPCONTROL_H

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
#define SOFTWARE_VERSION "0.9.0"

#include <QObject>
#include <QStandardPaths>
#include <QSerialPortInfo>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "global.h"
#include "videocapture.h"
#include "videorecord.h"
#include "videoadapter.h"
#include "serialcontrol.h"

class AppControl: public QObject, public IObserver
{
    Q_OBJECT

    Q_PROPERTY(QObject* videoAdapter READ videoAdapter NOTIFY signalVideoAdapter)
    Q_PROPERTY(QStringList serialPort READ serialPort NOTIFY signalVideoAdapter);
//    Q_PROPERTY(QString serialPortName READ serialPortName NOTIFY sigSerialPortNameChanged);
    Q_PROPERTY(QString messageTitle READ messageTitle NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(QString messageText READ messageText NOTIFY sigThrowSerialMessageRequested);


public:
    explicit AppControl(QObject* parent = Q_NULLPTR);
    ~AppControl();

private:
    VideoAdapter* _videoAdapter;

    VideoCapture* _videoCaptureIr;
    VideoCapture* _videoCaptureTv;

    VideoRecord* _videoRecordTv;
    VideoRecord* _videoRecordIr;

    QStringList _cameraList;
    QString _recodringLocation = "";

    QString _captureDeviceIr;
    QString _captureDeviceTv;

    bool _isTVRecordActivated;
    bool _isTRCRecordActivated;

    SerialControl _serialControl;
    bool _isSerialPortOpened;
    QStringList _serialPortList;

    QString _serialPortName;

    QString _messageTitle;
    QString _messageText;

public:
    Q_INVOKABLE int connectToSerialPort();
    Q_INVOKABLE void disconnectSerialPort();
    Q_INVOKABLE void setSerialPortName(QString portName);

//    QString serialPortName() const;

    QString messageTitle() const;

    QString messageText() const;

    void onReadyRead(QString name, uchar* data, long size);

    QString captureDeviceIr() const;

    QString captureDeviceTv() const;

    bool tvRecord() const;

    bool irRecord() const;

    VideoAdapter* videoAdapter() const;

    QStringList serialPort() const;


Q_SIGNALS:
    void sigCaptureDeviceTvChanged();
    void sigCaptureDeviceIrChanged();
//    void sigSerialPortNameChanged();
    void sigThrowSerialMessageRequested();
    void tvRecordChanged();
    void irRecordChanged();
    void signalVideoAdapter();
};


#endif // APPCONTROL_H
