/**
  ******************************************************************************
  * @file    xxx.c 
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

/* Includes ------------------------------------------------------------------*/
#include "LIB_Config.h"
#include "Fonts.h"
#include "LCD.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


static void lcd_write_byte(uint8_t chByte, uint8_t chCmd)
{
    if (chCmd) {
        __LCD_DC_SET();
    } else {
        __LCD_DC_CLR();
    }
    
    __LCD_CS_CLR();
    __LCD_WRITE_BYTE(chByte);
    __LCD_CS_SET();
}

static void lcd_write_word(uint16_t hwData)
{
    __LCD_DC_SET();
    __LCD_CS_CLR();
    __LCD_WRITE_BYTE(hwData >> 8);
    __LCD_WRITE_BYTE(hwData & 0xFF);
    __LCD_CS_SET();
}

static void lcd_write_register(uint8_t chRegister, uint8_t chValue)
{
	lcd_write_byte(chRegister, LCD_CMD);
	lcd_write_byte(chValue, LCD_DATA);
}

//set the specified position of cursor on lcd.
//hwXpos specify x position
//hwYpos specify y position
void lcd_set_cursor(uint16_t hwXpos, uint16_t hwYpos)
{
	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
    
    lcd_write_register(0x02, hwXpos >> 8);
	lcd_write_register(0x03, hwXpos & 0xFF); //Column Start
	lcd_write_register(0x06, hwYpos >> 8);
	lcd_write_register(0x07, hwYpos & 0xFF); //Row Start
}

//clear the lcd with the specified color.
void lcd_clear_screen(uint16_t hwColor)  
{
	uint32_t i, wCount = LCD_WIDTH;
	
	wCount *= LCD_HEIGHT;
	
	lcd_set_cursor(0, 0);
	lcd_write_byte(0x22, LCD_CMD);
    
    __LCD_DC_SET();
	__LCD_CS_CLR();
	for (i = 0; i < wCount; i ++) {
		__LCD_WRITE_BYTE(hwColor >> 8);
		__LCD_WRITE_BYTE(hwColor & 0xFF);
	}
	__LCD_CS_SET();
}

//draw a point on the lcd with the specified color.
//hwXpos specify x position.
//hwYpos specify y position.
//hwColor color of the point.
void lcd_draw_point(uint16_t hwXpos, uint16_t hwYpos, uint16_t hwColor) 
{
	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
	lcd_set_cursor(hwXpos, hwYpos);
	lcd_write_byte(0x22, LCD_CMD);
    lcd_write_word(hwColor);
}

//display a char at the specified position on lcd.
void lcd_display_char(uint16_t hwXpos, //specify x position.
                         uint16_t hwYpos, //specify y position.
                         uint8_t chChr,   //a char is display.
                         uint8_t chSize,  //specify the size of the char
                         uint16_t hwColor) //specify the color of the char
{      	
	uint8_t i, j, chTemp;
	uint16_t hwYpos0 = hwYpos, hwColorVal = 0;

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}

					   
    for (i = 0; i < chSize; i ++) {   
		if (FONT_1206 == chSize) {
			chTemp = c_chFont1206[chChr - 0x20][i];  
		} else if (FONT_1608 == chSize) { 
			chTemp = c_chFont1608[chChr - 0x20][i];
		}
		
        for (j = 0; j < 8; j ++) {
    		if (chTemp & 0x80) {
				hwColorVal = hwColor;
				lcd_draw_point(hwXpos, hwYpos, hwColorVal);
    		}			
			chTemp <<= 1;
			hwYpos ++;
			if ((hwYpos - hwYpos0) == chSize) {
				hwYpos = hwYpos0;
				hwXpos ++;
				break;
			}
		}  	 
    } 
}


//_pow
static uint32_t _pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	
	while(n --) result *= m;    
	return result;
}

//display a number at the specified position on lcd.
void lcd_display_num(uint16_t hwXpos,  //specify x position.
                          uint16_t hwYpos, //specify y position.
                          uint32_t chNum,  //a number is display.
                          uint8_t chLen,   //length ot the number
                          uint8_t chSize,  //specify the size of the number
                          uint16_t hwColor) //specify the color of the number
{         	
	uint8_t i;
	uint8_t chTemp, chShow = 0;

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
	for(i = 0; i < chLen; i ++) {
		chTemp = (chNum / _pow(10, chLen - i - 1)) % 10;
		if(chShow == 0 && i < (chLen - 1)) {
			if(chTemp == 0) {
				lcd_display_char(hwXpos + (chSize / 2) * i, hwYpos, ' ', chSize, hwColor);
				continue;
			} else {
				chShow = 1;
			}	 
		}
	 	lcd_display_char(hwXpos + (chSize / 2) * i, hwYpos, chTemp + '0', chSize, hwColor); 
	}
} 

//display a string at the specified position on lcd.
void lcd_display_string(uint16_t hwXpos, //specify x position.
                         uint16_t hwYpos,   //specify y position.
                         const uint8_t *pchString,  //a pointer to string
                         uint8_t chSize,    // the size of the string 
                         uint16_t hwColor)  // specify the color of the string 
{

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
    while (*pchString != '\0') {       
        if (hwXpos > (LCD_WIDTH - chSize / 2)) {
			hwXpos = 0;
			hwYpos += chSize;
			if (hwYpos > (LCD_HEIGHT - chSize)) {
				hwYpos = hwXpos = 0;
				lcd_clear_screen(0x00);
			}
		}
		
        lcd_display_char(hwXpos, hwYpos, (uint8_t)*pchString, chSize, hwColor);
        hwXpos += chSize / 2;
        pchString ++;
    } 
}


//initialize the lcd.
//phwDevId pointer to device ID of lcd
void lcd_init(void)
{
//	__LCD_RST_CLR();
//	delay_ms(100);
//	__LCD_RST_SET();
	__LCD_CS_SET();
	__LCD_BKL_SET();

	//Driving ability Setting
	lcd_write_register(0xEA,0x00); //PTBA[15:8]
	lcd_write_register(0xEB,0x20); //PTBA[7:0]
	lcd_write_register(0xEC,0x0C); //STBA[15:8]
	lcd_write_register(0xED,0xC4); //STBA[7:0]
	lcd_write_register(0xE8,0x38); //OPON[7:0]
	lcd_write_register(0xE9,0x10); //OPON1[7:0]
	lcd_write_register(0xF1,0x01); //OTPS1B
	lcd_write_register(0xF2,0x10); //GEN
	//Gamma 2.2 Setting
	lcd_write_register(0x40,0x01); //
	lcd_write_register(0x41,0x00); //
	lcd_write_register(0x42,0x00); //
	lcd_write_register(0x43,0x10); //
	lcd_write_register(0x44,0x0E); //
	lcd_write_register(0x45,0x24); //
	lcd_write_register(0x46,0x04); //
	lcd_write_register(0x47,0x50); //
	lcd_write_register(0x48,0x02); //
	lcd_write_register(0x49,0x13); //
	lcd_write_register(0x4A,0x19); //
	lcd_write_register(0x4B,0x19); //
	lcd_write_register(0x4C,0x16); //
	lcd_write_register(0x50,0x1B); //
	lcd_write_register(0x51,0x31); //
	lcd_write_register(0x52,0x2F); //
	lcd_write_register(0x53,0x3F); //
	lcd_write_register(0x54,0x3F); //
	lcd_write_register(0x55,0x3E); //
	lcd_write_register(0x56,0x2F); //
	lcd_write_register(0x57,0x7B); //
	lcd_write_register(0x58,0x09); //
	lcd_write_register(0x59,0x06); //
	lcd_write_register(0x5A,0x06); //
	lcd_write_register(0x5B,0x0C); //
	lcd_write_register(0x5C,0x1D); //
	lcd_write_register(0x5D,0xCC); //
	//Power Voltage Setting
	lcd_write_register(0x1B,0x1B); //VRH=4.65V
	lcd_write_register(0x1A,0x01); //BT (VGH~15V,VGL~-10V,DDVDH~5V)
	lcd_write_register(0x24,0x2F); //VMH(VCOM High voltage ~3.2V)
	lcd_write_register(0x25,0x57); //VML(VCOM Low voltage -1.2V)
	//****VCOM offset**///
	lcd_write_register(0x23,0x88); //for Flicker adjust //can reload from OTP
	//Power on Setting
	lcd_write_register(0x18,0x34); //I/P_RADJ,N/P_RADJ, Normal mode 60Hz
	lcd_write_register(0x19,0x01); //OSC_EN='1', start Osc
	lcd_write_register(0x01,0x00); //DP_STB='0', out deep sleep
	lcd_write_register(0x1F,0x88);// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	HAL_Delay(5);
	lcd_write_register(0x1F,0x80);// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	HAL_Delay(5);
	lcd_write_register(0x1F,0x90);// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	HAL_Delay(5);
	lcd_write_register(0x1F,0xD0);// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	HAL_Delay(5);
	//262k/65k color selection
	lcd_write_register(0x17,0x05); //default 0x06 262k color // 0x05 65k color
	//SET PANEL
	lcd_write_register(0x36,0x00); //SS_P, GS_P,REV_P,BGR_P
	//Display ON Setting
	lcd_write_register(0x28,0x38); //GON=1, DTE=1, D=1000
	HAL_Delay(40);
	lcd_write_register(0x28,0x3F); //GON=1, DTE=1, D=1100

	lcd_write_register(0x16,0x18); 
	//Set GRAM Area
	lcd_write_register(0x02,0x00);
	lcd_write_register(0x03,0x00); //Column Start
	lcd_write_register(0x04,0x00);
	lcd_write_register(0x05,0xEF); //Column End
	lcd_write_register(0x06,0x00);
	lcd_write_register(0x07,0x00); //Row Start
	lcd_write_register(0x08,0x01);
	lcd_write_register(0x09,0x3F); //Row End
    
    lcd_clear_screen(WHITE);
}


/*-------------------------------END OF FILE-------------------------------*/

