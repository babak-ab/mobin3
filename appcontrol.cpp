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

    // Fill the serial port names
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        _serialPortList.append(serialPortInfo.portName());
    }

    if (_serialPortList.count() > 0)
        _serialPortName = _serialPortList[0];

    _serialControl = new SerialControl;

}

AppControl::~AppControl()
{
}

void AppControl::processGamepadCommand(const CommandPacket &packet)
{
    switch (packet.command)
    {
    case Command_ZoomIn:
    {
        _serialControl->setZoomSpeed(packet.value);
        _serialControl->zoomIn();
        break;
    }
    case Command_ZoomOut:
    {
        _serialControl->setZoomSpeed(packet.value);
        _serialControl->zoomOut();
        break;
    }
    case Command_ZoomStop:
    {
        _serialControl->zoomStop();
        break;
    }
    case Command_FocusFar:
    {
        _serialControl->setFocusSpeed(packet.value);
        _serialControl->focusFar();
        break;
    }
    case Command_FocusNear:
    {
        _serialControl->setFocusSpeed(packet.value);
        _serialControl->focusNear();
        break;
    }
    case Command_FocusStop:
    {
        _serialControl->focusStop();
        break;
    }
    case Command_PanLeft:
    {
        _serialControl->setPanTiltSpeed(packet.value);
        _serialControl->panLeft();
        break;
    }
    case Command_PanRight:
    {
        _serialControl->setPanTiltSpeed(packet.value);
        _serialControl->panRight();
        break;
    }
    case Command_PanStop:
    {
        _serialControl->panStop();
        break;
    }
    case Command_TiltUp:
    {
        _serialControl->setPanTiltSpeed(packet.value);
        _serialControl->tiltUp();
        break;
    }
    case Command_TiltDown:
    {
        _serialControl->setPanTiltSpeed(packet.value);
        _serialControl->tiltDown();
        break;
    }
    case Command_TiltStop:
    {
        _serialControl->tiltStop();
        break;
    }
    default:
        return;
    }
}

void AppControl::setSerialPortName(QString portName)
{
    _serialPortName = portName;

    qDebug() << "_serialPortName: " << _serialPortName;
}

int AppControl::findSerialPortName(QString portName)
{
    for(int i = 0;i<_serialPortList.size();i++){
        qDebug() << "port name: " << _serialPortList[i] << portName;
        if(_serialPortList[i].contains(portName)){
            return i;
        }
    }

    return 0;
}

QString AppControl::messageTitle() const
{
    return _messageTitle;
}

QString AppControl::messageText() const
{
    return _messageText;
}

bool AppControl::connectToSerialPort()
{
    bool isConnected = _serialControl->connectToSerialPort(_serialPortName);

    if (isConnected) {
         return true;
    }

    _messageTitle = "Serial Port Error";
    _messageText = "Could not connect to " + _serialPortName + " port!";
    Q_EMIT sigThrowSerialMessageRequested();

    return false;
}

void AppControl::disconnectSerialPort()
{
    bool result = _serialControl->disconnectSerialPort();

    if (!result) {
         _messageTitle = "Serial Port Error";
         _messageText = "Could not get disconnected from " + _serialPortName + " port!";
         Q_EMIT sigThrowSerialMessageRequested();
    }
}

QStringList AppControl::serialPortList() const
{
    qDebug() << "_serialPortList: " << _serialPortList;
    return _serialPortList;
}

SerialControl *AppControl::serialControl() const
{
    return _serialControl;
}

void AppControl::setGamepadController(GamepadController *gamepadController)
{
    m_gamepadController = gamepadController;

    connect(m_gamepadController, &GamepadController::sigExecuteCommandRequested,
            this, &AppControl::sltExecuteCommandRequested);
}

void AppControl::sltExecuteCommandRequested(const CommandPacket &packet)
{
    qDebug() << " --------------------------------- gamepad " << packet.command << packet.value;
    processGamepadCommand(packet);
}
