#include <RTC_marine.h>
//#include <RTCZero.h>

RTC_marine my_rtc;

void setup() {
  Serial.begin(9600); // init serial monitor
  //rtc.begin(); // initialize RTC
}

void loop() {
  Serial.println(my_rtc.get_timestamp());
  delay(1000);
}
