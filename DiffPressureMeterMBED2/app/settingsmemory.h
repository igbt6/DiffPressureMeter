#ifndef SETTINGSMEMORY_H
#define SETTINGSMEMORY_H
#include "system.h"

struct SettingsFootprint
{
    uint32_t calibCoeffs[6];
    uint16_t measurementStartDelaySecs;
    uint16_t measurementTimeSecs;
};

class SettingsMemory
{

public:
    static SettingsMemory& instance();
    bool saveSettings(const SettingsFootprint &settings);
    bool readSettings(SettingsFootprint &settings);

private:
    SettingsMemory();
    SettingsMemory& operator=(const SettingsMemory& memory);
};



#endif