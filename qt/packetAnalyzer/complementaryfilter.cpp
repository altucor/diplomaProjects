#include "complementaryfilter.h"

ComplementaryFilter::ComplementaryFilter()
{

}

ComplementaryFilter::ComplementaryFilter(double coefficient)
    :m_coefficient(coefficient)
{

}

void ComplementaryFilter::setCoefficient(double coefficient)
{
    m_coefficient = coefficient;
}

double ComplementaryFilter::process(double accel, double gyro)
{
    double result = (1 - m_coefficient) * (m_prevoiusResult + gyro) + m_coefficient * accel;
    m_prevoiusResult = result;
    return result;
}
