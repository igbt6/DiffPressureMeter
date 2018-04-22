#include "system.h"
#include "LUTft.h"
#include "LUTouch.h"
#include "LUTouchCalibration.h"
#include "externalrtc.h"
#include "pressuresensor.h"
#include "interfaces.h"


using namespace rtctime;
// LUTFT
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
LUTFT tft(HwInterfaces::instance().getHwInterface<SPI__1>().handle());
LUTouch touch(HwInterfaces::instance().getHwInterface<SPI__1>().handle());


char uCase(char c)
{
  if ((c>='a') && (c<='z'))
    return (c-0x20);
  else
    return c;
}

void waitForTouchRelease()
{
  // Wait for release
  while (touch.dataAvailable()==true)
  {
    touch.read();
  }
}

void buttonWait(int x, int y)
{
  tft.setColor(255, 0, 0);
  tft.drawRoundRect(x, y, x+32, y+25);
  waitForTouchRelease();
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(x, y, x+32, y+25);
}

byte calcDOW(byte d, byte m, int y)
{
  int dow;
  byte mArr[12] = {6,2,2,5,0,3,5,1,4,6,2,4};
  
  dow = (y % 100);
  dow = dow*1.25;
  dow += d;
  dow += mArr[m-1];
  if (((y % 4)==0) && (m<3))
    dow -= 1;
  while (dow>7)
    dow -= 7;
    
  return dow;
}

/*void printDate()
{
  Time t_temp;
  
  t_temp = rtc.getTime();
  tft.setFont(BigFont);
  tft.setColor(0, 0, 0);
  tft.setBackColor(255, 255, 255);
  tft.print(rtc.getDOWStr(FORMAT_SHORT), 256, 8);
  if (t_temp.date<10)
    tft.printNumI(t_temp.date, 272, 28);
  else
    tft.printNumI(t_temp.date, 264, 28);
  tft.print(rtc.getMonthStr(FORMAT_SHORT), 256, 48);
  tft.printNumI(t_temp.year, 248, 65);
}
*/
//////////////////////////////////////////////////////////////////////////CLOCK SETTIN
void drawUpButton(int x, int y)
{
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(x, y, x+32, y+25);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(x, y, x+32, y+25);
  tft.setColor(128, 128, 255);
  for (int i=0; i<15; i++)
    tft.drawLine(x+6+(i/1.5), y+20-i, x+27-(i/1.5), y+20-i);
}

void drawDownButton(int x, int y)
{
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(x, y, x+32, y+25);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(x, y, x+32, y+25);
  tft.setColor(128, 128, 255);
  for (int i=0; i<15; i++)
    tft.drawLine(x+6+(i/1.5), y+5+i, x+27-(i/1.5), y+5+i);
}

void showDOW(byte dow)
{
  const char* str[] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
  
  tft.setColor(128, 128, 255);
  tft.setBackColor(0, 0, 0);
  tft.print(str[dow-1], 26, 167);
}

byte validateDate(byte d, byte m, word y)
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

byte validateDateForMonth(byte d, byte m, word y)
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

void setClock()
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
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(165, 200, 319, 239);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(165, 200, 319, 239);
  tft.setBackColor(64, 64, 128);
  tft.print("Save", 210, 212);
  tft.setBackColor(0, 0, 0);
  // Draw Cancel button
  tft.setColor(64, 64, 128);
  tft.fillRoundRect(0, 200, 154, 239);
  tft.setColor(255, 255, 255);
  tft.drawRoundRect(0, 200, 154, 239);
  tft.setBackColor(64, 64, 128);
  tft.print("Cancel", 29, 212);
  tft.setBackColor(0, 0, 0);
  
  // Draw frames
  tft.setColor(0, 0, 255);
  tft.drawRoundRect(0, 0, 319, 96);
  tft.drawRoundRect(0, 100, 319, 196);
  tft.print("Time:", 10, 40);
  tft.print(":", 154, 40);
  tft.print(":", 202, 40);
  tft.print("Date:", 10, 140);
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
  
  // Print current time and date
  tft.setColor(255, 255, 255);
  if (ExternalRTC::instance().getTime(t_temp))
  {
    printf("\r getTime ERROR occured !\t\n");  
  }
  if (t_temp.date==0)
  {
    t_temp.date=1;
    t_temp.month=1;
    t_temp.year=2010;
    t_temp.day=5;
    t_temp.hours=0;
    t_temp.minutes=0;
    t_temp.seconds=0;
    ct=true;
    cd=true;
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
                t_temp.year=2099;
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
        printf("Time will be set here!!!!");
        /*
        if (ct)
            rtc.setTime(t_temp.hours, t_temp.minutes, t_temp.seconds);
        if (cd)
        {
            rtc.setDate(t_temp.date, t_temp.month, t_temp.year);
            rtc.setDOW(t_temp.day);
        }
        */
    }
    
    //drawDisplay();
    //printDate();
}




///////////////////////////////////////////////////////////////////////////////////////
int main()
{
    printf("Hello! System here!");
    ///LCD///
    tft.InitLCD(LANDSCAPE);
    tft.setFont(SmallFont);
    //LUTFT::runDemo(tft, 20);
    touch.InitTouch(tft.getOrientation());
    LUTouchCalibration touchCalib(tft, touch);
    touchCalib.calibrate();
    setClock();
    LUTouch::runDemo(tft, touch, 3600);
    
    
    ///RTC///
    rtctime::Time time{2018, 4, 10, 1, 2, 40, 0};
    /*if (ExternalRTC::instance().setTime(time))
    {
        printf("\r setTime ERROR occured !\t\n");   
    }*/

    ///PRESSURE SENSOR///
    //
    while(1)
    {
        static int counter;
        wait_ms(5000);
        printf("\rCounter: %d\t\n", counter++);

        ///RTC///
        printf("\rTemperature %3.2f [C]\t\n", ExternalRTC::instance().getTemperature());
        if (ExternalRTC::instance().getTime(time))
        {
            printf("\r getTime ERROR occured !\t\n");  
        }
        else
        {
            printf("\rCurrent time: %s,  %s, %s, %s\t\n",  \
                        ExternalRTC::getDateString(time),          \
                        ExternalRTC::getTimeString(time),
                        ExternalRTC::getDayOfWeekString(time),
                        ExternalRTC::getMonthString(time));            
        }
        ///PRESSURE SENSOR///
        //PressureSensor::instance().performMeasurement();
        //printf("\rLast dif pressure: %3.3f\t\n", PressureSensor::instance().getLastResult());  
    }
}

