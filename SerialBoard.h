#pragma once

#include <iostream>

#include <QObject>
#include <QByteArray>

class SerialBoard : public QObject
{
    Q_OBJECT

public:
    enum Commands
    {
        Command_Normal          = 0x00,
        Command_Continuous      = 0x01,
        Command_Spotter         = 0x02,
        Command_NIR_Filter      = 0x03,
        Command_Snapshot        = 0x04,
        Command_VideoRecord     = 0x05,
        Command_Laser           = 0x06,
        Command_BeamAnglePlus   = 0x07,
        Command_BeamAngleMinus  = 0x08,
        Command_IntensityPlus   = 0x09,
        Command_IntensityMinus  = 0x0A,
        Command_SpeedPlus       = 0x0B,
        Command_SpeedMinus      = 0x0C,
        Command_MoveUp          = 0x0D,
        Command_MoveDown        = 0x0E,
        Command_MoveLeft        = 0x0F,
        Command_MoveRight       = 0x10,
        Command_ZoomPlus        = 0x11,
        Command_ZoomMinus       = 0x12,
        Command_FocusPlus       = 0x13,
        Command_FocusMinus      = 0x14,
        Command_AutoFocus       = 0x15,
    };

public:
    SerialBoard(QObject *parent = nullptr);
    ~SerialBoard();

    void processInboundPacket(
            const QByteArray &buffer);

    QByteArray createPacket(
            const uint8_t &command);

private:
    const uint8_t m_header;
    const uint8_t m_footer;

    QByteArray m_buffer;

    void analyzePacket();

    void interpretPacket(
            const QByteArray &packet);

Q_SIGNALS:
    void sigNewDataReceived(
            const Commands &command);

};
