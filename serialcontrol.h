#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include <QObject>
#include <QSerialPort>
#include <QtMath>
#include <QDebug>

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

public:
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
    bool connectToSerialPort(const QSerialPortInfo &portInfo);

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

    ///
    /// \brief The Commands enum
    ///
    /// this enum stores all commands which should handle by shortkey
    /// whether keyboard shortkey or gamepad shortkeys.
    ///
    enum Commands
    {
       Command_StartTrack        = 0xc1,
       Command_StopTrack         = 0xc2,
       Command_PanTiltOsd        = 0xc3,
       Command_PositionReport    = 0xc4,
       Command_ResetCmd          = 0xc5,
       Command_                  = 0xc7,
       Command_GateResize        = 0xc8,
       Command_ChangeTrackType   = 0xc9,
       Command_OsdExhibition     = 0xca,
       Command_ClaheActivation   = 0xCb,
       Command_ReticleMovement   = 0xe2
    };


    QSerialPort *m_serialPort;


    // Private Functions
    void writeDataOnPlatformsSerialPort(const QByteArray &data);
    void sendCommand(const QByteArray &data);
    quint8 crc8(const QByteArray &data);
    int bytesToInt(QByteArray data, int start, int length, bool reverse = false);



private Q_SLOTS:
    void sltReadData(QByteArray data);
    void sltReadSeialPortData();
    void sltWriteSerialPortData(QByteArray data);


Q_SIGNALS:
    /* platform property event signals */
    void sigWriteData(QByteArray data);
};

#endif // SERIALCONTROL_H
