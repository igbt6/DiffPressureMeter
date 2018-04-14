#include "system.h"
#include "LCD.h"
#include "XPT2046.h"
#include "Touch.h"
#include "ugui.h"
#include "externalrtc.h"
#include "pressuresensor.h"
 
using namespace rtctime;

SPI spii(LCD_SPI_MOSI_PIN, LCD_SPI_MISO_PIN, LCD_SPI_SCLK_PIN);
UG_GUI   gui;

static void lcd_draw_point(UG_S16 ul_x,UG_S16 ul_y, UG_COLOR ul_color)
{
    lcd.lcd_draw_point(ul_x, ul_y, ul_color);
}

static void window_1_callback( UG_MESSAGE* msg )
{
    if ( msg->type == MSG_TYPE_OBJECT ) {
        if ( msg->id == OBJ_TYPE_BUTTON ) {
            switch ( msg->sub_id ) {
                case BTN_ID_0: {
                printf("Button 0 clicked");
                    break;
                }
                case BTN_ID_1: {
                    printf("Button 1 clicked");
                    break;
                }
            }
        }
    }
}
int main()
{
    // LCD
    spii.format(8, 3);
    spii.frequency(50000000); // 50MHz clock
    spii.set_dma_usage(DMA_USAGE_ALWAYS);

    printf("HELLO WORLD !");  
    lcd.lcd_init();                                      // init TFT library
    const uint8_t *str = (const uint8_t *)"Helllo!";
    lcd.lcd_display_string(60, 120, (const uint8_t *)"Hello, world !", FONT_1608, RED);
    lcd.lcd_display_string(30, 152, (const uint8_t *)"2.8' TFT Touch Shield", FONT_1608, RED);
    // GUI LIBRARY
    #define MAX_OBJECTS 10
    UG_Init(&gui, lcd_draw_point, LCD_HEIGHT, LCD_WIDTH);
        UG_WINDOW window_1 ; /* Window */
    UG_BUTTON button_1; /* Button container */
    UG_BUTTON button_2; /* Button container */
    UG_OBJECT obj_buff_wnd_1 [MAX_OBJECTS] ; /* Object buffer */
    UG_WindowCreate ( &window_1 , obj_buff_wnd_1 , MAX_OBJECTS, window_1_callback);
    UG_WindowSetTitleTextFont ( &window_1 , &FONT_8X8 ) ;
    UG_WindowSetTitleText ( &window_1 , "Bike Computer" ) ;
    UG_WindowSetBackColor( &window_1 , C_GRAY ) ;
    UG_ButtonCreate (&window_1, &button_1, BTN_ID_0, 10 , 230 , 115 , 290 );
    UG_ButtonCreate (&window_1, &button_2, BTN_ID_1, 125 , 230 , 225 , 290 );
    UG_ButtonSetBackColor (&window_1 ,BTN_ID_0 , C_BLUE ) ;
    UG_ButtonSetForeColor (&window_1 ,BTN_ID_0 , C_WHITE ) ;
    UG_ButtonSetBackColor (&window_1 ,BTN_ID_1 , C_BLUE ) ;
    UG_ButtonSetForeColor (&window_1 ,BTN_ID_1 , C_WHITE ) ;
    UG_ButtonSetFont (&window_1 , BTN_ID_0 , &FONT_8X8) ;
    UG_ButtonSetFont (&window_1 , BTN_ID_1 , &FONT_8X8) ;
    UG_ButtonSetText (&window_1 , BTN_ID_0 , "Prev" );
    UG_ButtonSetText (&window_1 , BTN_ID_1 , "Next" );
    UG_WindowShow(&window_1);
    // TOUCH
    #if 0
    Tp.tp_init();
    Tp.tp_adjust();
    Tp.tp_dialog();
    Tp.tp_draw_board();
    #endif

    // RTC
    rtctime::Time time{2018, 4, 10, 1, 2, 40, 0};
    /*if (ExternalRTC::instance().setTime(time))
    {
        printf("\r setTime ERROR occured !\t\n");   
    }*/

    // PRESSURE SENSOR
    //
    while(1)
    {
        UG_Update();
        static int counter;
        wait_ms(5000);
        printf("\rCounter: %d\t\n", counter++);

        // RTC
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
        // PRESSURE SENSOR
        //PressureSensor::instance().performMeasurement();
        //printf("\rLast dif pressure: %3.3f\t\n", PressureSensor::instance().getLastResult());  
    }
}
 