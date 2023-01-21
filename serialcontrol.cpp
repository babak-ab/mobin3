#include "serialcontrol.h"

SerialControl::SerialControl(QObject* parent)
    : RingQueue("serialControl", 10, 10000, parent)
{
    RingQueue::initialize();
    QObject::connect(this, &RingQueue::sigReadData,
        this, &SerialControl::sltReadData, Qt::DirectConnection);
    QObject::connect(this, &RingQueue::sigWriteData,
        this, &SerialControl::sigWriteData, Qt::DirectConnection);

    m_serialPort = new QSerialPort;
    QObject::connect(m_serialPort, &QSerialPort::readyRead,
        this, &SerialControl::sltReadSeialPortData);

    QObject::connect(this, &SerialControl::sigWriteData,
        this, &SerialControl::sltWriteSerialPortData);

    _zoomSpeed = 100;
    _focusSpeed = 100;
    _panTiltSpeed = 100;
    _repeatCounter = 0;
}

SerialControl::~SerialControl()
{
    RingQueue::finalize();

    delete m_serialPort;
}

bool SerialControl::connectToSerialPort(const QString& portName)
{
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(QSerialPort::Baud38400);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        return false;
    }

    return true;
}

bool SerialControl::disconnectSerialPort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        return true;
    }

    return false;
}

QByteArray SerialControl::interpret(IRQueue<quint8>* queueRead)
{
    quint8 data1;
    if (!queueRead->dequeue(data1))
        return QByteArray();

    //    if (data1 == 0x80) {
    //        quint8 data2;
    //        if (!queueRead->dequeue(data2))
    //            return QByteArray();

    //        if (data2 == ReceiveType_ReticlePositionReport)
    //        {
    //            std::vector<quint8> arrValidData(10);
    //            arrValidData[0] = data1;
    //            arrValidData[1] = data2;

    //            bool state = queueRead->dequeue(arrValidData.data() + 2, 8);

    //            if (state == false)
    //                return QByteArray();

    //            return QByteArray(reinterpret_cast<const char*>(arrValidData.data()), 10);
    //        }
    //        else
    //        {
    //            return QByteArray();
    //        }
    //    }
    return QByteArray();
}

bool SerialControl::isConnected() const
{
    return m_serialPort->isOpen();
}

quint8 SerialControl::zoomSpeed() const
{
    return _zoomSpeed;
}

quint8 SerialControl::panTiltSpeed() const
{
    return _panTiltSpeed;
}

quint8 SerialControl::focusSpeed() const
{
    return _focusSpeed;
}

quint16 SerialControl::focusPosition() const
{
    return _focusPosition;
}

quint16 SerialControl::fovPosition() const
{
    return _fovPosition;
}

quint8 SerialControl::illuminatorBrightnessLevel() const
{
    return _illuminatorBrightness;
}

quint8 SerialControl::illuminatorAngleOffset() const
{
    return _illuminatorAngleOffset;
}

SerialControl::DefogMode SerialControl::defogMode() const
{
    return _defogMode;
}

SerialControl::GammaLevel SerialControl::gammaLevel() const
{
    return _gammaLevel;
}

bool SerialControl::digitalZoom() const
{
    return _isDigitalZoomEnabled;
}

void SerialControl::writeDataOnPlatformsSerialPort(const QByteArray& data)
{
    if (true /*m_serialPort->isOpen()*/) {
        m_serialPort->write(data);
        qDebug() << "write data :" << data.toHex(' ');
    }
}

quint8 SerialControl::crc8(const QByteArray& data)
{
    quint8 sum = 0;

    for (auto byte : data) {
        sum += byte;
    }

    return sum;
}

int SerialControl::bytesToInt(QByteArray data, int start, int length, bool reverse)
{
    int result = 0;
    int power;

    if (!reverse) {
        power = length - 1;
        for (int i = start; i < length + start; i++) {
            result += static_cast<quint8>(data[i]) * qPow(256, power);
            power--;
        }
    } else {
        power = 0;
        for (int i = start; i < start + length; i++) {
            result += static_cast<quint8>(data[i]) * qPow(256, power);
            power++;
        }
    }

    return result;
}

void SerialControl::sltReadData(QByteArray data)
{
    //    if (static_cast<quint8>(data.at(1) == ReceiveType::ReceiveType_ReticlePositionReport))
    //    {
    //        quint8 data2;
    //        data2 = static_cast<quint8>(data.at(2));

    //        quint8 trackerState;
    //        trackerState = ((data2 & 0x80) >> 7);

    //        QByteArray d2;
    //        d2.resize(1);
    //        d2[0] = static_cast<char>(data2);

    //        if (trackerState == 1)
    //        {
    //            Q_EMIT sigTrackerLostsTarget();
    //        }

    //        int xPosition;
    //        xPosition = bytesToInt(data, 3, 2);

    //        int yPosition;
    //        yPosition = bytesToInt(data, 5, 2);

    //        m_reticlePosition.setX(xPosition);
    //        m_reticlePosition.setY(yPosition);

    //        int reticleWidth;
    //        reticleWidth = bytesToInt(data, 7, 1) * 3;

    //        int reticleHeight;
    //        reticleHeight = bytesToInt(data, 8, 1) * 3;

    //        m_reticleSize = QSize(reticleWidth, reticleHeight);

    //        m_trackerState = trackerState;
    //    }
}

void SerialControl::sltReadSeialPortData()
{
    if (m_serialPort->isOpen()) {
        const QByteArray data = m_serialPort->readAll();
        //        qDebug() << "read data :" << data.toHex();
        insertToReadQueue(data);
    }
}

void SerialControl::sltWriteSerialPortData(QByteArray data)
{
    writeDataOnPlatformsSerialPort(data);
}

void SerialControl::zoomIn()
{
    sendCommand1(178, _zoomSpeed);
}

void SerialControl::zoomOut()
{
    sendCommand1(177, _zoomSpeed);
}

void SerialControl::zoomStop()
{
    sendCommand1(188, _zoomSpeed);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::zoomStop);
    else
        _repeatCounter = 0;
}

void SerialControl::focusFar()
{
    sendCommand1(180, _focusSpeed);
}

void SerialControl::focusNear()
{
    sendCommand1(184, _focusSpeed);
}

void SerialControl::focusStop()
{
    sendCommand1(179, _focusSpeed);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::focusStop);
    else
        _repeatCounter = 0;
}

void SerialControl::autoFocus()
{
    sendCommand1(199, 1);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 2)
        QTimer::singleShot(50, this, &SerialControl::autoFocus);
    else
        _repeatCounter = 0;
}

void SerialControl::manualFocus()
{
    sendCommand1(195, 1);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 2)
        QTimer::singleShot(50, this, &SerialControl::manualFocus);
    else
        _repeatCounter = 0;
}

void SerialControl::setZoomSpeed(const quint8 speed)
{
    _zoomSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setFocusSpeed(const quint8 speed)
{
    _focusSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setFovPosition(const quint16 position)
{
    _fovPosition = position;

    Q_EMIT sigDataChanged();
}

void SerialControl::gotoFov(const quint16 position)
{
    sendCommand2(59, position);
}

void SerialControl::setFocusPosition(const quint16 position)
{
    _focusPosition = position;

    Q_EMIT sigDataChanged();
}

void SerialControl::gotoFocus(const quint16 position)
{
    sendCommand2(60, position);
}

void SerialControl::gotoFovFocus(const quint16 fovPosition, const quint16 focusPosition)
{
    sendCommand3(60, fovPosition, focusPosition);
}

void SerialControl::setPosition(const quint8 positionNumber)
{
    sendCommand1(111, positionNumber);
}

void SerialControl::clearPosition(const quint8 positionNumber)
{
    sendCommand1(112, positionNumber);
}

void SerialControl::tiltUp()
{
    sendCommand1(173, _panTiltSpeed);
}

void SerialControl::tiltDown()
{
    sendCommand1(174, _panTiltSpeed);
}

void SerialControl::tiltStop()
{
    sendCommand1(175, 1);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::tiltStop);
    else
        _repeatCounter = 0;
}

void SerialControl::panLeft()
{
    sendCommand1(171, _panTiltSpeed);
}

void SerialControl::panRight()
{
    sendCommand1(170, _panTiltSpeed);
}

void SerialControl::panStop()
{
    sendCommand1(172, 1);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::panStop);
    else
        _repeatCounter = 0;
}

void SerialControl::setPanTiltSpeed(const quint8 speed)
{
    _panTiltSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setSelectedCamera(const SerialControl::CameraSelection camera)
{
    _selectedCamera = camera;

    sendCommand1(63, camera);
}

void SerialControl::setSelectedFilter(const SerialControl::FilterSelection filter)
{
    _selectedFilter = filter;

    sendCommand1(63, filter);
}

void SerialControl::setDefogMode(const SerialControl::DefogMode mode)
{
    _defogMode = mode;

    sendCommand1(60, mode);
}

void SerialControl::setGammaLevel(const SerialControl::GammaLevel level)
{
    _gammaLevel = (GammaLevel)(level + 1);

    sendCommand1(61, _gammaLevel);
}

void SerialControl::setNoiseReductionMode(const SerialControl::NoiseReductionMode mode)
{
    _noiseReductionMode = (NoiseReductionMode)(mode + 1);

    sendCommand1(62, _noiseReductionMode);
}

void SerialControl::enableDigitalZoom(const bool state)
{
    _isDigitalZoomEnabled = state;

    quint8 param;
    if (state) {
        param = 1;
    } else {
        param = 2;
    }

    sendCommand1(85, param);
}

void SerialControl::enableIlluminator(const bool state)
{
    _isIlluminatorEnabled = state;

    quint8 param;
    if (state) {
        param = 2;
    } else {
        param = 1;
    }

    sendCommand1(90, param);
}

void SerialControl::setIlluminatorBrightness(const quint8 brightness)
{
    _illuminatorBrightness = brightness;

    sendCommand1(91, _contrastLevel);
}

void SerialControl::setIlluminatorSmallerAngle()
{
    quint8 param = 1;

    sendCommand1(93, param);
}

void SerialControl::setIlluminatorLargerAngle()
{
    quint8 param = 2;

    sendCommand1(93, param);
}

void SerialControl::setIlluminatorAngleOffset(const quint8 offset)
{
    _illuminatorAngleOffset = offset;

    sendCommand1(92, offset);
}

void SerialControl::setRequestSendingMode()
{
    sendCommand1(12, 0);
}

void SerialControl::setStatusSendingMode()
{
    sendCommand4(80);
}

void SerialControl::setContinuousSendingMode(const quint8 interval)
{
    sendCommand1(12, interval);
}

QString SerialControl::serialportName() const
{
    return m_serialPort->portName();
}

void SerialControl::setContrastMode(const quint8 level)
{
    _contrastLevel = level + 1;

    sendCommand1(116, _contrastLevel);
}

void SerialControl::setBrightnessLevel(const quint8 level)
{
    _brightnessLevel = level + 1;

    sendCommand1(115, _brightnessLevel);
}

void SerialControl::setMode(const quint8 mode)
{
    _mode = mode + 1;

    sendCommand1(117, _mode);
}

void SerialControl::showMenuPressed()
{
    sendCommand1(64, 1);
}

void SerialControl::showMenuReleased()
{
    sendCommand1(64, 0);
}

void SerialControl::menuUpPressed()
{
    sendCommand1(65, 1);
}

void SerialControl::menuUpReleased()
{
    sendCommand1(65, 0);
}

void SerialControl::menuDownPressed()
{
    sendCommand1(66, 1);
}

void SerialControl::menuDownReleased()
{
    sendCommand1(66, 0);
}

void SerialControl::menuLeftPressed()
{
    sendCommand1(84, 1);
}

void SerialControl::menuLeftReleased()
{
    sendCommand1(84, 0);
}

void SerialControl::menuRightPressed()
{
    sendCommand1(83, 1);
}

void SerialControl::menuRightReleased()
{
    sendCommand1(83, 0);
}

void SerialControl::menuESCReleased()
{
    sendCommand1(63, 1);
}

void SerialControl::sendCommand1(const quint8& command,
    const quint8& param)
{
    quint8 checkSum = command + param + 1;

    QByteArray data;
    data.append(static_cast<quint8>(104));
    data.append(static_cast<quint8>(3));
    data.append(static_cast<quint8>(1));
    data.append(static_cast<quint8>(command));
    data.append(static_cast<quint8>(param));
    data.append(static_cast<quint8>(checkSum));

    Q_EMIT sigWriteData(data);
}

void SerialControl::sendCommand2(const quint8& command,
    const quint16& param)
{
    quint8 checkSum = command + (param / 256) + (param % 256) + 1;

    QByteArray data;
    data.append(static_cast<quint8>(104));
    data.append(static_cast<quint8>(4));
    data.append(static_cast<quint8>(1));
    data.append(static_cast<quint8>(command));
    data.append(static_cast<quint8>(param / 256));
    data.append(static_cast<quint8>(param % 256));
    data.append(static_cast<quint8>(checkSum));

    Q_EMIT sigWriteData(data);
}

void SerialControl::sendCommand3(const quint8& command,
    const quint16& param1,
    const quint16& param2)
{

    quint8 checkSum = command + (param1 / 256) + (param1 % 256) + (param2 / 256) + (param2 % 256) + 1;

    QByteArray data;
    data.append(static_cast<quint8>(104));
    data.append(static_cast<quint8>(6));
    data.append(static_cast<quint8>(1));
    data.append(static_cast<quint8>(command));
    data.append(static_cast<quint8>(param1 / 256));
    data.append(static_cast<quint8>(param1 % 256));
    data.append(static_cast<quint8>(param2 / 256));
    data.append(static_cast<quint8>(param2 % 256));
    data.append(static_cast<quint8>(checkSum));

    Q_EMIT sigWriteData(data);
}

void SerialControl::sendCommand4(const quint8& command)
{

    quint8 checkSum = command + 1;

    QByteArray data;
    data.append(static_cast<quint8>(104));
    data.append(static_cast<quint8>(2));
    data.append(static_cast<quint8>(1));
    data.append(static_cast<quint8>(command));
    data.append(static_cast<quint8>(checkSum));

    Q_EMIT sigWriteData(data);
}
