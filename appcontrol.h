#ifndef APPCONTROL_H
#define APPCONTROL_H

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
#define SOFTWARE_VERSION "V. 1.3.0"

#include <QAbstractItemModel>
#include <QElapsedTimer>
#include <QFont>
#include <QFontMetrics>
#include <QGamepadManager>
#include <QGuiApplication>
#include <QObject>

#include "GamepadController.h"
#include "buttonmodel.h"
#include "serialcontrol.h"
#include "videoadapter.h"
#include "videocapture.h"
#include "videorecord.h"

class AppControl : public QObject {
    Q_OBJECT

    Q_PROPERTY(QObject* videoAdapter READ videoAdapter NOTIFY signalVideoAdapter)

    Q_PROPERTY(SerialControl* serialControl READ serialControl NOTIFY signalVideoAdapter)
    Q_PROPERTY(QStringList serialPortList READ serialPortList NOTIFY signalVideoAdapter);
    Q_PROPERTY(QString messageTitle READ messageTitle NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(QString messageText READ messageText NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(bool recordVisible READ recordVisible WRITE setRecordVisible NOTIFY recordVisibleChanged)
    Q_PROPERTY(QString recordingLocation READ recordingLocation WRITE setRecordingLocation NOTIFY recordingLocationChanged)
    Q_PROPERTY(QString appVersion READ appVersion NOTIFY signalVideoAdapter)

public:
    explicit AppControl(QObject* parent = Q_NULLPTR);
    ~AppControl();

private:
    QString m_recordingLocation;
    QString m_captureDevice;
    VideoCapture* m_videoCapture;
    VideoAdapter* m_videoAdapter;
    VideoRecord* m_videoRecord;

    SerialControl* m_serialControl;
    GamepadController* m_gamepadController;

    QGamepadManager* m_gamepadManager;

    QTimer m_toggleIlluminatorTimer;
    QTimer m_shutDownSystemTimer;

    QStringList m_serialPortList;

    QString m_serialPortName;

    QString m_messageTitle;
    QString m_messageText;

    bool m_isSerialPortOpened;

    bool m_recordVisible;

    quint8 m_lastJoystickPanSpeed;
    quint8 m_lastJoystickPanDirection;
    quint8 m_lastJoystickTiltSpeed;
    quint8 m_lastJoystickTiltDirection;
    void processGamepadCommand(const CommandPacket& packet);

public:

    Q_INVOKABLE bool connectToSerialPort();
    Q_INVOKABLE void disconnectSerialPort();
    Q_INVOKABLE void setSerialPortName(QString portName);
    Q_INVOKABLE int findSerialPortName(QString portName);

    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();

    QString messageTitle() const;
    QString messageText() const;

    QStringList serialPortList() const;

    SerialControl* serialControl() const;

    void setGamepadController(GamepadController* gamepadController);
    void setGamepadManager(QGamepadManager* manager);

    VideoAdapter* videoAdapter() const;
    void setVideoAdapter(VideoAdapter* videoAdapter);

    QString recodringLocation() const;
    void setRecodringLocation(const QString& recodringLocation);

    QString recordingLocation() const;
    void setRecordingLocation(const QString& recordingLocation);

    bool recordVisible() const;
    void setRecordVisible(bool recordVisible);

    QString appVersion() const;

private Q_SLOTS:
    void sltExecuteCommandRequested(const CommandPacket& packet);

Q_SIGNALS:
    void signalVideoAdapter();
    void sigThrowSerialMessageRequested();
    void recordingLocationChanged();
    void recordVisibleChanged();
};

#endif // APPCONTROL_H
