/**
  ******************************************************************************
  * @file    LIB_Config.c 
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief     This files provide
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
#include <stdio.h>
#include <stdlib.h>

//Platform Configuration


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint16_t g_hwDevId = 0;

/* Private function prototypes -----------------------------------------------*/
void system_init(void);
static void device_init(void);
static void driver_init(void);
static void port_init(void);
static void spi1_init(void);


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_MUL                        = 4
  *            PLL_DIV                        = 2
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable HSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}


/**
  * @brief  System initialization.
  * @param  None
  * @retval  None
  */
void system_init(void)
{
    HAL_Init();
    SystemClock_Config();
    device_init();
    driver_init();
}

static void device_init(void)	
{
	port_init();
	spi1_init();
}


static void driver_init(void)
{
	lcd_init();
    tp_init();
}

static void port_init(void) 
{
	GPIO_InitTypeDef tGPIO;
	
	__GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
	/*----------------------------------------------------------------------------------*/
	//SPI   
    tGPIO.Pin = LCD_CLK_PIN;
    tGPIO.Mode = GPIO_MODE_AF_PP;
    tGPIO.Pull = GPIO_PULLUP;
    tGPIO.Speed = GPIO_SPEED_FAST;
    tGPIO.Alternate = GPIO_AF0_SPI1;
	HAL_GPIO_Init(GPIOA, &tGPIO);
    
    /* Configure SPI MOSI */
    tGPIO.Pin = LCD_SDI_PIN | LCD_SDO_PIN;
    tGPIO.Alternate = GPIO_AF0_SPI1;
    tGPIO.Pull  = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &tGPIO);
    
	/*----------------------------------------------------------------------------------*/
	//LCD
	tGPIO.Pin = LCD_CS_PIN;   			  
    tGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    tGPIO.Pull = GPIO_PULLUP;
    tGPIO.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(LCD_CS_GPIO, &tGPIO);
    
    tGPIO.Pin = LCD_DC_PIN;   			 
	tGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    tGPIO.Pull = GPIO_PULLUP;
    tGPIO.Speed = GPIO_SPEED_FAST; 
	HAL_GPIO_Init(LCD_DC_GPIO, &tGPIO);

	tGPIO.Pin = LCD_BKL_PIN;  			 
	tGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    tGPIO.Pull = GPIO_PULLUP;
    tGPIO.Speed = GPIO_SPEED_FAST; 
	HAL_GPIO_Init(LCD_BKL_GPIO, &tGPIO);
    
    /*----------------------------------------------------------------------------------*/
    //XPT2046
    tGPIO.Pin = XPT2046_CS_PIN | SD_CS_PIN;   			 
    tGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    tGPIO.Pull = GPIO_PULLUP;
    tGPIO.Speed = GPIO_SPEED_FAST; 
	HAL_GPIO_Init(GPIOB, &tGPIO);
    __SD_CS_SET();
    
    tGPIO.Pin = XPT2046_IRQ_PIN;
 	tGPIO.Mode = GPIO_MODE_INPUT;
    tGPIO.Pull = GPIO_PULLUP;
    tGPIO.Speed = GPIO_SPEED_FAST; 
	HAL_GPIO_Init(XPT2046_IRQ_GPIO, &tGPIO);
    
	/*----------------------------------------------------------------------------------*/
}

/**
  * @brief  SPIx Bus initialization
  * @param  None
  * @retval None
  */
static void spi1_init(void)
{
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    SpiHandle.Instance = SPI1;
    
    /* On STM32L0538-DISCO, EPD ID cannot be read then keep a common configuration */
    /* for EPD (SPI_DIRECTION_2LINES) */
    /* Note: To read a register a EPD, SPI_DIRECTION_1LINE should be set */
    SpiHandle.Init.Mode               = SPI_MODE_MASTER;
    SpiHandle.Init.Direction          = SPI_DIRECTION_2LINES;
    SpiHandle.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_4;
    SpiHandle.Init.DataSize           = SPI_DATASIZE_8BIT;
    SpiHandle.Init.CLKPhase           = SPI_PHASE_2EDGE;
    SpiHandle.Init.CLKPolarity        = SPI_POLARITY_HIGH;
    SpiHandle.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS                = SPI_NSS_SOFT;
    SpiHandle.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial      = 7;
    SpiHandle.Init.TIMode             = SPI_TIMODE_DISABLED;
    
    __SPI1_CLK_ENABLE();
    HAL_SPI_Init(&SpiHandle);
  }
}


/*-------------------------------END OF FILE-------------------------------*/

