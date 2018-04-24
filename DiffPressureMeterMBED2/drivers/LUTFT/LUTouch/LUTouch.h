/*
  LUTouch - library support for Color TFT LCD Touch screens with XPT2046 driver 
  @author Lukasz Uszko aka igbt6 (luszko@op.pl) - April 2018
  Library is based on URTouch library http://www.rinkydinkelectronics.com/library.php?id=92 by Hening Karlsen
*/

#ifndef LUTOUCH_H
#define LUTOUCH_H
#include "system.h"

// FOR runDemo function
#include "LUTft.h"

// *** Hardwarespecific defines ***
// XPT2046 touch panel controller
#define TP_CS_OUT()      _cs_pin(TP_CS_PIN)
#define TP_CS_HIGH()     _cs_pin.write(HIGH)
#define TP_CS_LOW()      _cs_pin.write(LOW)
#define TP_CS_DISABLE()  TP_CS_SET()

#define TP_IRQ_INIT()     _irq_pin(TP_IRQ_PIN)
#define TP_IRQ_OUT_HIGH() _irq_pin.output(); _irq_pin.write(HIGH)
#define TP_IRQ_OUT_LOW()  _irq_pin.output(); _irq_pin.write(LOW)
#define TP_IRQ_IN()       _irq_pin.input()
#define TP_IRQ_IN_READ()  _irq_pin.read()
// ********************************


#define regtype volatile uint32_t
#define regsize uint32_t
#define word uint16_t

#define PORTRAIT			0
#define LANDSCAPE			1

#define PREC_LOW			1
#define PREC_MEDIUM		2
#define PREC_HI				3
#define PREC_EXTREME	4

#define byte uint8_t
#define word uint16_t

typedef struct {
	uint16_t x;
	uint16_t y;
} TouchPoint;


typedef struct
{
	float a_x;
  float b_x;
  float d_x;
  float a_y;
  float b_y;
  float d_y;
} CalibCoefficients;

typedef struct
{
  union
  {
    CalibCoefficients calibCoeffs;
    uint32_t calibCoeffsMemory[6];
  };
} TouchCalibCoefficients;


class LUTouch
{

public:
		int16_t	TP_X ,TP_Y;

    LUTouch(SPI &spi);
		void	InitTouch(const TouchCalibCoefficients &calibCoeffs, byte orientation = LANDSCAPE);
		bool	read();
		bool	dataAvailable();
    void  setCalibCoefficients(const TouchCalibCoefficients &coefs); 
		int16_t	getX();
		int16_t	getY();
		void	setPrecision(byte precision);

		void	calibrateRead();
    static void runDemo(LUTFT &tft, LUTouch &touch, int demo_time_in_secs);
    
private:
    SPI &spi;
    DigitalInOut _irq_pin;
    DigitalOut _cs_pin;
    TouchCalibCoefficients coeffs;
		byte	orient;
    byte  default_orientation;
		byte	prec;
		long	disp_x_size, disp_y_size;

		byte	touch_WriteData(byte data);
		word	touch_ReadData();

};

#endif