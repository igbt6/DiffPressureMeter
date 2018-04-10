#include "system.h"
#include "stdio.h"
//#include "XPT2046.h"
//#include "Touch.h"
#include "LCD.h"
#include "Arial12x12.h"
#include "Arial24x23.h"

SPI spii(PA_7, PA_6, PA_5);

int main()
{
    spii.format(8,2);
    spii.frequency(100000); // 1MHz clock
    printf("\t\n*** Differential pressure meter - Basiaga2018 %s ***\n\t", DEVICE_VER);

    lcd.lcd_init();                                      // init TFT library
    const uint8_t * str = (const uint8_t *)"Helllo!";
	lcd.lcd_display_string(30, 50, str, 10, RED);
    //Tp.tp_init();
    //Tp.tp_adjust();
    //Tp.tp_dialog();

    while (true) {
        static int counter;
        wait_ms(1000);
        printf("HELLO %d\t\n", counter++);
    }
}
