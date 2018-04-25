#include "measurement.h"
#include "settingsmemory.h"

//-----------------------------------------------------------------------------
Measurement::Measurement(LUTFT &tft, LUTouch &touch)
    : tft(tft)
    , touch(touch)
    , pressureSensor()
    , relayPin(RELAY_OUT_PIN, LOW)
{
}

//-----------------------------------------------------------------------------
void Measurement::run()
{
    Timer measurementTimer;
    measurementTimer.start();
    int startTime = measurementTimer.read();
    relayPin.write(LOW);
    while ((measurementTimer.read() - startTime) < SettingsMemory::instance().appSettings().measurementTimeSecs)
    {
        pressureSensor.performMeasurement();
        DEBUG("Last_result: %f", pressureSensor.getLastResult());
    }
    relayPin.write(HIGH); 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------