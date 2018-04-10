#ifndef HWCONFIG_H
#define HWCONFIG_H

#include "mbed.h"

// HX8347D - TFT_LCD 
#define LCD_SPI_MOSI_PIN        PB_5 //PA_7 
#define LCD_SPI_MISO_PIN        PB_4 //PA_6
#define LCD_SPI_SCLK_PIN        PB_3 //PA_5 
#define LCD_DC_PIN              PB_1 //PA_8
#define LCD_BKL_PIN             PB_2 //PC_7
#define LCD_CS_PIN              PB_15//PC_4//PB_6


// DS3231 - RTC 
#define DS3231_I2C_SDA_PIN      PB_9 //I2C1_SDA
#define DS3231_I2C_SCL_PIN      PB_8 //I2C1_SCL
#define DS3231_SQW_PIN          PC_8     

// ADS1115 - 16BIT ADC 
#define ADS1115_I2C_SDA_PIN     PC_9 //I2C3_SDA
#define ADS1115_I2C_SCL_PIN     PA_8 //I2C3_SCL
#define ADS1115_ADDR_PIN        GND  //ADS1115 addr = 0x48 
#define ADS1115_ALERT_RDY_PIN   PC_8

// CSN_A2L - THERMAL_PRINTER
#define CSN_A2L_RX_PIN          PA_12 //UART6_RX
#define CSN_A2L_TX_PIN          PA_11 //UART6_TX

// ESP8266 - WIFI
#define ESP8266_RX_PIN          PB_3 //UART1_RX
#define ESP8266_TX_PIN          PB_6 //UART1_TX

#endif