/*
  LUTFT.h - Lukasz Uszko 2018 
  Library support for Color TFT LCD (driver HX8347D)
  based on LUTFT library developed by Henning Karlsen
  This is special porting for LUTFT shield LCD screen. 
  Copyright (C)2010-2014 Henning Karlsen. All right reserved 
*/

#ifndef LUTFT_HX8347D_H
#define LUTFT_HX8347D_H

#include "mbed.h"
#include "hwconfig.h"
#include <string>

#define HIGH 1
#define LOW 0

#define LUTFT_cbi(reg, bitmask) *reg &= ~bitmask
#define LUTFT_sbi(reg, bitmask) *reg |= bitmask

#define cport(port, data) port &= data
#define sport(port, data) port |= data

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define fontbyte(x) cfont.font[x]  

#define pgm_read_word(data) *data
#define pgm_read_byte(data) *data
#define bitmapdatatype unsigned short*

#define regtype volatile uint32_t
#define regsize uint32_t

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

#define SERIAL_4PIN     4
#define SERIAL_5PIN     5

#define NOTINUSE        255
#define HX8347D         123

#define LCD_CMD      0
#define LCD_DATA     1

#define _CS_OUT()    _cs(LCD_CS_PIN, HIGH)
#define _CS_CLR()    _cs.write(LOW)
#define _CS_SET()    _cs.write(HIGH)

#define _DC_OUT()    _dc(LCD_DC_PIN, HIGH)
#define _DC_CLR()    _dc.write(LOW)
#define _DC_SET()    _dc.write(HIGH)

#define _BKL_OUT()   _bkl(LCD_BKL_PIN, LOW)
#define _BKL_OFF()   _bkl.write(LOW)
#define _BKL_ON()    _bkl.write(HIGH)

//*********************************
// COLORS
//*********************************
// VGA color palette
#define VGA_BLACK       0x0000
#define VGA_WHITE       0xFFFF
#define VGA_RED         0xF800
#define VGA_GREEN       0x0400
#define VGA_BLUE        0x001F
#define VGA_SILVER      0xC618
#define VGA_GRAY        0x8410
#define VGA_MAROON      0x8000
#define VGA_YELLOW      0xFFE0
#define VGA_OLIVE       0x8400
#define VGA_LIME        0x07E0
#define VGA_AQUA        0x07FF
#define VGA_TEAL        0x0410
#define VGA_NAVY        0x0010
#define VGA_FUCHSIA     0xF81F
#define VGA_PURPLE      0x8010  
#define VGA_BRED        0XF81F
#define VGA_GRED 		0XFFE0
#define VGA_GBLUE	    0X07FF
#define VGA_MAGENTA     0xF81F
#define VGA_LGREEN      0x07E0
#define VGA_CYAN        0x7FFF
#define VGA_BROWN 		0XBC40 
#define VGA_BRRED 		0XFC07 
#define VGA_TRANSPARENT 0xFFFFFFFF

#define BMPIMAGEOFFSET 66
    
struct _current_font
{
    uint8_t* font;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t offset;
    uint8_t numchars;
};

class LUTFT
{
    public:
        LUTFT(PinName mosi, PinName miso, PinName sck, PinName cs = NC);
        void    InitLCD(uint8_t orientation=LANDSCAPE);
        void    clrScr();
        void    drawPixel(int x, int y);
        void    drawLine(int x1, int y1, int x2, int y2);
        void    fillScr(uint8_t r, uint8_t g, uint8_t b);
        void    fillScr(uint16_t color);
        void    drawRect(int x1, int y1, int x2, int y2);
        void    drawRoundRect(int x1, int y1, int x2, int y2);
        void    fillRect(int x1, int y1, int x2, int y2);
        void    fillRoundRect(int x1, int y1, int x2, int y2);
        void    drawCircle(int x, int y, int radius);
        void    fillCircle(int x, int y, int radius);
        void    setColor(uint8_t r, uint8_t g, uint8_t b);
        void    setColor(uint16_t color);
        uint16_t    getColor();
        void    setBackColor(uint8_t r, uint8_t g, uint8_t b);
        void    setBackColor(uint32_t color);
        uint16_t    getBackColor();
        void	print(char *st, int x, int y, int deg=0);
		void	print(std::string st, int x, int y, int deg=0);
        void	printNumI(long num, int x, int y, int length=0, char filler=' ');
        void	printNumF(double num, uint8_t dec, int x, int y, char divider='.', int length=0, char filler=' ');
        void	setFont(uint8_t* font);
        uint8_t* getFont();
        uint8_t	getFontXsize();
        uint8_t	getFontYsize();
        void    drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale=1);
        void    drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
        int     getDisplayXSize();
        int     getDisplayYSize();
        static void run_demo(LUTFT &tft, int loop_num);
        
        //***********not  use*****************
        void    lcdOff();
        void    lcdOn();
        void    setContrast(char c);
        void    setBrightness(uint8_t br);
        void    setDisplayPage(uint8_t page);
        void    setWritePage(uint8_t page);
        //***********************************

        //void dispBitmap(File inFile);

        /*
            The functions and variables below should not normally be used.
            They have been left publicly available for use in add-on libraries
        */
        uint8_t         fch, fcl, bch, bcl;
        uint8_t         orient;
        long            disp_x_size, disp_y_size;
        uint8_t         display_model, display_transfer_mode, display_serial_mode;
        _current_font   cfont;
        bool         _transparent;

        void LCD_Writ_Bus(char VH, char VL); 
        uint8_t LCD_Read_Bus(int address);
        void LCD_Write_COM(char VL);
        void LCD_Write_DATA(char VH, char VL);
        void LCD_Write_DATA(char VL);
        void LCD_Write_COM_DATA(char com1, int dat1);
        void LCD_Write_Register(char com1, int dat1);
        void setPixel(uint16_t color);
        void drawHLine(int x, int y, int l);
        void drawVLine(int x, int y, int l);
        void printChar(uint8_t c, int x, int y);
        void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
        void clrXY();
        void resetXY();
        void rotateChar(uint8_t c, int x, int y, int pos, int deg);
        void _set_direction_registers(uint8_t mode);
        void _convert_float(char *buf, double num, int width, uint8_t prec);
        
    private:
        SPI spi;                // does SPI MOSI, MISO and SCK        
        DigitalOut _cs;         // does SPI CE
        DigitalOut _dc;         // lcd dc pin
	    DigitalOut _bkl;        // lcd bkl
};

#endif