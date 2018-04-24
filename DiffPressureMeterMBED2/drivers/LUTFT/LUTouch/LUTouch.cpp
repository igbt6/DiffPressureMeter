/*
  LUTouch - library support for Color TFT LCD Touch screens with XPT2046 driver 
  @author Lukasz Uszko aka igbt6 (luszko@op.pl) - April 2018
  Library is based on URTouch library http://www.rinkydinkelectronics.com/library.php?id=92 by Hening Karlsen
*/

#include "LUTouch.h"


#define swap(type, i, j) {type t = i; i = j; j = t;}

LUTouch::LUTouch(SPI &spi)
	: spi(spi)
    , TP_IRQ_INIT()	
    , TP_CS_OUT()
{
	
}

void LUTouch::InitTouch(const TouchCalibCoefficients &calibCoeffs, byte orientation)
{
	orient				= orientation;
	default_orientation	= LANDSCAPE;
	prec				= 20;
	//memcpy(&coeffs, &calibCoeffs, sizeof(TouchCalibCoefficients)); // raw HEX from memory
	coeffs = calibCoeffs;
	TP_IRQ_OUT_HIGH();
 	TP_CS_HIGH();
}

byte LUTouch::touch_WriteData(byte data)
{

    byte temp;
    temp = data;
	return (byte)spi.write(temp);
}
 
word LUTouch::touch_ReadData()
{
    word data = 0;
	data = touch_WriteData(0x00);
	data <<= 8;
	data |= touch_WriteData(0x00);
	data >>= 4;
    return(data & 0x0FFF);
}

bool LUTouch::read()
{
	unsigned long tx=0, temp_x=0;
	unsigned long ty=0, temp_y=0;
	unsigned long minx=99999, maxx=0;
	unsigned long miny=99999, maxy=0;
	int datacount=0;

	TP_CS_LOW();
	TP_IRQ_IN();
	for (int i=0; i<prec; i++)
	{
		if (!TP_IRQ_IN_READ())
		{
			touch_WriteData(0x90);        
			temp_x=touch_ReadData();

			if (!TP_IRQ_IN_READ())
			{
				touch_WriteData(0xD0);      
				temp_y=touch_ReadData();
				if ((temp_x>0) and (temp_x<4096) and (temp_y>0) and (temp_y<4096))
				{
					tx+=temp_x;
					ty+=temp_y;
					if (prec>5)
					{
						if (temp_x<minx)
						{
							minx=temp_x;
						}
						if (temp_x>maxx)
						{
							maxx=temp_x;
						}
						if (temp_y<miny)
						{
							miny=temp_y;
						}
						if (temp_y>maxy)
						{
							maxy=temp_y;
						}
					}
					datacount++;
				}
			}
		}
	}
	TP_IRQ_OUT_HIGH();

	if (prec>5)
	{
		tx = tx-(minx+maxx);
		ty = ty-(miny+maxy);
		datacount -= 2;
	}

    TP_CS_HIGH();
    if ((datacount==(prec-2)) or (datacount==PREC_LOW))
    {
        if (orient == default_orientation)
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

bool LUTouch::dataAvailable()
{
    bool avail;
    TP_IRQ_IN();
    avail = !(TP_IRQ_IN_READ());
    TP_IRQ_OUT_HIGH();
    return avail;
}

void LUTouch::setCalibCoefficients(const TouchCalibCoefficients &calibCoeffs)
{
	coeffs = calibCoeffs;
}

int16_t LUTouch::getX()
{
	int16_t x;

	if ((TP_X==-1) or (TP_Y==-1))
	{
		return -1;
	}
	x = coeffs.calibCoeffs.a_x * TP_X + coeffs.calibCoeffs.b_x * TP_Y + coeffs.calibCoeffs.d_x;
	return x;
}

int16_t LUTouch::getY()
{
	int y;

	if ((TP_X==-1) or (TP_Y==-1))
	{
		return -1;
	}
	y = coeffs.calibCoeffs.a_y * TP_X + coeffs.calibCoeffs.b_y * TP_Y + coeffs.calibCoeffs.d_y;
	return y;
}

void LUTouch::setPrecision(byte precision)
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

void LUTouch::calibrateRead()
{
    unsigned long tx=0;
    unsigned long ty=0;
 
	TP_CS_LOW();
 
    touch_WriteData(0x90);        
    tx=touch_ReadData();
 
    touch_WriteData(0xD0);      
    ty=touch_ReadData();
 
	TP_CS_HIGH();
 
    TP_X=ty;
    TP_Y=tx;
}


////////////////////////////////////////////////////////////
/////////////////////// DEMO METHODS ///////////////////////
////////////////////////////////////////////////////////////
extern uint8_t BigFont[];
static int color = 0;
static word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA};
static int  bsize = 4;
static void drawColorMarkerAndBrushSizeHelper(LUTFT &tft, int col)
{
	tft.setColor(VGA_BLACK);
	tft.fillRect(25, 0, 31, 239);
	tft.fillRect(tft.getDisplayXSize()-31, 161, tft.getDisplayXSize()-1, 191);
	tft.setColor(VGA_WHITE);
	tft.drawPixel(25, (col*30)+15);
	for (int i=1; i<7; i++)
	{
		tft.drawLine(25+i, ((col*30)+15)-i, 25+i, ((col*30)+15)+i);
	}
	
	if (color==1)
	{
		tft.setColor(VGA_WHITE);
	}
	else
	{
		tft.setColor(colorlist[col]);
	}
	if (bsize==1)
	{
		tft.drawPixel(tft.getDisplayXSize()-15, 177);
	}
	else
	{
		tft.fillCircle(tft.getDisplayXSize()-15, 177, bsize);
	}
		
	tft.setColor(colorlist[col]);
}

void LUTouch::runDemo(LUTFT &tft, LUTouch &touch, int demo_time_in_secs)
{
	tft.clrScr();
  	tft.setFont(BigFont);

	touch.setPrecision(PREC_HI);
	
	tft.setColor(VGA_WHITE);
	tft.drawLine(32,0,32,tft.getDisplayYSize()-1);
	tft.drawLine(tft.getDisplayXSize()-32,0,tft.getDisplayXSize()-32,tft.getDisplayYSize()-1);
	tft.print("C", tft.getDisplayXSize()-24, 8);
	tft.print("L", tft.getDisplayXSize()-24, 24);
	tft.print("E", tft.getDisplayXSize()-24, 40);
	tft.print("A", tft.getDisplayXSize()-24, 56);
	tft.print("R", tft.getDisplayXSize()-24, 72);
	tft.print("+", tft.getDisplayXSize()-24, 136);
	tft.print("-", tft.getDisplayXSize()-24, 200);
	tft.fillRect(tft.getDisplayXSize()-32,96,tft.getDisplayXSize()-1,128);
	tft.drawLine(tft.getDisplayXSize()-32,160,tft.getDisplayXSize()-1,160);
	tft.drawLine(tft.getDisplayXSize()-32,192,tft.getDisplayXSize()-1,192);
	tft.drawLine(tft.getDisplayXSize()-32,224,tft.getDisplayXSize()-1,224);
	for (int i=0; i<8; i++)
	{
		tft.setColor(colorlist[i]);
		tft.fillRect(0, (i*30), 24, (((i+1)*30)-1));
	}
	drawColorMarkerAndBrushSizeHelper(tft, color);
	long x, y;

	Timer timer;
	timer.start();
	int start_time = timer.read();
	while (timer.read()-start_time < demo_time_in_secs)
	{
		while (touch.dataAvailable() == true)
		{
			touch.read();
			x = touch.getX();
			y = touch.getY();
			//printf("\rX: %d   Y: %d\n", x, y);
			if ((x!=-1) and (y!=-1))
			{
				if (x>(31+bsize) and (x<tft.getDisplayXSize() - (31+bsize)))
				{
					if (bsize == 1)
					{
						tft.drawPixel(x, y);
					}
					else
					{
						tft.fillCircle(x, y, bsize);
					}
				}
				else
				{
					if (x < (30+bsize))
					{
						if (y<240)
						{
							color = y / 30;
							drawColorMarkerAndBrushSizeHelper(tft, color);
							while (touch.dataAvailable()) {};
							wait_ms(50);
						}
					}
					else
					{
						if (y<96)
						{
							tft.setColor(VGA_BLACK);
							tft.fillRect(33, 0, tft.getDisplayXSize()-33, tft.getDisplayYSize()-1);
							tft.setColor(colorlist[color]);
						}
						if ((y > 128) and (y < 160))
						{
							if (bsize < 10)
							{
								bsize++;
								drawColorMarkerAndBrushSizeHelper(tft, color);
								while (touch.dataAvailable()) {};
								wait_ms(50);
							}
						}
						if ((y > 160) and (y < 192))
						{
							bsize = 4;
							drawColorMarkerAndBrushSizeHelper(tft, color);
							while (touch.dataAvailable()) {};
							wait_ms(50);
						}
						if ((y > 192) and (y < 224))
						{
							if (bsize>1)
							{
								bsize--;
								drawColorMarkerAndBrushSizeHelper(tft, color);
								while (touch.dataAvailable()) {};
								wait_ms(50);
							}
						}
					}
				}
			}
		}
  	}
	tft.clrScr();
	tft.setColor(VGA_WHITE);
	tft.print("TOUCH DEMO ENDED!", CENTER, tft.getDisplayYSize()/2);
	tft.print("Touch the screen...", CENTER, tft.getDisplayYSize()/2+20);
	while(!touch.dataAvailable());
	tft.clrScr();
}

