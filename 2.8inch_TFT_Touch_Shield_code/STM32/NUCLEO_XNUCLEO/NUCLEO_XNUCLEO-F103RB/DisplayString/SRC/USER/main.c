#include "LIB_Config.h"
#include "PF_Config.h"


int main(void) 
{  
	system_init();

    lcd_display_string(60, 120, (const uint8_t *)"Hello, world !", FONT_1608, RED);
    lcd_display_string(30, 152, (const uint8_t *)"2.8' TFT Touch Shield", FONT_1608, RED);
	
	while (1) {
		
	}
}

/*-------------------------------END OF FILE-------------------------------*/

