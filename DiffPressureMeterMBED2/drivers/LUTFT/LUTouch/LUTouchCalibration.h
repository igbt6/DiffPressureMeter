/*
  LUTouchCalibration - Calibration library support for Color TFT LCD Touch screens  with XPT2046 driver 
  @author Lukasz Uszko aka igbt6 (luszko@op.pl) - April 2018
  Library is based on URTouch library http://www.rinkydinkelectronics.com/library.php?id=92 by Hening Karlsen
*/
#ifndef LUTOUCHCALIBRATION_H
#define LUTOUCHCALIBRATION_H
#include "LUTft.h"
#include "LUTouch.h"



class LUTouchCalibration
{
public:
    LUTouchCalibration(LUTFT &tft, LUTouch &touch); 
    void setup();
    void drawCrossHair(int x, int y);
    void readCoordinates();
    void waitForTouch();
    void toHex(uint32_t num);
    void startup();
    void done();
    void fail();
    void calibrate();

private:
    void drawCalibrationPoint(word x, word y, word radius);   

private:
    LUTFT &tft;
    LUTouch &touch;
    CalibCoefficients coeffs;
    uint32_t cx, cy;
    int dispx, dispy, text_y_center;
    char buf[11];
};
#endif