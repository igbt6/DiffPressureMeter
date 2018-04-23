#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include "LUTft.h"
#include "LUTouch.h"

class SettingsHelper
{

public:
    SettingsHelper(LUTFT &tft, LUTouch &touch);

protected:
    void drawUpButton(int x, int y);   
    void drawDownButton(int x, int y);
    void drawCancelButton();
    void drawSaveButton();
    void waitForTouchRelease();
    void buttonWait(int x, int y);

private:
    LUTFT   &tft;
    LUTouch &touch;
};

#endif