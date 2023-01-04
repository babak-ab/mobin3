#include "appcontrol.h"


AppControl::AppControl(QObject *parent)
    : QObject(parent)
{
    qDebug() << "111111111";

    _videoCaptureTv = new VideoCapture("tv", _captureDeviceTv, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    _videoCaptureTv->initialize();
//    _videoCaptureTv->start();
//    _videoCaptureTv->attach(this);

    _videoCaptureIr = new VideoCapture("ir", _captureDeviceIr, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    _videoCaptureIr->initialize();
//    _videoCaptureIr->start();
//    _videoCaptureIr->attach(this);

    _videoRecordTv = new VideoRecord(QSize(FRAME_WIDTH, FRAME_HEIGHT));
    _videoRecordIr = new VideoRecord(QSize(FRAME_WIDTH, FRAME_HEIGHT));
//    _recodringLocation = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    _videoAdapter = new VideoAdapter(QSize(FRAME_WIDTH, FRAME_HEIGHT));

//    _isTVRecordActivated = false;
//    _isTRCRecordActivated = false;
//    _isSerialPortOpened = false;

//    qDebug() << "XXXXXX count: " << QSerialPortInfo::availablePorts().count();

//    // Fill the serial port names
//    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
//    {
//        _serialPortList.append(serialPortInfo.portName());
//        qDebug() << serialPortInfo.portName();
//    }

    _serialPortName = "COM1";
}

AppControl::~AppControl()
{

}

void AppControl::setSerialPortName(QString portName)
{
    _serialPortName = portName;
}

QString AppControl::messageTitle() const
{
    return _messageTitle;
}

QString AppControl::messageText() const
{
    return _messageText;
}

int AppControl::connectToSerialPort()
{
        qDebug() << "XXXXXXXXXXXX 1";

    bool isConnected = _serialControl.connectToSerialPort(_serialPortName);

    if (isConnected) {
         return _serialPortList.indexOf(QRegExp(_serialPortName, Qt::CaseInsensitive, QRegExp::W3CXmlSchema11));
    }

    _messageTitle = "Serial Port Error";
    _messageText = "Could not connect to " + _serialPortName + " port!";
    Q_EMIT sigThrowSerialMessageRequested();

    return 0;
}

void AppControl::disconnectSerialPort()
{
    bool result = _serialControl.disconnectSerialPort();

    if (!result) {
         _messageTitle = "Serial Port Error";
         _messageText = "Could not get disconnected from " + _serialPortName + " port!";
         Q_EMIT sigThrowSerialMessageRequested();
    }
}

void AppControl::onReadyRead(QString name, uchar* data, long size)
{
    if (name == "tv") {

        cv::Mat tempMatClone = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC4, data);
        cv::Mat tempMat = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);
        cv::cvtColor(tempMatClone, tempMat, cv::COLOR_BGRA2BGR);


        if (_isTVRecordActivated) {
            cv::cvtColor(tempMat, tempMatClone, cv::COLOR_BGR2BGRA);
            cv::Mat output(1080 * 1.5, 1920, CV_8UC1);
            cv::cvtColor(tempMatClone, output, cv::COLOR_BGRA2YUV_I420);
            _videoRecordTv->pushFrame(QByteArray((char*)output.data, 1920 * 1080 * 1.5));
        }

        if (!_isTVRecordActivated) {
            cv::cvtColor(tempMat, tempMatClone, cv::COLOR_BGR2BGRA);
        }

        _videoAdapter->onFrameReady(QByteArray((char*)tempMatClone.data, size));

    } else if (name == "ir") {
        //tic();
        cv::Mat tempMatClone = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC4, data);
        cv::Mat tempMat = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);
        cv::cvtColor(tempMatClone, tempMat, cv::COLOR_BGRA2BGR);

        if (_isTRCRecordActivated) {
            cv::cvtColor(tempMat, tempMatClone, cv::COLOR_BGR2BGRA);
            cv::Mat output(1080 * 1.5, 1920, CV_8UC1);
            cv::cvtColor(tempMatClone, output, cv::COLOR_BGRA2YUV_I420);
            _videoRecordIr->pushFrame(QByteArray((char*)output.data, 1920 * 1080 * 1.5));
        }

        if (!_isTRCRecordActivated) {
            cv::cvtColor(tempMat, tempMatClone, cv::COLOR_BGR2BGRA);
        }

        _videoAdapter->onFrameReady(QByteArray((char*)tempMatClone.data, size));
    }
}

VideoAdapter* AppControl::videoAdapter() const
{
    return _videoAdapter;
}

QStringList AppControl::serialPort() const
{

    qDebug() << "XXXXXXXXXXXX 0";

    return _serialPortList;
}

