#ifndef PACKET_H
#define PACKET_H

#include <QString>

class Packet
{
public:
    Packet();
    Packet(double _ax, double _ay, double _az, double _gx, double _gy, double _gz, double _temp);
    //Packet(Packet &&rval);
    Packet& operator = (Packet rvalue);
    Packet& operator += (Packet rvalue);
    QString toString();

    /* Getters */
    double ax() { return m_ax; };
    double ay() { return m_ay; };
    double az() { return m_az; };
    double gx() { return m_gx; };
    double gy() { return m_gy; };
    double gz() { return m_gz; };
    double temp() { return m_temp; };

    /* Setters */
    void ax(double val) { m_ax = val; };
    void ay(double val) { m_ay = val; };
    void az(double val) { m_az = val; };
    void gx(double val) { m_gx = val; };
    void gy(double val) { m_gy = val; };
    void gz(double val) { m_gz = val; };
    void temp(double val) { m_temp = val; };

private:
    double m_ax = 0.0;
    double m_ay = 0.0;
    double m_az = 0.0;
    double m_gx = 0.0;
    double m_gy = 0.0;
    double m_gz = 0.0;
    double m_temp = 0.0;
};

#endif // PACKET_H
