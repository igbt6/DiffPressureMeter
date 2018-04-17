/*
  LUTFT.cpp - Lukasz Uszko 2018 
  Library support for Color TFT LCD (driver HX8347D)
  based on LUTFT library developed by Henning Karlsen
  This is special porting for LUTFT shield LCD screen. 
  Copyright (C)2010-2014 Henning Karlsen All right reserved 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA-02110-1301-USA
*/

#include "LUTFT_HX8347D.h"

LUTFT::LUTFT(PinName mosi, PinName miso, PinName sck, PinName cs)
    : spi(mosi, miso, sck)
    , _CS_OUT()
    , _DC_OUT()
    , _BKL_OUT()

{   
    spi.format(8, 3);
    spi.frequency(50000000); // 50MHz clock
    _CS_SET();
    disp_x_size=239;
    disp_y_size=319;
    display_transfer_mode=SERIAL_5PIN;
    display_model=HX8347D;
}


void LUTFT::LCD_Writ_Bus(char VH, char VL)
{
    if ( VH == LCD_DATA )
    {
        _DC_SET();
    } else // LCD_CMD
    {
        _DC_CLR();
    }
    spi.write (VL);
}

uint8_t LUTFT::LCD_Read_Bus(int address) 
{
  uint8_t value = 0;
  _CS_CLR();
  spi.write(address);
  value = spi.write(0x00);
  _CS_SET();
  return value;
}

void LUTFT::LCD_Write_COM(char VL)  
{   
    LCD_Writ_Bus(0x00,VL);
}

void LUTFT::LCD_Write_DATA(char VL)
{
    LCD_Writ_Bus(0x01, VL);   
}

void LUTFT::LCD_Write_DATA(char VH, char VL)
{
    LCD_Writ_Bus(0x01, VH);
    LCD_Writ_Bus(0x01, VL);
}


void LUTFT::LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8, dat1);
}

void LUTFT::LCD_Write_Register(char com1, int dat1)
{
    LCD_Write_COM(com1);
    LCD_Write_DATA(dat1);
}

void LUTFT::InitLCD(uint8_t orientation)
{
    orient=orientation;

    _BKL_OFF();
    _CS_CLR();
	switch(display_model)
	{
        case HX8347D:
            #include "tft_drivers/hx8347d/initlcd.h"
            break;
    default:
        printf("NOT SUPPORTED LCD !!!!!");
        break;
    }
    
    if (display_transfer_mode == SERIAL_5PIN)
	{
		display_transfer_mode=1;
		display_serial_mode=SERIAL_5PIN;
	}
    setColor(255, 255, 255);
    setBackColor(0, 0, 0);
    cfont.font=0;
    _CS_SET();
    _BKL_ON();
    clrScr();
}

void LUTFT::setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (orient==LANDSCAPE)
    {
        swap(uint16_t, x1, y1);
        swap(uint16_t, x2, y2)
        y1=disp_y_size-y1;
        y2=disp_y_size-y2;
        swap(uint16_t, y1, y2)
    }
	LCD_Write_COM_DATA(0x02,x1>>8);
	LCD_Write_COM_DATA(0x03,x1);
	LCD_Write_COM_DATA(0x04,x2>>8);
	LCD_Write_COM_DATA(0x05,x2);
	LCD_Write_COM_DATA(0x06,y1>>8);
	LCD_Write_COM_DATA(0x07,y1);
	LCD_Write_COM_DATA(0x08,y2>>8);
	LCD_Write_COM_DATA(0x09,y2);
	LCD_Write_COM(0x22);   
}

void LUTFT::clrXY()
{
    if (orient==PORTRAIT)
        setXY(0,0,disp_x_size,disp_y_size);
    else
        setXY(0,0,disp_y_size,disp_x_size);
}

void LUTFT::resetXY()
{
    clrXY();
}

void LUTFT::drawRect(int x1, int y1, int x2, int y2)
{
    if (x1>x2)
    {
        swap(int, x1, x2);
    }
    if (y1>y2)
    {
        swap(int, y1, y2);
    }

    drawHLine(x1, y1, x2-x1);
    drawHLine(x1, y2, x2-x1);
    drawVLine(x1, y1, y2-y1);
    drawVLine(x2, y1, y2-y1);
}

void LUTFT::drawRoundRect(int x1, int y1, int x2, int y2)
{
    if (x1>x2)
    {
        swap(int, x1, x2);
    }
    if (y1>y2)
    {
        swap(int, y1, y2);
    }
    if ((x2-x1)>4 && (y2-y1)>4)
    {
        drawPixel(x1+1,y1+1);
        drawPixel(x2-1,y1+1);
        drawPixel(x1+1,y2-1);
        drawPixel(x2-1,y2-1);
        drawHLine(x1+2, y1, x2-x1-4);
        drawHLine(x1+2, y2, x2-x1-4);
        drawVLine(x1, y1+2, y2-y1-4);
        drawVLine(x2, y1+2, y2-y1-4);
    }
}

void LUTFT::fillRect(int x1, int y1, int x2, int y2)
{
    if (x1>x2)
    {
        swap(int, x1, x2);
    }
    if (y1>y2)
    {
        swap(int, y1, y2);
    }
    
    if (orient==PORTRAIT)
    {
        for (int i=0; i<((y2-y1)/2)+1; i++)
        {
            drawHLine(x1, y1+i, x2-x1);
            drawHLine(x1, y2-i, x2-x1);
        }
    }
    else
    {
        for (int i=0; i<((x2-x1)/2)+1; i++)
        {
            drawVLine(x1+i, y1, y2-y1);
            drawVLine(x2-i, y1, y2-y1);
        }
    }
}

void LUTFT::fillRoundRect(int x1, int y1, int x2, int y2)
{
    if (x1>x2)
    {
        swap(int, x1, x2);
    }
    if (y1>y2)
    {
        swap(int, y1, y2);
    }

    if ((x2-x1)>4 && (y2-y1)>4)
    {
        for (int i=0; i<((y2-y1)/2)+1; i++)
        {
            switch(i)
            {
            case 0:
                drawHLine(x1+2, y1+i, x2-x1-4);
                drawHLine(x1+2, y2-i, x2-x1-4);
                break;
            case 1:
                drawHLine(x1+1, y1+i, x2-x1-2);
                drawHLine(x1+1, y2-i, x2-x1-2);
                break;
            default:
                drawHLine(x1, y1+i, x2-x1);
                drawHLine(x1, y2-i, x2-x1);
            }
        }
    }
}

void LUTFT::drawCircle(int x, int y, int radius)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x1 = 0;
    int y1 = radius;
 
    _CS_CLR();
    setXY(x, y + radius, x, y + radius);
    LCD_Write_DATA(fch,fcl);
    setXY(x, y - radius, x, y - radius);
    LCD_Write_DATA(fch,fcl);
    setXY(x + radius, y, x + radius, y);
    LCD_Write_DATA(fch,fcl);
    setXY(x - radius, y, x - radius, y);
    LCD_Write_DATA(fch,fcl);
 
    while(x1 < y1)
    {
        if(f >= 0) 
        {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }
        x1++;
        ddF_x += 2;
        f += ddF_x;    
        setXY(x + x1, y + y1, x + x1, y + y1);
        LCD_Write_DATA(fch,fcl);
        setXY(x - x1, y + y1, x - x1, y + y1);
        LCD_Write_DATA(fch,fcl);
        setXY(x + x1, y - y1, x + x1, y - y1);
        LCD_Write_DATA(fch,fcl);
        setXY(x - x1, y - y1, x - x1, y - y1);
        LCD_Write_DATA(fch,fcl);
        setXY(x + y1, y + x1, x + y1, y + x1);
        LCD_Write_DATA(fch,fcl);
        setXY(x - y1, y + x1, x - y1, y + x1);
        LCD_Write_DATA(fch,fcl);
        setXY(x + y1, y - x1, x + y1, y - x1);
        LCD_Write_DATA(fch,fcl);
        setXY(x - y1, y - x1, x - y1, y - x1);
        LCD_Write_DATA(fch,fcl);
    }
    _CS_SET();
    clrXY();
}

void LUTFT::fillCircle(int x, int y, int radius)
{
    for(int y1=-radius; y1<=0; y1++) 
        for(int x1=-radius; x1<=0; x1++)
            if(x1*x1+y1*y1 <= radius*radius) 
            {
                drawHLine(x+x1, y+y1, 2*(-x1));
                drawHLine(x+x1, y-y1, 2*(-x1));
                break;
            }
}

void LUTFT::clrScr()
{
    long i;
    _CS_CLR();
    clrXY();

    for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
    {
        LCD_Writ_Bus(1,0);
        LCD_Writ_Bus(1,0);
    }
    _CS_SET();
}

void LUTFT::fillScr(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
    fillScr(color);
}

void LUTFT::fillScr(uint16_t color)
{
    long i;
    char ch, cl;
    
    ch=uint8_t(color>>8);
    cl=uint8_t(color & 0xFF);
    _CS_CLR();
    clrXY();
    
    for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
    {
        LCD_Writ_Bus(1,ch);
        LCD_Writ_Bus(1,cl);     
    }
    _CS_SET();
}

void LUTFT::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    fch=((r&248)|g>>5);
    fcl=((g&28)<<3|b>>3);
}

void LUTFT::setColor(uint16_t color)
{
    fch=uint8_t(color>>8);
    fcl=uint8_t(color & 0xFF);
}

uint16_t LUTFT::getColor()
{
    return (fch<<8) | fcl;
}

void LUTFT::setBackColor(uint8_t r, uint8_t g, uint8_t b)
{
    bch=((r&248)|g>>5);
    bcl=((g&28)<<3|b>>3);
    _transparent=false;
}

void LUTFT::setBackColor(uint32_t color)
{
    if (color==VGA_TRANSPARENT)
        _transparent=true;
    else
    {
        bch=uint8_t(color>>8);
        bcl=uint8_t(color & 0xFF);
        _transparent=false;
    }
}

uint16_t LUTFT::getBackColor()
{
    return (bch<<8) | bcl;
}

void LUTFT::setPixel(uint16_t color)
{
    LCD_Write_DATA((color>>8),(color&0xFF));    // rrrrrggggggbbbbb
}

void LUTFT::drawPixel(int x, int y)
{
    _CS_CLR();
    setXY(x, y, x, y);
    setPixel((fch<<8)|fcl);
    clrXY();
    _CS_SET();
}

void LUTFT::drawLine(int x1, int y1, int x2, int y2)
{
    if (y1==y2)
        drawHLine(x1, y1, x2-x1);
    else if (x1==x2)
        drawVLine(x1, y1, y2-y1);
    else
    {
        unsigned int    dx = (x2 > x1 ? x2 - x1 : x1 - x2);
        short           xstep =  x2 > x1 ? 1 : -1;
        unsigned int    dy = (y2 > y1 ? y2 - y1 : y1 - y2);
        short           ystep =  y2 > y1 ? 1 : -1;
        int             col = x1, row = y1;

        _CS_CLR();
        if (dx < dy)
        {
            int t = - (dy >> 1);
            while (true)
            {
                setXY (col, row, col, row);
                LCD_Write_DATA (fch, fcl);
                if (row == y2)
                    return;
                row += ystep;
                t += dx;
                if (t >= 0)
                {
                    col += xstep;
                    t   -= dy;
                }
            } 
        }
        else
        {
            int t = - (dx >> 1);
            while (true)
            {
                setXY (col, row, col, row);
                LCD_Write_DATA (fch, fcl);
                if (col == x2)
                    return;
                col += xstep;
                t += dy;
                if (t >= 0)
                {
                    row += ystep;
                    t   -= dx;
                }
            } 
        }
        _CS_SET();
    }
    clrXY();
}

void LUTFT::drawHLine(int x, int y, int l)
{
    if (l<0)
    {
        l = -l;
        x -= l;
    }
    _CS_CLR();
    setXY(x, y, x+l, y);
    for (int i=0; i<l+1; i++)
    {
        LCD_Write_DATA(fch, fcl);
    }
    _CS_SET();
    clrXY();
}

void LUTFT::drawVLine(int x, int y, int l)
{
    if (l<0)
    {
        l = -l;
        y -= l;
    }
    _CS_CLR();
    setXY(x, y, x, y+l);
    for (int i=0; i<l+1; i++)
    {
        LCD_Write_DATA(fch, fcl);
    }
    _CS_SET();
    clrXY();
}

void LUTFT::printChar(uint8_t c, int x, int y)
{
    uint8_t i,ch;
    uint16_t j;
    uint16_t temp; 

    _CS_CLR(); 
    if (!_transparent)
    {
        if (orient==PORTRAIT)
        {
            setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
      
            temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
            for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
            {
                ch=pgm_read_byte(&cfont.font[temp]);
                for(i=0;i<8;i++)
                {   
                    if((ch&(1<<(7-i)))!=0)   
                    {
                        setPixel((fch<<8)|fcl);
                    } 
                    else
                    {
                        setPixel((bch<<8)|bcl);
                    }   
                }
                temp++;
            }
        }
        else
        {
            temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

            for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
            {
                setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
                for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
                {
                    ch=pgm_read_byte(&cfont.font[temp+zz]);
                    for(i=0;i<8;i++)
                    {   
                        if((ch&(1<<i))!=0)   
                        {
                            setPixel((fch<<8)|fcl);
                        } 
                        else
                        {
                            setPixel((bch<<8)|bcl);
                        }   
                    }
                }
                temp+=(cfont.x_size/8);
            }
        }
    }
    else
    {
        temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
        for(j=0;j<cfont.y_size;j++) 
        {
            for (int zz=0; zz<(cfont.x_size/8); zz++)
            {
                ch=pgm_read_byte(&cfont.font[temp+zz]); 
                for(i=0;i<8;i++)
                {   
                    setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
                
                    if((ch&(1<<(7-i)))!=0)   
                    {
                        setPixel((fch<<8)|fcl);
                    } 
                }
            }
            temp+=(cfont.x_size/8);
        }
    }
    _CS_SET();
    clrXY();
}

void LUTFT::rotateChar(uint8_t c, int x, int y, int pos, int deg)
{
    uint8_t i,j,ch;
    uint16_t temp; 
    int newx,newy;
    double radian;
    radian=deg*0.0175;  
    _CS_CLR();
    temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
    for(j=0;j<cfont.y_size;j++) 
    {
        for (int zz=0; zz<(cfont.x_size/8); zz++)
        {
            ch=pgm_read_byte(&cfont.font[temp+zz]); 
            for(i=0;i<8;i++)
            {   
                newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
                newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

                setXY(newx,newy,newx+1,newy+1);
                
                if((ch&(1<<(7-i)))!=0)   
                {
                    setPixel((fch<<8)|fcl);
                } 
                else  
                {
                    if (!_transparent)
                        setPixel((bch<<8)|bcl);
                }   
            }
        }
        temp+=(cfont.x_size/8);
    }
    _CS_SET();
    clrXY();
}

void LUTFT::print(char *st, int x, int y, int deg)
{
    int stl, i;

    stl = strlen(st);

    if (orient==PORTRAIT)
    {
    if (x==RIGHT)
        x=(disp_x_size+1)-(stl*cfont.x_size);
    if (x==CENTER)
        x=((disp_x_size+1)-(stl*cfont.x_size))/2;
    }
    else
    {
    if (x==RIGHT)
        x=(disp_y_size+1)-(stl*cfont.x_size);
    if (x==CENTER)
        x=((disp_y_size+1)-(stl*cfont.x_size))/2;
    }

    for (i=0; i<stl; i++)
        if (deg==0)
            printChar(*st++, x + (i*(cfont.x_size)), y);
        else
            rotateChar(*st++, x, y, i, deg);
}

void LUTFT::print(std::string st, int x, int y, int deg)
{
    char buf[st.length()+1];
    print(st.c_str(), x, y, deg);
}

void LUTFT::printNumI(long num, int x, int y, int length, char filler)
{
    char buf[25];
    char st[27];
    bool neg=false;
    int c=0, f=0;
  
    if (num==0)
    {
        if (length!=0)
        {
            for (c=0; c<(length-1); c++)
                st[c]=filler;
            st[c]=48;
            st[c+1]=0;
        }
        else
        {
            st[0]=48;
            st[1]=0;
        }
    }
    else
    {
        if (num<0)
        {
            neg=true;
            num=-num;
        }
      
        while (num>0)
        {
            buf[c]=48+(num % 10);
            c++;
            num=(num-(num % 10))/10;
        }
        buf[c]=0;
      
        if (neg)
        {
            st[0]=45;
        }
      
        if (length>(c+neg))
        {
            for (int i=0; i<(length-c-neg); i++)
            {
                st[i+neg]=filler;
                f++;
            }
        }

        for (int i=0; i<c; i++)
        {
            st[i+neg+f]=buf[c-i-1];
        }
        st[c+neg+f]=0;

    }

    print(st,x,y);
}

void LUTFT::printNumF(double num, uint8_t dec, int x, int y, char divider, int length, char filler)
{
    char st[27];
    bool neg=false;

    if (dec<1)
        dec=1;
    else if (dec>5)
        dec=5;

    if (num<0)
        neg = true;

    _convert_float(st, num, length, dec);

    if (divider != '.')
    {
        for (int i=0; i<sizeof(st); i++)
            if (st[i]=='.')
                st[i]=divider;
    }

    if (filler != ' ')
    {
        if (neg)
        {
            st[0]='-';
            for (int i=1; i<sizeof(st); i++)
                if ((st[i]==' ') || (st[i]=='-'))
                    st[i]=filler;
        }
        else
        {
            for (int i=0; i<sizeof(st); i++)
                if (st[i]==' ')
                    st[i]=filler;
        }
    }

    print(st,x,y);
}

void LUTFT::setFont(uint8_t* font)
{
    cfont.font=font;
    cfont.x_size=fontbyte(0);
    cfont.y_size=fontbyte(1);
    cfont.offset=fontbyte(2);
    cfont.numchars=fontbyte(3);
}

uint8_t* LUTFT::getFont()
{
    return cfont.font;
}

uint8_t LUTFT::getFontXsize()
{
    return cfont.x_size;
}

uint8_t LUTFT::getFontYsize()
{
    return cfont.y_size;
}

void LUTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
    unsigned int col;
    int tx, ty, tc, tsx, tsy;

    if (scale==1)
    {
        if (orient==PORTRAIT)
        {
            _CS_CLR();
            setXY(x, y, x+sx-1, y+sy-1);
            for (tc=0; tc<(sx*sy); tc++)
            {
                col=pgm_read_word(&data[tc]);
                LCD_Write_DATA(col>>8,col & 0xff);
            }
            _CS_SET();
        }
        else
        {
            _CS_CLR();
            for (ty=0; ty<sy; ty++)
            {
                setXY(x, y+ty, x+sx-1, y+ty);
                for (tx=sx-1; tx>=0; tx--)
                {
                    col=pgm_read_word(&data[(ty*sx)+tx]);
                    LCD_Write_DATA(col>>8,col & 0xff);
                }
            }
            _CS_SET();
        }
    }
    else
    {
        if (orient==PORTRAIT)
        {
            _CS_CLR();
            for (ty=0; ty<sy; ty++)
            {
                setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
                for (tsy=0; tsy<scale; tsy++)
                    for (tx=0; tx<sx; tx++)
                    {
                        col=pgm_read_word(&data[(ty*sx)+tx]);
                        for (tsx=0; tsx<scale; tsx++)
                            LCD_Write_DATA(col>>8,col & 0xff);
                    }
            }
           _CS_SET();
        }
        else
        {
            _CS_CLR();
            for (ty=0; ty<sy; ty++)
            {
                for (tsy=0; tsy<scale; tsy++)
                {
                    setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
                    for (tx=sx-1; tx>=0; tx--)
                    {
                        col=pgm_read_word(&data[(ty*sx)+tx]);
                        for (tsx=0; tsx<scale; tsx++)
                            LCD_Write_DATA(col>>8,col & 0xff);
                    }
                }
            }
           _CS_SET();
        }
    }
    clrXY();
}

void LUTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
    unsigned int col;
    int tx, ty, newx, newy;
    double radian;
    radian=deg*0.0175;  

    if (deg==0)
        drawBitmap(x, y, sx, sy, data);
    else
    {
        _CS_CLR();
        for (ty=0; ty<sy; ty++)
            for (tx=0; tx<sx; tx++)
            {
                col=pgm_read_word(&data[(ty*sx)+tx]);

                newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
                newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

                setXY(newx, newy, newx, newy);
                LCD_Write_DATA(col>>8,col & 0xff);
            }
        _CS_SET();
    }
    clrXY();
}

void LUTFT::lcdOff()
{

}

void LUTFT::lcdOn()
{

}

void LUTFT::setContrast(char c)
{

}

int LUTFT::getDisplayXSize()
{
    if (orient==PORTRAIT)
        return disp_x_size+1;
    else
        return disp_y_size+1;
}

int LUTFT::getDisplayYSize()
{
    if (orient==PORTRAIT)
        return disp_y_size+1;
    else
        return disp_x_size+1;
}

void LUTFT::setBrightness(uint8_t br)
{

}

void LUTFT::setDisplayPage(uint8_t page)
{

}

void LUTFT::setWritePage(uint8_t page)
{

}

void LUTFT::_convert_float(char *buf, double num, int width, uint8_t prec)
{
  	char format[10];
	sprintf(format, "%%%i.%if", width, prec);
	sprintf(buf, format, num);  
}

void LUTFT::run_demo(LUTFT &tft, int loop_num)
{
    int loops = 0;
    while(loops < loop_num)
    {
        int buf[318];
        int x;
        int y;

        // Clear the screen and draw the frame
        tft.clrScr();

        tft.setColor(255, 0, 0);
        tft.fillRect(0, 0, 319, 13);
        tft.setColor(64, 64, 64);
        tft.fillRect(0, 226, 319, 239);
        tft.setColor(255, 255, 255);
        tft.setBackColor(255, 0, 0);
        tft.print("*LUTFT Display Library HX8347D*", CENTER, 1);
        tft.setBackColor(64, 64, 64);
        tft.setColor(255,255,0);
        tft.print("luszko@op.pl aka igbt6", CENTER, 227);

        tft.setColor(0, 0, 255);
        tft.drawRect(0, 14, 319, 225);

        // Draw crosshairs
        tft.setColor(0, 0, 255);
        tft.setBackColor(0, 0, 0);
        tft.drawLine(159, 15, 159, 224);
        tft.drawLine(1, 119, 318, 119);
        for (int i=9; i<310; i+=10)
            tft.drawLine(i, 117, i, 121);
        for (int i=19; i<220; i+=10)
            tft.drawLine(157, i, 161, i);

        // Draw sin-, cos- and tan-lines  
        tft.setColor(0,255,255);
        tft.print("Sin", 5, 15);
        for (int i=1; i<318; i++)
        {
            tft.drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
        }
        
        tft.setColor(255,0,0);
        tft.print("Cos", 5, 27);
        for (int i=1; i<318; i++)
        {
            tft.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
        }

        tft.setColor(255,255,0);
        tft.print("Tan", 5, 39);
        for (int i=1; i<318; i++)
        {
            tft.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
        }

        wait_ms(2000);

        tft.setColor(0,0,0);
        tft.fillRect(1,15,318,224);
        tft.setColor(0, 0, 255);
        tft.setBackColor(0, 0, 0);
        tft.drawLine(159, 15, 159, 224);
        tft.drawLine(1, 119, 318, 119);

        // Draw a moving sinewave
        x=1;
        for (int i=1; i<(318*20); i++) 
        {
            x++;
            if (x==319)
            x=1;
            if (i>319)
            {
            if ((x==159)||(buf[x-1]==119))
                tft.setColor(0,0,255);
            else
                tft.setColor(0,0,0);
            tft.drawPixel(x,buf[x-1]);
            }
            tft.setColor(0,255,255);
            y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
            tft.drawPixel(x,y);
            buf[x-1]=y;
        }

        wait_ms(2000);
        
        tft.setColor(0,0,0);
        tft.fillRect(1,15,318,224);

        // Draw some filled rectangles
        for (int i=1; i<6; i++)
        {
            switch (i)
            {
            case 1:
                tft.setColor(255,0,255);
                break;
            case 2:
                tft.setColor(255,0,0);
                break;
            case 3:
                tft.setColor(0,255,0);
                break;
            case 4:
                tft.setColor(0,0,255);
                break;
            case 5:
                tft.setColor(255,255,0);
                break;
            }
            tft.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
        }

        wait_ms(2000);
        
        tft.setColor(0,0,0);
        tft.fillRect(1,15,318,224);

        // Draw some filled, rounded rectangles
        for (int i=1; i<6; i++)
        {
            switch (i)
            {
            case 1:
                tft.setColor(255,0,255);
                break;
            case 2:
                tft.setColor(255,0,0);
                break;
            case 3:
                tft.setColor(0,255,0);
                break;
            case 4:
                tft.setColor(0,0,255);
                break;
            case 5:
                tft.setColor(255,255,0);
                break;
            }
            tft.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
        }
        
        wait_ms(2000);
        
        tft.setColor(0,0,0);
        tft.fillRect(1,15,318,224);

        // Draw some filled circles
        for (int i=1; i<6; i++)
        {
            switch (i)
            {
            case 1:
                tft.setColor(255,0,255);
                break;
            case 2:
                tft.setColor(255,0,0);
                break;
            case 3:
                tft.setColor(0,255,0);
                break;
            case 4:
                tft.setColor(0,0,255);
                break;
            case 5:
                tft.setColor(255,255,0);
                break;
            }
            tft.fillCircle(100+(i*20),60+(i*20), 30);
        }
        
        wait_ms(2000);
        
        tft.setColor(0,0,0);
        tft.fillRect(1,15,318,224);

        // Draw some lines in a pattern
        tft.setColor (255,0,0);
        for (int i=15; i<224; i+=5)
        {
            tft.drawLine(1, i, (i*1.44)-10, 224);
        }
        tft.setColor (255,0,0);
        for (int i=224; i>15; i-=5)
        {
            tft.drawLine(318, i, (i*1.44)-11, 15);
        }
        tft.setColor (0,255,255);
        for (int i=224; i>15; i-=5)
        {
            tft.drawLine(1, i, 331-(i*1.44), 15);
        }
        tft.setColor (0,255,255);
        for (int i=15; i<224; i+=5)
        {
            tft.drawLine(318, i, 330-(i*1.44), 224);
        }
        
        wait_ms(2000);
        
        tft.setColor(0,0,0);
        tft.fillRect(1,15,318,224);

        tft.fillScr(0, 0, 255);
        tft.setColor(255, 0, 0);
        tft.fillRoundRect(80, 70, 239, 169);
        
        tft.setColor(255, 255, 255);
        tft.setBackColor(255, 0, 0);
        tft.print("That's it!", CENTER, 93);
        tft.print("Restarting in a", CENTER, 119);
        tft.print("few seconds...", CENTER, 132);
        
        tft.setColor(0, 255, 0);
        tft.setBackColor(0, 0, 255);
        tft.print("Runtime: (msecs)", CENTER, 210);
        tft.printNumI(113535, CENTER, 225);
        
        wait_ms (10000);
    } 
}

