#ifndef EXTERNALRTC_H
#define EXTERNALRTC_H

#include "system.h"
#include "ds3231.h"

namespace rtctime
{

struct Time
{
    uint16_t year;
    uint8_t month;   
    uint8_t date; 
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;  
};

class ExternalRTC 
{
public:
    static ExternalRTC& instance();
    bool setTime(const Time& time);
    bool getTime(Time& time);
    float getTemperature();
    static char *getTimeString(const Time& time);
    static char *getDateString(const Time& time);
    static const char *getDayOfWeekString(const Time& time);
    static const char *getMonthString(const Time& time);
private:
    Ds3231 rtc;
    ExternalRTC ();
    ExternalRTC & operator=(const ExternalRTC& rtc);
};

}
#endif