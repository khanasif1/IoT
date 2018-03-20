
/*
** Hardware setup (ESP8266 <-> FC16):
** 3V3 <-> VCC, GND <-> GND, D7 <-> DIN, D8 <-> CS, D5 <-> CLK
* Clock
** SDA --> D2
** SCL -->D1 
*/
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
//#include <DS3232RTC.h> 
#include <DS1307RTC.h>
#include <Streaming.h> 
//#include <Time.h>             //http://playground.arduino.cc/Code/Time
//#include <Wire.h>             //http://arduino.cc/en/Reference/Wire
#include <TimeLib.h>
#include <ArduinoJson.h>
#include "WifiHelper.h"
#include "HttpHelper.h"
#include "CryptoMessageHelper.h"
#include "GTFSMessageHelper.h"
#include "ClockHelper.h"

int pinCS = 15; 
int numberOfHorizontalDisplays = 4; 
int numberOfVerticalDisplays = 1; 
String decodedMsg;
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
long displayTypeTimer = 0;
long cryptoTimer = 0;
long gtfsTimer = 0;
long displayType=0;
String displayText="";
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int refresh=0;
int wait = 20; 
int spacer = 2;
int width = 5 + spacer; 

char* host = "iotv2readcryptocurrency.azurewebsites.net";
char* gtfshost = "iotv2readgtfsnsw.azurewebsites.net";

void setup() {
  Serial.println("***********************Setup Begins*********************** ");
  Serial.begin(9600);
  delay(100);  
  setSyncProvider(RTC.get);//CLOCK
  // put your setup code here, to run once:
  matrix.setIntensity(13);
  matrix.setRotation(0, 1);        // 1 
  matrix.setRotation(1, 1);        // 2 
  matrix.setRotation(2, 1);        // 3 
  matrix.setRotation(3, 1);        // 4 
  ScrollText("Setup \x0B ");
  Serial.println("***********************Connected Started*********************** ");
  WifiHelper.Connect();      
  ScrollText("WiFi \x0B ");
  delay(500); 
  Serial.println("***********************Connected to WiFi*********************** ");
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
      Serial.println("In Loop");           
      //Serial.println("Display type timer interval : "+String(millis()- displayTypeTimer));
      if(displayTypeTimer == 0){
        
        Serial.println("First run pulling display type");
        DynamicJsonBuffer jsonBuffer(2000);
        String urlDisplayType ="/GetDevice?email=khanasif1%40gmail.com";
        String jsonMessageDisplay =HttpHelper.GetHttp(host,urlDisplayType);
        jsonBuffer.clear();       
        JsonObject& rootDisplay = jsonBuffer.parseObject(jsonMessageDisplay);                           
        displayType = rootDisplay["DisplayType"];                           
        Serial.println("Display Type "+String(displayType));
        jsonBuffer.clear();
        displayTypeTimer=millis();
      }
      else if(millis()- displayTypeTimer>30000){
        Serial.println("Getting display type");
        DynamicJsonBuffer jsonBuffer(2000);
        String urlDisplayType ="/GetDevice?email=khanasif1%40gmail.com";
        String jsonMessageDisplay =HttpHelper.GetHttp(host,urlDisplayType);
        jsonBuffer.clear();       
        JsonObject& rootDisplay = jsonBuffer.parseObject(jsonMessageDisplay);                           
        displayType = rootDisplay["DisplayType"];                           
        Serial.println("Display Type "+String(displayType));
        jsonBuffer.clear();        
        displayTypeTimer=millis();
      }
     
       if(displayType== 1 ){
                Serial.println("In Clock");

                float celsius;float fahrenheit;
                static time_t tLast;
                time_t t = now();
               //If reset values exist on searila
                if(hour(t)==0 && minute(t)==0){
                   
                    Serial.println("Getting time");
                    DynamicJsonBuffer jsonBufferTime(2000);
                    String urlTime ="/GetTime";
                    String jsonMessageTime =HttpHelper.GetHttp(host,urlTime);                         
                    JsonObject& rootTime = jsonBufferTime.parseObject(jsonMessageTime);                           
                    String years = rootTime["Year"]; 
                    String months = rootTime["Month"]; 
                    String days = rootTime["Day"]; 
                    String hours = rootTime["hour"]; 
                    String minutes = rootTime["minutes"];                           
                    String seconds = rootTime["seconds"];                           
                    Serial.println("Time "+hours+":"+minutes);               
                    jsonBufferTime.clear();     
                    ResetTime(years,months,days,hours,minutes,seconds);

                }else{
                    Serial.println("Time Available "+String(hour(t))+":"+String(minute(t)));                                    
                } 
                
                if(updCnt<=0) { 
                  updCnt = 10;
                  Serial.println("Getting data ...");        
                  int temp = 0;////RTC.temperature();
                  celsius = temp / 4.0;
                  fahrenheit = celsius * 9.0 / 5.0 + 32.0;
                  Serial.println("Data loaded");
                  clkTime = millis();
                  Serial.println(String(hour(t)));
                  Serial.println(String(minute(t)));
                  Serial.println(String(second(t)));
                  Serial.println("Temp : C = "+String(celsius)+" F = "+String(fahrenheit));
                }
                DisplayTime(hour(t),minute(t),second(t));               
                //ClockHelper.DisplayTime(hour(t),minute(t),second(t),pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
                                
                if(millis()-clkTime > 15000  && dots) {
                  /*int temp = 0;////RTC.temperature();
                  celsius = temp / 4.0;
                  fahrenheit = celsius * 9.0 / 5.0 + 32.0;
                  Serial.println("Data loaded");
                  clkTime = millis();
                  ScrollText("Temp C:"+String(celsius)+" F:"+String(fahrenheit));*/
                  updCnt--;
                  clkTime = millis();
                  }                     
               if(millis()-dotTime > 500) {
                  dotTime = millis();
                  dots = !dots;
                }               
              Serial.println(String(hour(t))+" : "+String(minute(t))+" : "+String(second(t))); 
       }
       else if(displayType== 2 ){
              Serial.println("Crypto timer interval : "+String(millis()- cryptoTimer));
              if(cryptoTimer == 0){
                Serial.println("First run pulling crypto");
                String jsonMessage=HttpHelper.GetHttp(host,"/GetIOTTicker?id=1"); 
                  displayText=CryptoMessageHelper.GetMessage(jsonMessage);
                  cryptoTimer=millis();
               }
              else if(millis()- cryptoTimer>120000){
                  String jsonMessage=HttpHelper.GetHttp(host,"/GetIOTTicker?id=1"); 
                  displayText=CryptoMessageHelper.GetMessage(jsonMessage);
                  cryptoTimer=millis();
                }                                 
                ScrollText(displayText);
        }else if (displayType== 3){
              Serial.println("GTFS timer interval : "+String(millis()- gtfsTimer));
              if(gtfsTimer == 0){
                Serial.println("First run pulling gtfs");
                String jsonMessage=HttpHelper.GetHttp(gtfshost,"/TrainsUpdate");
                displayText=GTFSMessageHelper.GetMessage(jsonMessage);
                gtfsTimer=millis();
              }
              else if(millis()- gtfsTimer>120000){
                String jsonMessage=HttpHelper.GetHttp(gtfshost,"/TrainsUpdate");
                displayText=GTFSMessageHelper.GetMessage(jsonMessage);
                gtfsTimer=millis();
              }               
                ScrollText(displayText);
        }else{
                  ScrollText("\x02 Display Type not selected \x02");
        }      
}

void DisplayTime(int h,int m,int s){
  
    matrix.fillScreen(LOW);
    int y = (matrix.height() - 8) / 2;
    
    if(s & 1){matrix.drawChar(14, y, (String(":"))[0], HIGH, LOW, 1);}
    else{matrix.drawChar(14, y, (String(" "))[0], HIGH, LOW, 1);}
        String hour1;String hour2 ;
          String min1; String min2;
        
        if(h>9){
           if(h>9 && h<20){
             hour1 = String (h);
             hour2 = String (h - 10);
           }
           else{
             hour1 = String (h);
             hour2 = String (h - 20);           
           }
          }else{          
           hour1 = String (0);
           hour2 = String (h);
          }
          
          if(m>9){
            if(m>9 &&m<20){
             min1 = String (m);
             min2 = String (m - 10);            
            }else if(m>19 &&m<30){
             min1 = String (m);
             min2 = String (m - 20);            
            }else if(m>29 &&m<40){
             min1 = String (m);
             min2 = String (m - 30);            
            }else if(m>39 &&m<50){
             min1 = String (m);
             min2 = String (m - 40);            
            }else if(m>49 &&m<60){
             min1 = String (m);
             min2 = String (m - 50);            
            }else{
             min1 = String (m);
             min2 = String (m);                          
            }
          }else{
           min1 = String (0);
           min2 = String (m);
          }
          String sec1 = String (3);
          String sec2 = String (3);
          int xh = 2;
          int xm = 19;
      //    int xs = 28;
      
          matrix.drawChar(xh, y, hour1[0], HIGH, LOW, 1);
          matrix.drawChar(xh+6, y, hour2[0], HIGH, LOW, 1);
          matrix.drawChar(xm, y, min1[0], HIGH, LOW, 1);
          matrix.drawChar(xm+6, y, min2[0], HIGH, LOW, 1);
          //matrix.drawChar(xs, y, sec1[0], HIGH, LOW, 1);
          //matrix.drawChar(xs+6, y, sec2[0], HIGH, LOW, 1);  
    matrix.write();
}
// =======================================================================
void DisplayText(String text){
    matrix.fillScreen(LOW);
    for (int i=0; i<text.length(); i++){
    
    int letter =(matrix.width())- i * (width-1);
    int x = (matrix.width() +1) -letter;
    int y = (matrix.height() - 8) / 2;
    matrix.drawChar(x, y, text[i], HIGH, LOW, 1);
    matrix.write();    
    }
}
// =======================================================================
void ScrollText (String text){
    for ( int i = 0 ; i < width * text.length() + matrix.width() - 1 - spacer; i++ ) {
    if (refresh==1) i=0;
    refresh=0;
    matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2;
 
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < text.length() ) {
        matrix.drawChar(x, y, text[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write();
    delay(wait);
  }
}
void ResetTime(String years, String months, String days, String hours, String minutes, String seconds){
     tmElements_t tm;
     time_t t;
      //check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
     //if (Serial.available() >= 12) {
          Serial.println("Datetime Reset values available");
          //note that the tmElements_t Year member is an offset from 1970,
          //but the RTC wants the last two digits of the calendar year.
          //use the convenience macros from Time.h to do the conversions.
          int y = years.toInt();
          if (y >= 100 && y < 1000)
              Serial.println("Error: Year must be two digits or four digits!");
          else {
              if (y >= 1000)
                  tm.Year = CalendarYrToTm(y);
              else    //(y < 100)
                  tm.Year = y2kYearToTm(y);
              tm.Month = months.toInt();
              tm.Day = days.toInt();
              tm.Hour = hours.toInt();
              tm.Minute = minutes.toInt();
              tm.Second = seconds.toInt();
              t = makeTime(tm);
              RTC.set(t);        //use the time_t value to ensure correct weekday is set
              setTime(t);        
              //Serial << F("RTC set to: ");
              Serial.println(String(day(t))+"-"+String(month(t))+"-"+String(year(t))+" "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));
              //Serial << endl;
              //dump any extraneous input
              while (Serial.available() > 0) Serial.read();
          }
      //}
}
