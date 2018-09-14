#ifndef COMPLEMENTARY_H
#define COMPLEMENTARY_H

class Complementary
{
public:
    Complementary();
    void setCoefficient(double coefficient);
    double getCoefficient();
    double filter(double inputAccel, double inputGyro, double &dublicateForNextUsage);
    double accelToAngle(double value);
    double gyroToAngle(double value);
    void setDeltaTime(double dt);
    double getDeltaTime();

private:
    double m_coefficient = 0.0;
    double m_accel = 0.0;
    double m_gyroAngle = 0.0;
    double m_result = 0.0;
    double m_dt = 5.0; // delta time for integration
    double m_1G = 2048.0;
    double m_toDeg = 57.29577951308232087679815481410517033f;
};

#endif // COMPLEMENTARY_H
