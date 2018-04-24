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
    setDefaults();
    readSettings(m_appSettings);
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
void SettingsMemory::setDefaults()
{
    // --- Set defaults ---
    // touch calib coeffs
    m_appSettings.calibCoeffs.calibCoeffsMemory[0] =    0xBB3C166F;
    m_appSettings.calibCoeffs.calibCoeffsMemory[1] =    0x3E353AF3;
    m_appSettings.calibCoeffs.calibCoeffsMemory[2] =    0xC225E49F;
    m_appSettings.calibCoeffs.calibCoeffsMemory[3] =    0xBE04C36F;
    m_appSettings.calibCoeffs.calibCoeffsMemory[4] =    0xBB9D85FD;
    m_appSettings.calibCoeffs.calibCoeffsMemory[5] =    0x43843904;
    // measuremnt times
    m_appSettings.measurementStartDelaySecs =           5;
    m_appSettings.measurementTimeSecs =                 10;
}

//-----------------------------------------------------------------------------
SettingsFootprint SettingsMemory::appSettings()
{
    return m_appSettings;
}
