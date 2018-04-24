#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "LUTft.h"
#include "LUTouch.h"

class Measurement
{

public:
    Measurement(LUTFT &tft, LUTouch &touch);
    void run();

private:
    LUTFT   &tft;
    LUTouch &touch;
};

#endif