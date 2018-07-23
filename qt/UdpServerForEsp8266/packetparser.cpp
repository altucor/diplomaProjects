#include "packetparser.h"

PacketParser::PacketParser()
{
    //
}

PacketParser::~PacketParser()
{
    //
}

void PacketParser::setCalibrationPacket(Packet zeroOffset)
{
    m_zeroOffset = zeroOffset;
}

void PacketParser::setPacket(QString packet)
{
    m_parsePacket(packet);
}

void PacketParser::setPacket(QByteArray packet)
{
    m_parsePacket(packet);
}

void PacketParser::m_parsePacket(QByteArray packet)
{
    m_parsePacket(QString(packet));
}

void PacketParser::m_parsePacket(QString packet)
{
    QStringList parsedPacket = packet.split(" ");
    m_parsedPacket.ax(parsedPacket[0].toInt() * 1.0);
    m_parsedPacket.ay(parsedPacket[1].toInt() * 1.0);
    m_parsedPacket.az(parsedPacket[2].toInt() * 1.0);
    m_parsedPacket.gx(parsedPacket[3].toInt() * 1.0);
    m_parsedPacket.gy(parsedPacket[4].toInt() * 1.0);
    m_parsedPacket.gz(parsedPacket[5].toInt() * 1.0);
    m_parsedPacket.temp(parsedPacket[6].toDouble());

    m_parsedPacket += m_zeroOffset;
}

Packet PacketParser::getPacket()
{
    switch(m_filterType){
        case NONE: {
            return m_parsedPacket;
            break;
        }
        case KALMAN: {
            return kalman(m_parsedPacket);
            break;
        }
    }
}

Packet PacketParser::kalman(Packet packet)
{
    Packet filteredPacket;
    filteredPacket.ax( m_kalmanFilterAx.filter( packet.ax() ) );
    filteredPacket.ay( m_kalmanFilterAy.filter( packet.ay() ) );
    filteredPacket.az( m_kalmanFilterAz.filter( packet.az() ) );

    filteredPacket.ax( m_kalmanFilterGx.filter( packet.gx() ) );
    filteredPacket.ay( m_kalmanFilterGy.filter( packet.gy() ) );
    filteredPacket.az( m_kalmanFilterGz.filter( packet.gz() ) );

    filteredPacket.temp(packet.temp());

    return filteredPacket;
}

void PacketParser::setFilter(int type)
{
    m_filterType = type;
}

QString PacketParser::getString()
{
    QString str = "";
    str += QString::number(m_parsedPacket.ax()) + " ";
    str += QString::number(m_parsedPacket.ay()) + " ";
    str += QString::number(m_parsedPacket.az()) + " ";

    str += QString::number(m_parsedPacket.gx()) + " ";
    str += QString::number(m_parsedPacket.gy()) + " ";
    str += QString::number(m_parsedPacket.gz()) + " ";

    str += QString::number(m_parsedPacket.temp()) + " ";

    return str;
}
