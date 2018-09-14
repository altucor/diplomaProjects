#include "Complementary.h"

#include <cmath>

Complementary::Complementary()
{

}

void Complementary::setCoefficient(double coefficient)
{
    m_coefficient = coefficient;
}

double Complementary::getCoefficient()
{
    return m_coefficient;
}

void Complementary::setDeltaTime(double dt)
{
    m_dt = dt;
}

double Complementary::getDeltaTime()
{
    return m_dt;
}

double Complementary::filter(double inputAccel, double inputGyro, double &dublicateForNextUsage)
{
    //inputAccel = accelToAngle(inputAccel);
    //inputGyro = gyroToAngle(inputGyro);
    m_result = inputGyro * (1 - m_coefficient) + inputAccel * m_coefficient;
    dublicateForNextUsage = m_result;
    return m_result;
}

double Complementary::accelToAngle(double value)
{
    // 16g - 2048 = 1G
    double angle = 0.0;
    double gValue = value / m_1G;

    if(gValue > 1.0){
        gValue = 1.0;
    } else if(gValue < -1.0) {
        gValue = -1.0;
    }

    if(gValue >= 0){
        angle = 90 - m_toDeg * acos(gValue);
    }else{
        angle = m_toDeg * acos(-gValue) - 90;
    }

    return angle;
}

double Complementary::gyroToAngle(double value)
{
    m_gyroAngle = m_gyroAngle + (value / 16.384) * m_dt / 1000.0;
    return m_gyroAngle;
}
