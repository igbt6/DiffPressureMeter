#include "externalrtc.h"

using namespace rtctime;
//-----------------------------------------------------------------------------
ExternalRTC & ExternalRTC::instance()
{
    static ExternalRTC rtc;
    return rtc;
}

//-----------------------------------------------------------------------------
ExternalRTC::ExternalRTC()
    : rtc(DS3231_I2C_SDA_PIN, DS3231_I2C_SCL_PIN)
{

}

//-----------------------------------------------------------------------------
bool ExternalRTC::setTime(const rtctime::Time& time)
{

    ds3231_time_t _time;
    _time.seconds = time.seconds;
    _time.minutes = time.minutes;
    _time.hours   = time.hours;
    _time.mode    = 0;
    _time.am_pm   = 0; // 24H

    ds3231_calendar_t _cal;
    _cal.day   = time.day;
    _cal.date  = time.date; 
    _cal.month = time.month; 
    _cal.year  = time.year;

    if (rtc.set_time(_time) != 0)
    {
        return 1; // error
    }
        if (rtc.set_calendar(_cal) != 0)
    {
        return 2; // error
    }
    return 0;
}

//-----------------------------------------------------------------------------
bool ExternalRTC::getTime(rtctime::Time& time)
{
    ds3231_time_t _time;
    ds3231_calendar_t _cal;

    if (rtc.get_time(&_time) != 0)
    {
        return 1; // error
    }
        if (rtc.get_calendar(&_cal) != 0)
    {
        return 2; // error
    }
    time.seconds   = _time.seconds;
    time.minutes   = _time.minutes;
    time.hours     = _time.hours;
    time.day       = _cal.day;
    time.date      = _cal.date; 
    time.month     = _cal.month; 
    time.year      = _cal.year;
    return 0;
}

//-----------------------------------------------------------------------------
float ExternalRTC::getTemperature()
{
    uint16_t temp = rtc.get_temperature();
    int sign = ((temp & 0x8000) == 0) ? 1 : -1;
    uint8_t msb = ((temp & 0x7F00) >> 8) & 0xFF;
    uint8_t lsb = ((temp & 0x00C0) >> 6) & 0xF;
    float result = (float)msb + ((lsb * 0.25) * sign);
    return result;
}

//-----------------------------------------------------------------------------
char* ExternalRTC::getTimeString(const rtctime::Time& time)
{
	static char str[] = "xxxxxxxx";
	if (time.hours < 10)
    {
		str[0] = '0';
    }
    else
    {
		str[0] = char((time.hours / 10) + '0');
    }
    str[1] = char((time.hours % 10) + '0');
	str[2] = 58;
	if (time.minutes < 10)
    {
		str[3] = '0';
    }
    else
    {
		str[3]=char((time.minutes/ 10) + '0');
    }
	str[4]=char((time.minutes % 10) + '0');
	str[5]=58;
    if (time.seconds<10)
    {
        str[6] = '0';
    }
    else
    {
        str[6]=char((time.seconds / 10) + '0');
    }
	str[7]=char((time.seconds % 10) + '0');
	str[8]='\0';

	return (char*)&str;
}

//-----------------------------------------------------------------------------
char *ExternalRTC::getDateString(const rtctime::Time& time)
{
	static char str[] = "xxxxxxxxxx";
    const char divider = '-';
    const uint8_t offset = 2;

    uint16_t yr = time.year;
    str[0]=char((yr / 1000) + '0');
    str[1]=char(((yr % 1000) / 100) + '0');
    str[2]=char(((yr % 100) / 10) + '0');
    str[3]=char((yr % 10) + '0');
    str[4]=divider;
    if (time.month < 10)
    {
        str[3+offset] = '0';
    }
    else
    {
        str[3+offset] = char((time.month/ 10) + '0');
    }
    str[4+offset] = char((time.month % 10) + '0');
    str[5+offset] = divider;
    if (time.date < 10)
    {
        str[6+offset] = '0';
    }
    else
    {
        str[6+offset] = char((time.date / 10) + '0');
    }
    str[7+offset]=char((time.date % 10) + '0');
    str[8+offset]='\0';
	return (char*)&str;
}

//-----------------------------------------------------------------------------
char *ExternalRTC::getTimeDateString(const Time& time)
{
    static char strTimeDate[30] = {' '};
    const int timeStrLen = 8;
    const int dateStrLen = 10;
    memcpy(strTimeDate, getTimeString(time), timeStrLen);
    memcpy(&strTimeDate[timeStrLen+5], getDateString(time), dateStrLen);
    // remove nulls
    for (int i = 0; i < 30; ++i)
    {
        if ( i < 24 && strTimeDate[i] == '\0')
        {
            strTimeDate[i] = ' ';
        }   
    }
    return (char*)&strTimeDate;
}

//-----------------------------------------------------------------------------
const char *ExternalRTC::getDayOfWeekString(const rtctime::Time& time)
{
	static const char *days[]  = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	return days[time.day - 1];
}

//-----------------------------------------------------------------------------
const char *ExternalRTC::getMonthString(const rtctime::Time& time)
{
	static const char *months[]  = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	return months[time.month - 1];
}

//-----------------------------------------------------------------------------