#include "LIB_Config.h"
#include "PF_Config.h"

char* pDirectoryFiles[MAX_BMP_FILES];
FATFS microSDFatFs;
uint8_t str[20];

void SDCard_Config(void)
{
  uint32_t counter = 0;
  
  /* Check the mounted device */
  if(f_mount(&microSDFatFs, (TCHAR const*)"/", 0) != FR_OK)
  {
      lcd_display_string(0, 16, "FATFS_NOT_MOUNTED", 16, RED);
  }  
  else
  {
    /* Initialize the Directory Files pointers (heap) */
    for (counter = 0; counter < MAX_BMP_FILES; counter++)
    {
      pDirectoryFiles[counter] = malloc(11); 
    }
  }
}

static void Display_Images(void);


int main(void) 
{   
	system_init();
    SDCard_Config();
	while (1) {
       Display_Images();
	}
}

static void Display_Images(void)
{    
    uint32_t bmplen = 0x00;
    uint32_t checkstatus = 0x00;
    uint32_t filesnumbers = 0x00;
    uint32_t bmpcounter = 0x00;
    DIR directory;
    FRESULT res;
   
    /* Open directory */
    res= f_opendir(&directory, "/");
    if((res != FR_OK))
    {
      if(res == FR_NO_FILESYSTEM)
      {
        /* Display message: SD card not FAT formated */
        lcd_display_string(0, 32, "SD_CARD_NOT_FORMATTED", 16, RED);  
          
      }
      else
      {
        /* Display message: Fail to open directory */
         lcd_display_string(0, 48, "SD_CARD_OPEN_FAIL", 16, RED);           
      }
    }
    
    /* Get number of bitmap files */
    filesnumbers = Storage_GetDirectoryBitmapFiles ("/", pDirectoryFiles);    
    /* Set bitmap counter to display first image */
    bmpcounter = 1; 
    
    while (1)
    {     
        sprintf((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter -1]);
        
        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
        
        if(checkstatus == 0)
        {
          /* Format the string */
          Storage_OpenReadFile(0, 0, (const char*)str); 
        }
        else if (checkstatus == 1)
        {
          /* Display message: SD card does not exist */
           lcd_display_string(0, 64, "SD_CARD_NOT_FOUND", 16, RED);  
        }
        else
        {
          /* Display message: File not supported */
            lcd_display_string(0, 80, "SD_CARD_FILE_NOT_SUPPORTED", 16, RED); 
        }
        
        bmpcounter ++;
        if(bmpcounter > filesnumbers)
        {
          bmpcounter = 1;
        }
        
        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
    }
}




























/*
unsigned char data[2048]; 
FATFS fs;
FRESULT res;
UINT br;
const char filename[4][11]={"flower.bmp","RedRose.bmp","tiger.bmp","tree.bmp"};
void Pic_Viewer(void);


void Pic_Viewer(void)
{
//    uint16_t k;
    uint16_t i,j=0;
    FIL file;
    unsigned int cnt,file_cnt,temp1;
    uint16_t temp;
  while(1)
  {       
    
          if(j>3)
          {
            //lcd_display_char(line9, 30, "BMP PLAY ...");
            lcd_display_string(0, 0, "BMP PLAY ...", 16, RED);
            delay_ms(3000);     //??5?
            break;
            //j=0;  //
          }
       
	   for(j=0;j<4;j++)
	     {    
                   //ili9481_Clear(WHITE);  //???????
//               lcd_clear_screen(WHITE);
	           res = f_mount(0, &fs);	
	           res = f_open(&file, filename[j], FA_OPEN_EXISTING | FA_READ);   //????  
                   if(j>3)
                   {
                     break;
                   }
                   
//                do {
//                   lcd_clear_screen(WHITE);
//                   res = f_mount(0, &fs);	
//                   res = f_open(&file, filename[j], FA_OPEN_EXISTING | FA_READ);   //????  
//                   if(j>3)
//                   {
//                     break;
//                   }
//                    
//                }while(res!=FR_OK);
//                lcd_display_string(0, 16, "BMP PLAY FAILD...", 16, RED);
		    if(res!=FR_OK)
		   {
		        //while(1);
                        //lcd_display_char(line5, 30, "BMP PLAY FAILD...");
               lcd_display_string(0, 16, "BMP PLAY FAILD...", 16, RED);
		    }
           else 
           {
               lcd_display_string(0, 16, "                 ", 16, RED);
           }
            
	            res = f_read(&file, data, 0x36, &br);     //???????
	            lcd_set_cursor(0,0);
		    if(data[0]==0x42 && data[1]==0x4D)        //???????BM
			{
			          //printp("\r\n ???3---??~!\r\n");
			          
			          temp = data[18]+(data[19]<<8)+(data[20]<<16)+(data[21]<<24);
			          temp1 = data[22]+(data[23]<<8)+(data[24]<<16)+(data[25]<<24);
			          if(temp==240 && temp1==320)             //?????:Width320*Height240
                      //if(temp==320 && temp1==480)             //?????:Width320*Height240
			          {

                                      temp = data[28]+(data[29]<<8);
                                        if(temp==32)                          //32???????
                                        {
                                          file_cnt = data[2]+(data[3]<<8)+(data[4]<<16)+(data[5]<<24);//??????
                                          cnt=36;
                                              while(1)
                                              { 
                                                res = f_read(&file, data, 2048, &br);   //??2048???
                                                cnt += sizeof(data);
                                                if(cnt >= file_cnt)                             //????????,??
                                                  break;

                                                        for(i=0;i<512;i++)                              //????
                                                        { 	
                                                          //ili9481_WriteData((data[i*4+2]>>3)<<11 | 
                                                                       //(data[i*4+1]>>2)<<5 |(data[i*4]>>3));
                                                            lcd_write_byte(0x22, LCD_CMD);
    
                                                            __LCD_DC_SET();
                                                            __LCD_CS_CLR();
                                                            __LCD_WRITE_BYTE(((data[i*4+2]>>3)<<11 | (data[i*4+1]>>2)<<5 |(data[i*4]>>3)) >> 8);
                                                            __LCD_WRITE_BYTE(((data[i*4+2]>>3)<<11 | (data[i*4+1]>>2)<<5 |(data[i*4]>>3)) & 0xFF);
                                                            __LCD_CS_SET();
                                                         
                                                        }    
                                                 
                                            
                                                    }   
                                                                      
                                               }
                                        else if(temp==24)                     //24????
                                        {
                                            file_cnt = data[2]+(data[3]<<8)+(data[4]<<16)+(data[5]<<24);  //??????
                                            cnt=36;
                                            while(1)
                                            {
                                              res = f_read(&file, data, 1536, &br);                     //??1536???
                                              cnt += 1536;
                                              if(cnt >= file_cnt)                                       //????????,??
                                                break;
                                              for(i=0;i<512;i++)
                                              {
//                                               ili9481_WriteData((data[i*3+2]*32/256)<<11 | 
//                                                              (data[i*3+1]*64/256)<<5 |(data[i*3]*32/256));
                                                  
                                                    lcd_write_byte(0x22, LCD_CMD);
                                                    __LCD_DC_SET();
                                                    __LCD_CS_CLR();
                                                    __LCD_WRITE_BYTE(((data[i*3+2]*32/256)<<11 | (data[i*3+1]*64/256)<<5 |(data[i*3]*32/256)) >> 8);
                                                    __LCD_WRITE_BYTE(((data[i*3+2]*32/256)<<11 | (data[i*3+1]*64/256)<<5 |(data[i*3]*32/256)) & 0xFF);
                                                    __LCD_CS_SET();
                                              }
                                            }                  
                                        }//end ese if
                                        
                                         else if(temp==16)                     //24????
                                        {
                                            file_cnt = data[2]+(data[3]<<8)+(data[4]<<16)+(data[5]<<24);  //??????
                                            cnt=36;
                                            while(1)
                                            {
                                              res = f_read(&file, data,1024, &br);                     //??1536???
                                              cnt += 1024;
                                              if(cnt >= file_cnt)                                       //????????,??
                                                break;
                                              for(i=0;i<512;i++)
                                              {
 
                                               //ili9481_WriteData(((data[i*2+1])<<9|(data[i*2])));
                                                lcd_write_byte(0x22, LCD_CMD);
                                                __LCD_DC_SET();
                                                __LCD_CS_CLR();
                                                __LCD_WRITE_BYTE(((data[i*2+1])<<9|(data[i*2])) >> 8);
                                                __LCD_WRITE_BYTE(((data[i*2+1])<<9|(data[i*2])) & 0xFF);
                                                __LCD_CS_SET();

                                              }
                                            }                  
                                        }//end ese if
				            
			          }
                                  else
                                  {
                                     lcd_clear_screen(WHITE);  //???????
                                      //delay_ms(500);     //??3?
                                      //d_WriteString(50,140," Image is too big... ",RED,YELLOW);
                                      //Lcd_WriteString(50,160,"  Play  Images~!   ",RED,YELLOW);
                                      //lcd_display_char(line5, 30, "BMP ERROR...");
                                      lcd_display_string(0, 32, "BMP ERROR...", 16, RED);
                                      delay_ms(500);     //??3?
                                      //ili9481_Clear(WHITE);  //???????
                                      lcd_clear_screen(WHITE); 
                                      //TFTSetXY(0,0);
                                      lcd_set_cursor(0, 0);
                                      file_cnt = data[2]+(data[3]<<8)+(data[4]<<16)+(data[5]<<24);//??????
                                          cnt=36;
                                              while(1)
                                              { 
                                                res = f_read(&file, data, 2048, &br);   //??2048???
                                                cnt += sizeof(data);
                                                if(cnt >= file_cnt)                             //????????,??
                                                  break;

                                                    for(i=0;i<512;i++)                              //????
                                                    { 	
//                                                      ili9481_WriteData((data[i*4+2]>>3)<<11 | 
//                                                                   (data[i*4+1]>>2)<<5 |(data[i*4]>>3));
                                                        lcd_write_byte(0x22, LCD_CMD);
    
                                                        __LCD_DC_SET();
                                                        __LCD_CS_CLR();
                                                        __LCD_WRITE_BYTE(((data[i*4+2]>>3)<<11 | (data[i*4+1]>>2)<<5 |(data[i*4]>>3)) >> 8);
                                                        __LCD_WRITE_BYTE(((data[i*4+2]>>3)<<11 | (data[i*4+1]>>2)<<5 |(data[i*4]>>3)) & 0xFF);
                                                        __LCD_CS_SET();
                                                     
                                                    }    
                                                                                                                         
                                               }
                                   }//end else
		        }//end if			
		   // Close open files 
	      f_close(&file);
	      // Unregister work area prior to discard it 
	      f_mount(0, NULL);	  
	      delay_ms(2000);     //??3?
	      }//end for(;;)
		  
       }//end while(1)
}
*/
/*-------------------------------END OF FILE-------------------------------*/

