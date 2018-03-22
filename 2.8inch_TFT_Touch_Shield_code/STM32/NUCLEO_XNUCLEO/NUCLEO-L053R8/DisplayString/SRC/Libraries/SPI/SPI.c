/**
  ******************************************************************************
  * @file    SPI.c 
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   This file provides a set of functions to manage the communication between SPI peripherals.
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
#include "SPI.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef SpiHandle;


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Read or write an byte from or to SPI bus.
  * @param  tSPIx: where x can be 1 , 2 or 3 to select the SPI pripheral.
  * @retval chByte: Receive an byte from SPI bus
  */
uint8_t spi_read_write_byte(SPI_TypeDef* tSPIx, uint8_t chByte)
{	
    uint8_t chData;
    
    (void)tSPIx;

    HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &chByte, (uint8_t*) &chData, 1, 0x1000);
    
    return chData;
}

/*-------------------------------END OF FILE-------------------------------*/

