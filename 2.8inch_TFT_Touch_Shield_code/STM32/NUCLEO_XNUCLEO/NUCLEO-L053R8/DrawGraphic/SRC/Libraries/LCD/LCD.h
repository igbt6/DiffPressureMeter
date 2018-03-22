/**
  ******************************************************************************
  * @file    xxx.h
  * @author  Waveshare Team
  * @version 
  * @date    xx-xx-2014
  * @brief   xxxxx.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LCD_H_
#define _LCD_H_

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"
#include <math.h>
#include <stdlib.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define LCD_WIDTH    240
#define LCD_HEIGHT   320

#define FONT_1206    12
#define FONT_1608    16

#define WHITE          0xFFFF
#define BLACK          0x0000	  
#define BLUE           0x001F  
#define BRED           0XF81F
#define GRED 		   0XFFE0
#define GBLUE		   0X07FF
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define BROWN 		   0XBC40 
#define BRRED 		   0XFC07 
#define GRAY  		   0X8430 

#define LCD_CMD                0
#define LCD_DATA               1

/* Exported functions ------------------------------------------------------- */

extern void lcd_init(void);
extern void lcd_draw_point(uint16_t hwXpos, uint16_t hwYpos, uint16_t hwColor);
extern void lcd_display_char(uint16_t hwXpos, //specify x position.
                         uint16_t hwYpos, //specify y position.
                         uint8_t chChr,   //a char is display.
                         uint8_t chSize,  //specify the size of the char
                         uint16_t hwColor); //specify the color of the char
extern void lcd_display_num(uint16_t hwXpos,  //specify x position.
                          uint16_t hwYpos, //specify y position.
                          uint32_t chNum,  //a number is display.
                          uint8_t chLen,   //length ot the number
                          uint8_t chSize,  //specify the size of the number
                          uint16_t hwColor); //specify the color of the number
extern void lcd_display_string(uint16_t hwXpos, //specify x position.
                         uint16_t hwYpos,   //specify y position.
                         const uint8_t *pchString,  //a pointer to string
                         uint8_t chSize,    // the size of the string 
                         uint16_t hwColor);  // specify the color of the string 



#endif
/*-------------------------------END OF FILE-------------------------------*/

