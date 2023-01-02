#include "appcontrol.h"

AppControl::AppControl(QObject *parent)
{
    _videoCaptureTv = new VideoCapture("tv", _captureDeviceTv, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    _videoCaptureTv->initialize();
    _videoCaptureTv->start();
    _videoCaptureTv->attach(this);

    _videoCaptureIr = new VideoCapture("ir", _captureDeviceIr, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    _videoCaptureIr->initialize();
    _videoCaptureIr->start();
    _videoCaptureIr->attach(this);

    _videoRecordTv = new VideoRecord(QSize(1920, 1080));
    _videoRecordIr = new VideoRecord(QSize(1920, 1080));
    _recodringLocation = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    _videoAdapter = new VideoAdapter(QSize(FRAME_WIDTH, FRAME_HEIGHT));
}

AppControl::~AppControl()
{

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
