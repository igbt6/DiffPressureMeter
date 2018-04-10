#ifndef SYSTEM_H
#define SYSTEM_H

#include "mbed.h"
#include <stdint.h>


#define DEVICE_VER "0.0.1"

#define HIGH 1
#define LOW 0

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


#endif