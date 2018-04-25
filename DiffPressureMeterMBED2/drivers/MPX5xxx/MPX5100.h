/*
** Driver for the Freescale MPX5100 family of pressure sensors.
**
** http://www.freescale.com/
*/
#ifndef MPX5100_h
#define MPX5100_h

#include "system.h"
#include "ADS1015.h"

// MPX5100 datasheet values
#define VOUT_MIN 0.2   
#define VOUT_MAX 4.8

#define kPa_MIN    0
#define kPa_MAX  100

#define PSI_MIN    0
#define PSI_MAX 14.5

#define mV_per_kPa 45
#define mV_per_PSI 6.525

#ifndef A2D_FULL_SCALE
#define A2D_FULL_SCALE 0x7FFF // +/-16 bit
#endif // A2D_FULL_SCALE
/*
** For the through hole, dual-port versions (6 in-line pins) the reference side
** of the part is the one on which the device number is printed. Pin one is
** usually on the left (but see the data sheet for some of the other parts it is
** on the right and the surface mount parts have a different pin layout). There
** appears to be a notch in pin one on many (all?) of the packages.
**
** Pin assignments:
** - pin 1: Vout
** - pin 2: GND
** - pin 3: Vss - 5V
** - pin 4-6: no connection
**
** Transfer functions and error calcualtions are described in MPX5100.cpp.
*/

class MPX5100 {

 public:
  MPX5100(ADS1015 &adc);
  float autoCalibrate();	// Automatically determine and set offset.
  void  calibrate(float counts);	// Manually set the offset.
  float read();
  float pointAverage(uint8_t samples, int msDelay);
  float rollingAverage(uint8_t samples);
  float error();

 private:
  /*
   * Full scale output voltage is nominally 4.7 V, but it may go as high
   * as 4.925 V. So to support the full range of an MPX5100 family device
   * an analog reference voltage of 5.0 V is required. However if the
   * application does not require the full range of the device a lower
   * AREF voltage can be used with the benefit of having more resolution
   * in the active range.
   */
  float m_refVolts;
  float m_Voffset;		      // The voltage offset when a zero reading is expected.
  float m_transferConstant;	// Device specific constant for transfer function.
  ADS1015 &m_adc;             // reference to adc converter

  float convert(float reading);
  float supplyVoltage();	// Get the value of Vss
  // This function is identical to the Arduino map but uses floats.
  inline float fmap(float x, float inMin, float inMax, float outMin, float outMax)
  {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
  }
};

#endif // MPX5100_h
