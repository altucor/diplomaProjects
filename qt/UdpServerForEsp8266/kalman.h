#ifndef KALMAN_H
#define KALMAN_H

#include "packet.h"

#include <vector>

class Kalman
{
public:
    explicit Kalman();
    double filter(double val);

private:
    double previousValue = 0.0;
    double coefficient = 0.5;
};

#endif // KALMAN_H
