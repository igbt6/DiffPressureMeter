#include "settingsmemory.h"


//-----------------------------------------------------------------------------
SettingsMemory& SettingsMemory::instance()
{
    static SettingsMemory settings;
    return settings;   
}
//-----------------------------------------------------------------------------

SettingsMemory::SettingsMemory()
{

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

