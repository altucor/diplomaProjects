#include "packet.h"

Packet::Packet()
{
    //
}

Packet::Packet(double _ax, double _ay, double _az, double _gx, double _gy, double _gz, double _temp)
    : m_ax(_ax), m_ay(_ay), m_az(_az), m_gx(_gx), m_gy(_gy), m_gz(_gz), m_temp(_temp)
{
    //
}

Packet& Packet::operator = (Packet rvalue)
{
    m_ax = rvalue.m_ax;
    m_ay = rvalue.m_ay;
    m_az = rvalue.m_az;

    m_gx = rvalue.m_gx;
    m_gy = rvalue.m_gy;
    m_gz = rvalue.m_gz;

    m_temp = rvalue.m_temp;
    return *this;
}

Packet& Packet::operator += (Packet rvalue)
{
    m_ax += rvalue.m_ax;
    m_ay += rvalue.m_ay;
    m_az += rvalue.m_az;

    m_gx += rvalue.m_gx;
    m_gy += rvalue.m_gy;
    m_gz += rvalue.m_gz;

    m_temp += rvalue.m_temp;
    return *this;
}

QString Packet::toString()
{
    QString str = "";
    str += QString::number(m_ax) + " ";
    str += QString::number(m_ay) + " ";
    str += QString::number(m_az) + " ";

    str += QString::number(m_gx) + " ";
    str += QString::number(m_gy) + " ";
    str += QString::number(m_gz) + " ";

    str += QString::number(m_temp) + " ";

    return str;
}
