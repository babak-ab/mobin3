#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <QObject>
#include <QSerialPort>
#include <QtMath>
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>

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

    Q_PROPERTY(quint8 zoomSpeed READ zoomSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 panTiltSpeed READ panTiltSpeed WRITE setPanTiltSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 zoomSpeed READ zoomSpeed WRITE setZoomSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint8 focusSpeed READ focusSpeed WRITE setFocusSpeed NOTIFY sigDataChanged)
    Q_PROPERTY(quint16 focusPosition READ focusPosition WRITE setFocusPosition NOTIFY sigDataChanged)
    Q_PROPERTY(quint16 fovPosition READ fovPosition WRITE setFovPosition NOTIFY sigDataChanged)
    Q_PROPERTY(DefogMode defogMode READ defogMode WRITE setDefogMode NOTIFY sigDataChanged)
    Q_PROPERTY(GammaLevel gammaLevel READ gammaLevel WRITE setGammaLevel NOTIFY sigDataChanged)
    Q_PROPERTY(bool digitalZoom READ digitalZoom WRITE enableDigitalZoom NOTIFY sigDataChanged)

public:

    enum CameraSelection {
        CameraSelection_ContinuousZoom = 1,
        CameraSelection_Spotter = 2,
        CameraSelection_SWIRSpotter = 3
    };
    Q_ENUM(CameraSelection)

    enum FilterSelection {
        FilterSelection_ColorFilter = 0,
        FilterSelection_NIRFilter = 1,
        FilterSelection_LaserFilter = 2
    };
    Q_ENUM(FilterSelection)

    enum DefogMode {
        DefogMode_Off = 0,
        DefogMode_Low = 1,
        DefogMode_Medium = 2,
        DefogMode_High = 3
    };
    Q_ENUM(DefogMode)

    enum GammaLevel {
        GammaLevel_Level1 = 1,
        GammaLevel_Level2 = 2,
    };
    Q_ENUM(GammaLevel)

    enum NoiseReductionMode {
        NoiseReductionMode_Low = 1,
        NoiseReductionMode_High = 2,
    };
    Q_ENUM(NoiseReductionMode)

    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE void zoomStop();
    Q_INVOKABLE void focusFar();
    Q_INVOKABLE void focusNear();
    Q_INVOKABLE void focusStop();
    Q_INVOKABLE void autoFocus();
    Q_INVOKABLE void manualFocus();
    Q_INVOKABLE void setZoomSpeed(const quint8 speed);
    Q_INVOKABLE void setFocusSpeed(const quint8 speed);
    Q_INVOKABLE void setFovPosition(const quint16 position);
    Q_INVOKABLE void gotoFov(const quint16 position);
    Q_INVOKABLE void setFocusPosition(const quint16 position);
    Q_INVOKABLE void gotoFocus(const quint16 position);
    Q_INVOKABLE void gotoFovFocus(const quint16 fovPosition,
                                  const quint16 focusPosition);
    Q_INVOKABLE void setPosition(const quint8 positionNumber);
    Q_INVOKABLE void clearPosition(const quint8 positionNumber);


    Q_INVOKABLE void tiltUp();
    Q_INVOKABLE void tiltDown();
    Q_INVOKABLE void tiltStop();
    Q_INVOKABLE void panLeft();
    Q_INVOKABLE void panRight();
    Q_INVOKABLE void panStop();
    Q_INVOKABLE void setPanTiltSpeed(const quint8 speed);

    Q_INVOKABLE void setSelectedCamera(const CameraSelection camera);
    Q_INVOKABLE void setSelectedFilter(const FilterSelection filter);
    Q_INVOKABLE void setDefogMode(const DefogMode mode);
    Q_INVOKABLE void setGammaLevel(const GammaLevel level);
    Q_INVOKABLE void setNoiseReductionMode(const NoiseReductionMode mode);

    Q_INVOKABLE void enableDigitalZoom(const bool state);




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
    bool isConnected() const;

    quint8 zoomSpeed() const;

    quint8 panTiltSpeed() const;

    quint8 focusSpeed() const;

    quint16 focusPosition() const;

    quint16 fovPosition() const;

    DefogMode defogMode() const;

    GammaLevel gammaLevel() const;

    bool digitalZoom() const;
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

    quint8 _zoomSpeed;
    quint8 _focusSpeed;
    quint16 _focusPosition;
    quint16 _fovPosition;
    quint8 _panTiltSpeed;
    int _repeatCounter;

    CameraSelection _selectedCamera = CameraSelection_ContinuousZoom;
    FilterSelection _selectedFilter = FilterSelection_ColorFilter;
    DefogMode _defogMode = DefogMode_High;
    GammaLevel _gammaLevel = GammaLevel_Level1;
    NoiseReductionMode _noiseReductionMode = NoiseReductionMode_High;
    bool _isDigitalZoomEnabled = false;

    // Private Functions
    void writeDataOnPlatformsSerialPort(const QByteArray &data);
    quint8 crc8(const QByteArray &data);
    int bytesToInt(QByteArray data, int start, int length, bool reverse = false);
    void sendCommand1(const quint8 &command, const quint8 &param);
    void sendCommand2(const quint8 &command, const quint16 &param);
    void sendCommand3(const quint8 &command, const quint16 &param1, const quint16 &param2);

private Q_SLOTS:
    void sltReadData(QByteArray data);
    void sltReadSeialPortData();
    void sltWriteSerialPortData(QByteArray data);


Q_SIGNALS:
    /* platform property event signals */
    void sigWriteData(QByteArray data);
    void sigDataChanged();

};

#endif // SERIALCONTROL_H
