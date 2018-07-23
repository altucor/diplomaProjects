#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QString>
#include <QStringList>
#include <QByteArray>

#include "packet.h"
#include "kalman.h"

enum FilterTypes{
    NONE = 0,
    KALMAN = 1
};

class PacketParser
{
public:
    explicit PacketParser();
    ~PacketParser();
    void setCalibrationPacket(Packet zeroOffset);
    void setPacket(QString packet);
    void setPacket(QByteArray packet);
    Packet getPacket();
    void setFilter(int type);
    QString getString();

private:
    void m_parsePacket(QByteArray packet);
    void m_parsePacket(QString packet);
    Packet kalman(Packet packet);

private:
    Packet m_zeroOffset;
    Packet m_parsedPacket;
    Kalman m_kalmanFilterAx;
    Kalman m_kalmanFilterAy;
    Kalman m_kalmanFilterAz;
    Kalman m_kalmanFilterGx;
    Kalman m_kalmanFilterGy;
    Kalman m_kalmanFilterGz;
    int m_filterType = 0;
};

#endif // PACKETPARSER_H
