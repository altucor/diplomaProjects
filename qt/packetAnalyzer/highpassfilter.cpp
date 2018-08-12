#include "highpassfilter.h"

HighPassFilter::HighPassFilter(double coefficient)
{

}

double HighPassFilter::process(double input)
{
    // Y(n) = Alfa*Y(n-1) + Beta*X(n)
    double result = m_coefficient * m_previousValue + (1 - m_coefficient) * input;
}
