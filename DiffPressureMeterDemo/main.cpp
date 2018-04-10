#include "system.h"
#include "LCD.h"
#include "externalrtc.h"
#include "pressuresensor.h"
 
using namespace rtctime;

SPI spii(LCD_SPI_MOSI_PIN, LCD_SPI_MISO_PIN, LCD_SPI_SCLK_PIN);
Serial usb(USBTX, USBRX);

int main()
{
    // LCD
    spii.format(8,3);
    spii.frequency(100000); // 1MHz clock
    usb.printf("HELLO WORLD !");  
    lcd.lcd_init();                                      // init TFT library
    const uint8_t *str = (const uint8_t *)"Helllo!";
    lcd.lcd_display_string(30, 50, str, 10, RED);
    // RTC
    rtctime::Time time{2018, 4, 10, 1, 2, 40, 0};
    //if (ExternalRTC::instance().setTime(time))
    {
        usb.printf("\r setTime ERROR occured !\t\n");   
    }

    // PRESSURE SENSOR
    //
    while(1)
    {
        static int counter;
        wait_ms(5000);
        usb.printf("\rCounter: %d\t\n", counter++);

        // RTC
        usb.printf("\rTemperature %3.2f [C]\t\n", ExternalRTC::instance().getTemperature());
        if (ExternalRTC::instance().getTime(time))
        {
            usb.printf("\r getTime ERROR occured !\t\n");  
        }
        else
        {
            usb.printf("\rCurrent time: %s,  %s, %s, %s\t\n",  \
                        ExternalRTC::getDateString(time),          \
                        ExternalRTC::getTimeString(time),
                        ExternalRTC::getDayOfWeekString(time),
                        ExternalRTC::getMonthString(time));            
        }
        // PRESSURE SENSOR
        PressureSensor::instance().performMeasurement();
        usb.printf("\rLast dif pressure: %3.3f\t\n", PressureSensor::instance().getLastResult());  
    }
}
 