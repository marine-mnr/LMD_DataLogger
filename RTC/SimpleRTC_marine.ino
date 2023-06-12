/*
  Simple RTC for Arduino Zero and MKR1000

  Demonstrates the use of the RTC library for the Arduino Zero and MKR1000

  This example code is in the public domain

  http://arduino.cc/en/Tutorial/SimpleRTC

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>
  15 Jun 2015
  modified 
  18 Feb 2016
  modified by Andrea Richetta <a.richetta@arduino.cc>
  24 Aug 2016
*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

// /* Change these values to set the current initial time */
// const byte seconds = 20;
// const byte minutes = 23;
// const byte hours = 11;

// /* Change these values to set the current initial date */
// const byte day = 12;
// const byte month = 06;
// const byte year = 2023;

void set_RTC(const byte day, const byte month, const byte year, const byte hours, const byte minutes, const byte seconds){
  rtc.setDate(day, month, year);
  rtc.setTime(hours, minutes, seconds);
}


void setup()
{
  Serial.begin(9600); // init serial monitor

  rtc.begin(); // initialize RTC


}

bool done_once=1;
void loop()
{
  if (done_once==0) {
    // Set the time and date here
    set_RTC(12,06,23,12,03,00);
    done_once=1;
  }


  Serial.println(get_timestamp());
  delay(1000);
}


char timestamp[18]="01/01/01 12:00:00";

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
