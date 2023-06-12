
#include <RTCZero.h>
const bool debug=1;

/* Create an rtc object */
RTCZero rtc;

/*Creat a timestamp array*/
char timestamp[18]="01/01/01 12:00:00";

void setup()
{
  Serial.begin(9600); // init serial monitor
  rtc.begin(); // initialize RTC
}

void loop()
{
  if (debug){
    Serial.println(get_timestamp());
    delay(1000);
  }
}



char* get_timestamp(){
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
