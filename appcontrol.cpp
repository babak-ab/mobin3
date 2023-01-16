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
