#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "system.h"
#include "ADS1015.h"
#include "MPX5xxx.h"


class PressureSensor
{
public:
    static PressureSensor& instance();
    bool performMeasurement(); // TODO
    float getLastResult() const;

private:
    I2C m_i2c;
    ADS1115 m_ads1115;
    MPX5xxx m_mpx5100;
    float m_lastPressureResult;

    // non copyable 
    PressureSensor ();
    PressureSensor & operator=(const PressureSensor& sensor);
};

#endif