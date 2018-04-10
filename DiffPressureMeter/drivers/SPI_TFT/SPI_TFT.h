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

#ifndef MBED_SPI_TFT_H
#define MBED_SPI_TFT_H


#include "mbed.h"
#include "GraphicsDisplay.h"

#define RGB(r,g,b)  (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

#define SPI_START   (0x70)              /* Start byte for SPI transfer        */
#define SPI_RD      (0x01)              /* WR bit 1 within start              */
#define SPI_WR      (0x00)              /* WR bit 0 within start              */
#define SPI_DATA    (0x02)              /* RS bit 1 within start byte         */
#define SPI_INDEX   (0x00)              /* RS bit 0 within start byte         */


/* some RGB color definitions                                                 */
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 0   */
#define White           0xFFFF      /* 255, 255, 255 */

/** Display control class, based on GraphicsDisplay and TextDisplay
 *
 * Example:
 * @code
 * #include "stdio.h"
 * #include "mbed.h"
 * #include "SPI_TFT.h"
 * #include "string"
 * #include "Arial12x12.h"
 * #include "Arial24x23.h"
 * 
 *
 *
 * // the TFT is connected to SPI pin 5-7 
 * SPI_TFT TFT(p5, p6, p7, p8, p15,"TFT"); // mosi, miso, sclk, cs, reset
 * 
 * int main() {
 *     TFT.claim(stdout);      // send stdout to the TFT display 
 *     //TFT.claim(stderr);      // send stderr to the TFT display
 *
 *     TFT.background(Black);    // set background to black
 *     TFT.foreground(White);    // set chars to white
 *     TFT.cls();                // clear the screen
 *     TFT.set_font((unsigned char*) Arial12x12);  // select the font
 *     
 *     TFT.set_orientation(0);
 *     TFT.locate(0,0);
 *     printf("  Hello Mbed 0");
 *     TFT.set_font((unsigned char*) Arial24x23);  // select font 2
 *     TFT.locate(2,5);
 *     TFT.printf("Bigger Font");
 *  }
 * @endcode
 */
 class SPI_TFT : public GraphicsDisplay {
 public:

  /** Create a SPI_TFT object connected to SPI and two pins
   *
   * @param mosi,miso,sclk SPI
   * @param cs pin connected to CS of display
   * @param reset pin connected to RESET of display
   *
   */ 
  SPI_TFT(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName reset,const char* name ="TFT");
    
  /** Get the width of the screen in pixel
   *
   * @param 
   * @returns width of screen in pixel
   *
   */    
  virtual int width();

  /** Get the height of the screen in pixel
   *
   * @returns height of screen in pixel 
   *
   */     
  virtual int height();
    
  /** Draw a pixel at x,y with color 
   *  
   * @param x horizontal position
   * @param y vertical position
   * @param color 16 bit pixel color
   */    
  virtual void pixel(int x, int y, int colour);
    
  /** draw a circle
   *
   * @param x0,y0 center
   * @param r radius
   * @param color 16 bit color                                                                 *
   *
   */    
  void circle(int x, int y, int r, int colour); 
  
  /** draw a filled circle
   *
   * @param x0,y0 center
   * @param r radius
   * @param color 16 bit color                                                                 *
   *
   * use circle with different radius,
   * can miss some pixel
   */    
  void fillcircle(int x, int y, int r, int colour); 
  
 
    
  /** draw a 1 pixel line
   *
   * @param x0,y0 start point
   * @param x1,y1 stop point
   * @param color 16 bit color
   *
   */    
  void line(int x0, int y0, int x1, int y1, int colour);
    
  /** draw a rect
   *
   * @param x0,y0 top left corner
   * @param x1,y1 down right corner
   * @param color 16 bit color
   *                                                   *
   */    
  void rect(int x0, int y0, int x1, int y1, int colour);
    
  /** draw a filled rect
   *
   * @param x0,y0 top left corner
   * @param x1,y1 down right corner
   * @param color 16 bit color
   *
   */    
  void fillrect(int x0, int y0, int x1, int y1, int colour);
    
  /** setup cursor position
   *
   * @param column 0 to max
   * @param row 0 to max 
   * max depend on font size
   */   
  void locate(int column, int row);
    
  /** Fill the screen with _backgroun color
   *
   */   
  virtual void cls (void);   
    
  /** calculate the max number of char in a line
   *
   * @returns max columns
   * depends on actual font size
   *
   */    
  int columns(void);
    
  /** calculate the max number of columns
   *
   * @returns max column
   * depends on actual font size
   *
   */   
  int rows(void);
    
  /** put a char on the screen
   *
   * @param value char to print
   * @returns printed char
   *
   */
  int _putc(int value);
    
  /** draw a character on given position out of the active font to the TFT
   *
   * @param col column
   * @param row row
   * @param c char to print
   *
   */    
  virtual void character(int col, int row, int c);
    
  /** paint a bitmap on the TFT 
   *
   * @param x,y : upper left corner 
   * @param w width of bitmap
   * @param h high of bitmap
   * @param *bitmap pointer to the bitmap data
   *
   *   bitmap format: 16 bit R5 G6 B5
   * 
   *   use Gimp to create / load , save as BMP, option 16 bit R5 G6 B5            
   *   use winhex to load this file and mark data stating at offset 0x46 to end
   *   use edit -> copy block -> C Source to export C array
   *   paste this array into your program
   * 
   */    
  void Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h,unsigned char *bitmap);
    
  /** select the font to use
   *
   * @param f pointer to font array 
   *                                                                              
   *   font array can created with GLCD Font Creator from http://www.mikroe.com
   *   you have to add 4 parameter at the beginning of the font array to use: 
   *   - the number of byte / char
   *   - the vertial size in pixel
   *   - the horizontal size in pixel
   *   - the number of byte per vertical line
   *   you also have to change the array to char[]
   *
   */   
  void set_font(unsigned char* f);
   
  /** Set the orientation of the screen
   *  x,y: 0,0 is always top left 
   *
   * @param o direction to use the screen (0-3) 90&#65533; Steps  
   *
   */  
  void set_orientation(unsigned int o);
    
  SPI _spi;
  DigitalOut _cs;
  DigitalOut _reset;
  unsigned char* font;
   
protected:

  /** draw a horizontal line
   *
   * @param x0 horizontal start
   * @param x1 horizontal stop
   * @param y vertical position
   * @param color 16 bit color                                               
   *
   */
  void hline(int x0, int x1, int y, int colour);
    
  /** draw a vertical line
   *
   * @param x horizontal position
   * @param y0 vertical start 
   * @param y1 vertical stop
   * @param color 16 bit color
   */
  void vline(int y0, int y1, int x, int colour);
    
  /** Set draw window region
   *
   * @param x horizontal position
   * @param y vertical position
   * @param w window width in pixel
   * @param h window height in pixels
   */    
  void window (unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    
  /** Set draw window region to whole screen
   *
   */  
  void WindowMax (void);
    
  /** Init the HX8347D controller 
   *
   */    
  void tft_reset();
    
   /** Write data to the LCD controller
   *
   * @param dat data written to LCD controller
   * 
   */   
  void wr_dat(int value);
    
  /** Write a command the LCD controller 
   *
   * @param cmd: command to be written   
   *
   */   
  void wr_cmd(int value);
    
   /** Start data sequence to the LCD controller
   * 
   */   
  void wr_dat_start();
    
  /** Stop of data writing to the LCD controller
   *   
   */  
  void wr_dat_stop();
    
  /** write data to the LCD controller
   *
   * @param data to be written 
   *                                           *
   */    
  void wr_dat_only(unsigned short dat);
    
  /** Read data from the LCD controller
   *
   * @returns data from LCD controller
   *  
   */    
  unsigned short rd_dat(void);
    
  /** Write a value to the to a LCD register
   *
   * @param reg register to be written
   * @param val data to be written
   */   
  void wr_reg (unsigned char reg, unsigned short val);
    
  /** Read a LCD register
   *
   * @param reg register to be read
   * @returns value of the register 
   */    
  unsigned short rd_reg (unsigned char reg);
    
  unsigned int orientation;
  unsigned int char_x;
    
};

#endif
