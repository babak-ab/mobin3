#include "appcontrol.h"
#include <QCameraInfo>
#include <QCursor>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QProcess>
#include <QScreen>
#include <QStandardPaths>
#include <QUuid>
#include <iostream>

AppControl::AppControl(QObject* parent)
    : QObject(parent) ,
      m_resetSerialStateDelay(200)
{

    gst_init(NULL,NULL);


    fillSerialPortNames();

    m_serialControl = new SerialControl;
    m_reticle = new Reticle;

#ifdef Q_OS_WIN32
    int counter = 0;
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo& cameraInfo : cameras) {
        counter++;
    }
    m_captureDevice = QString::number(counter - 1);
#endif

#ifdef Q_OS_LINUX
    m_captureDevice = "/dev/video0";
#endif

    m_recordingLocation = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    m_videoCapture = new VideoCapture(m_captureDevice, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    m_videoCapture->initialize();
    m_videoCapture->start();

    m_recordVisible = false;

    m_reticleVisible = false;

    m_videoRecord = new VideoRecord(QSize(FRAME_WIDTH, FRAME_HEIGHT));

    m_videoAdapter = new VideoAdapter(QSize(FRAME_WIDTH, FRAME_HEIGHT));

    connect(m_videoCapture, &VideoCapture::sigFrameReady, m_videoAdapter, &VideoAdapter::onFrameReady);
    connect(m_videoCapture, &VideoCapture::sigFrameReady, this, &AppControl::restartElapsedTimerRequested);
    connect(m_videoCapture, &VideoCapture::sigI420_FrameReady, m_videoRecord, &VideoRecord::pushFrame);

    m_toggleIlluminatorTimer.setInterval(5000);
    connect(&m_toggleIlluminatorTimer, &QTimer::timeout,
            this, [this]()
    {
        m_toggleIlluminatorTimer.stop();

        m_serialControl->toggleIlluminator();
    });

    m_shutDownSystemTimer.setInterval(5000);
    connect(&m_shutDownSystemTimer, &QTimer::timeout,
            this, [this]()
    {
        m_shutDownSystemTimer.stop();

        qApp->exit();
    });

    m_lastJoystickPanSpeed    = 0;
    m_lastJoystickTiltSpeed   = 0;
    m_lastJoystickPanDirection = 0;
    m_lastJoystickTiltDirection = 0;

    m_timerTvWatchdog.setInterval(300);
    connect(&m_timerTvWatchdog, &QTimer::timeout, this, &AppControl::sltCheckTVCapture);
    m_elapsedTimerTvCaptureWatchdog.restart();
    m_timerTvWatchdog.start();

    connect(m_reticle, &Reticle::sigDataChanged,
            this, &AppControl::reticleVisibleChanged);

    connect(&m_serialBoardController,
            &SerialController::sigNewDataReceived,
            this, &AppControl::sltNewDataReceived);

    connect(&m_serialBoard, &SerialBoard::
            sigNewDataReceived, this,
            &AppControl::sltSerialBoardDataReceived);
//            &AppControl::sigSerialBoardDataReceived);

    connect(m_serialControl, &SerialControl::sigDataChanged,
            this, &AppControl::sltPlatformNewDataReceived);

    m_boardSerialInboundState = false;
    m_boardSerialOutboundState = false;

    m_isKeyboardCommandChanged = false;


    // Automatic connect to serial ports
    int32_t errorCode;

#ifdef __linux__
    m_serialControl->connectToSerialPort("ttyTHS0");
    m_serialBoardController.openConnection(
                "ttyTHS1", 115200, errorCode);
#elif defined(WIN32)
    m_serialControl->connectToSerialPort("COM1");
    m_serialBoardController.openConnection(
                "COM2", 9600, errorCode);
#endif
}

AppControl::~AppControl()
{
    m_gamepadManager->deleteLater();

    delete m_gamepadController;
}

QString AppControl::recordingLocation() const
{
    return m_recordingLocation;
}

void AppControl::setRecordingLocation(const QString &recordingLocation)
{
    m_recordingLocation = recordingLocation;
    Q_EMIT recordingLocationChanged();
}

VideoAdapter* AppControl::videoAdapter() const
{
    return m_videoAdapter;
}

void AppControl::setVideoAdapter(VideoAdapter* videoAdapter)
{
    m_videoAdapter = videoAdapter;
}

bool AppControl::recordVisible() const
{
    return m_recordVisible;
}

void AppControl::setRecordVisible(bool recordVisible)
{
    m_recordVisible = recordVisible;

    Q_EMIT recordVisibleChanged();
}

QString AppControl::appVersion() const
{
    return SOFTWARE_VERSION;
}

void AppControl::processGamepadCommand(const CommandPacket& packet)
{
    switch (packet.command)
    {
    case Command_ZoomIn:
    {
        std::cerr << "Command_ZoomIn" << std::endl;

        m_serialControl->setZoomSpeed(packet.value);
        m_serialControl->zoomIn();
        break;
    }
    case Command_ZoomOut:
    {
        std::cerr << "Command_ZoomOut" << std::endl;

        m_serialControl->setZoomSpeed(packet.value);
        m_serialControl->zoomOut();
        break;
    }
    case Command_ZoomStop:
    {
        std::cerr << "Command_ZoomStop" << std::endl;

        m_serialControl->zoomStop();
        break;
    }
    case Command_FocusFar:
    {
        std::cerr << "Command_FocusFar" << std::endl;

        m_serialControl->setFocusSpeed(packet.value);
        m_serialControl->focusFar();
        break;
    }
    case Command_FocusNear:
    {
        std::cerr << "Command_FocusNear" << std::endl;

        m_serialControl->setFocusSpeed(packet.value);
        m_serialControl->focusNear();
        break;
    }
    case Command_FocusStop:
    {
        std::cerr << "Command_FocusStop" << std::endl;
        m_serialControl->focusStop();
        break;
    }
    case Command_ToggleFocusMode:
    {
        if (packet.value == 0)
        {
            return;
        }

        bool previousFocusMode = m_serialControl->focusMode();
        m_serialControl->setFocusMode(!previousFocusMode);
        break;
    }
    case Command_PanLeft:
    {
        m_lastJoystickPanSpeed = packet.value;
        m_lastJoystickPanDirection = 0;
        m_serialControl->joyStickPanTilt(m_lastJoystickPanDirection, m_lastJoystickPanSpeed,
                                         m_lastJoystickTiltDirection, m_lastJoystickTiltSpeed);
        break;
    }
    case Command_PanRight:
    {
        m_lastJoystickPanSpeed = packet.value;
        m_lastJoystickPanDirection = 1;
        m_serialControl->joyStickPanTilt(m_lastJoystickPanDirection, m_lastJoystickPanSpeed,
                                         m_lastJoystickTiltDirection, m_lastJoystickTiltSpeed);

        break;
    }
    case Command_PanStop:
    {
        m_lastJoystickPanSpeed = 0;
        m_lastJoystickPanDirection = 1;
        m_serialControl->joyStickPanTilt(m_lastJoystickPanDirection, m_lastJoystickPanSpeed,
                                         m_lastJoystickTiltDirection, m_lastJoystickTiltSpeed);

        break;
    }
    case Command_TiltUp:
    {
        m_lastJoystickTiltSpeed = packet.value;
        m_lastJoystickTiltDirection = 1;
        m_serialControl->joyStickPanTilt(m_lastJoystickPanDirection, m_lastJoystickPanSpeed,
                                         m_lastJoystickTiltDirection, m_lastJoystickTiltSpeed);
        break;
    }
    case Command_TiltDown:
    {
        m_lastJoystickTiltSpeed = packet.value;
        m_lastJoystickTiltDirection = 0;
        m_serialControl->joyStickPanTilt(m_lastJoystickPanDirection, m_lastJoystickPanSpeed,
                                         m_lastJoystickTiltDirection, m_lastJoystickTiltSpeed);
        break;
    }
    case Command_TiltStop:
    {
        m_lastJoystickTiltSpeed = 0;
        m_lastJoystickTiltDirection = 1;
        m_serialControl->joyStickPanTilt(m_lastJoystickPanDirection, m_lastJoystickPanSpeed,
                                         m_lastJoystickTiltDirection, m_lastJoystickTiltSpeed);

        break;
    }
    case Command_NextCamera:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->setNextCamera();
        break;
    }
    case Command_NextFilter:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->setNextFilter();
        break;
    }
    case Command_NextDefogLevel:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->setNextDefogMode();
        break;
    }
    case Command_NextGammaLevel:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->setNextGammaLevel();
        break;
    }
    case Command_NextNoiseReductionLevel:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->setNextNoiseReductionMode();
        break;
    }
    case Command_ToggleDigitalZoom:
    {
        if (packet.value == 0)
        {
            return;
        }

        bool previousDigitalZoomValue = m_serialControl->digitalZoom();

        m_serialControl->enableDigitalZoom(!previousDigitalZoomValue);
        break;
    }
    case Command_OpenCameraMenu:
    {
        if (packet.value == 0)
        {
            return;
        }

        qDebug()  << "XXXX  Command_OpenCameraMenu";

        m_serialControl->toggleMenu();

        break;
    }
    case Command_MenuItemUp:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->menuUpPressed();
        m_serialControl->menuUpReleased();
        break;
    }
    case Command_MenuItemDown:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->menuDownPressed();
        m_serialControl->menuDownReleased();
        break;
    }
    case Command_MenuItemLeft:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->menuLeftPressed();
        m_serialControl->menuLeftReleased();
        break;
    }
    case Command_MenuItemRight:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->menuRightPressed();
        m_serialControl->menuRightReleased();
        break;
    }
    case Command_CloseCameraMenu:
    {
        if (packet.value == 0)
        {
            return;
        }

        m_serialControl->menuESCReleased();
        break;
    }
    case Command_ShutdownSystem:
    {
        if (packet.value == 1.0)
        {
            if (m_shutDownSystemTimer.isActive() == false)
            {
                m_shutDownSystemTimer.start();
            }
        }
        else
        {
            if (m_shutDownSystemTimer.isActive() == true)
            {
                m_shutDownSystemTimer.stop();
            }
        }

        break;
    }
    case Command_ToggleIlluminator:
    {
        if (packet.value == static_cast<quint8>(true))
        {
            if (m_toggleIlluminatorTimer.isActive() == false)
            {
                m_toggleIlluminatorTimer.start();
            }
        }
        else
        {
            if (m_toggleIlluminatorTimer.isActive() == true)
            {
                m_toggleIlluminatorTimer.stop();
            }
        }

        break;
    }
    case Command_IncreaseAngleOfIlluminator:
    {
        m_serialControl->setIlluminatorAngleOffset(packet.value);
        break;
    }
    case Command_IncreaseBrightnessOfIlluminator:
    {
        m_serialControl->setIlluminatorBrightness(packet.value);
        break;
    }
    default:
        return;
    }
}

void AppControl::setSerialPortName(QString portName)
{
    m_serialPortName = portName;
}

int AppControl::findSerialPortName(QString portName)
{
    for (int i = 0; i < m_serialPortList.size(); i++) {
        if (m_serialPortList[i].contains(portName)) {
            return i;
        }
    }

    return 0;
}

void AppControl::startRecord()
{
    m_videoRecord->initialize(m_recordingLocation);
    m_videoRecord->start();

    setRecordVisible(true);
}

void AppControl::stopRecord()
{
    m_videoRecord->stop();

    setRecordVisible(false);
}

bool AppControl::reticleVisible() const
{
    return m_reticleVisible;
}

void AppControl::setReticleVisible(bool reticleVisible)
{
    m_reticleVisible = reticleVisible;
    Q_EMIT reticleVisibleChanged();
}

void AppControl::sendMouseEvent(
        QObject *object, const bool &isPressed)
{
    QMouseEvent::Type type =
            QMouseEvent::Type::MouseButtonPress;

    if (isPressed == false)
    {
        type = QMouseEvent::Type::MouseButtonRelease;
    }

    QMouseEvent *event = new QMouseEvent(
                type, QPoint(0, 0),
                Qt::LeftButton,
                Qt::LeftButton,
                Qt::NoModifier);

    QApplication::postEvent(object, event);
}

void AppControl::takeSnapshot()
{

}



bool AppControl::boardSerialInboundState() const
{
    return m_boardSerialInboundState;
}

bool AppControl::boardSerialOutboundState() const
{
    return m_boardSerialOutboundState;
}

QString AppControl::messageTitle() const
{
    return m_messageTitle;
}

QString AppControl::messageText() const
{
    return m_messageText;
}

bool AppControl::connectToSerialPort()
{
    bool isConnected = m_serialControl->connectToSerialPort(m_serialPortName);

    if (isConnected) {
        return true;
    }

    m_messageTitle = "Serial Port Error";
    m_messageText = "Could not connect to " + m_serialPortName + " port!";
    Q_EMIT sigThrowSerialMessageRequested();

    return false;
}

void AppControl::disconnectSerialPort()
{
    bool result = m_serialControl->disconnectSerialPort();

    if (!result) {
        m_messageTitle = "Serial Port Error";
        m_messageText = "Could not get disconnected from " + m_serialPortName + " port!";
        Q_EMIT sigThrowSerialMessageRequested();
    }
}

void AppControl::fillSerialPortNames()
{
    m_serialPortList.clear();

    // Fill the serial port names
    foreach (const QSerialPortInfo& serialPortInfo, QSerialPortInfo::availablePorts()) {
        m_serialPortList.append(serialPortInfo.portName());
    }

    if (m_serialPortList.count() > 0)
        m_serialPortName = m_serialPortList[0];
}

void AppControl::changeBoardSerialInboundState(const bool &state)
{
    m_boardSerialInboundState = state;
    Q_EMIT sigSerialStateChanged();
}

void AppControl::changeBoardSerialOutboundState(const bool &state)
{
    m_boardSerialOutboundState = state;
    Q_EMIT sigSerialStateChanged();
}

QStringList AppControl::serialPortList()
{
    fillSerialPortNames();

    qDebug() << "_serialPortList: " << m_serialPortList;
    return m_serialPortList;
}

SerialControl* AppControl::serialControl() const
{
    return m_serialControl;
}

Reticle *AppControl::reticle() const
{
    return m_reticle;
}

void AppControl::setGamepadController(GamepadController* gamepadController)
{
    m_gamepadController = gamepadController;

    connect(m_gamepadController, &GamepadController::sigExecuteCommandRequested,
        this, &AppControl::sltExecuteCommandRequested);
}

void AppControl::setGamepadManager(QGamepadManager* manager)
{
    m_gamepadManager = manager;
}

void AppControl::sltExecuteCommandRequested(const CommandPacket& packet)
{
    processGamepadCommand(packet);
}

void AppControl::sltCheckTVCapture()
{
    if (m_elapsedTimerTvCaptureWatchdog.elapsed() > 3000) {

        m_videoCapture->checkConnection();

        m_elapsedTimerTvCaptureWatchdog.restart();
    }
}

void AppControl::restartElapsedTimerRequested(const QByteArray& data)
{
    m_elapsedTimerTvCaptureWatchdog.restart();
}

void AppControl::sltNewDataReceived(const QByteArray &packet)
{
    changeBoardSerialOutboundState(true);
    QTimer::singleShot(m_resetSerialStateDelay, this,
                       [this](){changeBoardSerialOutboundState(false);});

    m_serialBoard.processInboundPacket(packet);
}

void AppControl::sltPlatformNewDataReceived()
{
    const SerialControl::CameraSelection camera =
            m_serialControl->selectedCamera();

    const SerialControl::FilterModes filterMode =
            m_serialControl->selectedFilter();

    const bool isRecording =
            m_videoRecord->isActive();

    const bool &isIlluminatorEnabled =
            m_serialControl->illuminator();

    const bool &isAutoFocusEnabled =
            m_serialControl->focusMode();

    QByteArray data(5, char(0x00));
    data[0] = camera == SerialControl::CameraSelection_Spotter ? 0x01 : 0x00;
    data[1] = filterMode == SerialControl::NIR ? 0x01 : 0x00;
    data[2] = isRecording == true ? 0x01 : 0x00;
    data[3] = isIlluminatorEnabled == true ? 0x01 : 0x00;
    data[4] = isAutoFocusEnabled == true ? 0x01 : 0x00;

    const QByteArray packet =
            m_serialBoard.createFeedback(data);

    m_serialBoardController.writeData(packet);

    changeBoardSerialInboundState(true);
    QTimer::singleShot(m_resetSerialStateDelay, this,
                       [this](){changeBoardSerialInboundState(false);});
}

void AppControl::
sltSerialBoardDataReceived(
        const int &keyboardCommand)
{
    const SerialBoard::Commands command =
            static_cast<SerialBoard::Commands>(
                keyboardCommand);

    qCritical() << "       command:" << command << endl
                << "  last command:" << m_lastCommand << endl
                << "keyboard state:" << m_isKeyboardCommandChanged << endl;

    if (command == SerialBoard::Command_Normal)
    {
        m_isKeyboardCommandChanged = false;
    }

    if (m_isKeyboardCommandChanged == true)
    {
        return;
    }

    if (command != SerialBoard::Command_Normal)
    {
        m_isKeyboardCommandChanged = true;
    }

    switch (command)
    {
    case SerialBoard::Command_Normal:
    {
        if (m_lastCommand ==
                SerialBoard::Command_MoveUp ||
                m_lastCommand ==
                SerialBoard::Command_MoveDown)
        {
            m_serialControl->tiltStop();
        }
        else if (m_lastCommand ==
                 SerialBoard::Command_MoveLeft ||
                 m_lastCommand ==
                 SerialBoard::Command_MoveRight)
        {
            m_serialControl->panStop();
        }
        else if (m_lastCommand ==
                 SerialBoard::Command_ZoomPlus ||
                 m_lastCommand ==
                 SerialBoard::Command_ZoomMinus)
        {
            m_serialControl->zoomStop();
        }
        else if (m_lastCommand ==
                 SerialBoard::Command_FocusPlus ||
                 m_lastCommand ==
                 SerialBoard::Command_FocusMinus)
        {
            m_serialControl->focusStop();
        }

        break;
    }
    case SerialBoard::Command_Continuous:
    {
        m_serialControl->
                setSelectedCamera(
                    SerialControl::
                    CameraSelection_ContinuousZoom);
        break;
    }
    case SerialBoard::Command_Spotter:
    {
        m_serialControl->
                setSelectedCamera(
                    SerialControl::
                    CameraSelection_Spotter);
        break;
    }
    case SerialBoard::Command_NIR_Filter:
    {
        const SerialControl::
                FilterModes lastState =
                m_serialControl->selectedFilter();

        if (lastState == SerialControl::Color)
        {
            m_serialControl->setSelectedFilter(
                        SerialControl::NIR);
        }
        else
        {
            m_serialControl->setSelectedFilter(
                        SerialControl::Color);
        }

        break;
    }
    case SerialBoard::Command_Snapshot:
    {
        takeSnapshot();

        break;
    }
    case SerialBoard::Command_VideoRecord:
    {
        if (m_videoRecord->isActive() == true)
        {
            stopRecord();
        }
        else
        {
            startRecord();
        }

        break;
    }
    case SerialBoard::Command_Laser:
    {
        const bool shouldEnabled =
                !m_serialControl->illuminator();

        m_serialControl->
                enableIlluminator(shouldEnabled);

        break;
    }
    case SerialBoard::Command_BeamAnglePlus:
    {
        m_serialControl->
                setIlluminatorLargerAngle();
        break;
    }
    case SerialBoard::Command_BeamAngleMinus:
    {
        m_serialControl->
                setIlluminatorSmallerAngle();
        break;
    }
    case SerialBoard::Command_IntensityPlus:
    {
        uint8_t intensityPlus =
                m_serialControl->
                illuminatorBrightnessLevel();

        if (intensityPlus + 10 > 255)
        {
            intensityPlus = 255;
        }
        else
        {
            intensityPlus += 10;
        }

        m_serialControl->
                setIlluminatorBrightness(
                    intensityPlus);
        break;
    }
    case SerialBoard::Command_IntensityMinus:
    {
        uint8_t intensityPlus =
                m_serialControl->
                illuminatorBrightnessLevel();

        if (intensityPlus - 10 < 0)
        {
            intensityPlus = 0;
        }
        else
        {
            intensityPlus -= 10;
        }

        m_serialControl->
                setIlluminatorBrightness(
                    intensityPlus);

        break;
    }
    case SerialBoard::Command_SpeedPlus:
    {
        uint8_t speedPlus =
                m_serialControl->panTiltSpeed();

        if (speedPlus + 10 > 255)
        {
            speedPlus = 255;
        }
        else
        {
            speedPlus += 10;
        }

        m_serialControl->
                setPanTiltSpeed(speedPlus);

        break;
    }
    case SerialBoard::Command_SpeedMinus:
    {
        uint8_t speedPlus =
                m_serialControl->panTiltSpeed();

        if (speedPlus + 10 > 255)
        {
            speedPlus = 255;
        }
        else
        {
            speedPlus += 10;
        }

        m_serialControl->
                setPanTiltSpeed(speedPlus);

        break;
    }
    case SerialBoard::Command_MoveUp:
    {
        m_serialControl->tiltUp();

        break;
    }
    case SerialBoard::Command_MoveDown:
    {
        m_serialControl->tiltDown();

        break;
    }
    case SerialBoard::Command_MoveLeft:
    {
        m_serialControl->panLeft();

        break;
    }
    case SerialBoard::Command_MoveRight:
    {
        m_serialControl->panRight();

        break;
    }
    case SerialBoard::Command_ZoomPlus:
    {
        m_serialControl->zoomIn();

        break;
    }
    case SerialBoard::Command_ZoomMinus:
    {
        m_serialControl->zoomOut();

        break;
    }
    case SerialBoard::Command_FocusPlus:
    {
        m_serialControl->focusFar();

        break;
    }
    case SerialBoard::Command_FocusMinus:
    {
        m_serialControl->focusNear();

        break;
    }
    case SerialBoard::Command_AutoFocus:
    {
        const bool newFocusMode =
                !m_serialControl->focusMode();

        m_serialControl->
                setFocusMode(newFocusMode);

        break;
    }
    }

    m_lastCommand = command;
    qCritical() << "m_lastCommand:" << m_lastCommand << endl;
}
