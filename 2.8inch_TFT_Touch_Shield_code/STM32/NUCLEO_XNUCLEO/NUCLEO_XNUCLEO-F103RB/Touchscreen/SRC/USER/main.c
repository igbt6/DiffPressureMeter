#include "LIB_Config.h"
#include "PF_Config.h"


int main(void) 
{  
	system_init();
    
    tp_adjust();
    tp_dialog();
	
	while (1) {
		tp_draw_board();
	}
}

/*-------------------------------END OF FILE-------------------------------*/

