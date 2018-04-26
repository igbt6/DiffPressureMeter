#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "system.h"
#include "LUTft.h"
#include "LUTouch.h"
#include "pressuresensor.h"
#include "resultprinter.h"

class Measurement
{

public:
    Measurement(LUTFT &tft, LUTouch &touch);
    void run();

private:
    bool start();
    bool measure();
    bool postProcess();
private:
    LUTFT   &tft;
    LUTouch &touch;
    PressureSensor pressureSensor;
    DigitalOut relayPin;
};

#endif