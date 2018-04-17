/*
  URTouch.h - Arduino/chipKit library support for Color TFT LCD Touch screens 
  Copyright (C)2016 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef URTOUCH_H
#define URTOUCH_H
#include "system.h"

#define swap(type, i, j) {type t = i; i = j; j = t;}
#define regtype volatile uint32_t
#define regsize uint32_t

// *** Hardwarespecific defines ***
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask
#define rbi(reg, bitmask) ((*reg) & bitmask)
#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);
#define swap(type, i, j) {type t = i; i = j; j = t;}
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
class URTouch
{

public:
		int16_t	TP_X ,TP_Y;

		URTouch(PinName tclk, PinName tcs, PinName tdin, PinName dout, PinName irq);

		void	InitTouch(byte orientation = LANDSCAPE);
		bool	read();
		bool	dataAvailable();
		int16_t	getX();
		int16_t	getY();
		void	setPrecision(byte precision);

		void	calibrateRead();
    
private:
    DigitalOut P_CLK, P_CS, P_DIN;
    DigitalIn P_DOUT;
    DigitalInOut P_IRQ;
		long	_default_orientation;
		byte	orient;
		byte	prec;
		byte	display_model;
		long	disp_x_size, disp_y_size, default_orientation;
		long	touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;
    int16_t touch_tresholdLow;
    int16_t touch_tresholdHigh;

		void	touch_WriteData(byte data);
		word	touch_ReadData();

};

#endif