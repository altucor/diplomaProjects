#include "kalman.h"

Kalman::Kalman()
{
    //
}

double Kalman::filter(double val)
{
    double result = coefficient * val + (1 - coefficient) * previousValue;
    previousValue = result;
    return result;
}
