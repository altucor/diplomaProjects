#include "kalman.h"

Kalman::Kalman()
{
    //
}

double Kalman::filter(double val)
{
    double result = m_coefficient * val + (1 - m_coefficient) * m_previousValue;
    m_previousValue = result;
    return result;
}

void Kalman::setCoefficient(double coeff)
{
    m_coefficient = coeff;
}
