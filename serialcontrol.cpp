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

    m_zoomSpeed = 100;
    m_focusSpeed = 100;
    m_panTiltSpeed = 100;
    m_repeatCounter = 0;

    m_selectedCamera = CameraSelection_ContinuousZoom;
    m_selectedFilter = FilterSelection_ColorFilter;
    m_defogMode = DefogMode_High;
    m_gammaLevel = GammaLevel_Level1;
    m_noiseReductionMode = NoiseReductionMode_High;
    m_isDigitalZoomEnabled = false;
    m_isIlluminatorEnabled = false;
    m_contrastLevel = ContrastLevel_Level2;
    m_brightnessLevel = 1;
    m_mode = 1;
    m_illuminatorBrightness = 0;
    milluminatorAngleOffset = 1;

    m_focusMode = true;

    init_crc8();
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
    quint8 firstByte = 0x00;
    quint8 lengthByte = 0x00;
    quint8 crcValue = 0x00;

    if (!queueRead->dequeue(firstByte))
    {
        return QByteArray();
    }

    if (firstByte != 0xAA)
    {
        return QByteArray();
    }


    if (!queueRead->dequeue(lengthByte))
    {
        return QByteArray();
    }

    if (lengthByte != static_cast<char>(0x0A) &&
            lengthByte != static_cast<char>(0x0B))
    {
        return QByteArray();
    }

    QByteArray packet(lengthByte + 2, 0x00);

    packet[0] = firstByte;
    packet[1] = lengthByte;

    for (int i = 2; i < lengthByte + 2; i++)
    {
        quint8 data;

        if (!queueRead->dequeue(data))
        {
            return QByteArray();
        }

        packet[i] = data;
    }

    std::cerr << packet.toHex(' ').toStdString() << std::endl;

    // calculate crc
    {
        quint8 *crcInput = new quint8;

        for (int i = 2; i < packet.count() - 2; i++)
        {
            std::cerr << packet.toHex().at(2 * i) << packet.toHex().at(2 * i + 1) << " ";
            crcInput[i - 2] = packet.at(i);
        }
        std::cerr << std::endl;

        crcValue = crc8(crcInput, lengthByte - 2);

        delete crcInput;
    }

    QByteArray crc(2, 0x00);
    crc[0] = crcValue;
    crc[1] = packet.at(10);

//    std::cerr << "CHECKSUM : " << crc.toHex(' ').toStdString() << std::endl;


    // check first, second and crc bytes
    if (/*crcValue == packet.at(lengthByte)*/ true)
    {
        // byte 2 and byte 3    => FOV
        {
            quint16 fovValue = 0x0000;
            fovValue |= (0xFF00) & (static_cast<quint16>(packet.at(2)) << 8);
            fovValue |= (0x00FF) & (static_cast<quint16>(packet.at(3)));

            m_fovPosition = fovValue;
        }
        // byte 4 and byte 5    => Focus
        {
            quint16 focusValue = 0x0000;
            focusValue |= (0xFF00) & (static_cast<quint16>(packet.at(4)) << 8);
            focusValue |= (0x00FF) & (static_cast<quint16>(packet.at(5)));

            m_focusPosition = focusValue;
        }
        // byte 6 and byte 7    => Status
        {
            quint16 statusValue = 0x0000;
            statusValue |= (0x00FF) & (static_cast<quint16>(packet.at(6)));
            statusValue |= (0xFF00) & (static_cast<quint16>(packet.at(7)) << 8);

            GammaLevel gammaLevel           = static_cast<GammaLevel>((0x0003 & statusValue));                          // 0000 0000 0000 00XX
            NoiseReductionMode noiseLevel   = static_cast<NoiseReductionMode>(0x0003 & ((0x000C & statusValue) >> 2));  // 0000 0000 0000 XX00
            FocusModes focusMode            = static_cast<FocusModes>(0x0003 & ((0x0030 & statusValue) >> 4));          // 0000 0000 00XX 0000
            SendingModes sendingMode        = static_cast<SendingModes>(0x0003 & ((0x00C0 & statusValue) >> 6));        // 0000 0000 XX00 0000

            VideoModes videoMode            = static_cast<VideoModes>(0x0003 & ((0x0300 & statusValue) >> 8));          // 0000 00XX 0000 0000
            FilterModes filterMode          = static_cast<FilterModes>(0x0007 & ((0x1C00 & statusValue) >> 10));        // 000X XX00 0000 0000
            DefogMode defogMode             = static_cast<DefogMode>(0x0007 & ((0xE000 & statusValue) >> 13));          // XXX0 0000 0000 0000

            m_gammaLevel = gammaLevel;
            m_noiseReductionMode = noiseLevel;
            m_focusMode = focusMode; // boolean
            m_sendingMode = sendingMode;

            m_videoMode = videoMode;
            m_filterMode = filterMode;
            m_defogMode = defogMode;
        }
        // byte 8               => Sensor
        {
            SensorValues sensorValue = static_cast<SensorValues>(packet.at(8));

            m_sensorValue = sensorValue;
        }
        // byte 9               => Version
        {
            quint8 panelVersion = packet.at(9);

            m_panelVersion = panelVersion;
        }
        // byte 10              => Extra Status
        {
            if (packet.count() == 13)
            {
                quint8 extraStatus = packet.at(10);

                ContrastLevel contrasLevel      = static_cast<ContrastLevel>((0x0003 & extraStatus));                   // 0000 00XX
                BrightnessLevel brightnessLevel = static_cast<BrightnessLevel>(0x0003 & ((0x000C & extraStatus) >> 2)); // 0000 XX00
                ModeLevels modeLevel            = static_cast<ModeLevels>(0x0007 & ((0x0700 & extraStatus) >> 4));      // 0XXX 0000

                m_contrastLevel = contrasLevel;
                m_brightnessLevel = brightnessLevel;
                m_mode = modeLevel;
            }
        }
    }

    return QByteArray();
}

bool SerialControl::isConnected() const
{
    return m_serialPort->isOpen();
}

quint8 SerialControl::zoomSpeed() const
{
    return m_zoomSpeed;
}

quint8 SerialControl::panTiltSpeed() const
{
    return m_panTiltSpeed;
}

quint8 SerialControl::focusSpeed() const
{
    return m_focusSpeed;
}

quint16 SerialControl::focusPosition() const
{
    return m_focusPosition;
}

quint16 SerialControl::fovPosition() const
{
    return m_fovPosition;
}

quint8 SerialControl::illuminatorBrightnessLevel() const
{
    return m_illuminatorBrightness;
}

quint8 SerialControl::illuminatorAngleOffset() const
{
    return milluminatorAngleOffset;
}

SerialControl::DefogMode SerialControl::defogMode() const
{
    return m_defogMode;
}

SerialControl::GammaLevel SerialControl::gammaLevel() const
{
    return m_gammaLevel;
}

bool SerialControl::digitalZoom() const
{
    return m_isDigitalZoomEnabled;
}

bool SerialControl::focusMode() const
{
        qDebug()  << " XXXXx: " << m_focusMode;

    return m_focusMode;
}

void SerialControl::writeDataOnPlatformsSerialPort(const QByteArray& data)
{
    if (true /*m_serialPort->isOpen()*/) {
        m_serialPort->write(data);
        qDebug() << "write data :" << data.toHex(' ');
    }
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
    Q_UNUSED(data);
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
    sendCommand1(178, m_zoomSpeed);
}

void SerialControl::zoomOut()
{
    sendCommand1(177, m_zoomSpeed);
}

void SerialControl::zoomStop()
{
    sendCommand1(188, m_zoomSpeed);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::zoomStop);
    else
        m_repeatCounter = 0;
}

void SerialControl::focusFar()
{
    sendCommand1(180, m_focusSpeed);
}

void SerialControl::focusNear()
{
    sendCommand1(184, m_focusSpeed);
}

void SerialControl::focusStop()
{
    sendCommand1(179, m_focusSpeed);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::focusStop);
    else
        m_repeatCounter = 0;
}

void SerialControl::setFocusMode(const bool mode)
{
    if (mode)
        autoFocus();
    else
        manualFocus();

    m_focusMode = mode;

    Q_EMIT sigDataChanged();
}

void SerialControl::autoFocus()
{
    sendCommand1(199, 1);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 2)
        QTimer::singleShot(50, this, &SerialControl::autoFocus);
    else
        m_repeatCounter = 0;
}

void SerialControl::manualFocus()
{
    sendCommand1(195, 1);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 2)
        QTimer::singleShot(50, this, &SerialControl::manualFocus);
    else
        m_repeatCounter = 0;
}

void SerialControl::setZoomSpeed(const quint8 speed)
{
    m_zoomSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setFocusSpeed(const quint8 speed)
{
    m_focusSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setFovPosition(const quint16 position)
{
    m_fovPosition = position;

    Q_EMIT sigDataChanged();
}

void SerialControl::gotoFov(const quint16 position)
{
    sendCommand2(59, position);
}

void SerialControl::setFocusPosition(const quint16 position)
{
    m_focusPosition = position;

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
    sendCommand1(173, m_panTiltSpeed);
}

void SerialControl::tiltDown()
{
    sendCommand1(174, m_panTiltSpeed);
}

void SerialControl::tiltStop()
{
    sendCommand1(175, 1);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::tiltStop);
    else
        m_repeatCounter = 0;
}

void SerialControl::panLeft()
{
    sendCommand1(171, m_panTiltSpeed);
}

void SerialControl::panRight()
{
    sendCommand1(170, m_panTiltSpeed);
}

void SerialControl::panStop()
{
    sendCommand1(172, 1);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 6)
        QTimer::singleShot(50, this, &SerialControl::panStop);
    else
        m_repeatCounter = 0;
}

void SerialControl::setPanTiltSpeed(const quint8 speed)
{
    m_panTiltSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setSelectedCamera(const SerialControl::CameraSelection camera)
{
    m_selectedCamera = camera;

    sendCommand1(63, camera);
}

void SerialControl::setSelectedFilter(const SerialControl::FilterSelection filter)
{
    m_selectedFilter = filter;

    sendCommand1(63, filter);
}

void SerialControl::setDefogMode(const SerialControl::DefogMode mode)
{
    m_defogMode = mode;

    sendCommand1(60, mode);
}

void SerialControl::setGammaLevel(const SerialControl::GammaLevel level)
{
    m_gammaLevel = (GammaLevel)(level + 1);

    sendCommand1(61, m_gammaLevel);
}

void SerialControl::setNoiseReductionMode(const SerialControl::NoiseReductionMode mode)
{
    m_noiseReductionMode = (NoiseReductionMode)(mode + 1);

    sendCommand1(62, m_noiseReductionMode);
}

void SerialControl::enableDigitalZoom(const bool state)
{
    m_isDigitalZoomEnabled = state;

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
    m_isIlluminatorEnabled = state;

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
    m_illuminatorBrightness = brightness;

    sendCommand1(91, m_contrastLevel);
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
    milluminatorAngleOffset = offset;

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
    m_contrastLevel = level + 1;

    sendCommand1(116, m_contrastLevel);
}

void SerialControl::setBrightnessLevel(const quint8 level)
{
    m_brightnessLevel = level + 1;

    sendCommand1(115, m_brightnessLevel);
}

void SerialControl::setMode(const quint8 mode)
{
    m_mode = mode + 1;

    sendCommand1(117, m_mode);
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

void SerialControl::init_crc8()
{
    qint32 k = 0;
    quint8 crc;

    for (quint8 i = 0; i < 255; i++)
    {
        crc = i;

        for (int j = 0; j < 8; j++)
        {
            if ((crc & 0x80) == 0x80)
            {
                k = 7;
            }
            else
            {
                k = 0;
            }

            crc = static_cast<quint8>((crc << 1) ^ k);

            m_crc8_table[i] = crc & 0xFF;
        }
    }
}

quint8 SerialControl::crc8(quint8 buf[], quint8 len) const
{
    quint8 crc = 0xFF;

    for (int i = 0; i < len; i++)
    {
        crc = m_crc8_table[(crc & 0xFF) ^ buf[i]];
    }
//    for (quint8 *p = buf; len > 0; ++p, --len)
//        crc = m_crc8_table[(crc & 0xFF) ^ *p];0

    return crc ^ 0xFF;
}
