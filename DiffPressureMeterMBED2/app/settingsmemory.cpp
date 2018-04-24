#include "settingsmemory.h"


static const SettingsFootprint kDefaultAppSettings =
{
    calibCoeffs = 
    {

    }
    measurementStartDelaySecs = 5;
    measurementTimeSecs = 10;
}

//-----------------------------------------------------------------------------
SettingsMemory& SettingsMemory::instance()
{
    static SettingsMemory settings;
    return settings;   
}
//-----------------------------------------------------------------------------

SettingsMemory::SettingsMemory()
    : appSettings(kDefaultAppSettings)
{
    readSettings(appSettings));
}
//-----------------------------------------------------------------------------
bool SettingsMemory::saveSettings(const SettingsFootprint &settings)
{
    return false;
}

//-----------------------------------------------------------------------------
bool SettingsMemory::readSettings(SettingsFootprint &settings)
{
    return false; //TODO
}

//-----------------------------------------------------------------------------
SettingsFootprint SettingsMemory::appSettings()
{
    return appSettings;
}
