
#include <RTC_marine.h>
#include <RTCZero.h>

/*attention il faut avoir fait "rtc.begin()" et "RTCZero rtc;" au préalable*/

void RTC_marine::RTC_marine()
{
    rtc.begin();
}

/*Permet d'affecter la date et l'heure à une carte disposant d'une RTC */
void RTC_Marine::setDateTime(const byte day, const byte month, const byte year, const byte hours, const byte minutes, const byte seconds){
  rtc.setDate(day, month, year);
  rtc.setTime(hours, minutes, seconds);
}

/*Renvoie un timestamp, tableau de caractères contenant la date et l'heure actuelle*/
char* RTC_Marine::get_timestamp(){
  // Get the current date and time from the RTC
  int day = rtc.getDay();
  int month = rtc.getMonth();
  int year = rtc.getYear();
  int hours = rtc.getHours();
  int minutes = rtc.getMinutes();
  int seconds = rtc.getSeconds();

  // Format the date and time into the timestamp string
  sprintf(timestamp, "%02d/%02d/%02d %02d:%02d:%02d", day, month, year, hours, minutes, seconds);

  return(timestamp);
}
