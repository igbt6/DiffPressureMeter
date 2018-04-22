/*
** Driver for the Freescale MPX5xxx family of pressure sensors.
**
** http://www.freescale.com/
*/
#ifndef MPX5xxx_h
#define MPX5xxx_h

#include "system.h"
#include "ADS1015.h"

#ifndef A2D_FULL_SCALE
#define A2D_FULL_SCALE 1024
#endif // A2D_FULL_SCALE
#ifndef VSS
#define VSS 5.0
#endif // VSS


/*
** Supported device types
*/
typedef enum 
{
  MPX5010 = 0,
  MPX5100 = 1,
  MPX5700 = 2
} MPX5xxxDevType;

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
** Transfer functions and error calcualtions are described in MPX5xxx.cpp.
*/

class MPX5xxx {

 public:
  MPX5xxx(MPX5xxxDevType devType, ADS1015 &adc);
  float autoCalibrate();	// Automatically determine and set offset.
  void  calibrate(float counts);	// Manually set the offset.
  float read();
  float pointAverage(uint8_t samples, int msDelay);
  float rollingAverage(uint8_t samples);
  float error();

 private:
  /*
   * Full scale output voltage is nominally 4.7 V, but it may go as high
   * as 4.925 V. So to support the full range of an MPX5xxx family device
   * an analog reference voltage of 5.0 V is required. However if the
   * application does not require the full range of the device a lower
   * AREF voltage can be used with the benefit of having more resolution
   * in the active range.
   */
  float m_refVolts;
  uint8_t  m_devType;		    // Device id, should be enum.
  float m_Voffset;		      // The voltage offset when a zero reading is expected.
  float m_transferConstant;	// Device specific constant for transfer function.
  ADS1015 &m_adc;             // reference to adc converter

  float convert(float reading);
  float supplyVoltage();	// Get the value of Vss
};

#endif // MPX5xxx_h
