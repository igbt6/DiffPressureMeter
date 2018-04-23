#ifndef SETTINGSDATETIME_H
#define SETTINGSDATETIME_H

#include "settingshelper.h" 

class SettingsDateTime : public SettingsHelper
{

public:
    SettingsDateTime(LUTFT &tft, LUTouch &touch);
    void setClock();
    
private:
    void showDOW(byte dow);
    byte validateDateForMonth(byte d, byte m, word y);
    static char uCase(char c);
    static byte calcDOW(byte d, byte m, int y);
    static byte validateDate(byte d, byte m, word y);

private:
    LUTFT   &tft;
    LUTouch &touch;
};

#endif