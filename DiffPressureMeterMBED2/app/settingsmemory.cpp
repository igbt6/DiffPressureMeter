#include "settingsmemory.h"

#define EEPROM_ADDR 0x07   // I2c EEPROM address is 0xAE
//-----------------------------------------------------------------------------
SettingsMemory& SettingsMemory::instance()
{
    static SettingsMemory settings;
    return settings;   
}
//-----------------------------------------------------------------------------

SettingsMemory::SettingsMemory()
    : eeprom(DS3231_I2C_SDA_PIN, DS3231_I2C_SCL_PIN, EEPROM_ADDR)
{
    setDefaults();
    readSettings(m_appSettings);
    testEeprom(eeprom);
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


//-----------------------------------------------------------------------------
void SettingsMemory::testEeprom(EEPROM &ep)
{
    ep.Write(3, (unsigned char)0xac);
    uint8_t value = 0xcc;
    if (ep.Read(3, &value))
    {
        DEBUG("VALUE read from addr 128: 0x%x", value);
    }
    else
    {
        DEBUG_ERROR("VALUE read from addr 128 failed!");
    }
    //ep.DumpMemoryArea(0, 30);
}

//-----------------------------------------------------------------------------