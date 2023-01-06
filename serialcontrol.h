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


public:

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


    Q_INVOKABLE void tiltUp();
    Q_INVOKABLE void tiltDown();
    Q_INVOKABLE void tiltStop();
    Q_INVOKABLE void panLeft();
    Q_INVOKABLE void panRight();
    Q_INVOKABLE void panStop();
    Q_INVOKABLE void setPanTiltSpeed(const quint8 speed);




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
    quint8 _panTiltSpeed;
    int _repeatCounter;

    // Private Functions
    void writeDataOnPlatformsSerialPort(const QByteArray &data);
    quint8 crc8(const QByteArray &data);
    int bytesToInt(QByteArray data, int start, int length, bool reverse = false);
    void sendCommand1(const quint8 &command, const quint8 &param);

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
