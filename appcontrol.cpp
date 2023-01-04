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

    _buttonModel = new ButtonModel;
    _buttonModel->addButton(new Button("Button1", QPoint(500, 5)));
    _buttonModel->addButton(new Button("Button2", QPoint(900, 5)));
    _buttonModel->addButton(new Button("Button3", QPoint(1300, 5)));


    // Fill the serial port names
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        _serialPortList.append(serialPortInfo.portName());
        qDebug() << serialPortInfo.portName();
    }

    qDebug() << "XXXXXX count: " << QSerialPortInfo::availablePorts().count();

    _serialPortName = "COM1";

}

AppControl::~AppControl()
{
}

ButtonModel* AppControl::buttonModel() const
{
    return _buttonModel;
}

void AppControl::setButtonModel(ButtonModel* buttonModel)
{
    _buttonModel = buttonModel;
}

void AppControl::setSerialPortName(QString portName)
{
    _serialPortName = portName;
}

QString AppControl::messageTitle() const
{
    return _messageTitle;
}

QString AppControl::messageText() const
{
    return _messageText;
}

int AppControl::connectToSerialPort()
{
        qDebug() << "XXXXXXXXXXXX 1";

    bool isConnected = _serialControl.connectToSerialPort(_serialPortName);

    if (isConnected) {
         return _serialPortList.indexOf(QRegExp(_serialPortName, Qt::CaseInsensitive, QRegExp::W3CXmlSchema11));
    }

    _messageTitle = "Serial Port Error";
    _messageText = "Could not connect to " + _serialPortName + " port!";
    Q_EMIT sigThrowSerialMessageRequested();

    return 0;
}

void AppControl::disconnectSerialPort()
{
    bool result = _serialControl.disconnectSerialPort();

    if (!result) {
         _messageTitle = "Serial Port Error";
         _messageText = "Could not get disconnected from " + _serialPortName + " port!";
         Q_EMIT sigThrowSerialMessageRequested();
    }
}

QStringList AppControl::serialPort() const
{

    qDebug() << "XXXXXXXXXXXX 0";

    return _serialPortList;
}
