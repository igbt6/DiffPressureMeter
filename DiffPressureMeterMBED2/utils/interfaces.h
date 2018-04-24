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


// I2C_1
class I2C__1 : public HwInterface<I2C>
{
public:
    I2C__1();
    I2C& handle();
private:
    static const PinName sda = DS3231_I2C_SDA_PIN;
    static const PinName scl = DS3231_I2C_SCL_PIN;
	I2C i2c;
};

///// Hardware Interface class //////
template <class I>
struct HwInterfaceType { typedef I type; };

class HwInterfaces
{

public:
    static HwInterfaces& instance();    
    template<typename I> typename HwInterfaceType<I>::type& getHwInterface();
    // template<> typename HwInterfaceType<I2C__1>::type& getHwInterface(); //TODO
private:
    HwInterfaces();
    // defined hw interfaces 
    SPI__1 spi1;
    I2C__1 i2c1;

};

//-----------------------------------------------------------------------------
template<typename I> typename HwInterfaceType<I>::type& HwInterfaces::getHwInterface()
{
  if (is_same<I, SPI__1>::value)
  {
    return spi1;
  }
  //else if (is_same<I, I2C__1>::value)
  {
    //return i2c1;
  }
}

//-----------------------------------------------------------------------------


#endif
