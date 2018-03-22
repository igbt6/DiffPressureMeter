#include "LIB_Config.h"
#include "PF_Config.h"


int main(void) 
{  
	system_init();
   
    lcd_draw_rect(30, 40, 150, 100, RED);
    lcd_draw_circle(120, 160, 50, BLUE);
    lcd_draw_line(30, 40, 180, 140, RED);
    
    lcd_draw_line(30, 220, 210, 240, RED);
    lcd_draw_line(30, 220, 120, 280, RED);
    lcd_draw_line(120, 280, 210, 240, RED);
	
	while (1) {
		
	}
}

/*-------------------------------END OF FILE-------------------------------*/

