#include "settingsmemory.h"

#define EEPROM_ADDR 0x07   // I2c EEPROM address is 0xAE
#define MAX_SETTINGS_LEN 8192 // 8kB
static const uint8_t k_settingsHeader[] = "LUx1"; // 1 - current version 
static const uint32_t k_settingsHeaderLen = 4;
static uint8_t dataBuffer[MAX_SETTINGS_LEN];
//-----------------------------------------------------------------------------
SettingsMemory& SettingsMemory::instance()
{
    static SettingsMemory settings;
    return settings;
}
//-----------------------------------------------------------------------------

SettingsMemory::SettingsMemory()
    : eeprom(DS3231_I2C_SDA_PIN, DS3231_I2C_SCL_PIN, EEPROM_ADDR, EEPROM::T24C32)
{
    // saveDefaults(); // call only once after whole eeprom erase done
    if (!readSettings())
    {
        setDefaults();
    }
}
//-----------------------------------------------------------------------------
bool SettingsMemory::saveSettings(const SettingsFootprint &settings)
{
    const uint32_t dataLen = sizeof(SettingsFootprint);
    m_appSettings = settings;
    eeprom.write(k_settingsHeaderLen, (int8_t*)&settings, dataLen);
    if(eeprom.getError() != 0)
    {
        DEBUG_ERROR("saveSettings error: %s", eeprom.getErrorMessage().c_str());
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool SettingsMemory::readSettings()
{
    const uint32_t dataLen = k_settingsHeaderLen + sizeof(SettingsFootprint);
    for(int i = 0; i < dataLen; i++)
    {
        int8_t ival;
        eeprom.read(i,(int8_t&)ival);
        dataBuffer[i] = (uint8_t)ival;
        if(eeprom.getError() != 0)
        {
            DEBUG_ERROR(eeprom.getErrorMessage().c_str());
            return false;
        }
    }
    DEBUG("\nResults: readSettings of size: %d bytes :\n", dataLen);
    for(int i = 0; i < dataLen/16; i++)
    {
        for(int j = 0;j < 16;j++)
        {
            uint32_t addr = i * 16 + j;
            DEBUG("0x%x ",(uint8_t)dataBuffer[addr]);
        }
    }
    // check header
    memcpy(&m_appSettings, &dataBuffer[k_settingsHeaderLen], sizeof(SettingsFootprint));
    return true;
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
    #define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
    #define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
    uint8_t data[256],data_r[256];
    int8_t ival;
    uint16_t s;
    int16_t sdata,sdata_r;
    int32_t ldata[1024];
    int32_t eeprom_size,max_size;
    uint32_t addr;
    int32_t idata,idata_r;
    uint32_t i,j,k,l,t,id;
    eeprom_size = ep.getSize();
    max_size = MIN(eeprom_size,256);
    DEBUG("Test EEPROM I2C model %s of %d bytes\n", ep.getName(), eeprom_size);
    // Test write byte (max_size first bytes)
    DEBUG("\nTest write %d first bytes...\n",max_size);
    for(i = 0;i < max_size;i++)
        data[i] = i;
    
    for(i = 0;i < max_size;i++)
    {
        ep.write(i,(int8_t)data[i]);
        if(ep.getError() != 0)
        {
            DEBUG_ERROR(ep.getErrorMessage().c_str());
        }
    }
    
    DEBUG("Test read %d first bytes...",max_size);   
    // Test read byte (max_size first bytes)
    for(i = 0;i < max_size;i++)
    {
        ep.read(i,(int8_t&)ival);
        data_r[i] = (uint8_t)ival;
        if(ep.getError() != 0)
        {
            DEBUG_ERROR(ep.getErrorMessage().c_str());
        }
    }
    
    DEBUG("\nResults: Test write and read %d first bytes :\n",max_size);
    for(i = 0;i < max_size/16;i++)
    {
        for(j = 0;j < 16;j++)
        {
            addr = i * 16 + j;
            DEBUG("%3d ",(uint8_t)data_r[addr]);
        }
    }
}

//-----------------------------------------------------------------------------
bool SettingsMemory::saveDefaults()
{
    const uint32_t dataLen = sizeof(SettingsFootprint);
    setDefaults();
    eeprom.write(0, (int8_t*)k_settingsHeader, k_settingsHeaderLen);
    if(eeprom.getError() != 0)
    {
        DEBUG_ERROR(eeprom.getErrorMessage().c_str());
        return false;
    }
    eeprom.write(k_settingsHeaderLen, (int8_t*)&m_appSettings, dataLen);
    if(eeprom.getError() != 0)
    {
        DEBUG_ERROR(eeprom.getErrorMessage().c_str());
        return false;
    }
    return true;
}