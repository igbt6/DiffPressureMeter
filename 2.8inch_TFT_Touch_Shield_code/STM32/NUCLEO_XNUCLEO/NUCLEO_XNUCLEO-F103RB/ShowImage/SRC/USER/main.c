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

