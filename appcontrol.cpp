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
    // Fill the serial port names
    foreach (const QSerialPortInfo& serialPortInfo, QSerialPortInfo::availablePorts()) {
        m_serialPortList.append(serialPortInfo.portName());
    }

    if (m_serialPortList.count() > 0)
        m_serialPortName = m_serialPortList[0];

    m_serialControl = new SerialControl;

#ifdef Q_OS_WIN32
    m_captureDevice = "0";
#endif

#ifdef Q_OS_LINUX
    _captureDevice = "/dev/video0";

#endif


    m_recordingLocation = QStandardPaths::MoviesLocation;

    m_videoCapture = new VideoCapture(m_captureDevice, QSize(FRAME_WIDTH, FRAME_HEIGHT));
    m_videoCapture->initialize();
    m_videoCapture->start();

    m_recordVisible = false;

    m_videoRecord = new VideoRecord(QSize(FRAME_WIDTH, FRAME_HEIGHT));


    m_videoAdapter = new VideoAdapter(QSize(FRAME_WIDTH, FRAME_HEIGHT));


    connect(m_videoCapture, &VideoCapture::sigFrameReady, m_videoAdapter, &VideoAdapter::onFrameReady);
    connect(m_videoCapture, &VideoCapture::sigFrameReady, m_videoRecord, &VideoRecord::pushFrame);
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
    switch (packet.command) {
    case Command_ZoomIn: {
        m_serialControl->setZoomSpeed(packet.value);
        m_serialControl->zoomIn();
        break;
    }
    case Command_ZoomOut: {
        m_serialControl->setZoomSpeed(packet.value);
        m_serialControl->zoomOut();
        break;
    }
    case Command_ZoomStop: {
        m_serialControl->zoomStop();
        break;
    }
    case Command_FocusFar: {
        m_serialControl->setFocusSpeed(packet.value);
        m_serialControl->focusFar();
        break;
    }
    case Command_FocusNear: {
        m_serialControl->setFocusSpeed(packet.value);
        m_serialControl->focusNear();
        break;
    }
    case Command_FocusStop: {
        m_serialControl->focusStop();
        break;
    }
    case Command_ToggleFocusMode: {
        if (packet.value == 0) {
            return;
        }

        bool previousFocusMode = m_serialControl->focusMode();
        m_serialControl->setFocusMode(!previousFocusMode);
        break;
    }
    case Command_PanLeft: {
        m_serialControl->setPanTiltSpeed(packet.value);
        m_serialControl->panLeft();
        break;
    }
    case Command_PanRight: {
        m_serialControl->setPanTiltSpeed(packet.value);
        m_serialControl->panRight();
        break;
    }
    case Command_PanStop: {
        m_serialControl->panStop();
        break;
    }
    case Command_TiltUp: {
        m_serialControl->setPanTiltSpeed(packet.value);
        m_serialControl->tiltUp();
        break;
    }
    case Command_TiltDown: {
        m_serialControl->setPanTiltSpeed(packet.value);
        m_serialControl->tiltDown();
        break;
    }
    case Command_TiltStop: {
        m_serialControl->tiltStop();
        break;
    }
    case Command_NextCamera: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->setNextCamera();
        break;
    }
    case Command_NextDefogLevel: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->setNextDefogMode();
        break;
    }
    case Command_NextGammaLevel: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->setNextGammaLevel();
        break;
    }
    case Command_NextNoiseReductionLevel: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->setNextNoiseReductionMode();
        break;
    }
    case Command_ToggleDigitalZoom: {
        if (packet.value == 0) {
            return;
        }

        bool previousDigitalZoomValue = m_serialControl->digitalZoom();

        m_serialControl->enableDigitalZoom(!previousDigitalZoomValue);
        break;
    }
    case Command_OpenCameraMenu: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->showMenuPressed();
        m_serialControl->showMenuReleased();
        break;
    }
    case Command_MenuItemUp: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->menuUpPressed();
        m_serialControl->menuUpReleased();
        break;
    }
    case Command_MenuItemDown: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->menuDownPressed();
        m_serialControl->menuDownReleased();
        break;
    }
    case Command_MenuItemLeft: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->menuLeftPressed();
        m_serialControl->menuLeftReleased();
        break;
    }
    case Command_MenuItemRight: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->menuRightPressed();
        m_serialControl->menuRightReleased();
        break;
    }
    case Command_CloseCameraMenu: {
        if (packet.value == 0) {
            return;
        }

        m_serialControl->menuESCReleased();
        break;
    }

    case Command_ShutdownSystem: {
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

QStringList AppControl::serialPortList() const
{
    qDebug() << "_serialPortList: " << m_serialPortList;
    return m_serialPortList;
}

SerialControl* AppControl::serialControl() const
{
    return m_serialControl;
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
