#ifndef SETTINGSMEASUREMENT_H
#define SETTINGSMEASUREMENT_H

#include "settingshelper.h" 

class SettingsMeasurement : public SettingsHelper
{

public:
    SettingsMeasurement(LUTFT &tft, LUTouch &touch);
    void setMeasurement();
    
private:
    static bool validateStartMeasurementTime(word seconds);
    static bool validateMeasurementTime(word seconds);

private:
    LUTFT   &tft;
    LUTouch &touch;
};

#endif