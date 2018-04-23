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
    bool ct=false;
    bool cd=false;
    
    waitForTouchRelease();    
    
    tft.clrScr();
    tft.setFont(BigFont);
    
    // Draw Save button
    drawSaveButton();
    // Draw Cancel button
    drawCancelButton();
    
    // Draw frames
    tft.setColor(0, 0, 255);
    tft.drawRoundRect(0, 0, 319, 96);
    tft.drawRoundRect(0, 100, 319, 196);
    tft.setFont(SmallFont);
    tft.print("Meas start delay:", 10, 40);
    tft.print("Meas time:", 10, 140);

    tft.setFont(BigFont);
    tft.print(":", 230, 40);
    tft.print(":", 230, 140);
    
    // Draw Buttons
    drawUpButton(180, 10);
    drawUpButton(260, 10);
    drawDownButton(180, 60);
    drawDownButton(260, 60);
    
    drawUpButton(180, 110);
    drawUpButton(260, 110);
    drawDownButton(180, 160);
    drawDownButton(260, 160);
    
    // Print current times
    tft.setColor(255, 255, 255);
    SettingsFootprint settings;
    if (!SettingsMemory::instance().readSettings(settings))
    {
        // defaults
        settings.measurementStartDelaySecs = 0;
        settings.measurementTimeSecs = 0;
        ct=true;
        cd=true;
        printf("\r Get current settings from memory error occured!\t\n");  
    }

    // measurement start delay time
    uint32_t min = settings.measurementStartDelaySecs / 60;
    uint32_t sec = settings.measurementStartDelaySecs % 60;
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

    // measurement time
    min = settings.measurementTimeSecs / 60;
    sec = settings.measurementTimeSecs % 60;
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
   /*
    while (res==0)
    {
        if (touch.dataAvailable())
        {
            touch.read();
            x=touch.getX();
            y=touch.getY();
            if ((y>=10) && (y<=35)) // Buttons: measurement delay time UP
            {
                if ((x>=180) && (x<=212))
                {
                    buttonWait(180, 10);
                    settings.measurementStartDelaySecs += 1;
                    uint32_t min = settings.measurementStartDelaySecs / 60;
                    uint32_t sec = settings.measurementStartDelaySecs % 60;
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
                    if (ct==false)
                    {
                        ct=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 10);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
                else if ((x>=218) && (x<=250))
                {
                    buttonWait(218, 10);
                    measurementTimeSecs += 1;
                     min = settings.measurementTimeSecs / 60;
                    sec = settings.measurementTimeSecs % 60;
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
                    if (ct==false)
                    {
                        ct=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 10);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
            }
            else if ((y>=61) && (y<=86)) // Buttons: Time DOWN
            {
                if ((x>=122) && (x<=154))
                {
                    buttonWait(122, 61);
                    t_temp.hours-=1;
                    if (t_temp.hours==255)
                        t_temp.hours=23;
                    if (t_temp.hours<10)
                    {
                        tft.printNumI(0, 122, 40);
                        tft.printNumI(t_temp.hours, 138, 40);
                    }
                    else
                    {
                        tft.printNumI(t_temp.hours, 122, 40);
                    }
                    if (ct==false)
                    {
                        ct=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 10);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
                else if ((x>=170) && (x<=202))
                {
                    buttonWait(170, 61);
                    t_temp.minutes-=1;
                    if (t_temp.minutes==255)
                        t_temp.minutes=59;
                    if (t_temp.minutes<10)
                    {
                        tft.printNumI(0, 170, 40);
                        tft.printNumI(t_temp.minutes, 186, 40);
                    }
                    else
                    {
                        tft.printNumI(t_temp.minutes, 170, 40);
                    }
                    if (ct==false)
                    {
                        ct=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 10);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
                else if ((x>=218) && (x<=250))
                {
                    buttonWait(218, 61);
                    t_temp.seconds-=1;
                    if (t_temp.seconds==255)
                        t_temp.seconds=59;
                    if (t_temp.seconds<10)
                    {
                        tft.printNumI(0, 218, 40);
                        tft.printNumI(t_temp.seconds, 234, 40);
                    }
                    else
                    {
                        tft.printNumI(t_temp.seconds, 218, 40);
                    }
                    if (ct==false)
                    {
                        ct=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 10);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
            }
            if ((y>=110) && (y<=135)) // Buttons: Date UP
            {
                if ((x>=122) && (x<=154))
                {
                    buttonWait(122, 110);
                    t_temp.date+=1;
                    t_temp.date=validateDate(t_temp.date, t_temp.month, t_temp.year);
                    if (t_temp.date<10)
                    {
                        tft.printNumI(0, 122, 140);
                        tft.printNumI(t_temp.date, 138, 140);
                    }
                    else
                    {
                        tft.printNumI(t_temp.date, 122, 140);
                    }
                    if (cd==false)
                    {
                        cd=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 110);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
                else if ((x>=170) && (x<=202))
                {
                    buttonWait(170, 110);
                    t_temp.month+=1;
                    if (t_temp.month==13)
                        t_temp.month=1;
                    if (t_temp.month<10)
                    {
                        tft.printNumI(0, 170, 140);
                        tft.printNumI(t_temp.month, 186, 140);
                    }
                    else
                    {
                        tft.printNumI(t_temp.month, 170, 140);
                    }
                    t_temp.date=validateDateForMonth(t_temp.date, t_temp.month, t_temp.year);
                    if (cd==false)
                    {
                        cd=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 110);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
                else if ((x>=218) && (x<=250))
                {
                    buttonWait(234, 110);
                    t_temp.year+=1;
                    if (t_temp.year==2100)
                        t_temp.year=2000;
                    tft.printNumI(t_temp.year, 218, 140);
                    t_temp.date=validateDateForMonth(t_temp.date, t_temp.month, t_temp.year);
                    if (cd==false)
                    {
                        cd=true;
                        tft.setColor(64, 64, 128);
                        tft.setFont(SmallFont);
                        tft.print("Changed!", 10, 110);
                        tft.setColor(255, 255, 255);
                        tft.setFont(BigFont);
                    }
                }
                t_temp.day=calcDOW(t_temp.date, t_temp.month, t_temp.year);
                showDOW(t_temp.day);
            }
            else if ((y>=161) && (y<=186)) // Buttons: Date DOWN
            {
            if ((x>=122) && (x<=154))
            {
                buttonWait(122, 161);
                t_temp.date-=1;
                t_temp.date=validateDate(t_temp.date, t_temp.month, t_temp.year);
                if (t_temp.date<10)
                {
                    tft.printNumI(0, 122, 140);
                    tft.printNumI(t_temp.date, 138, 140);
                }
                else
                {
                    tft.printNumI(t_temp.date, 122, 140);
                }
                if (cd==false)
                {
                cd=true;
                tft.setColor(64, 64, 128);
                tft.setFont(SmallFont);
                tft.print("Changed!", 10, 110);
                tft.setColor(255, 255, 255);
                tft.setFont(BigFont);
                }
            }
            else if ((x>=170) && (x<=202))
            {
                buttonWait(170, 161);
                t_temp.month-=1;
                if (t_temp.month==0)
                    t_temp.month=12;
                if (t_temp.month<10)
                {
                    tft.printNumI(0, 170, 140);
                    tft.printNumI(t_temp.month, 186, 140);
                }
                else
                {
                    tft.printNumI(t_temp.month, 170, 140);
                }
                t_temp.date=validateDateForMonth(t_temp.date, t_temp.month, t_temp.year);
                if (cd==false)
                {
                    cd=true;
                    tft.setColor(64, 64, 128);
                    tft.setFont(SmallFont);
                    tft.print("Changed!", 10, 110);
                    tft.setColor(255, 255, 255);
                    tft.setFont(BigFont);
                }
            }
            else if ((x>=218) && (x<=250))
            {
                buttonWait(234, 161);
                t_temp.year-=1;
                if (t_temp.year==1999)
                {
                t_temp.year=2099;
                }
                tft.printNumI(t_temp.year, 218, 140);
                t_temp.date=validateDateForMonth(t_temp.date, t_temp.month, t_temp.year);
                if (cd==false)
                {
                    cd=true;
                    tft.setColor(64, 64, 128);
                    tft.setFont(SmallFont);
                    tft.print("Changed!", 10, 110);
                    tft.setColor(255, 255, 255);
                    tft.setFont(BigFont);
                }
            }
            t_temp.day=calcDOW(t_temp.date, t_temp.month, t_temp.year);
            showDOW(t_temp.day);
            }
            else if ((y>=200) && (y<=239)) // SAVE/CANCEL buttons
            {
                if ((x>=165) && (x<=319))
                {
                // SAVE button clicked
                res=1;
                tft.setColor (255, 0, 0);
                tft.drawRoundRect(165, 200, 319, 239);
                }
                else if ((x>=0) && (x<=154))
                {
                res=2;
                tft.setColor (255, 0, 0);
                tft.drawRoundRect(0, 200, 154, 239);
                }
            }
        }
    }
    */
    waitForTouchRelease();

    if (res==1)
    {
        printf("settings will be set here");
        if (ct or cd)
        {
            SettingsMemory::instance().saveSettings(settings);
        }
    }

}

