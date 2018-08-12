#include "kalmanpacketfilter.h"

KalmanPacketFilter::KalmanPacketFilter()
{

}

void KalmanPacketFilter::setKalmanCoefficient(double coeff)
{
    m_FilterAcX.setCoefficient(coeff);
    m_FilterAcY.setCoefficient(coeff);
    m_FilterAcZ.setCoefficient(coeff);
    m_FilterGyX.setCoefficient(coeff);
    m_FilterGyY.setCoefficient(coeff);
    m_FilterGyZ.setCoefficient(coeff);
}

Packet KalmanPacketFilter::processPacket(Packet in)
{
    Packet out;

    out.ax(m_FilterAcX.filter(in.ax()));
    out.ay(m_FilterAcY.filter(in.ay()));
    out.az(m_FilterAcZ.filter(in.az()));

    out.gx(m_FilterGyX.filter(in.gx()));
    out.gy(m_FilterGyY.filter(in.gy()));
    out.gz(m_FilterGyZ.filter(in.gz()));

    return out;
}
