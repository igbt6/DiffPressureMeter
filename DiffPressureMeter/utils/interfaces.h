#ifndef INTERFACES_H
#define INTERFACES_H
#include "system.h"

/* Interfaces classes declarations */


template<typename I>
class HwInterface
{
public:
    virtual I& handle() = 0;
};

// SPI_1
class SPI__1 : public HwInterface<SPI>
{
public:
    SPI__1();
    SPI& handle();
private:
    static const PinName sclk = PA_5;
    static const PinName miso = PA_6;
    static const PinName mosi = PA_7;
	SPI spi;
};

///// Hardware Interface class //////
template <class I>
struct HwInterfaceType { typedef I type; };

class HwInterfaces
{

public:
    static HwInterfaces& instance();    
    template<typename I> typename HwInterfaceType<I>::type& getHwInterface();
    // specializations for HW interfaces
    SPI &getHandleSPI1()
    {
        return spi1;
    }
private:
    HwInterfaces();
    // defined hw interfaces 
    SPI__1 spi;

    //-------
    SPI spi1;
};


//-----------------------------------------------------------------------------
template<typename I> typename HwInterfaceType<I>::type& HwInterfaces::getHwInterface()
{
  if (is_same<I,SPI__1>::value)
  {
    return spi;
  }
}




#endif
