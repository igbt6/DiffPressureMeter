#ifndef __XPT2046_H
#define __XPT2046_H

#include "system.h"


#define BIT(__N)  (uint8_t)(1 << (__N))

#define TP_IRQ_PIN         PB_3
#define TP_CS_PIN          PB_5
#define SD_CS_PIN          PB_4

#define __XPT2046_CS_OUT()      tp_cs_pin(TP_CS_PIN)
#define __XPT2046_CS_SET()      tp_cs_pin.write(HIGH)
#define __XPT2046_CS_CLR()      tp_cs_pin.write(LOW)
#define __XPT2046_CS_DISABLE()  __XPT2046_CS_SET()

#define __XPT2046_IRQ_IN()      tp_irq_pin(TP_IRQ_PIN, PullUp)
#define __XPT2046_IRQ_READ()    tp_irq_pin.read()

#define __SD_CS_DISABLE()       do { \
                                    DigitalOut sd_cs_pin(SD_CS_PIN); \
                                    sd_cs_pin.write(HIGH); \
                                } while (0)

extern SPI spii;                             
#define __XPT2046_WRITE_BYTE(__DATA)  spii.write(__DATA)










class XPT
{

public:

    XPT();
    
    inline uint8_t xpt2046_write_byte(uint8_t chData)
    {
        return __XPT2046_WRITE_BYTE(chData);
    }
    
    inline uint8_t xpt2046_read_input_pin_status()
    {
        return __XPT2046_IRQ_READ();
    }

    uint16_t xpt2046_read_ad_value(uint8_t chCmd)
    {
        uint16_t hwData = 0;
    
        __XPT2046_CS_CLR();
        xpt2046_write_byte(chCmd);
        hwData = xpt2046_write_byte(0x00);
        hwData <<= 8;
        hwData |= xpt2046_write_byte(0x00);
        hwData >>= 4;
       __XPT2046_CS_SET();
        
        return hwData;
    }
    
#define READ_TIMES  5
#define LOST_NUM    1
    uint16_t xpt2046_read_average(uint8_t chCmd)
    {
        uint8_t i, j;
        uint16_t hwbuffer[READ_TIMES], hwSum = 0, hwTemp;

        for (i = 0; i < READ_TIMES; i ++) {
            hwbuffer[i] = xpt2046_read_ad_value(chCmd);
        }
        for (i = 0; i < READ_TIMES - 1; i ++) {
            for (j = i + 1; j < READ_TIMES; j ++) {
                if (hwbuffer[i] > hwbuffer[j]) {
                    hwTemp = hwbuffer[i];
                    hwbuffer[i] = hwbuffer[j];
                    hwbuffer[j] = hwTemp;
                }
            }
        }
        for (i = LOST_NUM; i < READ_TIMES - LOST_NUM; i ++) {
            hwSum += hwbuffer[i];
        }
        hwTemp = hwSum / (READ_TIMES - 2 * LOST_NUM);

        return hwTemp;
    }
    
    void xpt2046_init(void);
    void xpt2046_read_xy(uint16_t *phwXpos, uint16_t *phwYpos);
    bool xpt2046_twice_read_xy(uint16_t *phwXpos, uint16_t *phwYpos);
    
private:
    DigitalOut tp_cs_pin;
    DigitalIn tp_irq_pin;
};

extern uint8_t g_chXcmd, g_chYcmd;
extern XPT xpt;

#endif




