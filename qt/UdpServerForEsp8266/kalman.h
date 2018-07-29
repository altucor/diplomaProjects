#ifndef KALMAN_H
#define KALMAN_H

#include "packet.h"

#include <vector>

class Kalman
{
public:
    explicit Kalman();
    double filter(double val);
    void setCoefficient(double coeff);
private:
    double m_previousValue = 0.0;
    double m_coefficient = 0.3;
};

#endif // KALMAN_H
