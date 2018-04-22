/*
  LUTouchCalibration - Calibration library support for Color TFT LCD Touch screens  with XPT2046 driver 
  @author Lukasz Uszko aka igbt6 (luszko@op.pl) - April 2018
  Library is based on URTouch library http://www.rinkydinkelectronics.com/library.php?id=92 by Hening Karlsen
*/


#include "LUTouchCalibration.h"

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

LUTouchCalibration::LUTouchCalibration(LUTFT &tft, LUTouch &touch)
    :  tft(tft)
    ,  touch(touch)
{
  setup(); 
}

void LUTouchCalibration::setup()
{
    tft.clrScr();
    tft.setFont(SmallFont);

    touch.InitTouch(tft.getOrientation());
    dispx=tft.getDisplayXSize();
    dispy=tft.getDisplayYSize();
    text_y_center=(dispy/2)-6;
}

void LUTouchCalibration::drawCrossHair(int x, int y)
{
    tft.drawRect(x-10, y-10, x+10, y+10);
    tft.drawLine(x-5, y, x+5, y);
    tft.drawLine(x, y-5, x, y+5);
}

void LUTouchCalibration::readCoordinates()
{
    int iter = 5000;
    int failcount = 0;
    int cnt = 0;
    uint32_t tx=0;
    uint32_t ty=0;
    bool OK = false;

    while (OK == false)
    {
        tft.setColor(255, 255, 255);
        tft.print("*  PRESS  *", CENTER, text_y_center);
        while (touch.dataAvailable() == false) {}
        tft.print("*  HOLD!  *", CENTER, text_y_center);
        while ((touch.dataAvailable() == true) && (cnt<iter) && (failcount < 10000))
        {
            touch.calibrateRead();
            if (!((touch.TP_X == 0xFFFF) || (touch.TP_Y == 0xFFFF)))
            {
              tx += touch.TP_X;
              ty += touch.TP_Y;
              cnt++;
            }
            else
            {
              failcount++;
            }
        }
        if (cnt>=iter)
        {
            OK = true;
        }
        else
        {
          tx = 0;
          ty = 0;
          cnt = 0;
        }
        if (failcount>=10000)
        {
          fail();
        }
    }

    cx = tx / iter;
    cy = ty / iter;
}

void LUTouchCalibration::waitForTouch()
{
  while (touch.dataAvailable() == true) {}
  while (touch.dataAvailable() == false) {}
  while (touch.dataAvailable() == true) {}
}

void LUTouchCalibration::toHex(uint32_t num)
{
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = '\0';
  for (int zz=9; zz>1; zz--)
  {
    if ((num & 0xF) > 9)
    {
      buf[zz] = (num & 0xF) + 55;
    }
    else
    {
      buf[zz] = (num & 0xF) + 48;
    }
    num = num>>4;
  }
}

void LUTouchCalibration::startup()
{
    tft.setColor(255, 0, 0);
    tft.fillRect(0, 0, dispx-1, 13);
    tft.setColor(255, 255, 255);
    tft.setBackColor(255, 0, 0);
    tft.drawLine(0, 14, dispx-1, 14);
    tft.print("LUTouch Calibration", CENTER, 1);
    tft.setBackColor(0, 0, 0);

    tft.print("INSTRUCTIONS", CENTER, 30);
    tft.print("Use a pen or something similar to", LEFT, 50);
    tft.print("touch as close to the center of the", LEFT, 62);
    tft.print("highlighted crosshair as possible. Keep", LEFT, 74);
    tft.print("as still as possible and keep holding", LEFT, 86);
    tft.print("until the highlight is removed. Repeat", LEFT, 98);
    tft.print("for all crosshairs in sequence.", LEFT, 110);

    tft.print("Further instructions will be displayed", LEFT, 134);
    tft.print("when the calibration is complete.", LEFT, 146);

    tft.print("Do NOT use your finger as a calibration", LEFT, 170);
    tft.print("a pen or the result WILL BE imprecise.", LEFT, 182);

    tft.print("Touch screen to continue", CENTER, 226);

    waitForTouch();
    tft.clrScr();
}

void LUTouchCalibration::done()
{
    tft.clrScr();
    tft.setColor(255, 0, 0);
    tft.fillRect(0, 0, dispx-1, 13);
    tft.setColor(255, 255, 255);
    tft.setBackColor(255, 0, 0);
    tft.drawLine(0, 14, dispx-1, 14);
    tft.print("LUTouch Calibration", CENTER, 1);
    tft.setBackColor(0, 0, 0);
    
    tft.print("CALIBRATION COMPLETE", CENTER, 90);
    tft.print("Settings will be saved to memory", CENTER, 120);


    tft.print("a_x", LEFT, 150);
    tft.print("b_x", LEFT, 162);
    tft.print("d_x", LEFT, 174);
    tft.print("a_y", LEFT, 186);
    tft.print("b_y", LEFT, 198);
    tft.print("d_y", LEFT, 210);
    toHex(coeffs.a_x);
    tft.print(buf, 75, 150);
    toHex(coeffs.b_x);
    tft.print(buf, 75, 162);
    toHex(coeffs.d_x);
    tft.print(buf, 75, 174);
    toHex(coeffs.a_y);
    tft.print(buf, 75, 186);
    toHex(coeffs.b_y);
    tft.print(buf, 75, 198);
    toHex(coeffs.d_y);
    tft.print(buf, 75, 210);

    waitForTouch();
    touch.setCalibCoefficients(coeffs);
    tft.clrScr();
}

void LUTouchCalibration::fail()
{
    tft.clrScr();
    tft.setColor(255, 0, 0);
    tft.fillRect(0, 0, dispx-1, 13);
    tft.setColor(255, 255, 255);
    tft.setBackColor(255, 0, 0);
    tft.drawLine(0, 14, dispx-1, 14);
    tft.print("LUTouch Calibration FAILED", CENTER, 1);
    tft.setBackColor(0, 0, 0);
    
    tft.print("Unable to read the position", LEFT, 30);
    tft.print("of the press. This is", LEFT, 42);
    tft.print("probably a hardware issue!", 88, 54);
    while(true) {};
}


void LUTouchCalibration::drawCalibrationPoint(word x, word y, word radius)
{
	tft.drawCircle(x, y, radius);
	tft.fillCircle(x, y, radius - 20);
}

void LUTouchCalibration::calibrate()
{
  startup();
  tft.clrScr();
  TouchPoint p1, p2, p3;
	TouchPoint t1, t2, t3;
	// point 1 is at 25%,50%, 2 is at 75%,25% and 3 is at 75%,75%
  word size_x = tft.getDisplayXSize();
	word size_y = tft.getDisplayYSize();
	p1.x = (size_x * 25) / 100;
	p1.y = (size_y * 50) / 100;
	p2.x = (size_x * 75) / 100;
	p2.y = (size_y * 25) / 100;
	p3.x = (size_x * 75) / 100;
	p3.y = (size_y * 75) / 100;
  tft.setColor(255, 255, 255);
  tft.setBackColor(255, 0, 0);
  tft.print("***********", CENTER, text_y_center-12);
  tft.print("***********", CENTER, text_y_center+12);
  
  tft.setColor(VGA_RED);
  drawCalibrationPoint(p1.x, p1.y, 30);
  readCoordinates();
  t1.x = cx;
  t1.y = cy;
  tft.setColor(VGA_WHITE);
  tft.print("* RELEASE *", CENTER, text_y_center);
  tft.setColor(VGA_GREEN);
  drawCalibrationPoint(p1.x, p1.y, 30);
  wait_ms(2000);

  tft.setColor(VGA_RED);
  drawCalibrationPoint(p2.x, p2.y, 30);
  readCoordinates();
  t2.x = cx;
  t2.y = cy;
  tft.setColor(VGA_WHITE);
  tft.print("* RELEASE *", CENTER, text_y_center);
  tft.setColor(VGA_GREEN);
  drawCalibrationPoint(p2.x, p2.y, 30);
  wait_ms(2000);

  tft.setColor(VGA_RED);
  drawCalibrationPoint(p3.x, p3.y, 30);
  readCoordinates();
  t3.x = cx;
  t3.y = cy;
  tft.setColor(VGA_WHITE);
  tft.print("* RELEASE *", CENTER, text_y_center);
  tft.setColor(VGA_GREEN);
  drawCalibrationPoint(p3.x, p3.y, 30);
  wait_ms(2000);
  //Formula based on:
	//https://www.maximintegrated.com/en/app-notes/index.mvp/id/5296
	//and
	//http://www.ti.com/lit/an/slyt277/slyt277.pdf
	int32_t delta, deltaX1, deltaX2, deltaX3, deltaY1, deltaY2, deltaY3;
	// intermediate values for the calculation
	delta = ((int32_t) (t1.x - t3.x) * (int32_t) (t2.y - t3.y))
			  - ((int32_t) (t2.x - t3.x) * (int32_t) (t1.y - t3.y));

	deltaX1 = ((int32_t) (p1.x - p3.x) * (int32_t) (t2.y - t3.y))
			      - ((int32_t) (p2.x - p3.x) * (int32_t) (t1.y - t3.y));
	deltaX2 = ((int32_t) (t1.x - t3.x) * (int32_t) (p2.x - p3.x))
			      - ((int32_t) (t2.x - t3.x) * (int32_t) (p1.x - p3.x));
	deltaX3 = p1.x * ((int32_t) t2.x * (int32_t) t3.y
						- (int32_t) t3.x * (int32_t) t2.y)
					  - p2.x * (t1.x * (int32_t) t3.y
						- (int32_t) t3.x * (int32_t) t1.y)
					  + p3.x * ((int32_t) t1.x * (int32_t) t2.y
						- (int32_t) t2.x * (int32_t) t1.y);

	deltaY1 = ((int32_t) (p1.y - p3.y) * (int32_t) (t2.y - t3.y))
			      - ((int32_t) (p2.y - p3.y) * (int32_t) (t1.y - t3.y));
	deltaY2 = ((int32_t) (t1.x - t3.x) * (int32_t) (p2.y - p3.y))
			      - ((int32_t) (t2.x - t3.x) * (int32_t) (p1.y - p3.y));
	deltaY3 = p1.y * ((int32_t) t2.x * (int32_t) t3.y
						- (int32_t) t3.x * (int32_t) t2.y)
					  - p2.y * ((int32_t) t1.x * (int32_t) t3.y
						- (int32_t) t3.x * (int32_t) t1.y)
					  + p3.y * ((int32_t) t1.x * (int32_t) t2.y
						- (int32_t) t2.x * t1.y);

	// final values
	coeffs.a_x = (float) deltaX1 / (float) delta;
	coeffs.b_x = (float) deltaX2 / (float) delta;
	coeffs.d_x = (float) deltaX3 / (float) delta;

	coeffs.a_y = (float) deltaY1 / (float) delta;
	coeffs.b_y = (float) deltaY2 / (float) delta;
	coeffs.d_y = (float) deltaY3 / (float) delta;

  wait_ms(3000);
  // CHECK
  const uint8_t successThreshold = 20;
  TouchPoint testPoint;
  TouchPoint result;
  testPoint.x = size_x/2;
	testPoint.y = size_y/2;
  tft.setColor(VGA_YELLOW);
  drawCalibrationPoint(testPoint.x, testPoint.y, 30);
  readCoordinates();
	result.x = coeffs.a_x * cx + coeffs.b_x * cy + coeffs.d_x;
	result.y = coeffs.a_y * cx + coeffs.b_y * cy + coeffs.d_y;
  tft.setColor(VGA_WHITE);
  tft.print("* RELEASE *", CENTER, text_y_center);
  wait_ms(500);
  tft.setColor(VGA_BLUE);
  drawCalibrationPoint(testPoint.x, testPoint.y, 30);
  tft.clrScr();
  tft.setFont(BigFont);
  tft.setColor(VGA_WHITE);
  if(((result.x) > (testPoint.x+successThreshold)) ||
    ((result.x) < (testPoint.x-successThreshold)) ||
    ((result.y) > (testPoint.y+successThreshold)) ||
    ((result.y) < (testPoint.y-successThreshold)))
  {
    tft.print("Calibration failed:(", CENTER, text_y_center-40);
    tft.print("X: ", dispx/2-50, text_y_center-10);
    tft.printNumI(result.x, dispx/2+20, text_y_center-10);
    tft.print("Y: ", dispx/2-50, text_y_center+20);
    tft.printNumI(result.y, dispx/2+20, text_y_center+20);
  }
  else
  {
    tft.print("Calibration success:)", CENTER, text_y_center-40);
    tft.print("X: ", dispx/2-50, text_y_center-10);
    tft.printNumI(result.x, dispx/2+20, text_y_center-10);
    tft.print("Y: ", dispx/2-50, text_y_center+20);
    tft.printNumI(result.y, dispx/2+20, text_y_center+20);
  }
  tft.setFont(SmallFont);
  wait_ms(4000);
  done();
}