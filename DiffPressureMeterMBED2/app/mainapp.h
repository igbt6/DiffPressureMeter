#ifndef MAINAPP_H
#define MAINAPP_H

#include "system.h"
#include "LUTft.h"
#include "LUTouch.h"
#include "LUTouchCalibration.h"
#include "externalrtc.h"
#include "pressuresensor.h"
#include "interfaces.h"
#include "settingsdatetime.h"
#include "settingsmeasurement.h"
#include "measurement.h"
#include "settingsmemory.h"




class MainApp
{
public:
    enum AppState
    {
        sIntro        = -1,
        sMainMenu     =  0,
        sMeasurement  =  1,
        sSettingsTime =  2,
        sSettingsMeas =  3,
        sInfo         =  4,
    };

    static MainApp& instance();
    void run();

private:
  MainApp();
  MainApp & operator=(const MainApp& rtc);
  void mainMenu();

private:
    LUTFT tft;
    LUTouch touch;
    SettingsDateTime settingsDateTime;
    SettingsMeasurement settingsMeasurement;
    Measurement measurement;
    AppState currentState;
};


#endif