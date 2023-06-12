#ifndef RTC_marine_h
#define RTC_marine_h

#include <RTCZero.h>

class RTC_marine
{
    public:
        void set_RTC(const byte day, const byte month, const byte year, const byte hours, const byte minutes, const byte seconds);
        char* get_timestamp();
                
    private:
        RTCZero rtc;
        char timestamp[18];

}
#endif
