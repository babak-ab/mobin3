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

void SerialControl::writeDataOnPlatformsSerialPort(const QByteArray &data)
{
    if (m_serialPort->isOpen())
    {
        m_serialPort->write(data);
        qDebug() << "write data :" << data.toHex();
    }
}

void SerialControl::sendCommand(const QByteArray &data)
{
    QByteArray dataBuffer;
    dataBuffer.resize(9);

    dataBuffer[0] = static_cast<char>(0x80);
    dataBuffer[1] = static_cast<char>(data.at(0));
    dataBuffer[2] = static_cast<char>(data.at(1));
    dataBuffer[3] = static_cast<char>(data.at(2));
    dataBuffer[4] = static_cast<char>(data.at(3));
    dataBuffer[5] = static_cast<char>(data.at(4));
    dataBuffer[6] = static_cast<char>(data.at(5));
    dataBuffer[7] = static_cast<char>(data.at(6));
    dataBuffer[8] = static_cast<char>(0xff);

    quint8 checkSum = crc8(dataBuffer);

    QByteArray sendBuffer;
    sendBuffer.resize(10);
    sendBuffer[0] = dataBuffer.at(0);
    sendBuffer[1] = dataBuffer.at(1);
    sendBuffer[2] = dataBuffer.at(2);
    sendBuffer[3] = dataBuffer.at(3);
    sendBuffer[4] = dataBuffer.at(4);
    sendBuffer[5] = dataBuffer.at(5);
    sendBuffer[6] = dataBuffer.at(6);
    sendBuffer[7] = dataBuffer.at(7);
    sendBuffer[8] = dataBuffer.at(8);
    sendBuffer[9] = static_cast<char>(checkSum);

    Q_EMIT sigWriteData(sendBuffer);
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
