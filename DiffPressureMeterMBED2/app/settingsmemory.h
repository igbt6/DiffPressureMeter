#ifndef SETTINGSMEMORY_H
#define SETTINGSMEMORY_H
#include "system.h"
#include "LUTouch.h" // for TouchCalibCoefficients
#include "24LCxx_I2C.h"

typedef _24LCXX_I2C::C24LCXX_I2C  EEPROM;
struct SettingsFootprint
{
    TouchCalibCoefficients calibCoeffs;
    uint16_t measurementStartDelaySecs;
    uint16_t measurementTimeSecs;
};

class SettingsMemory
{

public:
    static SettingsMemory& instance();
    bool saveSettings(const SettingsFootprint &settings);
    bool readSettings(SettingsFootprint &settings);
    void setDefaults();
    // returns copy of the app settings
    SettingsFootprint appSettings();

    // eeprom tests
    static void testEeprom(EEPROM &ep);

private:
    SettingsMemory();
    SettingsMemory& operator=(const SettingsMemory& memory);
    SettingsFootprint m_appSettings;
    EEPROM eeprom;
};



#endif