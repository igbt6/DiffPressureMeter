#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "system.h"
#include "LUTft.h"
#include "LUTouch.h"
#include "pressuresensor.h"

class Measurement
{

public:
    Measurement(LUTFT &tft, LUTouch &touch);
    void run();

private:
    LUTFT   &tft;
    LUTouch &touch;
    PressureSensor pressureSensor;
    DigitalOut relayPin;
};

#endif