#ifndef APPCONTROL_H
#define APPCONTROL_H

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
#define SOFTWARE_VERSION "V. 1.5.1_1"

#include <QAbstractItemModel>
#include <QElapsedTimer>
#include <QFont>
#include <QFontMetrics>
#include <QGamepadManager>
#include <QGuiApplication>
#include <QObject>

#include "GamepadController.h"
#include "serialcontrol.h"
#include "videoadapter.h"
#include "videocapture.h"
#include "videorecord.h"

///
/// \brief The AppControl class
/// This class is responsible for controlling all the interaction inside the appication.
/// In fact, this class is the controller in the MVC software architecture.
class AppControl : public QObject {
    Q_OBJECT

    // The properties for notifying the QML of the changes in the code
    Q_PROPERTY(QObject* videoAdapter READ videoAdapter NOTIFY signalVideoAdapter)
    Q_PROPERTY(SerialControl* serialControl READ serialControl NOTIFY signalVideoAdapter)
    Q_PROPERTY(QStringList serialPortList READ serialPortList NOTIFY signalVideoAdapter);
    Q_PROPERTY(QString messageTitle READ messageTitle NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(QString messageText READ messageText NOTIFY sigThrowSerialMessageRequested);
    Q_PROPERTY(bool recordVisible READ recordVisible WRITE setRecordVisible NOTIFY recordVisibleChanged)
    Q_PROPERTY(QString recordingLocation READ recordingLocation WRITE setRecordingLocation NOTIFY recordingLocationChanged)
    Q_PROPERTY(QString appVersion READ appVersion NOTIFY signalVideoAdapter)


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
    QStringList serialPortList() const;

    ///
    /// \brief serialControl
    /// A function for getting the instance of the serial port controller
    /// \return
    ///
    SerialControl* serialControl() const;

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

private Q_SLOTS:
    void sltExecuteCommandRequested(const CommandPacket& packet);

Q_SIGNALS:
    void signalVideoAdapter();
    void sigThrowSerialMessageRequested();
    void recordingLocationChanged();
    void recordVisibleChanged();
};

#endif // APPCONTROL_H
