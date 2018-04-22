#ifndef INTERFACES_H
#define INTERFACES_H
#include "system.h"

/* Interfaces classes declarations */

template<typename T, typename U>
struct is_same 
{
    static const bool value = false; 
};

template<typename T>
struct is_same<T,T>  //specialization
{ 
   static const bool value = true; 
};


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
    static const PinName sclk = LCD_SPI_SCLK_PIN;
    static const PinName miso = LCD_SPI_MISO_PIN;
    static const PinName mosi = LCD_SPI_MOSI_PIN;
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
private:
    HwInterfaces();
    // defined hw interfaces 
    SPI__1 spi;

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
