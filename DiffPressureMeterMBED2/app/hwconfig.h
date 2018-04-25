#ifndef HWCONFIG_H
#define HWCONFIG_H

#include "mbed.h"

// HX8347D - TFT_LCD 
#define LCD_SPI_MOSI_PIN        PA_7 
#define LCD_SPI_MISO_PIN        PA_6
#define LCD_SPI_SCLK_PIN        PA_5 
#define LCD_DC_PIN              PA_8
#define LCD_BKL_PIN             PC_7
#define LCD_CS_PIN              PB_6
#define TP_IRQ_PIN              PB_3
#define TP_CS_PIN               PB_5
#define SD_CS_PIN               PB_4 // NOT USED


// DS3231 - RTC 
#define DS3231_I2C_SDA_PIN      PB_9 //I2C1_SDA
#define DS3231_I2C_SCL_PIN      PB_8 //I2C1_SCL
#define DS3231_SQW_PIN          PC_8 // NOT USED   

// ADS1115 - 16BIT ADC 
#define ADS1115_I2C_SDA_PIN     PB_9 //I2C1_SDA TODO
#define ADS1115_I2C_SCL_PIN     PB_8 //I2C1_SCL
#define ADS1115_ADDR_PIN        GND  //ADS1115 addr = 0x48 
#define ADS1115_ALERT_RDY_PIN   PC_8 // NOT_USED

// CSN_A2L - THERMAL_PRINTER
#define CSN_A2L_RX_PIN          PA_12 //UART6_RX
#define CSN_A2L_TX_PIN          PA_11 //UART6_TX

// RELAY
#define RELAY_OUT_PIN           PC_0

// ESP8266 - WIFI
#define ESP8266_RX_PIN          PA_3 //UART2_RX
#define ESP8266_TX_PIN          PA_2 //UART2_TX

#endif