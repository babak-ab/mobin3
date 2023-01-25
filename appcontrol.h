#ifndef APPCONTROL_H
#define APPCONTROL_H

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
#define SOFTWARE_VERSION "0.9.0"

#include <QAbstractItemModel>
#include <QElapsedTimer>
#include <QFont>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QObject>

#include "buttonmodel.h"
#include "serialcontrol.h"
#include "GamepadController.h"

class AppControl : public QObject {
    Q_OBJECT

    Q_PROPERTY(SerialControl* serialControl READ serialControl NOTIFY signalVideoAdapter)
    Q_PROPERTY(QStringList serialPortList READ serialPortList NOTIFY signalVideoAdapter);
    Q_PROPERTY(QString messageTitle READ messageTitle NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(QString messageText READ messageText NOTIFY sigThrowSerialMessageRequested);

public:
    explicit AppControl(QObject* parent = Q_NULLPTR);
    ~AppControl();

private:
//    ButtonModel* _buttonModel;

    SerialControl *_serialControl;
    GamepadController m_gamepad;

    bool _isSerialPortOpened;
    QStringList _serialPortList;

    QString _serialPortName;

    QString _messageTitle;
    QString _messageText;
public:

    Q_INVOKABLE bool connectToSerialPort();
    Q_INVOKABLE void disconnectSerialPort();
    Q_INVOKABLE void setSerialPortName(QString portName);
    Q_INVOKABLE int findSerialPortName(QString portName);

    QString messageTitle() const;
    QString messageText() const;

    QStringList serialPortList() const;

    SerialControl* serialControl() const;

private Q_SLOTS:

Q_SIGNALS:
    void signalVideoAdapter();
    void sigThrowSerialMessageRequested();
};

#endif // APPCONTROL_H
