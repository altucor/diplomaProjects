#ifndef KALMANPACKETFILTER_H
#define KALMANPACKETFILTER_H

#include "packet.h"
#include "kalman.h"

class KalmanPacketFilter
{
public:
    explicit KalmanPacketFilter();
    void setKalmanCoefficient(double coeff);
    Packet processPacket(Packet in);

private:
    Kalman m_FilterAcX;
    Kalman m_FilterAcY;
    Kalman m_FilterAcZ;
    Kalman m_FilterGyX;
    Kalman m_FilterGyY;
    Kalman m_FilterGyZ;
};

#endif // KALMANPACKETFILTER_H
