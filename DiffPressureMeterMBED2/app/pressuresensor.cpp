#include "pressuresensor.h"

//-----------------------------------------------------------------------------
PressureSensor & PressureSensor::instance()
{
    static PressureSensor pressureSensor;
    return pressureSensor;
}

//-----------------------------------------------------------------------------
PressureSensor::PressureSensor()
    : m_i2c(ADS1115_I2C_SDA_PIN, ADS1115_I2C_SCL_PIN)
    , m_ads1115(&m_i2c)
    , m_mpx5100(MPX5100, m_ads1115)
{

}

//-----------------------------------------------------------------------------
bool PressureSensor::performMeasurement()
{
    m_lastPressureResult = m_mpx5100.pointAverage(100, 1);
    return true;    
}


//-----------------------------------------------------------------------------
float PressureSensor::getLastResult() const
{
    return m_lastPressureResult;
}

//-----------------------------------------------------------------------------