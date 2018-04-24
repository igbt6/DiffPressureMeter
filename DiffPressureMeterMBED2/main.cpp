#if 0

#include "system.h"
#include "LUTft.h"
#include "LUTouch.h"
#include "LUTouchCalibration.h"
#include "externalrtc.h"
#include "pressuresensor.h"
#include "interfaces.h"
#include "settingsdatetime.h"
#include "settingsmeasurement.h"
#include "settingsmemory.h"

using namespace rtctime;
// LUTFT
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
LUTFT tft(HwInterfaces::instance().getHwInterface<SPI__1>().handle());
LUTouch touch(HwInterfaces::instance().getHwInterface<SPI__1>().handle());


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



///////////////////////////////////////////////////////////////////////////////////////
int main()
{
    printf("Hello! System here!");
    ///LCD///
    tft.InitLCD(LANDSCAPE);
    tft.setFont(SmallFont);
    //LUTFT::runDemo(tft, 20);
    touch.InitTouch(SettingsMemory::instance().appSettings().calibCoeffs, tft.getOrientation());
    //LUTouchCalibration touchCalib(tft, touch);
    //touchCalib.calibrate();
    //SettingsDateTime setDateTime(tft, touch);
    //setDateTime.setClock();
    SettingsMeasurement setMeasurement(tft, touch);
    setMeasurement.setMeasurement();
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

#endif


#include "mainapp.h"

///////////////////////////////////////////////////////////////////////////////////////
int main()
{
    MainApp::instance().run();   
}