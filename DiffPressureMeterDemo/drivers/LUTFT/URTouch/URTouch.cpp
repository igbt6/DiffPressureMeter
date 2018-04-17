/*
  URTouch.cpp - Arduino/chipKit library support for Color TFT LCD Touch screens 
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

#include "URTouch.h"
#include "URTouchCD.h"

URTouch::URTouch(PinName tclk, PinName tcs, PinName din, PinName dout, PinName irq)
	: P_CLK(tclk)
	, P_CS(tcs)
	, P_DIN(din)
	, P_DOUT(dout)
	, P_IRQ(irq)
{
}

void URTouch::InitTouch(byte orientation)
{
	orient					= orientation;
	_default_orientation	= CAL_S>>31;
	touch_x_left			= (CAL_X>>14) & 0x3FFF;
	touch_x_right			= CAL_X & 0x3FFF;
	touch_y_top				= (CAL_Y>>14) & 0x3FFF;
	touch_y_bottom			= CAL_Y & 0x3FFF;
	disp_x_size				= (CAL_S>>12) & 0x0FFF;
	disp_y_size				= CAL_S & 0x0FFF;
	prec					= 10;

    P_IRQ.output();
 
    P_CS = HIGH;
    P_CLK = HIGH;
    P_DIN = HIGH;
    P_IRQ = HIGH;
    touch_tresholdLow = -1024;
    touch_tresholdHigh = 2048;
}

void URTouch::touch_WriteData(byte data)
{
    byte temp;
 
    temp=data;
    P_CLK = LOW;
 
    for(byte count=0; count<8; count++)
    {
        if(temp & 0x80)
            P_DIN = HIGH;
        else
            P_DIN = LOW;
        temp = temp << 1; 
        P_CLK = LOW;
        P_CLK = HIGH;
    }
}
 
word URTouch::touch_ReadData()
{
    word data = 0;
 
    for(byte count=0; count<12; count++)
    {
        data <<= 1;
        P_CLK = HIGH;
        P_CLK = LOW;
        if (P_DOUT)
            data++;
    }
    return(data);
}

bool URTouch::read()
{
	unsigned long tx=0, temp_x=0;
	unsigned long ty=0, temp_y=0;
	unsigned long minx=99999, maxx=0;
	unsigned long miny=99999, maxy=0;
	int datacount=0;

    P_CS = LOW;
    P_IRQ.input();
	for (int i=0; i<prec; i++)
	{
		if (!P_IRQ)
		{
			touch_WriteData(0x90);        
			P_CLK = HIGH; P_CLK = LOW;
			temp_x=touch_ReadData();

			if (!P_IRQ)
			{
				touch_WriteData(0xD0);      
				P_CLK = HIGH; P_CLK = LOW;
				temp_y=touch_ReadData();

				if ((temp_x>0) and (temp_x<4096) and (temp_y>0) and (temp_y<4096))
				{
					tx+=temp_x;
					ty+=temp_y;
					if (prec>5)
					{
						if (temp_x<minx)
							minx=temp_x;
						if (temp_x>maxx)
							maxx=temp_x;
						if (temp_y<miny)
							miny=temp_y;
						if (temp_y>maxy)
							maxy=temp_y;
					}
					datacount++;
				}
			}
		}
	}
	P_IRQ.output();

	if (prec>5)
	{
		tx = tx-(minx+maxx);
		ty = ty-(miny+maxy);
		datacount -= 2;
	}

    P_CS = HIGH;
    if ((datacount==(prec-2)) or (datacount==PREC_LOW))
    {
        if (orient == _default_orientation)
        {
            TP_X=ty/datacount;
            TP_Y=tx/datacount;
            return true;
        }
        else
        {
            TP_X=tx/datacount;
            TP_Y=ty/datacount;
            return true;
        }
    }
    else
    {
        TP_X=-1;
        TP_Y=-1;
        return false;
    }
}

bool URTouch::dataAvailable()
{
    bool avail;
    P_IRQ.input();
    avail = !(P_IRQ);
    P_IRQ.output();
    return avail;
}

int16_t URTouch::getX()
{
	long c;

	if ((TP_X==-1) or (TP_Y==-1))
		return -1;
	if (orient == _default_orientation)
	{
		c = long(long(TP_X - touch_x_left) * (disp_x_size)) / long(touch_x_right - touch_x_left);
		if (c<0)
			c = 0;
		if (c>disp_x_size)
			c = disp_x_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = long(long(TP_X - touch_y_top) * (-disp_y_size)) / long(touch_y_bottom - touch_y_top) + long(disp_y_size);
		else
			c = long(long(TP_X - touch_y_top) * (disp_y_size)) / long(touch_y_bottom - touch_y_top);
		if (c<0)
			c = 0;
		if (c>disp_y_size)
			c = disp_y_size;
	}
	return c;
}

int16_t URTouch::getY()
{
	int c;

	if ((TP_X==-1) or (TP_Y==-1))
		return -1;
	if (orient == _default_orientation)
	{
		c = long(long(TP_Y - touch_y_top) * (disp_y_size)) / long(touch_y_bottom - touch_y_top);
		if (c<0)
			c = 0;
		if (c>disp_y_size)
			c = disp_y_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = long(long(TP_Y - touch_x_left) * (disp_x_size)) / long(touch_x_right - touch_x_left);
		else
			c = long(long(TP_Y - touch_x_left) * (-disp_x_size)) / long(touch_x_right - touch_x_left) + long(disp_x_size);
		if (c<0)
			c = 0;
		if (c>disp_x_size)
			c = disp_x_size;
	}
	return c;
}

void URTouch::setPrecision(byte precision)
{
	switch (precision)
	{
		case PREC_LOW:
			prec=1;		// DO NOT CHANGE!
			break;
		case PREC_MEDIUM:
			prec=12;	// Iterations + 2
			break;
		case PREC_HI:
			prec=27;	// Iterations + 2
			break;
		case PREC_EXTREME:
			prec=102;	// Iterations + 2
			break;
		default:
			prec=12;	// Iterations + 2
			break;
	}
}

void URTouch::calibrateRead()
{
    unsigned long tx=0;
    unsigned long ty=0;
 
    P_CS = LOW;
 
    touch_WriteData(0x90);        
    P_CLK = HIGH; P_CLK = LOW;
    tx=touch_ReadData();
 
    touch_WriteData(0xD0);      
    P_CLK = HIGH; P_CLK = LOW;
    ty=touch_ReadData();
 
    P_CS = HIGH;
 
    TP_X=ty;
    TP_Y=tx;
}

