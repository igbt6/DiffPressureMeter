/**
  ******************************************************************************
  * @file    LIB_Config.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief     This file provides configurations for low layer hardware libraries.
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
#ifndef _USE_LIB_CONFIG_H_
#define _USE_LIB_CONFIG_H_
//Macro Definition

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"
#include "stm32f10x.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------*/
//delay
#include "delay.h"

/*------------------------------------------------------------------------------------------------------*/
//SPI
#include "SPI.h"
/*------------------------------------------------------------------------------------------------------*/
//LCD
#include "LCD.h"
#include "Fonts.h"

//#define LCD_RST_GPIO   GPIOB
#define LCD_BKL_GPIO   GPIOC
#define LCD_DC_GPIO    GPIOA
#define LCD_CS_GPIO    GPIOB
#define LCD_CLK_GPIO   GPIOA
#define LCD_SDI_GPIO   GPIOA
#define LCD_SDO_GPIO   GPIOA


//#define LCD_RST_PIN    GPIO_Pin_0
#define LCD_BKL_PIN    GPIO_Pin_7
#define LCD_DC_PIN     GPIO_Pin_8
#define LCD_CS_PIN     GPIO_Pin_6
#define LCD_CLK_PIN    GPIO_Pin_5
#define LCD_SDO_PIN    GPIO_Pin_6
#define LCD_SDI_PIN    GPIO_Pin_7

//#define __LCD_RST_SET()     GPIO_WriteBit(LCD_RST_GPIO, LCD_RST_PIN, Bit_SET)
//#define __LCD_RST_CLR()     GPIO_WriteBit(LCD_RST_GPIO, LCD_RST_PIN, Bit_RESET)

#define __LCD_BKL_SET()    GPIO_WriteBit(LCD_BKL_GPIO, LCD_BKL_PIN, Bit_SET)
#define __LCD_BKL_CLR()    GPIO_WriteBit(LCD_BKL_GPIO, LCD_BKL_PIN, Bit_RESET)

#define __LCD_CS_SET()     GPIO_WriteBit(LCD_CS_GPIO, LCD_CS_PIN, Bit_SET)
#define __LCD_CS_CLR()     GPIO_WriteBit(LCD_CS_GPIO, LCD_CS_PIN, Bit_RESET)

#define __LCD_DC_SET()     GPIO_WriteBit(LCD_DC_GPIO, LCD_DC_PIN, Bit_SET)
#define __LCD_DC_CLR()     GPIO_WriteBit(LCD_DC_GPIO, LCD_DC_PIN, Bit_RESET)

#define __LCD_WRITE_BYTE(__DATA)       spi_read_write_byte(SPI1, __DATA)

/*------------------------------------------------------------------------------------------------------*/
#include "XPT2046.h"
#include "Touch.h"

#define XPT2046_CS_PIN          GPIO_Pin_5
#define XPT2046_IRQ_PIN         GPIO_Pin_3

#define XPT2046_CS_GPIO         GPIOB
#define XPT2046_IRQ_GPIO        GPIOB

#define __XPT2046_CS_SET()      GPIO_WriteBit(XPT2046_CS_GPIO, XPT2046_CS_PIN, Bit_SET)
#define __XPT2046_CS_CLR()      GPIO_WriteBit(XPT2046_CS_GPIO, XPT2046_CS_PIN, Bit_RESET)

#define __XPT2046_IRQ_READ()    GPIO_ReadInputDataBit(XPT2046_IRQ_GPIO, XPT2046_IRQ_PIN)

#define __XPT2046_WRITE_BYTE(__DATA)       spi_read_write_byte(SPI1, __DATA)


#define SD_CS_PIN          GPIO_Pin_4
#define SD_CS_GPIO         GPIOB
#define __SD_CS_SET()      GPIO_WriteBit(SD_CS_GPIO, SD_CS_PIN, Bit_SET)
/*------------------------------------------------------------------------------------------------------*/
#include "PF_Config.h"

/*------------------------------------------------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


#endif

/*-------------------------------END OF FILE-------------------------------*/

