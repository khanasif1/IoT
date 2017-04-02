#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
uint32_t old_ts;

void setup() {
    Serial.begin(57600);
    Wire.begin();
    rtc.begin();
}

void loop() {
  PrintTime();
  PrintTemp();
  delay(1000);
}
void PrintTime(){
  
    DateTime now = rtc.now(); //get the current date-time
    uint32_t ts = now.getEpoch();
    Serial.print(String(now.year())+'/'+String(now.month())+'/'+String(now.date())+' '+String(now.hour())+':'+String(now.minute())+':'+String(now.second())+' '+weekDay[now.dayOfWeek()]);
    Serial.println();
   
  }

  void PrintTemp(){
    
    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature()); //read registers and display the temperature
    Serial.println("deg C");
    
  }
  void ResetDS3231(DateTime dt){
    rtc.setDateTime(dt);
    
  }
