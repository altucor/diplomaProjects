#ifndef COMPLEMENTARYPACKETFILTER_H
#define COMPLEMENTARYPACKETFILTER_H

#include "packet.h"
#include "complementaryfilter.h"

class ComplementaryPacketFilter
{
public:
    ComplementaryPacketFilter();
    void setCoefficient(double coefficient);
    Packet processPacket(Packet input);
private:
    ComplementaryFilter m_f1;
    ComplementaryFilter m_f2;
    ComplementaryFilter m_f3;
};

#endif // COMPLEMENTARYPACKETFILTER_H
