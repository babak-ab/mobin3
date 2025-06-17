#include "SerialBoard.h"

SerialBoard::SerialBoard(
        QObject *parent) :
    QObject(parent),
    m_header(0x7A),
    m_footer(0xA7)
{

}

SerialBoard::~SerialBoard()
{

}

void SerialBoard::
processInboundPacket(
        const QByteArray &buffer)
{
    m_buffer.append(buffer);

    analyzePacket();
}

QByteArray SerialBoard::
createPacket(const uint8_t &command)
{
    QByteArray packet(3, char(0x00));

    packet[0] = m_header;
    packet[1] = command;
    packet[2] = m_footer;

    return packet;
}

QByteArray SerialBoard::
createFeedback(const QByteArray &data)
{
    QByteArray packet(data.length() + 2, char(0x00));

    packet[0] = m_header;

    for (int i = 0; i < data.length(); i++)
    {
        packet[1 + i] = data.at(i);
    }

    packet[data.length() + 1] = m_footer;

    return packet;
}

void SerialBoard::
analyzePacket()
{
    while (m_buffer.length() >= 3)
    {
        int16_t startIndex =
                m_buffer.indexOf(m_header);

        if (startIndex < 0)
        {
            m_buffer.clear();

            return;
        }

        if (startIndex + 2 >=
                m_buffer.length())
        {
            return;
        }

        QByteArray packet =
                m_buffer.mid(startIndex, 3);

        if (static_cast<uint8_t>(packet.at(2)) ==
                static_cast<uint8_t>(m_footer))
        {
            interpretPacket(packet);
        }

        m_buffer.remove(0, startIndex + 3);
    }
}

void SerialBoard::
interpretPacket(const QByteArray &packet)
{
    const Commands command =
            static_cast<Commands>(packet.at(1));

    Q_EMIT sigNewDataReceived(command);
}
