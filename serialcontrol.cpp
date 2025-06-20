#include "serialcontrol.h"

SerialControl::SerialControl(QObject* parent)
    : RingQueue("serialControl", 10, 10000, parent),
      m_resetSerialStateDelay(200)
{
    RingQueue::initialize();
//    QObject::connect(this, &RingQueue::sigReadData,
//                     this, &SerialControl::sltReadData);
    QObject::connect(this, &RingQueue::sigWriteData,
                     this, &SerialControl::sigWriteData);

    m_serialPort = new QSerialPort;
    QObject::connect(m_serialPort, &QSerialPort::readyRead,
                     this, &SerialControl::sltReadSeialPortData);

    QObject::connect(this, &SerialControl::sigWriteData,
                     this, &SerialControl::sltWriteSerialPortData);

    m_zoomSpeed = 100;
    m_focusSpeed = 100;
    m_panTiltSpeed = 100;
    m_repeatCounter = 0;

    m_roll = 0.0;
    m_pitch = 0.0;

    m_selectedCamera = CameraSelection_ContinuousZoom;
    m_filterMode = Color;
    m_defogMode = D_High;
    m_gammaLevel = Level1;
    m_noiseReductionMode = NR_High;
    m_isDigitalZoomEnabled = false;
    m_isIlluminatorEnabled = false;
    m_isUpdatingFovEnabled = true;
    m_isUpdatingFocusEnabled = true;
    m_contrastLevel = ContrastLevel_Level2;
    m_brightnessLevel = BrightnessLevel_Level2;
    m_mode = 1;
    m_illuminatorBrightness = 255;
    m_illuminatorAngleOffset = 50;
    m_continuousModeInterval = 50;

    m_focusMode = true;
    m_sendingMode = SendingMode_Request;

    m_showLoginWindow = false;
    m_isPasswordReceived = false;

    m_isMenuOpened = false;

    m_message = "";
	
	m_imageType = ImageType_Unknown;

    m_boardVersion = 0;
    m_sensorValue = SensorValue_Unknown;


    m_platformSerialInboundState = false;
    m_platformSerialOutboundState = false;

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
        std::cerr << "======= Cannot connect to serial port " << m_serialPort->errorString().toStdString() << std::endl;

        return false;
    }

    return true;
}

bool SerialControl::disconnectSerialPort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        Q_EMIT sigDataChanged();
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

    if (lengthByte != static_cast<char>(0x0A)
        && lengthByte != static_cast<char>(0x0B)
        && lengthByte != static_cast<char>(0x0C)
        && lengthByte != static_cast<char>(0x10))
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

//    // calculate crc
//    {
//        bug, it crahes here, this variable is not an array >> quint8 *crcInput = new quint8;

//        for (int i = 2; i < packet.count() - 2; i++)
//        {
//            crcInput[i - 2] = packet.at(i);
//        }

//        crcValue = crc8(crcInput, lengthByte - 2);

//        delete crcInput;
//    }

//    QByteArray crc(2, 0x00);
//    crc[0] = crcValue;
//    crc[1] = packet.at(10);

//    std::cerr << "CHECKSUM : " << crc.toHex(' ').toStdString() << std::endl;

    // check first, second and crc bytes
    if (/*crcValue == packet.at(lengthByte)*/ true)
    {
        // byte 2 and byte 3    => FOV
        {
            quint16 fovValue = 0x0000;
            fovValue |= (0xFF00) & (static_cast<quint16>(packet.at(2)) << 8);
            fovValue |= (0x00FF) & (static_cast<quint16>(packet.at(3)));

            if (m_isUpdatingFovEnabled)
            {
                m_fovPosition = fovValue;
            }
        }
        // byte 4 and byte 5    => Focus
        {
            quint16 focusValue = 0x0000;
            focusValue |= (0xFF00) & (static_cast<quint16>(packet.at(4)) << 8);
            focusValue |= (0x00FF) & (static_cast<quint16>(packet.at(5)));

            if (m_isUpdatingFocusEnabled)
            {
                m_focusPosition = focusValue;
            }
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

            CameraSelection videoMode       = static_cast<CameraSelection>(0x0003 & ((0x0300 & statusValue) >> 8));     // 0000 00XX 0000 0000
            FilterModes filterMode          = static_cast<FilterModes>(0x0007 & ((0x1C00 & statusValue) >> 10));        // 000X XX00 0000 0000
            DefogMode defogMode             = static_cast<DefogMode>(0x0007 & ((0xE000 & statusValue) >> 13));          // XXX0 0000 0000 0000

            m_gammaLevel = gammaLevel;
            m_noiseReductionMode = noiseLevel;

            if (focusMode == 2)
                m_focusMode = true; // boolean
            else if (focusMode == 1)
                m_focusMode = false;

            m_sendingMode = sendingMode;

            m_selectedCamera = videoMode;
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
            quint8 boardVersion = packet.at(9);

            m_boardVersion = boardVersion;
        }
        // byte 10              => Extra Status1
        {
            if (packet.count() == 13
                    || packet.count() == 14)
            {
                quint8 extraStatus = packet.at(10);

                ContrastLevel contrasLevel      = static_cast<ContrastLevel>((0x03 & extraStatus));                 // 0000 00XX
                BrightnessLevel brightnessLevel = static_cast<BrightnessLevel>(0x03 & ((0x0C & extraStatus) >> 2)); // 0000 XX00
                ModeLevels modeLevel            = static_cast<ModeLevels>(0x07 & ((0x70 & extraStatus) >> 4));      // 0XXX 0000
                bool value                      = static_cast<bool>(0x01 & ((0x80 & extraStatus) >> 7));            // X000 0000

                m_contrastLevel = contrasLevel;
                m_brightnessLevel = brightnessLevel;
                m_mode = modeLevel;
				
				
                if (packet.count() == 13)
                    m_isDigitalZoomEnabled = value;
                else if (packet.count() == 14)
                    m_isIlluminatorEnabled = value;
            }
        }
        // byte 11              => Extra Status2
        {
            if (packet.count() == 14)
            {
                quint8 extraStatus = packet.at(11);

                ImageType imageType  = static_cast<ImageType>((0x03 & extraStatus));                     // 0000 00XX
                bool digitalZoom     = static_cast<BrightnessLevel>(0x01 & ((0x04 & extraStatus) >> 2)); // 0000 0X00

                m_imageType = imageType;
                m_isDigitalZoomEnabled = digitalZoom;
            }
        }
        // byte 12-13              => AngleX
        {
            if (packet.count() == 18)
            {
                m_roll = static_cast<float>(packet.at(12) * 256 + packet.at(13)) / 100.0;
            }
        }
        // byte 14-15              => Angle Y
        {
            if (packet.count() == 18)
            {
                m_pitch = static_cast<float>(packet.at(14) * 256 + packet.at(15)) / 100.0;
            }
        }
    }

//    qDebug() << "m_filterMode: " << m_filterMode
//             << "m_selectedCamera: " << m_selectedCamera;

    Q_EMIT sigDataChanged();

    return QByteArray();
}

bool SerialControl::isConnected()
{
    Q_EMIT sigDataChanged();
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
    return m_illuminatorAngleOffset;
}

SerialControl::DefogMode SerialControl::defogMode() const
{
    return m_defogMode;
}

SerialControl::GammaLevel SerialControl::gammaLevel() const
{
    return (GammaLevel)(m_gammaLevel - 1);
}

SerialControl::NoiseReductionMode SerialControl::noiseReductionMode() const
{
    return (NoiseReductionMode)(m_noiseReductionMode - 1);
}

bool SerialControl::digitalZoom() const
{
    return m_isDigitalZoomEnabled;
}

bool SerialControl::illuminator() const
{
    return m_isIlluminatorEnabled;
}

bool SerialControl::focusMode() const
{
    return m_focusMode;
}

SerialControl::FilterModes SerialControl::selectedFilter() const
{
    return m_filterMode;
}

SerialControl::CameraSelection SerialControl::selectedCamera() const
{
    return m_selectedCamera;
}

SerialControl::ContrastLevel SerialControl::contrastLevel() const
{
    return (ContrastLevel) (m_contrastLevel - 1);
}

QString SerialControl::imageType() const
{
    switch (m_imageType) {
    case ImageType_Normal:
        return "Normal";
    case ImageType_LicensePlateRecognition1:
        return "License Plate Recognition 1";
    case ImageType_LicensePlateRecognition2:
        return "License Plate Recognition 2";

    default:
        return "Unknown";
    }
}

SerialControl::BrightnessLevel SerialControl::brightnessLevel() const
{
    return (BrightnessLevel) (m_brightnessLevel);
}

quint8 SerialControl::mode() const
{
    return (m_mode - 1);
}

quint8 SerialControl::boardVersion() const
{
    return m_boardVersion;
}

QString SerialControl::sensor() const
{
    switch (m_sensorValue) {

    case SensorValue_Farabin3:
        return "Farabin3";
    case SensorValue_Farabin5:
        return "Farabin5";
    case SensorValue_Farabin5B:
        return "Farabin5B";
    case SensorValue_Farabin9:
        return "Farabin9";
    case SensorValue_Farabin18:
        return "Farabin18";
    case SensorValue_Farabin15:
        return "Farabin15";
    case SensorValue_Farabin15i3:
        return "Farabin15i3";

    default:
        return "Unknown";
    }
}

quint8 SerialControl::sendingMode() const
{
    return (quint8)(m_sendingMode);
}

quint8 SerialControl::continuousModeInterval() const
{
    return m_continuousModeInterval;
}

QString SerialControl::messageBox() const
{
    return m_message;
}

bool SerialControl::showLoginWindow() const
{
    return m_showLoginWindow;
}

float SerialControl::roll() const
{
    return m_roll;
}

float SerialControl::pitch() const
{
    return m_pitch;
}

QVariant SerialControl::getNoiseReductionType()
{
    return QVariant::fromValue(m_noiseReductionMode);
}

QVariant SerialControl::getDefogType()
{
    return QVariant::fromValue(m_defogMode);
}

QVariant SerialControl::getGammaType()
{
    return QVariant::fromValue(m_gammaLevel);
}

QVariant SerialControl::getFilterType()
{
    return QVariant::fromValue(m_filterMode);
}

void SerialControl::joyStickPanTilt(const quint8 &panDirection, const quint8 &panSpeed,
                                    const quint8 &tiltDirection, const quint8 &tiltSpeed)
{
    sendCommand3(61, panDirection, panSpeed, tiltDirection, tiltSpeed);
}

void SerialControl::writeDataOnPlatformsSerialPort(const QByteArray& data)
{
    //qDebug() << "write data :" << data.toHex(' ');

    if (m_serialPort->isOpen()) {
        changePlatformSerialInboundState(true);
        QTimer::singleShot(m_resetSerialStateDelay,
                           this, [this](){changePlatformSerialInboundState(false);});

        m_serialPort->write(data);
    }
    else
    {
        m_message = "Serial Port is not connected!";

        Q_EMIT sigMessageBoxRequested();
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
        changePlatformSerialOutboundState(true);
        QTimer::singleShot(m_resetSerialStateDelay,
                           this, [this](){changePlatformSerialOutboundState(false);});

        const QByteArray data = m_serialPort->readAll();
        //        qDebug() << "read data :" << data.toHex();
        insertToReadQueue(data);
    }
}

void SerialControl::sltWriteSerialPortData(QByteArray data)
{
    writeDataOnPlatformsSerialPort(data);
}

void SerialControl::sltSetCameraMode()
{
    setSelectedCamera(m_selectedCamera);
}

void SerialControl::sltSetFilterMode()
{
    setSelectedFilter(m_filterMode);
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
    if (m_repeatCounter++ < 1)
        QTimer::singleShot(50, this, &SerialControl::autoFocus);
    else
        m_repeatCounter = 0;
}

void SerialControl::manualFocus()
{
    sendCommand1(195, 1);

    //Repeat command to increase reliability of stopping process
    if (m_repeatCounter++ < 1)
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



void SerialControl::gotoFov(const quint16 position)
{
    m_fovPosition = position;

    sendCommand2(59, m_fovPosition);

    Q_EMIT sigDataChanged();
}

//void SerialControl::setFocusPosition(const quint16 position)
//{
//    m_focusPosition = position;
//    qDebug() << " YYYYY: " << m_focusPosition;

//    Q_EMIT sigDataChanged();
//}

void SerialControl::gotoFocus(const quint16 position)
{
    m_focusPosition = position;

    sendCommand2(60, m_focusPosition);

    Q_EMIT sigDataChanged();
}

//void SerialControl::gotoFovFocus(const quint16 fovPosition, const quint16 focusPosition)
//{
//    sendCommand3(60, fovPosition, focusPosition);
//}

void SerialControl::setPosition(const quint8 positionNumber)
{
    sendCommand1(111, positionNumber);
}

void SerialControl::clearPosition(const quint8 positionNumber)
{
    sendCommand1(112, positionNumber);
}

void SerialControl::gotoPosition(const quint8 positionNumber)
{
    sendCommand1(113, positionNumber);
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


}

void SerialControl::setPanTiltSpeed(const quint8 speed)
{
    m_panTiltSpeed = speed;

    Q_EMIT sigDataChanged();
}

void SerialControl::setNextCamera()
{
    switch (m_selectedCamera)
    {
    case CameraSelection_ContinuousZoom:
    {
        m_selectedCamera = CameraSelection_Spotter;
        break;
    }
    case CameraSelection_Spotter:
    {
        m_selectedCamera = CameraSelection_ContinuousZoom;
        break;
    }
//    case CameraSelection_SWIRSpotter:
//    {
//        m_selectedCamera = CameraSelection_ContinuousZoom;
//        break;
//    }
    }

    setSelectedCamera(m_selectedCamera);
}

void SerialControl::setSelectedCamera(const SerialControl::CameraSelection camera)
{
    m_selectedCamera = camera;

    sendCommand1(63, camera);

    Q_EMIT sigDataChanged();
}

void SerialControl::setNextFilter()
{
    switch (m_filterMode)
    {
    case Color:
    {
        m_filterMode = NIR;
        break;
    }
    case NIR:
    {
        m_filterMode = Color;
        break;
    }
    }

    setSelectedFilter(m_filterMode);
}

void SerialControl::setSelectedFilter(const SerialControl::FilterModes filter)
{
    m_filterMode = filter;

    sendCommand1(75, filter);

    Q_EMIT sigDataChanged();
}

void SerialControl::setNextDefogMode()
{
    switch(m_defogMode)
    {
    case D_Off:
    {
        m_defogMode = D_Low;
        break;
    }
    case D_Low:
    {
        m_defogMode = D_Medium;
        break;
    }
    case D_Medium:
    {
        m_defogMode = D_High;
        break;
    }
    case D_High:
    {
        m_defogMode = D_Off;
        break;
    }
    }

    setDefogMode(m_defogMode);
}

void SerialControl::setDefogMode(const SerialControl::DefogMode mode)
{
    m_defogMode = mode;

    sendCommand1(60, mode);

    Q_EMIT sigDataChanged();
}

void SerialControl::setNextGammaLevel()
{
    switch(m_gammaLevel)
    {
    case Level1:
    {
        m_gammaLevel = Level2;
        break;
    }
    case Level2:
    {
        m_gammaLevel = Level1;
        break;
    }
    }


    sendCommand1(61, m_gammaLevel);

    Q_EMIT sigDataChanged();
}

void SerialControl::setGammaLevel(const SerialControl::GammaLevel level)
{
    m_gammaLevel = (GammaLevel)(level + 1);

    sendCommand1(61, m_gammaLevel);

    Q_EMIT sigDataChanged();
}

void SerialControl::setNextNoiseReductionMode()
{
    switch(m_noiseReductionMode)
    {
    case NR_Low:
    {
        m_noiseReductionMode = NR_High;
        break;
    }
    case NR_High:
    {
        m_noiseReductionMode = NR_Low;
        break;
    }
    }

    sendCommand1(62, m_noiseReductionMode);

    Q_EMIT sigDataChanged();
}

void SerialControl::setNoiseReductionMode(const SerialControl::NoiseReductionMode mode)
{
    m_noiseReductionMode = (NoiseReductionMode)(mode + 1);

    sendCommand1(62, m_noiseReductionMode);

    Q_EMIT sigDataChanged();
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

    Q_EMIT sigDataChanged();
}

void SerialControl::enableFovUpdate(const bool state)
{
    m_isUpdatingFovEnabled = state;
}

void SerialControl::enableFocusUpdate(const bool state)
{
    m_isUpdatingFocusEnabled = state;
}

void SerialControl::enableIlluminator(const bool state)
{
    if (m_serialPort->isOpen())
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
    else
    {
        m_message = "Serial Port is not connected!";

        Q_EMIT sigMessageBoxRequested();
    }

    Q_EMIT sigDataChanged();
}

void SerialControl::toggleIlluminator()
{
    enableIlluminator(!m_isIlluminatorEnabled);
}

void SerialControl::setIlluminatorBrightness(const quint8 brightness)
{
    m_illuminatorBrightness = brightness;

    sendCommand1(91, m_illuminatorBrightness);

    Q_EMIT sigDataChanged();
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

    sendCommand1(92, offset);

    m_illuminatorAngleOffset = offset;

    Q_EMIT sigDataChanged();
}

void SerialControl::setRequestSendingMode()
{
    sendCommand1(12, 0);

    m_sendingMode = SendingMode_Request;

    Q_EMIT sigDataChanged();
}

void SerialControl::setStatusSendingMode()
{
    sendCommand4(80);
}

void SerialControl::setContinuousSendingMode(const quint8 interval)
{

    sendCommand1(12, interval);

    m_continuousModeInterval = interval;

    Q_EMIT sigDataChanged();
}

QString SerialControl::serialportName() const
{
    return m_serialPort->portName();
}

void SerialControl::login()
{
    m_isPasswordReceived = true;

    showMenuPressed();
    showMenuReleased();
}

void SerialControl::setImageType(const quint8 type)
{
    m_imageType = ImageType(type + 1);

    sendCommand1(94, m_imageType);

    Q_EMIT sigDataChanged();
}

void SerialControl::bootLoader()
{
    sendCommand1(110, 0);
}

bool SerialControl::
platformSerialInboundState() const
{
    return m_platformSerialInboundState;
}

bool SerialControl::
platformSerialOutboundState() const
{
    return m_platformSerialOutboundState;
}

void SerialControl::setContrastMode(const quint8 level)
{
    m_contrastLevel = (ContrastLevel)(level + 1);

    sendCommand1(116, m_contrastLevel);

    Q_EMIT sigDataChanged();
}

void SerialControl::setBrightnessLevel(const quint8 level)
{
    m_brightnessLevel = (BrightnessLevel) (level);

    sendCommand1(115, level + 1);

    Q_EMIT sigDataChanged();
}

void SerialControl::setMode(const quint8 mode)
{
    m_mode = mode + 1;

    sendCommand1(117, m_mode);

    Q_EMIT sigDataChanged();
}

void SerialControl::toggleMenu()
{
    if (m_isMenuOpened == false) // show menu
    {
        qDebug() << " show menu requested";
        showMenuPressedRequested();
        showMenuReleased();
    }
    else // hide menu
    {
        qDebug() << " hide menu requested";
        menuESCReleased();
    }
}

void SerialControl::showMenuPressedRequested()
{
    if (!m_isPasswordReceived)
    {
        m_showLoginWindow = true;

        Q_EMIT sigLoginWindowRequested();
    }
    else
    {
        showMenuPressed();
        showMenuReleased();
    }
}

void SerialControl::showMenuPressed()
{
    sendCommand1(64, 1);
    m_isMenuOpened = true;
    qDebug() << " menu is opened now ";
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
    sendCommand1(58, 1);
    m_isMenuOpened = false;
    qDebug() << " menu is hide now ";
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
                                 const quint8& param1,
                                 const quint8& param2,
                                 const quint8& param3,
                                 const quint8& param4)
{

    quint8 checkSum = command + param1 + param2 + param3 + param4 + 1;

    QByteArray data;
    data.append(static_cast<quint8>(104));
    data.append(static_cast<quint8>(6));
    data.append(static_cast<quint8>(1));
    data.append(static_cast<quint8>(command));
    data.append(static_cast<quint8>(param1));
    data.append(static_cast<quint8>(param2));
    data.append(static_cast<quint8>(param3));
    data.append(static_cast<quint8>(param4));
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

void SerialControl::changePlatformSerialInboundState(const bool &state)
{
    m_platformSerialInboundState = state;
    Q_EMIT sigSerialStateChanged();
}

void SerialControl::changePlatformSerialOutboundState(const bool &state)
{
    m_platformSerialOutboundState = state;
    Q_EMIT sigSerialStateChanged();
}

