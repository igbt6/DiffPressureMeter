#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "system.h"
#include "ADS1015.h"
#include "MPX5100.h"


class PressureSensor
{
public:
    PressureSensor();
    bool performMeasurement();
    float getLastResult() const;

private:
    I2C m_i2c;
    ADS1115 m_ads1115;
    MPX5100 m_mpx5100;
    float m_lastPressureResult;
};

#endif