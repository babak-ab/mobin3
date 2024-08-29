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
    : QObject(parent)
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

    m_recordingLocation = QStandardPaths::MoviesLocation;

    m_videoCapture = new VideoCapture(m_captureDevice, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    m_videoCapture->initialize();
    m_videoCapture->start();

    m_recordVisible = false;

    m_reticleVisible = true;

    m_videoRecord = new VideoRecord(QSize(FRAME_WIDTH, FRAME_HEIGHT));

    m_videoAdapter = new VideoAdapter(QSize(FRAME_WIDTH, FRAME_HEIGHT));

    connect(m_videoCapture, &VideoCapture::sigFrameReady, m_videoAdapter, &VideoAdapter::onFrameReady);
    connect(m_videoCapture, &VideoCapture::sigFrameReady, this, &AppControl::restartElapsedTimerRequested);
    connect(m_videoCapture, &VideoCapture::sigFrameReady, m_videoRecord, &VideoRecord::pushFrame);

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
