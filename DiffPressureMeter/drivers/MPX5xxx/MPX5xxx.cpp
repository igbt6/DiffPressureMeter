/*
** Driver for the Freescale MPX5xxx family of pressure sensors.
**
** Tested on:
** - MPX5010 (10 kPa)
** - MPX5100 (100 kPa)
** - MPX5700 (700 kPa)
*/

#define ANALOG_PIN PC_0 //TO be removed. 5V incompatible PIN, VRef = 3.3V

#include "MPX5xxx.h"

MPX5xxx::MPX5xxx(uint8_t devType, const PinName pin)
  : _pin(pin)
{
  _devType = devType;
  _aRefVolts = 3.3; //3.3V by NUCLEO
}


/*
** Manually set the device offset. Should it return anything? Previous value of the offset,
** or if called with no value, should it return the offset?
*/

void MPX5xxx::calibrate(float counts)
{
  _Voffset = (counts * _aRefVolts) / A2D_FULL_SCALE;
}

/*
** Calibrate the device by reading the current value from the sensor and storing it as the
** offset value. Assumes that the sensor is in a state where it should be reading zero.
** Return the new offset.
*/

float MPX5xxx::autoCalibrate()
{
}

/*
** Read the sensor and return the current pressure in kPa.
**
** Transfer functions (from datasheets):
**
** MPX5010: Vout = Vs * (0.0900000 * P + 0.04) +/- Error (5.0% Vfss)
** MPX5100: Vout = Vs * (0.0090000 * P + 0.04) +/- Error (Perror * Temp Factor * 0.009 * Vs)
** MPX5700: Vout = Vs * (0.0012858 * P + 0.04) +/- Error
*/

float MPX5xxx::read()
{
  return convert(_pin.read_u16());
}

float MPX5xxx::pointAverage(uint8_t samples, int msDelay)
{
  long readings = 0;
  float average;

  for (uint8_t i = 0; i < samples; i++) {
    readings += _pin.read_u16();
    wait_ms(msDelay);
  }
  average = readings / samples;

  return convert(average);
}

float MPX5xxx::rollingAverage(uint8_t samples)
{
  static int readings[20];	      // XXX - for a max of 20.
  static uint8_t cur = 0;
  static uint8_t allValid = 0;
  float average = 0;

  readings[cur++ % samples] = _pin.read_u16();

  //printf("readings[] = ", readings[0], ", " , readings[1], ", " , readings[2], ", "  , readings[3], ", "  , readings[4]);

  if (allValid) {
    for (uint8_t i = 0; i < samples; i++) {
      average += readings[i];
    }
    average /= samples;
  } else {
    for (uint8_t i = 0; i < cur; i++) {
      average += readings[i];
    }
    average /= cur;
    if (cur == samples) {
      allValid = 1;
    }
  }
  return convert(average);
}

/*
** Return a symetric error value for the current operating condition of the sensor.
*/

float MPX5xxx::error()
{
}

/*
** Private functions.
*/

float MPX5xxx::convert(float reading)
{
  float transferConstant;
  float Vout = (reading * _aRefVolts) / A2D_FULL_SCALE;

  switch (_devType) {
  case MPX5010:
    transferConstant = 0.0900000;
    break;
  case MPX5100:
    transferConstant = 0.0090000;
    break;
  case MPX5700:
    transferConstant = 0.0012858;
    break;
  }

  float pressure = (Vout -  _Voffset) / (transferConstant * supplyVoltage() );
  printf("\t", reading, "\t", Vout, "\t", pressure);

  return pressure;
}

/*
** Return the value of the supply voltage for the sensor. Could be overridden in a design
** using this sensor if there is a way to get the actual value of the voltage, or to get
** it in real time.
*/

float MPX5xxx::supplyVoltage()
{
  /*
  ** Should be a virtual function that can be overridden by the designer if there is a way
  ** to measure the actual supply voltage.
  */
  return VSS;
}
