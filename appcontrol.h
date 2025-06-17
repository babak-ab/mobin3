#ifndef APPCONTROL_H
#define APPCONTROL_H

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080


#include <QAbstractItemModel>
#include <QElapsedTimer>
#include <QFont>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QGamepadManager>
#include <QGuiApplication>
#include <QObject>

#include "GamepadController.h"
#include "serialcontrol.h"
#include "reticle.h"
#include "videoadapter.h"
#include "videocapture.h"
#include "videorecord.h"
#include "SerialController.h"
#include "SerialBoard.h"

///
/// \brief The AppControl class
/// This class is responsible for controlling all the interaction inside the appication.
/// In fact, this class is the controller in the MVC software architecture.
class AppControl : public QObject {
    Q_OBJECT

    // The properties for notifying the QML of the changes in the code
    Q_PROPERTY(QObject* videoAdapter READ videoAdapter NOTIFY signalVideoAdapter)
    Q_PROPERTY(SerialControl* serialControl READ serialControl NOTIFY signalVideoAdapter)
    Q_PROPERTY(Reticle* reticle READ reticle NOTIFY signalVideoAdapter)
    Q_PROPERTY(QStringList serialPortList READ serialPortList NOTIFY signalVideoAdapter);
    Q_PROPERTY(QString messageTitle READ messageTitle NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(QString messageText READ messageText NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(bool recordVisible READ recordVisible WRITE setRecordVisible NOTIFY recordVisibleChanged)
    Q_PROPERTY(QString recordingLocation READ recordingLocation WRITE setRecordingLocation NOTIFY recordingLocationChanged)
    Q_PROPERTY(QString appVersion READ appVersion NOTIFY signalVideoAdapter)
    Q_PROPERTY(bool reticleVisible READ reticleVisible WRITE setReticleVisible NOTIFY reticleVisibleChanged)
    Q_PROPERTY(int lastSerialCommand READ lastSerialCommand WRITE setLastSerialCommand)
    Q_PROPERTY(bool boardSerialInboundState READ boardSerialInboundState NOTIFY sigSerialStateChanged)
    Q_PROPERTY(bool boardSerialOutboundState READ boardSerialOutboundState NOTIFY sigSerialStateChanged)

public:
    ///
    /// \brief AppControl
    /// The constructor.
    /// \param parent
    /// The parent.
    explicit AppControl(QObject* parent = Q_NULLPTR);

    ~AppControl();

    ///
    /// \brief messageTitle
    /// A function for returing a message title.
    /// \return
    /// The message title.
    QString messageTitle() const;

    ///
    /// \brief messageText
    /// A function for returing a message text.
    /// \return
    /// The message text.
    QString messageText() const;

    ///
    /// \brief serialPortList
    /// This function used for returning the list of serial ports detected at
    /// the beginning of the application.
    /// \return
    /// The dectected serial port lists, which is used in the UI (QML)
    QStringList serialPortList();

    ///
    /// \brief serialControl
    /// A function for getting the instance of the serial port controller
    /// \return
    ///
    SerialControl* serialControl() const;

    ///
    /// \brief reticle
    /// A function for getting the instance of the reticle controller
    /// \return
    ///
    Reticle* reticle() const;

    ///
    /// \brief setGamepadController
    /// A function for setting the gamepad controller.
    /// \param gamepadController
    /// The gamepad controller.
    void setGamepadController(GamepadController* gamepadController);

    ///
    /// \brief setGamepadManager
    /// A function for setting the gamepad manager.
    /// \param manager
    /// The gamepad manager.
    void setGamepadManager(QGamepadManager* manager);

    ///
    /// \brief videoAdapter
    /// A function for getting the instance of the video adapter object.
    /// \return
    /// The video adapter.
    VideoAdapter* videoAdapter() const;

    ///
    /// \brief setVideoAdapter
    /// A function for returing the video adapter.
    /// \param videoAdapter
    /// The video adapter.
    void setVideoAdapter(VideoAdapter* videoAdapter);

    ///
    /// \brief recodringLocation
    /// A function for getting the recording location.
    /// \return
    /// The recording location.
    QString recordingLocation() const;

    ///
    /// \brief setRecodringLocation
    /// A function for setting the recording location.
    /// \param recodringLocation
    /// The recording location.
    void setRecordingLocation(const QString& recordingLocation);

    ///
    /// \brief recordVisible
    /// A function for checking the the recording process is undergoing or not.
    /// \return
    /// The result.
    bool recordVisible() const;

    ///
    /// \brief setRecordVisible
    /// A function for enabling/disabling the recording state.
    /// \param recordVisible
    /// The enabling/disabling state.
    void setRecordVisible(bool recordVisible);

    ///
    /// \brief appVersion
    /// A function which returns the application version.
    /// \return
    /// The version.
    QString appVersion() const;

    ///
    /// \brief connectToSerialPort
    /// A funtion for getting connected to the serial port.
    /// \return
    /// The result of the connection.
    Q_INVOKABLE bool connectToSerialPort();

    ///
    /// \brief disconnectSerialPort
    /// A function for disconnecting from serial port.
    Q_INVOKABLE void disconnectSerialPort();

    ///
    /// \brief setSerialPortName
    /// A function for setting serial port name.
    /// \param portName
    /// The port name.
    Q_INVOKABLE void setSerialPortName(QString portName);

    ///
    /// \brief findSerialPortName
    /// An auxiliary function for finding the correct port name for further use.
    /// \param portName
    /// The port name.
    /// \return
    /// The index of the port name in the QList.
    Q_INVOKABLE int findSerialPortName(QString portName);

    ///
    /// \brief startRecord
    /// This function starts the recording.
    Q_INVOKABLE void startRecord();

    ///
    /// \brief stopRecord
    /// This finction stops the recording.
    Q_INVOKABLE void stopRecord();

    bool reticleVisible() const;
    Q_INVOKABLE void setReticleVisible(bool reticleVisible);

    Q_INVOKABLE int lastSerialCommand();

    Q_INVOKABLE void setLastSerialCommand(const int &lastCommand);

    Q_INVOKABLE void sendMouseEvent(QObject *object,
                                    const bool &isPressed);

    Q_INVOKABLE void takeSnapshot();

    Q_INVOKABLE bool boardSerialInboundState() const;
    Q_INVOKABLE bool boardSerialOutboundState() const;

private:
    QString m_recordingLocation;
    QString m_captureDevice;

    VideoCapture* m_videoCapture;
    VideoAdapter* m_videoAdapter;
    VideoRecord* m_videoRecord;

    SerialControl* m_serialControl;

    SerialController m_serialBoardController;
    SerialBoard m_serialBoard;

    Reticle* m_reticle;
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

    bool m_reticleVisible;

    bool m_boardSerialInboundState;
    bool m_boardSerialOutboundState;

    SerialBoard::Commands m_lastCommand;

    quint8 m_lastJoystickPanSpeed;
    quint8 m_lastJoystickPanDirection;
    quint8 m_lastJoystickTiltSpeed;
    quint8 m_lastJoystickTiltDirection;

    QElapsedTimer m_elapsedTimerTvCaptureWatchdog;
    QTimer m_timerTvWatchdog;

    const int m_resetSerialStateDelay;

    void processGamepadCommand(const CommandPacket& packet);
    void fillSerialPortNames();

    void changeBoardSerialInboundState(const bool &state);
    void changeBoardSerialOutboundState(const bool &state);

private Q_SLOTS:
    void sltExecuteCommandRequested(const CommandPacket& packet);
    void sltCheckTVCapture();
    void restartElapsedTimerRequested(const QByteArray &data);
    void sltNewDataReceived(const QByteArray &packet);

Q_SIGNALS:
    void signalVideoAdapter();
    void sigThrowSerialMessageRequested();
    void recordingLocationChanged();
    void recordVisibleChanged();
    void reticleVisibleChanged();
    void sigSerialBoardDataReceived(const int &command);
    void sigSerialStateChanged();
};

#endif // APPCONTROL_H
