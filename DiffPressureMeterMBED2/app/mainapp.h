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
        sSettings     =  2,
        sInfo         =  3,
    };

    static MainApp& instance();
    void run();

private:
  MainApp();
  MainApp & operator=(const MainApp& rtc);
  int mainMenu();
  int settingsMenu();
  void drawFrame();
  void updateTime();
  void updateTimeTask();

private:
    LUTFT tft;
    LUTouch touch;
    SettingsDateTime settingsDateTime;
    SettingsMeasurement settingsMeasurement;
    Measurement measurement;
    AppState currentState;
    volatile bool _updateTime;
};


#endif