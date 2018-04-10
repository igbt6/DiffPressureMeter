#include "interfaces.h"



//-----------------------------------------------------------------------------
SPI__1::SPI__1()
    : spi(SPI__1::mosi, SPI__1::miso, SPI__1::sclk)
{
    spi.format(8,3);
    spi.frequency(10000000); // 1MHz clock
}

//-----------------------------------------------------------------------------
SPI& SPI__1::handle()
{
    return spi;
}



//-----------------------------------------------------------------------------
/* Interfaces classes definitions */
HwInterfaces::HwInterfaces()
    : spi()
    , spi1(PA_7, PA_6, PA_5)
{
    spi1.format(8,3);
    spi1.frequency(1000000); // 1MHz clock
}

//----------------------------------------------------------------------------
HwInterfaces& HwInterfaces::instance()
{
    static HwInterfaces interfaces;
    return interfaces;
}

//-----------------------------------------------------------------------------
