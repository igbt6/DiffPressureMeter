/* mbed library for 240*320 pixel display TFT based on HX8347D LCD Controller
 * Copyright (c) 2011 Peter Drescher - DC2PD
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SPI_TFT.h"
#include "mbed.h"


#define BPP         16                  // Bits per pixel                



SPI_TFT::SPI_TFT(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName reset, const char *name)
        : _spi(mosi, miso, sclk), _cs(cs), _reset(reset),GraphicsDisplay(name) {
    tft_reset();
    orientation = 0;
    char_x = 0;
}

int SPI_TFT::width() {
    if (orientation == 0 || orientation == 2) return 240;
    else return 320;
}


int SPI_TFT::height() {
    if (orientation == 0 || orientation == 2) return 320;
    else return 240;
}



void SPI_TFT::set_orientation(unsigned int o) {
    orientation = o;
    switch (orientation) {
        case 0:
            wr_reg(0x16, 0x0008);
            break;
        case 1:
            wr_reg(0x16, 0x0068);
            break;
        case 2:
            wr_reg(0x16, 0x00C8);
            break;
        case 3:
            wr_reg(0x16, 0x00A8);
            break;
    }
}



void SPI_TFT::wr_cmd(int cmd) {
    _cs = 0;
    _spi.write(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0   */
    _spi.write(cmd);
    _cs = 1;
}



void SPI_TFT::wr_dat(int dat) {
    _cs = 0;
    _spi.write(SPI_START | SPI_WR | SPI_DATA);    // Write : RS = 1, RW = 0
    _spi.format(16,3);                            // switch to 16 bit Mode 3
    _spi.write(dat);                              // Write D0..D15
    _spi.format(8,3);                             // 8 bit Mode 3
    _cs = 1;
}



void SPI_TFT::wr_dat_start(void) {
    _cs = 0;
    _spi.write(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
}



void SPI_TFT::wr_dat_stop (void) {
    _cs = 1;
}



void SPI_TFT::wr_dat_only (unsigned short dat) {

    _spi.format(16,3);                        // switch to 16 bit Mode 3
    _spi.write(dat);                          // Write D0..D15
    _spi.format(8,3);                         // 8 bit Mode 3
}



unsigned short SPI_TFT::rd_dat (void) {
    unsigned short val = 0;

    _cs = 0;
    _spi.write(SPI_START | SPI_RD | SPI_DATA);    /* Read: RS = 1, RW = 1         */
    _spi.write(0);                                /* Dummy read 1                 */
    val   = _spi.write(0);                        /* Read D8..D15                 */
    val <<= 8;
    val  |= _spi.write(0);                        /* Read D0..D7                  */
    _cs = 1;
    return (val);
}



void SPI_TFT::wr_reg (unsigned char reg, unsigned short val) {

    wr_cmd(reg);
    wr_dat(val);
}



unsigned short SPI_TFT::rd_reg (unsigned char reg) {

    wr_cmd(reg);
    return(rd_dat());
}



void SPI_TFT::tft_reset() {
    static unsigned short driverCode;
    _spi.format(8,3);                 // 8 bit spi mode 3
    _spi.frequency(48000000);         // 48Mhz SPI clock
    _reset = 0;                       // reset
    _cs = 1;
    wait_us(50);
    _reset = 1;                       // end reset
    wait_ms(5);

    driverCode = rd_reg(0x00);        // read controller ID
    //printf("Disp_ID = %x",driverCode);

    /* Start Initial Sequence ----------------------------------------------------*/
    wr_reg(0xEA, 0x0000);                 /* Reset Power Control 1                */
    wr_reg(0xEB, 0x0020);                 /* Power Control 2                      */
    wr_reg(0xEC, 0x000C);                 /* Power Control 3                      */
    wr_reg(0xED, 0x00C4);                 /* Power Control 4                      */
    wr_reg(0xE8, 0x0040);                 /* Source OPON_N                        */
    wr_reg(0xE9, 0x0038);                 /* Source OPON_I                        */
    wr_reg(0xF1, 0x0001);                 /*                                      */
    wr_reg(0xF2, 0x0010);                 /*                                      */
    wr_reg(0x27, 0x00A3);                 /* Display Control 2                    */

    /* Power On sequence ---------------------------------------------------------*/
    wr_reg(0x1B, 0x001B);                 /* Power Control 2                      */
    wr_reg(0x1A, 0x0001);                 /* Power Control 1                      */
    wr_reg(0x24, 0x002F);                 /* Vcom Control 2                       */
    wr_reg(0x25, 0x0057);                 /* Vcom Control 3                       */
    wr_reg(0x23, 0x008D);                 /* Vcom Control 1                       */

    /* Gamma settings  -----------------------------------------------------------*/
    wr_reg(0x40,0x00);   //
    wr_reg(0x41,0x00);   //
    wr_reg(0x42,0x01);   //
    wr_reg(0x43,0x13);   //
    wr_reg(0x44,0x10);   //
    wr_reg(0x45,0x26);   //
    wr_reg(0x46,0x08);   //
    wr_reg(0x47,0x51);   //
    wr_reg(0x48,0x02);   //
    wr_reg(0x49,0x12);   //
    wr_reg(0x4A,0x18);   //
    wr_reg(0x4B,0x19);   //
    wr_reg(0x4C,0x14);   //
    wr_reg(0x50,0x19);   //
    wr_reg(0x51,0x2F);   //
    wr_reg(0x52,0x2C);   //
    wr_reg(0x53,0x3E);   //
    wr_reg(0x54,0x3F);   //
    wr_reg(0x55,0x3F);   //
    wr_reg(0x56,0x2E);   //
    wr_reg(0x57,0x77);   //
    wr_reg(0x58,0x0B);   //
    wr_reg(0x59,0x06);   //
    wr_reg(0x5A,0x07);   //
    wr_reg(0x5B,0x0D);   //
    wr_reg(0x5C,0x1D);   //
    wr_reg(0x5D,0xCC);   //

    /* Power + Osc ---------------------------------------------------------------*/
    wr_reg(0x18, 0x0036);                 /* OSC Control 1                        */
    wr_reg(0x19, 0x0001);                 /* OSC Control 2                        */
    wr_reg(0x01, 0x0000);                 /* Display Mode Control                 */
    wr_reg(0x1F, 0x0088);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */
    wr_reg(0x1F, 0x0080);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */
    wr_reg(0x1F, 0x0090);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */
    wr_reg(0x1F, 0x00D0);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */

    wr_reg(0x17, 0x0005);                 /* Colmod 16Bit/Pixel                   */

    wr_reg(0x36, 0x0000);                 /* Panel Characteristic                 */
    wr_reg(0x28, 0x0038);                 /* Display Control 3                    */
    wait_ms(40);
    wr_reg(0x28, 0x003C);                 /* Display Control 3                    */
    switch (orientation) {
        case 0:
            wr_reg(0x16, 0x0008);
            break;
        case 1:
            wr_reg(0x16, 0x0068);
            break;
        case 2:
            wr_reg(0x16, 0x00C8);
            break;
        case 3:
            wr_reg(0x16, 0x00A8);
            break;
    }

    WindowMax ();
}




void SPI_TFT::pixel(int x, int y, int color) {
    wr_reg(0x03, (x >> 0));
    wr_reg(0x02, (x >> 8));
    wr_reg(0x07, (y >> 0));
    wr_reg(0x06, (y >> 8));
    wr_cmd(0x22);
    wr_dat(color);
}




void SPI_TFT::window (unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    wr_reg(0x03, (x >> 0));
    wr_reg(0x02, (x >> 8));
    wr_reg(0x05, (x+w-1 >> 0));
    wr_reg(0x04, (x+w-1 >> 8));
    wr_reg(0x07, ( y >> 0));
    wr_reg(0x06, ( y >> 8));
    wr_reg(0x09, ( y+h-1 >> 0));
    wr_reg(0x08, ( y+h-1 >> 8));
    //wr_cmd(0x22);
}


void SPI_TFT::WindowMax (void) {
    window (0, 0, width(),  height());
}


void SPI_TFT::cls (void) {
    unsigned int i;
    WindowMax();
    wr_cmd(0x22);
    wr_dat_start();
    _spi.format(16,3);         // 16 bit Mode 3
    for (i = 0; i < ( width() * height()); i++)
        _spi.write(_background);
    _spi.format(8,3);         // 8 bit Mode 3
    wr_dat_stop();
}


void SPI_TFT::circle(int x0, int y0, int r, int color) {

    int draw_x0, draw_y0;
    int draw_x1, draw_y1;
    int draw_x2, draw_y2;
    int draw_x3, draw_y3;
    int draw_x4, draw_y4;
    int draw_x5, draw_y5;
    int draw_x6, draw_y6;
    int draw_x7, draw_y7;
    int xx, yy;
    int di;
    WindowMax();
    if (r == 0) {       /* no radius */
        return;
    }

    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < height()) {
        pixel(draw_x0, draw_y0, color);     /* 90 degree */
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0) {
        pixel(draw_x2, draw_y2, color);    /* 270 degree */
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < width()) {
        pixel(draw_x4, draw_y4, color);     /* 0 degree */
    }

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5>=0) {
        pixel(draw_x5, draw_y5, color);     /* 180 degree */
    }

    if (r == 1) {
        return;
    }

    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while (xx < yy) {

        if (di < 0) {
            di += 4*xx + 6;
        } else {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        if ( (draw_x0 <= width()) && (draw_y0>=0) ) {
            pixel(draw_x0, draw_y0, color);
        }

        if ( (draw_x1 >= 0) && (draw_y1 >= 0) ) {
            pixel(draw_x1, draw_y1, color);
        }

        if ( (draw_x2 <= width()) && (draw_y2 <= height()) ) {
            pixel(draw_x2, draw_y2, color);
        }

        if ( (draw_x3 >=0 ) && (draw_y3 <= height()) ) {
            pixel(draw_x3, draw_y3, color);
        }

        if ( (draw_x4 <= width()) && (draw_y4 >= 0) ) {
            pixel(draw_x4, draw_y4, color);
        }

        if ( (draw_x5 >= 0) && (draw_y5 >= 0) ) {
            pixel(draw_x5, draw_y5, color);
        }
        if ( (draw_x6 <=width()) && (draw_y6 <= height()) ) {
            pixel(draw_x6, draw_y6, color);
        }
        if ( (draw_x7 >= 0) && (draw_y7 <= height()) ) {
            pixel(draw_x7, draw_y7, color);
        }
    }
    return;
}

void SPI_TFT::fillcircle(int x, int y, int r, int color){
    int i;
    for (i = 0;i <= r; i++)
        circle(x,y,i,color);
}



void SPI_TFT::hline(int x0, int x1, int y, int color) {
    int w;
    w = x1 - x0 + 1;
    window(x0,y,w,1);
    wr_cmd(0x22);
    wr_dat_start();
    _spi.format(16,3);          // pixel are send in 16 bit mode to speed up
    for (int x=0; x<w; x++) {
        _spi.write(color);
    }
    _spi.format(8,3);
    wr_dat_stop();
    return;
}



void SPI_TFT::vline(int x, int y0, int y1, int color) {
    int h;
    h = y1 - y0 + 1;
    window(x,y0,1,h);
    wr_cmd(0x22);
    wr_dat_start();
    _spi.format(16,3);          // pixel are send in 16 bit mode to speed up
    for (int y=0; y<h; y++) {
        _spi.write(color);
    }
    _spi.format(8,3);
    wr_dat_stop();
    return;
}



void SPI_TFT::line(int x0, int y0, int x1, int y1, int color) {
    WindowMax();
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;

    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0) {        /* vertical line */
        if (y1 > y0) vline(x0,y0,y1,color);
        else vline(x0,y1,y0,color);
        return;
    }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 > x0) hline(x0,x1,y0,color);
        else  hline(x1,x0,y0,color);
        return;
    }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

            pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        pixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
            pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        pixel(x0, y0, color);
    }
    return;
}




void SPI_TFT::rect(int x0, int y0, int x1, int y1, int color) {

    if (x1 > x0) hline(x0,x1,y0,color);
    else  hline(x1,x0,y0,color);

    if (y1 > y0) vline(x0,y0,y1,color);
    else vline(x0,y1,y0,color);

    if (x1 > x0) hline(x0,x1,y1,color);
    else  hline(x1,x0,y1,color);

    if (y1 > y0) vline(x1,y0,y1,color);
    else vline(x1,y1,y0,color);

    return;
}



void SPI_TFT::fillrect(int x0, int y0, int x1, int y1, int color) {

    int h = y1 - y0 + 1;
    int w = x1 - x0 + 1;
    int pixel = h * w;
    window(x0,y0,w,h);
    wr_cmd(0x22);
    wr_dat_start();
    _spi.format(16,3);          // pixel are send in 16 bit mode to speed up
    for (int p=0; p<pixel; p++) {
        _spi.write(color);
    }
    _spi.format(8,3);
    wr_dat_stop();
    return;
}



void SPI_TFT::locate(int column, int row) {
    _column = column;
    char_x = font[1] * column;   // get the horz. size of the actual font
    _row = row;
}



int SPI_TFT::columns() {
    return width() / font[1];
}



int SPI_TFT::rows() {
    return height() / font[2];
}



int SPI_TFT::_putc(int value) {
    if (value == '\n') {
        _column = 0;
        char_x = 0;
        _row++;
        if (_row >= rows()) {
            _row = 0;
        }
    } else {
        character(_column, _row, value);
        _column++;
    }
    return value;
}




void SPI_TFT::character(int col, int row, int c) {
    unsigned int hor,vert,offset,bpl,j,i,b;
    unsigned char* zeichen;
    unsigned char z,w;

    if ((c < 31) || (c > 127)) return;   // test char range

    // read font parameter from start of array
    offset = font[0];                    // bytes / char
    hor = font[1];                       // get hor size of font
    vert = font[2];                      // get vert size of font
    bpl = font[3];                       // bytes per line

    if (char_x + hor > width()) {
        char_x = 0;
        _column = 0;
        _row ++;
        row++;
        if (_row >= rows()) {
            _row = 0;
            row=0;
        }
    }

    window(char_x, row * vert,hor,vert); // char box
    wr_cmd(0x22);
    wr_dat_start();
    zeichen = &font[((c -32) * offset) + 4]; // start of char bitmap
    w = zeichen[0];                          // width of actual char
    _spi.format(16,3);                       // pixel are 16 bit

    for (j=0; j<vert; j++) {  //  vert line
        for (i=0; i<hor; i++) {   //  horz line
            z =  zeichen[bpl * i + ((j & 0xF8) >> 3)+1];
            b = 1 << (j & 0x07);
            if (( z & b ) == 0x00) {
                _spi.write(_background);
            } else {
                _spi.write(_foreground);
            }
        }
    }
    _spi.format(8,3);                      // 8 bit
    wr_dat_stop();
    if ((w + 2) < hor) {                   // x offset to next char
        char_x += w + 2;
    } else char_x += hor;
}





void SPI_TFT::set_font(unsigned char* f) {
    font = f;
}



void SPI_TFT::Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned char *bitmap) {
    unsigned int    i,j;
    unsigned short *bitmap_ptr = (unsigned short *)bitmap;
    window(x, y, w, h);
    wr_cmd(0x22);
    wr_dat_start();
    _spi.format(16,3);
    bitmap_ptr += ((h - 1)*w);
    for (j = 0; j < h; j++) {        //Lines
        for (i = 0; i < w; i++) {     // copy pixel data to TFT
            _spi.write(*bitmap_ptr);    // one line
            bitmap_ptr++;
        }
        bitmap_ptr -= 2*w;
    }
    _spi.format(8,3);
    wr_dat_stop();
}