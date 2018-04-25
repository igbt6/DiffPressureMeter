/*
** Driver for the Freescale MPX5100 pressure sensor.
** MPX5100 (100 kPa)
*/

#include "MPX5100.h"

MPX5100::MPX5100(ADS1015 &adc)
  : m_adc(adc)
{
  m_refVolts = supplyVoltage();
}


/*
** Manually set the device offset. Should it return anything? Previous value of the offset,
** or if called with no value, should it return the offset?
*/

void MPX5100::calibrate(float counts)
{
  m_Voffset = (counts * m_refVolts) / A2D_FULL_SCALE;
}

/*
** Calibrate the device by reading the current value from the sensor and storing it as the
** offset value. Assumes that the sensor is in a state where it should be reading zero.
** Return the new offset.
*/

float MPX5100::autoCalibrate()
{
}

/*
** Read the sensor and return the current pressure in kPa.
**
** Transfer functions (from datasheets):
**
** MPX5100: Vout = Vs * (0.0090000 * P + 0.04) +/- Error (Perror * Temp Factor * 0.009 * Vs)
*/

float MPX5100::read()
{
  return convert(m_adc.readADC_SingleEnded(0));
}

float MPX5100::pointAverage(uint8_t samples, int msDelay)
{
  long readings = 0;
  float average;

  for (uint8_t i = 0; i < samples; i++)
  {
      uint16_t raw = m_adc.readADC_SingleEnded(0);
      //DEBUG("Raw adc: 0x%x, %f", raw, ((float)raw * m_adc.getMiliVoltPerBit())/1000);
      readings += raw;
      wait_ms(msDelay);
  }
  average = readings / samples;

  return convert(average);
}

float MPX5100::rollingAverage(uint8_t samples)
{
  static int readings[20];	      // XXX - for a max of 20.
  static uint8_t cur = 0;
  static uint8_t allValid = 0;
  float average = 0;

  readings[cur++ % samples] = m_adc.readADC_SingleEnded(0);

  //printf("readings[] = ", readings[0], ", " , readings[1], ", " , readings[2], ", "  , readings[3], ", "  , readings[4]);

  if (allValid)
  {
    for (uint8_t i = 0; i < samples; i++)
    {
      average += readings[i];
    }
    average /= samples;
  } 
  else 
  {
    for (uint8_t i = 0; i < cur; i++)
    {
      average += readings[i];
    }
    average /= cur;
    if (cur == samples)
    {
      allValid = 1;
    }
  }
  return convert(average);
}

/*
** Return a symetric error value for the current operating condition of the sensor.
*/

float MPX5100::error()
{
}

/*
** Private functions.
*/

float MPX5100::convert(float reading)
{
  float transferConstant = 0.0090000;
  float Vout = reading * (m_adc.getMiliVoltPerBit()/1000);
  //Vout = fmap(Vout, VOUT_MIN, VOUT_MAX, 0, VOUT_MAX - VOUT_MIN);
  float Vss = supplyVoltage();
  float pressure = (Vout - 0.04f*Vss) / (transferConstant*Vss);
  DEBUG("reading: %f, Vss: %f, Vout: %f, pressure: %f", reading, Vss, Vout, pressure);

  return pressure;
}

/*
** Return the value of the supply voltage for the sensor. Could be overridden in a design
** using this sensor if there is a way to get the actual value of the voltage, or to get
** it in real time.
*/

float MPX5100::supplyVoltage()
{
  long readings = 0;
  float average;
  const int samples = 20;
  for (uint8_t i = 0; i < samples; i++)
  {
      uint16_t raw = m_adc.readADC_SingleEnded(1);
      DEBUG("Raw adc: 0x%x, %f", raw, ((float)raw * m_adc.getMiliVoltPerBit())/1000);
      readings += raw;
      wait_ms(1);
  }
  average = (readings / samples) * m_adc.getMiliVoltPerBit()/1000;
  DEBUG("MPX5100 VSS volatge: %f [V]", average);
  return average;
}
