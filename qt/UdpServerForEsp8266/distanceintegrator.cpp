#include "distanceintegrator.h"

DistanceIntegrator::DistanceIntegrator()
{

}

void DistanceIntegrator::processPacket(Packet &packet)
{
    processChannel(packet.gx(), m_velocityX, m_positionX);
    processChannel(packet.gy(), m_velocityY, m_positionY);
    processChannel(packet.gz(), m_velocityZ, m_positionZ);

    packet.ax(0);
    packet.ay(0);
    packet.az(0);

    m_distanceFullX += m_velocityX - m_distanceFullX;
    m_distanceFullY += m_velocityY - m_distanceFullY;
    m_distanceFullZ += m_velocityZ - m_distanceFullZ;

    packet.gx(m_distanceFullX);
    packet.gy(m_distanceFullY);
    packet.gz(m_distanceFullZ);
}

void DistanceIntegrator::processChannel(double acceleration, double &velocity, double &position)
{
    velocityIntegration(acceleration, velocity);
    distanceIntegration(velocity, position);
}

void DistanceIntegrator::velocityIntegration(double acceleration, double &velocity)
{
    velocity = velocity + acceleration * m_dt;
}

void DistanceIntegrator::distanceIntegration(double velocity, double &position)
{
    position = position + velocity * m_dt * 2;
}
