#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVector>

#include "packet.h"
#include "kalman.h"
#include "Complementary.h"
#include "distanceintegrator.h"

enum FilterType{
    RAW = 0,
    NONE = 1,
    KALMAN = 2,
    COMPLEMENTARY = 3
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
    void setFilter(FilterType type);
    FilterType getCurrentFilter();
    QString getString();
    void setKalmanCoefficient(double coeff);
    void setComplementaryCoefficient(double coeff);
    void setDeltaTime(double dt);
    Packet toAngle(Packet packet);
    double accelDataToAngle(double accelData);
    void gyroDataToAngle(double gyroData, double &resultGyroAngle);
    void firstIntegration(double in, double &out);
    void secondIntegration(double in, double &out);

private:
    void m_parsePacket(QByteArray packet);
    int16_t fromTwoBytes(uint8_t val1, uint8_t val2);
    void m_parsePacket(QString packet);
    Packet kalman(Packet packet);
    Packet complementary(Packet packet);


private:
    Packet m_zeroOffset;
    Packet m_parsedPacket;
    FilterType m_filterType = RAW;
    Kalman m_kalmanFilterAx;
    Kalman m_kalmanFilterAy;
    Kalman m_kalmanFilterAz;
    Kalman m_kalmanFilterGx;
    Kalman m_kalmanFilterGy;
    Kalman m_kalmanFilterGz;
    Complementary m_complementaryFilterX;
    Complementary m_complementaryFilterY;
    Complementary m_complementaryFilterZ;
    double m_gyroAngleX = 0.0;
    double m_gyroAngleY = 0.0;
    double m_gyroAngleZ = 0.0;
    double m_dt = 5.0; // delta time for integration
    double m_1G = 2048.0;
    double m_toDeg = 57.29577951308232087679815481410517033f;
};

#endif // PACKETPARSER_H
