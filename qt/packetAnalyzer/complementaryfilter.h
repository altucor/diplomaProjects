#ifndef COMPLEMENTARYFILTER_H
#define COMPLEMENTARYFILTER_H


class ComplementaryFilter
{
public:
    explicit ComplementaryFilter();
    explicit ComplementaryFilter(double coefficient);
    void setCoefficient(double coefficient);
    double process(double accel, double gyro);
private:
    double m_coefficient = 0.0;
    double m_prevoiusResult = 0.0;
};

#endif // COMPLEMENTARYFILTER_H
