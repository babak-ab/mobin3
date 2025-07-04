#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <QObject>
#include <QSerialPort>
#include <QtMath>
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>

#include <iostream>

#include "utils/cursor.h"
#include "utils/ringqueue.h"

using namespace Utils;

///
/// \brief model part in MVC princible
///
/// model in mvc principle, this class stores external modules objects and connect
/// modules to each other in order to have modular project.
/// most importance responsibility of this clss is creating oghab command packet and
/// controling of mobin platform.
///

class SerialControl : public RingQueue
{
    Q_OBJECT

    // The properties used in the UI (QML) layer.
    Q_PROPERTY(quint8 panTiltSpeed READ panTiltSpeed WRITE setPanTiltSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 zoomSpeed READ zoomSpeed WRITE setZoomSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 focusSpeed READ focusSpeed WRITE setFocusSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint16 focusPosition READ focusPosition NOTIFY sigDataChanged)
    Q_PROPERTY(quint16 fovPosition READ fovPosition NOTIFY sigDataChanged)
    Q_PROPERTY(DefogMode defogMode READ defogMode WRITE setDefogMode NOTIFY sigDataChanged)
    Q_PROPERTY(GammaLevel gammaLevel READ gammaLevel WRITE setGammaLevel NOTIFY sigDataChanged)
    Q_PROPERTY(NoiseReductionMode noiseReductionMode READ noiseReductionMode WRITE setNoiseReductionMode NOTIFY sigDataChanged)
    Q_PROPERTY(bool digitalZoom READ digitalZoom WRITE enableDigitalZoom NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 illuminatorBrightnessLevel READ illuminatorBrightnessLevel WRITE setIlluminatorBrightness NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 illuminatorAngleOffset READ illuminatorAngleOffset WRITE setIlluminatorAngleOffset NOTIFY sigDataChanged)
    Q_PROPERTY(bool illuminator READ illuminator WRITE enableIlluminator NOTIFY sigDataChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY sigDataChanged)
    Q_PROPERTY(bool focusMode READ focusMode WRITE setFocusMode NOTIFY sigDataChanged)
    Q_PROPERTY(CameraSelection selectedCamera READ selectedCamera WRITE setSelectedCamera NOTIFY sigDataChanged)
    Q_PROPERTY(FilterModes selectedFilter READ selectedFilter WRITE setSelectedFilter NOTIFY sigDataChanged)
    Q_PROPERTY(ContrastLevel contrastLevel READ contrastLevel WRITE setContrastMode NOTIFY sigDataChanged)
    Q_PROPERTY(BrightnessLevel brightnessLevel READ brightnessLevel WRITE setBrightnessLevel NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 mode READ mode WRITE setMode NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 sendingMode READ sendingMode NOTIFY sigDataChanged)
    Q_PROPERTY(quint16 continuousModeInterval READ continuousModeInterval WRITE setContinuousSendingMode NOTIFY sigDataChanged)
    Q_PROPERTY(QString messageBox READ messageBox NOTIFY sigMessageBoxRequested)
    Q_PROPERTY(bool showLoginWindow READ showLoginWindow NOTIFY sigLoginWindowRequested)
    Q_PROPERTY(float roll READ roll NOTIFY sigDataChanged)
    Q_PROPERTY(float pitch READ pitch NOTIFY sigDataChanged)

    // For updating labels
    Q_PROPERTY(QVariant noiseReductionType READ getNoiseReductionType NOTIFY sigDataChanged)
    Q_PROPERTY(QVariant defogType READ getDefogType NOTIFY sigDataChanged)
    Q_PROPERTY(QVariant gammaType READ getGammaType NOTIFY sigDataChanged)
    Q_PROPERTY(QVariant filterType READ getFilterType NOTIFY sigDataChanged)
	Q_PROPERTY(quint8 boardVersion READ boardVersion NOTIFY sigDataChanged)
    Q_PROPERTY(QString sensor READ sensor NOTIFY sigDataChanged)
    Q_PROPERTY(QString imageType READ imageType NOTIFY sigDataChanged)
    Q_PROPERTY(bool platformSerialInboundState READ platformSerialInboundState NOTIFY sigSerialStateChanged)
    Q_PROPERTY(bool platformSerialOutboundState READ platformSerialOutboundState NOTIFY sigSerialStateChanged)

public:

    ///
    /// \brief The CameraSelection enum
    /// An enum for selecting between the camera types.
    enum CameraSelection {
        CameraSelection_Unknown         = 0x00,
        CameraSelection_ContinuousZoom  = 0x01,
        CameraSelection_Spotter         = 0x02,
        CameraSelection_SWIRSpotter     = 0x03
    };
    Q_ENUM(CameraSelection)

    ///
    /// \brief The GammaLevel enum
    /// An enum for selecting between gamma levels
    enum GammaLevel {
        Level1                          = 0x01,
        Level2                          = 0x02,
    };
    Q_ENUM(GammaLevel)

    ///
    /// \brief The NoiseReductionMode enum
    /// An enum for selecting between noise reduction levels
    enum NoiseReductionMode {
        NR_Low                             = 0x01,
        NR_High                            = 0x02,
    };
    Q_ENUM(NoiseReductionMode)

    ///
    /// \brief The FocusModes enum
    /// An enum for switching between focus mode
    enum FocusModes
    {
        FocusMode_Unknown               = -1,
        FocusMode_Manual                = 0x01,
        FocusMode_Auto                  = 0x02
    };
    Q_ENUM(FocusModes)

    ///
    /// \brief The SendingModes enum
    /// An enum for switching between sending modes
    enum SendingModes
    {
        SendingMode_Unknown             = -1,
        SendingMode_Continouse          = 0x01,
        SendingMode_Request             = 0x02
    };
    Q_ENUM(SendingModes)

    ///
    /// \brief The FilterModes enum
    /// An enum for switching between filters
    enum FilterModes
    {
        Unknown                         = -1,
        Fault                           = 0x00,
        Color                           = 0x01,
        NIR                             = 0x02,
        F1064                           = 0x03
    };
    Q_ENUM(FilterModes)

    ///
    /// \brief The DefogMode enum
    /// An enum for switching between defogging modes
    enum DefogMode {
        D_Off                   = 0x00,
        D_Low                   = 0x01,
        D_Medium                = 0x02,
        D_High                  = 0x03
    };
    Q_ENUM(DefogMode)

    ///
    /// \brief The SensorValues enum
    /// An enum for labeling of returned sensor types
    enum SensorValues
    {
        SensorValue_Unknown             = -1,
        SensorValue_Farabin3            = 0x01,
        SensorValue_Farabin5            = 0x02,
        SensorValue_Farabin5B           = 0x03,
        SensorValue_Farabin9            = 0x04,
        SensorValue_Farabin18           = 0x05,
        SensorValue_Farabin15          = 0x06,
        SensorValue_Farabin15i3        = 0x07,
    };
    Q_ENUM(SensorValues)

    ///
    /// \brief The ContrastLevel enum
    /// An enum for switching between sending modes
    enum ContrastLevel {
        ContrastLevel_Level1            = 0x01,
        ContrastLevel_Level2            = 0x02,
        ContrastLevel_Level3            = 0x03
    };
    Q_ENUM(ContrastLevel)

    ///
    /// \brief The BrightnessLevel enum
    /// An enum for switching between brightness levels
    enum BrightnessLevel {
        BrightnessLevel_Level1          = 0x00,
        BrightnessLevel_Level2          = 0x01,
        BrightnessLevel_Level3          = 0x02,
        BrightnessLevel_Level4          = 0x03
    };
    Q_ENUM(BrightnessLevel)

    ///
    /// \brief The ModeLevels enum
    /// An enum for switching between mode levels
    enum ModeLevels
    {
        ModeLevel_Unknown               = -1,
        ModeLevel_Level1                = 0x01,
        ModeLevel_Level2                = 0x02,
        ModeLevel_Level3                = 0x03,
        ModeLevel_Level4                = 0x04,
        ModeLevel_Level5                = 0x05
    };
    Q_ENUM(ModeLevels)
	
	enum ImageType {
        ImageType_Unknown                            = -1,
        ImageType_Normal                             = 0x01,
        ImageType_LicensePlateRecognition1           = 0x02,
        ImageType_LicensePlateRecognition2           = 0x03
    };
    Q_ENUM(ImageType)

    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE void zoomStop();
    Q_INVOKABLE void focusFar();
    Q_INVOKABLE void focusNear();
    Q_INVOKABLE void focusStop();
    Q_INVOKABLE void setFocusMode(const bool mode);
    Q_INVOKABLE void setZoomSpeed(const quint8 speed);
    Q_INVOKABLE void setFocusSpeed(const quint8 speed);
    Q_INVOKABLE void gotoFov(const quint16 position);
    Q_INVOKABLE void gotoFocus(const quint16 position);
    Q_INVOKABLE void setPosition(const quint8 positionNumber);
    Q_INVOKABLE void clearPosition(const quint8 positionNumber);
    Q_INVOKABLE void gotoPosition(const quint8 positionNumber);
    Q_INVOKABLE void tiltUp();
    Q_INVOKABLE void tiltDown();
    Q_INVOKABLE void tiltStop();
    Q_INVOKABLE void panLeft();
    Q_INVOKABLE void panRight();
    Q_INVOKABLE void panStop();
    Q_INVOKABLE void setPanTiltSpeed(const quint8 speed);
    Q_INVOKABLE void setNextCamera();
    Q_INVOKABLE void setSelectedCamera(const CameraSelection camera);
    Q_INVOKABLE void setNextFilter();
    Q_INVOKABLE void setSelectedFilter(const FilterModes filter);
    Q_INVOKABLE void setNextDefogMode();
    Q_INVOKABLE void setDefogMode(const DefogMode mode);
    Q_INVOKABLE void setNextGammaLevel();
    Q_INVOKABLE void setGammaLevel(const GammaLevel level);
    Q_INVOKABLE void setNextNoiseReductionMode();
    Q_INVOKABLE void setNoiseReductionMode(const NoiseReductionMode mode);
    Q_INVOKABLE void enableDigitalZoom(const bool state);
    Q_INVOKABLE void enableFovUpdate(const bool state);
    Q_INVOKABLE void enableFocusUpdate(const bool state);
    Q_INVOKABLE void setContrastMode(const quint8 level);
    Q_INVOKABLE void setBrightnessLevel(const quint8 level);
    Q_INVOKABLE void setMode(const quint8 mode);
    Q_INVOKABLE void toggleMenu();
    Q_INVOKABLE void showMenuPressedRequested();
    Q_INVOKABLE void showMenuPressed();
    Q_INVOKABLE void showMenuReleased();
    Q_INVOKABLE void menuUpPressed();
    Q_INVOKABLE void menuUpReleased();
    Q_INVOKABLE void menuDownPressed();
    Q_INVOKABLE void menuDownReleased();
    Q_INVOKABLE void menuLeftPressed();
    Q_INVOKABLE void menuLeftReleased();
    Q_INVOKABLE void menuRightPressed();
    Q_INVOKABLE void menuRightReleased();
    Q_INVOKABLE void menuESCReleased();
    Q_INVOKABLE void enableIlluminator(const bool state);
    Q_INVOKABLE void toggleIlluminator();
    Q_INVOKABLE void setIlluminatorBrightness(const quint8 brightness);
    Q_INVOKABLE void setIlluminatorSmallerAngle();
    Q_INVOKABLE void setIlluminatorLargerAngle();
    Q_INVOKABLE void setIlluminatorAngleOffset(const quint8 offset);
    Q_INVOKABLE void setRequestSendingMode();
    Q_INVOKABLE void setStatusSendingMode();
    Q_INVOKABLE void setContinuousSendingMode(const quint8 interval);
    Q_INVOKABLE QString serialportName() const;
    Q_INVOKABLE void login();
    Q_INVOKABLE void setImageType(const quint8 type);
    Q_INVOKABLE void bootLoader();
    Q_INVOKABLE bool platformSerialInboundState() const;
    Q_INVOKABLE bool platformSerialOutboundState() const;

    ///
    /// \brief SerialControl default constructor
    ///
    /// \param parent
    /// pointer to class object which creates an instance of SerialControl.
    ///
    explicit SerialControl(QObject* parent = Q_NULLPTR);

    virtual ~SerialControl();

    ///
    /// \brief connect to oghab's serial port
    ///
    /// this method will connect simorgh to oghab's particular serial port.
    ///
    /// \return
    /// true if connection established
    /// false for other states.
    ///
    /// @see disconnectSerialPort()
    ///
    bool connectToSerialPort(const QString &portName);

    ///
    /// \brief disconnect from serial port
    ///
    /// this method will disconnect the serial port.
    /// \return
    /// true if connection removes successfully
    /// otherwise false.
    ///
    /// @see connectToSerialPort()
    ///
    bool disconnectSerialPort();

    ///
    /// \brief interpret
    /// Interprets the received data
    /// \param queueRead
    /// \return
    /// The received data
    ///
    QByteArray interpret(IRQueue<quint8>* queueRead);

    ///
    /// \brief isConnected
    /// Gets the current connection state of the serial port
    /// \return
    /// The result
    bool isConnected();

    ///
    /// \brief zoomSpeed
    /// Resturns the current zoom speed.
    /// \return
    /// The current zoom speed
    quint8 zoomSpeed() const;

    ///
    /// \brief panTiltSpeed
    /// Returns the current pan/tilt speed.
    /// \return
    /// The speed.
    quint8 panTiltSpeed() const;

    ///
    /// \brief focusSpeed
    /// Returns the focus speed.
    /// \return
    /// The speed.
    quint8 focusSpeed() const;

    ///
    /// \brief focusPosition
    /// Returns the focus position.
    /// \return
    /// The position.
    quint16 focusPosition() const;

    ///
    /// \brief fovPosition
    /// Returns the FOV position.
    /// \return
    ///
    quint16 fovPosition() const;

    ///
    /// \brief illuminatorBrightnessLevel
    /// Returns the brightness of the illumiantor.
    /// \return
    /// The brightness value.
    quint8 illuminatorBrightnessLevel() const;

    ///
    /// \brief illuminatorAngleOffset
    /// Returns the ratio of the angle offset of the illuminator.
    /// \return
    /// The value for the offset angle of the illumiantor.
    quint8 illuminatorAngleOffset() const;

    ///
    /// \brief defogMode
    /// Returns the current defog mode.
    /// \return
    /// The mode.
    DefogMode defogMode() const;

    ///
    /// \brief gammaLevel
    /// Returns the gamma level.
    /// \return
    /// The value of the gamma level.
    GammaLevel gammaLevel() const;

    ///
    /// \brief noiseReductionMode
    /// Returns the current noise reduction mode.
    /// \return
    /// The current noise reduction mode.
    NoiseReductionMode noiseReductionMode() const;

    ///
    /// \brief digitalZoom
    /// Returns that the digital zoom is on or not.
    /// \return
    /// The result.
    bool digitalZoom() const;

    ///
    /// \brief illuminator
    /// Returns that the illumiantor is on or not.
    /// \return
    /// The result.
    bool illuminator() const;

    ///
    /// \brief focusMode
    /// Returns the focus mode. TRUE means the focus mode is AUTO,
    /// and FALSE means MANUAL focus mode.
    /// \return
    /// The result.
    bool focusMode() const;

    ///
    /// \brief selectedFilter
    /// Returns the current filter type.
    /// \return
    /// The value for the filter type.
    FilterModes selectedFilter() const;

    ///
    /// \brief selectedCamera
    /// Returns the current camera type.
    /// \return
    /// The camera type.
    CameraSelection selectedCamera() const;

    ///
    /// \brief contrastLevel
    /// Returns the current contrast level.
    /// \return
    /// The level.
    ContrastLevel contrastLevel() const;
	
    ///
    /// \brief imageType
    /// \return
    ///
    QString imageType() const;

    ///
    /// \brief brightnessLevel
    /// Returns the current brightness level.
    /// \return
    /// The level.
    BrightnessLevel brightnessLevel() const;

    ///
    /// \brief mode
    /// Returns the current mode.
    /// \return
    /// The mode.
    quint8 mode() const;

    ///
    /// \brief boardVersion
    /// Returns the current board version.
    /// \return
    /// A number indicating board version.
    quint8 boardVersion() const;

    ///
    /// \brief sensor
    ///  Returns sensor type.
    /// \return
    /// The name of the sensor.
    QString sensor() const;

    ///
    /// \brief sendingMode
    /// Returns the current sending mode.
    /// \return
    /// The mode.
    quint8 sendingMode() const;

    ///
    /// \brief continuousModeInterval
    /// Returns the current interval of the continuous mode.
    /// \return
    /// The interval.
    quint8 continuousModeInterval() const;

    ///
    /// \brief messageBox
    /// Returns the current text of the messageBox.
    /// \return
    /// The text.
    QString messageBox() const;

    ///
    /// \brief showLoginWindow
    /// Returns the permision for showing the login window
    /// for camera menu. This permision will be verified only for the first time.
    /// \return
    /// The permision result.
    bool showLoginWindow() const;

    ///
    /// \brief roll
    /// Returns the roll angle.
    /// \return
    /// The roll value.
    float roll() const;

    ///
    /// \brief pitch
    /// Returns the pitch value.
    /// \return
    /// The pitch value.
    float pitch() const;

    ///
    /// \brief getNoiseReductionType
    /// Returns the text for current noise reduction type.
    /// \return
    /// The text.
    QVariant getNoiseReductionType();

    ///
    /// \brief getDefogType
    /// Returns the text for current defog type.
    /// \return
    /// The text.
    QVariant getDefogType();

    ///
    /// \brief getGammaType
    /// Returns the text for current gamma type.
    /// \return
    /// The text.
    QVariant getGammaType();

    ///
    /// \brief getFilterType
    /// Returns the text for current for filter type.
    /// \return
    /// The text.
    QVariant getFilterType();

    ///
    /// \brief joyStickPanTilt
    /// A function for sending the pan and tilt direction and speed simultaneously
    /// for both motors of the pan and tilt. This function is applicable while using a joystick
    /// or gamepad.
    /// \param panDirection
    /// The direction of the pan.
    /// \param panSpeed
    /// The desired speed for pan.
    /// \param tiltDirection
    /// The direction of the tilt.
    /// \param tiltSpeed
    /// The desired speed for tilt.
    void joyStickPanTilt(const quint8& panDirection, const quint8& panSpeed,
                         const quint8& tiltDirection, const quint8& tiltSpeed);

private:

    ///
    /// \brief enumerator which stores all attributes require for connecting to
    /// a serial port
    ///
    /// by help of this structure, we can transport desire serial port's information
    /// from two classes.
    ///
    struct SerialPortAttributes
    {
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };

    QSerialPort *m_serialPort;

    quint16 m_fovPosition;
    quint16 m_focusPosition;
    quint8 m_zoomSpeed;
    quint8 m_focusSpeed;
    quint8 m_panTiltSpeed;
    int m_repeatCounter;
    bool m_focusMode;
    QString m_message;

    float m_roll;
    float m_pitch;

    CameraSelection m_selectedCamera;

    GammaLevel m_gammaLevel;
    NoiseReductionMode m_noiseReductionMode;
    DefogMode m_defogMode;
    SendingModes m_sendingMode; // initialize in constructor
    FilterModes m_filterMode; // initialize
    SensorValues m_sensorValue; // initialize
    ContrastLevel m_contrastLevel;
    BrightnessLevel m_brightnessLevel;
	ImageType m_imageType;
    quint8 m_mode;
    quint8 m_illuminatorBrightness;
    quint8 m_illuminatorAngleOffset;
    quint8 m_crc8_table[256];
    quint8 m_boardVersion; // initialize
    quint16 m_continuousModeInterval;

    bool m_isDigitalZoomEnabled;
    bool m_isIlluminatorEnabled;
    bool m_isUpdatingFovEnabled;
    bool m_isUpdatingFocusEnabled;

    bool m_showLoginWindow;
    bool m_isPasswordReceived;

    bool m_isMenuOpened;

    bool m_platformSerialInboundState;
    bool m_platformSerialOutboundState;

    const int m_resetSerialStateDelay;

    // Private Functions
    void writeDataOnPlatformsSerialPort(const QByteArray &data);
    int bytesToInt(QByteArray data, int start, int length, bool reverse = false);
    void sendCommand1(const quint8 &command, const quint8 &param);
    void sendCommand2(const quint8 &command, const quint16 &param);
    void sendCommand3(const quint8& command,
                      const quint8& param1, const quint8& param2,
                      const quint8& param3, const quint8& param4);
    void sendCommand4(const quint8 &command);

    void autoFocus();
    void manualFocus();

    void init_crc8();
    quint8 crc8(quint8 buf[], quint8 len) const;

    void changePlatformSerialInboundState(const bool &state);
    void changePlatformSerialOutboundState(const bool &state);

private Q_SLOTS:
    void sltReadData(QByteArray data);
    void sltReadSeialPortData();
    void sltWriteSerialPortData(QByteArray data);
    void sltSetCameraMode();
    void sltSetFilterMode();

Q_SIGNALS:
    /* platform property event signals */
    void sigWriteData(QByteArray data);
    void sigDataChanged();
    void sigMessageBoxRequested();
    void sigLoginWindowRequested();
    void sigSerialStateChanged();
};


#endif // SERIALCONTROL_H
