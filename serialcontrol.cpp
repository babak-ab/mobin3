#include "serialcontrol.h"

SerialControl::SerialControl(QObject *parent)
    : RingQueue("serialControl", 10, 10000, parent)
{
    RingQueue::initialize();
    QObject::connect(this, &RingQueue::sigReadData,
                     this, &SerialControl::sltReadData, Qt::DirectConnection);
    QObject::connect(this, &RingQueue::sigWriteData,
                     this, &SerialControl::sigWriteData, Qt::DirectConnection);


    m_serialPort = new QSerialPort;
    QObject::connect(m_serialPort, &QSerialPort::readyRead,
                     this ,&SerialControl::sltReadSeialPortData);

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

bool SerialControl::connectToSerialPort(const QString &portName)
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
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
        return true;
    }

    return false;
}

QByteArray SerialControl::interpret(IRQueue<quint8> *queueRead)
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
    qDebug() << "sdfsd" << _zoomSpeed;
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

void SerialControl::writeDataOnPlatformsSerialPort(const QByteArray &data)
{
    if (true/*m_serialPort->isOpen()*/)
    {
        m_serialPort->write(data);
        qDebug() << "write data :" << data.toHex(' ');
    }
}

quint8 SerialControl::crc8(const QByteArray &data)
{
    quint8 sum = 0;

    for (auto byte : data)
    {
        sum += byte;
    }

    return sum;
}

int SerialControl::bytesToInt(QByteArray data, int start, int length, bool reverse)
{
    int result = 0;
    int power;

    if (!reverse)
    {
        power = length - 1;
        for (int i = start; i < length + start; i++) {
            result += static_cast<quint8>(data[i]) * qPow(256, power);
            power--;
        }
    }
    else
    {
        power = 0;
        for (int i = start; i < start + length; i++)
        {
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
    if (m_serialPort->isOpen())
    {
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
    sendCommand1(195, 1);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 2)
        QTimer::singleShot(50, this, &SerialControl::autoFocus);
    else
        _repeatCounter = 0;
}

void SerialControl::manualFocus()
{
    sendCommand1(199, 1);

    //Repeat command to increase reliability of stopping process
    if (_repeatCounter++ < 2)
        QTimer::singleShot(50, this, &SerialControl::manualFocus);
    else
        _repeatCounter = 0;
}

void SerialControl::setZoomSpeed(const quint8 speed)
{
    _zoomSpeed = speed;

    qDebug() << "SPPED1" << speed;
    Q_EMIT sigDataChanged();
}

void SerialControl::setFocusSpeed(const quint8 speed)
{
    _focusSpeed = speed;

    Q_EMIT sigDataChanged();
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

    qDebug() << "SSSSSSSSSSSSSSSSSSS" << speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::sendCommand1(const quint8 &command,
                                           const quint8 &param)
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
