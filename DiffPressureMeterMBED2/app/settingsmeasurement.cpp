#include "settingsmeasurement.h"
#include "settingsmemory.h"
#include "externalrtc.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

using namespace rtctime;

//-----------------------------------------------------------------------------
SettingsMeasurement::SettingsMeasurement(LUTFT &tft, LUTouch &touch)
  : SettingsHelper(tft, touch)
  , tft(tft)
  , touch(touch)
{

}

//-----------------------------------------------------------------------------
void SettingsMeasurement::setMeasurement()
{
    int x, y;
    int res = 0;
    bool measTimeChanged = false;
    bool measStartDelayTimeChanged = false;
    
    waitForTouchRelease();    
    
    tft.clrScr();
    tft.setFont(BigFont);
    
    // Draw Save button
    drawSaveButton();
    // Draw Cancel button
    drawCancelButton();
    
    // Draw frames
    tft.setColor(VGA_BLUE);
    tft.drawRoundRect(0, 0, 319, 96);
    tft.drawRoundRect(0, 100, 319, 196);
    tft.setFont(SmallFont);
    tft.setColor(VGA_WHITE);
    tft.print("Opoznienie startu", 10, 40);
    tft.print("pomiaru:", 10, 52);
    tft.print("Czas pomiaru:", 10, 140);

    tft.setFont(BigFont);
    tft.print(":", 230, 40);
    tft.print(":", 230, 140);
    
    // Draw Buttons
    // buttons measurement start delay time
    drawUpButton(180, 10);
    drawUpButton(260, 10);
    drawDownButton(180, 60);
    drawDownButton(260, 60);
    // buttons measurement time
    drawUpButton(180, 110);
    drawUpButton(260, 110);
    drawDownButton(180, 160);
    drawDownButton(260, 160);
    
    // get current time
    tft.setColor(VGA_WHITE);
    SettingsFootprint settings = SettingsMemory::instance().appSettings();
    
    validateStartMeasurementDelayTime(settings.measurementStartDelaySecs);
    validateMeasurementTime(settings.measurementTimeSecs);

    while (res==0)
    {
        if (touch.dataAvailable())
        {
            touch.read();
            x=touch.getX();
            y=touch.getY();
            if ((y>=10) && (y<=35))  // buttons measurement start delay time UP
            {
                if ((x>=180) && (x<=212)) 
                {
                    buttonWait(180, 10);
                    settings.measurementStartDelaySecs += 60;
                    measStartDelayTimeChanged = true;
                }
                else if ((x>=260) && (x<=292))
                {
                    buttonWait(260, 10);
                    settings.measurementStartDelaySecs += 1;
                    measStartDelayTimeChanged = true;
                }
            }
            else if ((y>=60) && (y<=85)) // buttons measurement start delay time DOWN
            {
                if ((x>=180) && (x<=212))
                {
                    buttonWait(180, 60);
                    settings.measurementStartDelaySecs -= 60;
                    measStartDelayTimeChanged = true;
                }
                else if ((x>=260) && (x<=292))
                {
                    buttonWait(260, 60);
                    settings.measurementStartDelaySecs -= 1;
                    measStartDelayTimeChanged = true;
                }
            }
            if ((y>=110) && (y<=135)) // buttons measurement time UP
            {
                if ((x>=180) && (x<=212)) // min
                {
                    buttonWait(180, 110);
                    settings.measurementTimeSecs += 60;
                    measTimeChanged = true;
                }
                else if ((x>=260) && (x<=292)) // sec
                {
                    buttonWait(260, 110);
                    settings.measurementTimeSecs += 1;
                    measTimeChanged = true;
                }
            }
            else if ((y>=160) && (y<=185)) // buttons measurement time DOWN
            {
                if ((x>=180) && (x<=212)) // min 
                {
                    buttonWait(180, 160);
                    settings.measurementTimeSecs -= 60;
                    measTimeChanged = true;
                }
                else if ((x>=260) && (x<=292)) // sec
                {
                    buttonWait(260, 160);
                    settings.measurementTimeSecs -= 1;
                    measTimeChanged = true;
                }
            }
            else if ((y>=200) && (y<=239)) // SAVE/CANCEL buttons
            {
                if ((x>=165) && (x<=319))
                {
                    // SAVE button clicked
                    res = 1;
                    tft.setColor (255, 0, 0);
                    tft.drawRoundRect(165, 200, 319, 239);
                }
                else if ((x>=0) && (x<=154))
                {
                    res = 2;
                    tft.setColor (255, 0, 0);
                    tft.drawRoundRect(0, 200, 154, 239);
                }
            }
            if (measTimeChanged)
            {
                validateMeasurementTime(settings.measurementTimeSecs);
                measTimeChanged = false;
            }
            if (measStartDelayTimeChanged)
            {
                validateStartMeasurementDelayTime(settings.measurementStartDelaySecs);
                measStartDelayTimeChanged = false;
            }
        }
    }
    waitForTouchRelease();

    if (res==1)
    {
        if (measTimeChanged or measStartDelayTimeChanged)
        {
            DEBUG("Measurement setting will be saved here...");
            SettingsMemory::instance().saveSettings(settings);
        }
    }
}

bool SettingsMeasurement::validateStartMeasurementDelayTime(word &seconds)
{
    // measurement start delay time
    if (seconds > 3599)
    {
        seconds = 0;
    }

    uint32_t min = seconds / 60;
    uint32_t sec = seconds % 60;
    if (min < 10)
    {
        tft.printNumI(0, 180, 40);
        tft.printNumI(min, 196, 40);
    }
    else
    {
        tft.printNumI(min, 180, 40);  
    }
    if (sec < 10)
    {
        tft.printNumI(0, 260, 40);
        tft.printNumI(sec, 276, 40);
    }
    else
    {
        tft.printNumI(sec, 260, 40);  
    }
    return true;
}

bool SettingsMeasurement::validateMeasurementTime(word &seconds)
{
    // measurement time
    if (seconds > 3599)
    {
        seconds = 0;
    }
    uint32_t min = seconds / 60;
    uint32_t sec = seconds % 60;
    if (min < 10)
    {
        tft.printNumI(0, 180, 140);
        tft.printNumI(min, 196, 140);
    }
    else
    {
        tft.printNumI(min, 180, 140);  
    }
    if (sec < 10)
    {
        tft.printNumI(0, 260, 140);
        tft.printNumI(sec, 276, 140);
    }
    else
    {
        tft.printNumI(sec, 260, 140);  
    }
    return true;
}