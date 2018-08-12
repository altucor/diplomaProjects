#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H


class HighPassFilter
{
public:
    explicit HighPassFilter(double coefficient);
    double process(double input);
private:
    double m_previousValue = 0.0;
    double m_coefficient = 0.0;
};

#endif // HIGHPASSFILTER_H
