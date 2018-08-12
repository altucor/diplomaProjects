#include "complementarypacketfilter.h"

ComplementaryPacketFilter::ComplementaryPacketFilter()
{

}

void ComplementaryPacketFilter::setCoefficient(double coefficient)
{
    m_f1.setCoefficient(coefficient);
    m_f2.setCoefficient(coefficient);
    m_f3.setCoefficient(coefficient);
}

Packet ComplementaryPacketFilter::processPacket(Packet input)
{
    Packet out;

    //out.ax(m_f1.process(input.ax(), input.gx()));
    //out.ay(m_f2.process(input.ay(), input.gy()));
    //out.az(m_f3.process(input.az(), input.gz()));

    out.ax(m_f1.process(input.ax(), input.gy()));
    out.ay(m_f2.process(input.ay(), input.gx()));
    out.az(m_f3.process(input.az(), input.gz()));

    out.gx(0);
    out.gy(0);
    out.gz(0);

    out.temp(0);

    return out;
}
