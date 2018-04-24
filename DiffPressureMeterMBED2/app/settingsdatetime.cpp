#include "settingsdatetime.h"
#include "externalrtc.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

using namespace rtctime;

//-----------------------------------------------------------------------------
SettingsDateTime::SettingsDateTime(LUTFT &tft, LUTouch &touch)
  : SettingsHelper(tft, touch)
  , tft(tft)
  , touch(touch)
{

}

//-----------------------------------------------------------------------------
void SettingsDateTime::setDateTime()
{
  Time t_temp;
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
  tft.setColor(VGA_BLUE);
  tft.drawRoundRect(0, 0, 319, 96);
  tft.drawRoundRect(0, 100, 319, 196);

  tft.setColor(VGA_WHITE);
  tft.print("Czas:", 10, 40);
  tft.print(":", 154, 40);
  tft.print(":", 202, 40);
  tft.print("Data:", 10, 140);
  tft.print(".", 154, 140);
  tft.print(".", 202, 140);
  
  // Draw Buttons
  drawUpButton(122, 10);
  drawUpButton(170, 10);
  drawUpButton(218, 10);
  drawDownButton(122, 61);
  drawDownButton(170, 61);
  drawDownButton(218, 61);
  drawUpButton(122, 110);
  drawUpButton(170, 110);
  drawUpButton(234, 110);
  drawDownButton(122, 161);
  drawDownButton(170, 161);
  drawDownButton(234, 161);
  
  // getcurrent time and date
  tft.setColor(VGA_WHITE);
  if (ExternalRTC::instance().getTime(t_temp))
  {
    // defaults
    t_temp.date=1;
    t_temp.month=1;
    t_temp.year=2018;
    t_temp.day=0;
    t_temp.hours=0;
    t_temp.minutes=0;
    t_temp.seconds=0;
    ct=true;
    cd=true;
    printf("\r RTC-getTime ERROR occured !\t\n");  
  }
  
  if (t_temp.hours<10)
  {
    tft.printNumI(0, 122, 40);
    tft.printNumI(t_temp.hours, 138, 40);
  }
  else
  {
    tft.printNumI(t_temp.hours, 122, 40);
  }
  if (t_temp.minutes<10)
  {
    tft.printNumI(0, 170, 40);
    tft.printNumI(t_temp.minutes, 186, 40);
  }
  else
  {
    tft.printNumI(t_temp.minutes, 170, 40);
  }
  if (t_temp.seconds<10)
  {
    tft.printNumI(0, 218, 40);
    tft.printNumI(t_temp.seconds, 234, 40);
  }
  else
  {
    tft.printNumI(t_temp.seconds, 218, 40);
  }
  if (t_temp.date<10)
  {
    tft.printNumI(0, 122, 140);
    tft.printNumI(t_temp.date, 138, 140);
  }
  else
  {
    tft.printNumI(t_temp.date, 122, 140);
  }
  if (t_temp.month<10)
  {
    tft.printNumI(0, 170, 140);
    tft.printNumI(t_temp.month, 186, 140);
  }
  else
  {
    tft.printNumI(t_temp.month, 170, 140);
  }
  tft.printNumI(t_temp.year, 218, 140);
  showDOW(t_temp.day);
  
  while (res==0)
  {
        if (touch.dataAvailable())
        {
        touch.read();
        x=touch.getX();
        y=touch.getY();
        if ((y>=10) && (y<=35)) // Buttons: Time UP
        {
            if ((x>=122) && (x<=154))
            {
            buttonWait(122, 10);
            t_temp.hours+=1;
            if (t_temp.hours==24)
                t_temp.hours=0;
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
            buttonWait(170, 10);
            t_temp.minutes+=1;
            if (t_temp.minutes==60)
                t_temp.minutes=0;
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
            buttonWait(218, 10);
            t_temp.seconds+=1;
            if (t_temp.seconds==60)
                t_temp.seconds=0;
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
        else if ((y>=200) && (y<=239)) // Buttons: CANCEL / SAVE
        {
            if ((x>=165) && (x<=319))
            {
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
    
    waitForTouchRelease();

    if (res==1)
    {
        if (ct or cd)
        {
            ExternalRTC::instance().setTime(t_temp);
        }
    }
    
    //drawDisplay();
    //printDate();
}

//-----------------------------------------------------------------------------
byte SettingsDateTime::validateDateForMonth(byte d, byte m, word y)
{
  byte mArr[12] = {31,0,31,30,31,30,31,31,30,31,30,31};
  byte od;
  bool dc=false;
  
  if (m==2)
  {
    if ((y % 4)==0)
    {
      if (d>29)
      {
        d=29;
        dc=true;
      }
    }
    else
    {  
      if (d>28)
      {
        d=28;
        dc=true;
      }
    }
  }
  else
  {
    if (d>mArr[m-1])
    {
      d=mArr[m-1];
      dc=true;
    }
  }
  
  if (dc)
  {
    if (d<10)
    {
      tft.printNumI(0, 122, 140);
      tft.printNumI(d, 138, 140);
    }
    else
    {
      tft.printNumI(d, 122, 140);
    }
  }
  return d;
}

//-----------------------------------------------------------------------------
void SettingsDateTime::showDOW(byte dow)
{
  const char* str[] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
  
  tft.setColor(128, 128, 255);
  tft.setBackColor(0, 0, 0);
  tft.print(str[dow-1], 26, 167);
}

//-----------------------------------------------------------------------------
char SettingsDateTime::uCase(char c)
{
  if ((c>='a') && (c<='z'))
    return (c-0x20);
  else
    return c;
}

//-----------------------------------------------------------------------------
byte SettingsDateTime::calcDOW(byte d, byte m, int y)
{
  int dow;
  byte mArr[12] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  dow = (y % 100);
  dow = dow*1.25;
  dow += d;
  dow += mArr[m-1];
  if (((y % 4)==0) && (m<3))
  {
    dow -= 1;
  }
  while (dow>7)
  {
    dow -= 7;
  }
    
  return dow;
}

//-----------------------------------------------------------------------------
byte SettingsDateTime::validateDate(byte d, byte m, word y)
{
  byte mArr[12] = {31,0,31,30,31,30,31,31,30,31,30,31};
  byte od;
  
  if (m==2)
  {
    if ((y % 4)==0)
    {
      if (d==30)
        od=1;
      else if (d==0)
        od=29;
      else
        od=d;
    }
    else
    {
      if (d==29)
        od=1;
      else if (d==0)
        od=28;
      else
        od=d;
    }
  }
  else
  {
    if (d==0)
      od=mArr[m-1];
    else if (d==(mArr[m-1]+1))
      od=1;
    else
      od=d;
  }
  
  return od;
}
//-----------------------------------------------------------------------------