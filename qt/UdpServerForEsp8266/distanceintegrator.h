#ifndef DISTANCEINTEGRATOR_H
#define DISTANCEINTEGRATOR_H

#include "packet.h"

class DistanceIntegrator
{
public:
    DistanceIntegrator();
    void processPacket(Packet &packet);

private:
    void processChannel(double acceleration, double &velocity, double &position);
    void velocityIntegration(double acceleration, double &velocity);
    void distanceIntegration(double velocity, double &position);

private:
    double m_velocityX = 0.0;
    double m_velocityY = 0.0;
    double m_velocityZ = 0.0;
    double m_positionX = 0.0;
    double m_positionY = 0.0;
    double m_positionZ = 0.0;
    double m_distanceFullX = 0.0;
    double m_distanceFullY = 0.0;
    double m_distanceFullZ = 0.0;
    double m_dt = 5.0; // delta time for integration
};

#endif // DISTANCEINTEGRATOR_H
