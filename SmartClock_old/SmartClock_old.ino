/******************************************************************************
** TEXT SCROLL SAMPLE FOR ALTAIRIS ESP8266-FC16 LIBRARY
**
** This sample will scroll text on the display. Available characters are
** defined in FC16_Font.h. In addition to the ASCII table, the following glyphs
** are defined (inspired by CP437, but not fully compatible):
** \x01 :-)
** \x02 :-(
** \x03 heart
** \x04 diamond
** \x05 club
** \x06 spade
** \x07 bullet
** \x08 empty bullet
** \x09 big bullet
** \x0A big bullet inverse
** \x0B checkbox empty
** \x0C checkbox cross
** \x0D checkbox check
** \x0E checkbox full
** \x0F sunny
** \x10 triangle right
** \x11 triangle left
** \x18 arrow up
** \x19 arrow down
** \x1A arrow right
** \x1B arrow left
** \x7F full-width space (8 pixels)
**
** Hardware setup (ESP8266 <-> FC16):
** 3V3 <-> VCC, GND <-> GND, D7 <-> DIN, D8 <-> CS, D5 <-> CLK
* Clock
** SDA --> D2
** SCL -->D1 
******************************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include "WifiHelper.h"
#include "MessageBuilder.h"
#include <ArduinoJson.h>
#include <DS3232RTC.h>        //http://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        //http://arduiniana.org/libraries/streaming/
#include <Time.h>             //http://playground.arduino.cc/Code/Time
#include <Wire.h>             //http://arduino.cc/en/Reference/Wire
#include <TimeLib.h>
#include <Max72xxPanel.h>
#include <Adafruit_GFX.h>

//const char* host = "iothome.azurewebsites.net";
const char* host = "iotv2readgtfsnsw.azurewebsites.net";


int i=0;
int firstLoop=0;
DynamicJsonBuffer jsonBuffer;
int pinCS = 15;
const int csPin = D8;      // CS pin used to connect FC16
const int csPin2 = D0;      // CS pin used to connect FC16
const int displayCount = 8;   // Number of displays; usually 4 or 8
const int scrollDelay = 50;   // Scrolling speed - pause in ms
//FC16 display = FC16(csPin, displayCount);
//FC16 display2 = FC16(csPin2, displayCount);
int numberOfHorizontalDisplays = 4; 
int numberOfVerticalDisplays = 1; 
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
String _msg="";


void setup() {
      setSyncProvider(RTC.get);//CLOCK

      //******MAX7219 Config*******
      matrix.setIntensity(13);  
      matrix.setRotation(0, 1);        // 1 
      matrix.setRotation(1, 1);        // 2 
      matrix.setRotation(2, 1);        // 3 
      matrix.setRotation(3, 1);        // 4 
      
      // put your setup code here, to run once:
      Serial.begin(9600);
      delay(100);  
      //InitDisplay();
      DisplayMessage("Trying Server Connection");
      Serial.println("***********************Connected Started*********************** ");
      WifiHelper.Connect();      
      //if you get here you have connected to the WiFi
      DisplayMessage("Connected to WiFi");
      //display.update();
      delay(500);
      Serial.println("***********************Connected to WiFi*********************** ");
      pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() { 
        digitalWrite(LED_BUILTIN, HIGH);
        static time_t tLast;
        time_t t;
        tmElements_t tm;
       /*if (Serial.available() >= 12) {
              //note that the tmElements_t Year member is an offset from 1970,
              //but the RTC wants the last two digits of the calendar year.
              //use the convenience macros from Time.h to do the conversions.
              int y = Serial.parseInt();
              if (y >= 100 && y < 1000)
                  Serial << F("Error: Year must be two digits or four digits!") << endl;
              else {
                  if (y >= 1000)
                      tm.Year = CalendarYrToTm(y);
                  else    //(y < 100)
                      tm.Year = y2kYearToTm(y);
                  tm.Month = Serial.parseInt();
                  tm.Day = Serial.parseInt();
                  tm.Hour = Serial.parseInt();
                  tm.Minute = Serial.parseInt();
                  tm.Second = Serial.parseInt();
                  t = makeTime(tm);
                  RTC.set(t);        //use the time_t value to ensure correct weekday is set
                  setTime(t);        
                  Serial << F("RTC set to: ");
                  printDateTime(t);
                  Serial << endl;
                  //dump any extraneous input
                  while (Serial.available() > 0) Serial.read();
              }
          }*/


          t = now();          
          if (t != tLast) {
              tLast = t;
              //printDateTime(t);
              if (second(t) == 0) {
                  float c = RTC.temperature() / 4.;
                  float f = c * 9. / 5. + 32.;
                  Serial << F("  ") << c << F(" C  ") << f << F(" F");
              }             
          }
          Serial.println(String(day(t)));
          Serial.println(String(monthShortStr(month(t))));
          Serial.println(String(year(t)));
          Serial.println(String(hour(t)));
          Serial.println(String(minute(t)));
          Serial.println(String(second(t)));

        
          /*DisplayMessage(String(hour(t)));            
          display.update();
          DisplayMessage2(String(minute(t)));
          display2.update();
          delay(scrollDelay);*/
          digitalWrite(LED_BUILTIN, LOW);
          DisplayTime(String(hour(t)),String(minute(t)),String(second(t)));
       /* // put your main code here, to run repeatedly:
        i++;
        Serial.println("Loop"+String(i));
        if(firstLoop==0){
          Serial.println("Calling API");
                 String jsonMessage=HTTPRequestHelper();
                 DynamicJsonBuffer jsonBuffer;
                 JsonObject& root = jsonBuffer.parseObject(jsonMessage);
                 JsonArray& requests = root["root"];
                 DisplayMessage( "::Schedule::");
                 for (auto& request : requests) {
                     String Stop_headsign = request["Stop_headsign"];
                     String Arrival_time = request["Arrival_time"];
                     String Departure_time = request["Departure_time"];
                     String ArriveDelay = request["ArriveDelay"];
                     String DepartDelay = request["DepartDelay"];           
                     
                    Serial.println(">>"+String(Stop_headsign)+">>"+String(Arrival_time)+">>"+String(Departure_time)+">>"+String(ArriveDelay)+">>"+String(DepartDelay));  
                   _msg =_msg + String(Stop_headsign)+"   Arrival Time :  "+String(Arrival_time)+"   Departure Time :  "+String(Departure_time)+" Arrive Delay :  "+String(ArriveDelay)+" Depart Delay :  "+String(DepartDelay);
                   delay(100);
                  }
               
                //Serial.println("Message build"+_msg);
                DisplayMessage(_msg); 
                firstLoop++;              
        }else
        {
          if(i==3000){
                 Serial.println("Calling API");
                 String jsonMessage=HTTPRequestHelper();
                 DynamicJsonBuffer jsonBuffer;
                 JsonObject& root = jsonBuffer.parseObject(jsonMessage);
                 JsonArray& requests = root["root"];
                 DisplayMessage( "::Schedule::");
                 for (auto& request : requests) {
                     String Stop_headsign = request["Stop_headsign"];
                     String Arrival_time = request["Arrival_time"];
                     String Departure_time = request["Departure_time"];
                     String ArriveDelay = request["ArriveDelay"];
                     String DepartDelay = request["DepartDelay"];           
                     
                    Serial.println(">>"+String(Stop_headsign)+">>"+String(Arrival_time)+">>"+String(Departure_time)+">>"+String(ArriveDelay)+">>"+String(DepartDelay));  
                   _msg =_msg + String(Stop_headsign)+"   Arrival Time :  "+String(Arrival_time)+"   Departure Time :  "+String(Departure_time)+" Arrive Delay :  "+String(ArriveDelay)+" Depart Delay :  "+String(DepartDelay);
                   delay(5);
                  }
               
                //Serial.println("Message build"+_msg);
                DisplayMessage(_msg);
                i=0;
              }
                 
        }
        display.update();
        delay(scrollDelay);
        digitalWrite(LED_BUILTIN, LOW);*/
}

void DisplayTime(String h,String m,String s){          
          matrix.fillScreen(LOW);
          int y = (matrix.height() - 8) / 2;
      
          
          if(s & 1){matrix.drawChar(14, y, (String(":"))[0], HIGH, LOW, 1);}
          else{matrix.drawChar(14, y, (String(" "))[0], HIGH, LOW, 1);}
          
          String hour1 = String (1);
          String hour2 = String (1);
          String min1 = String (2);
          String min2 = String (2);
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
//print date and time to Serial
void printDateTime(time_t t)
{
    printDate(t);
    Serial << ' ';
    printTime(t);
}
//print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}
//print date to Serial
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial << monthShortStr(month(t)) << _DEC(year(t));
}

//Print an integer in "00" format (with leading zero),
//followed by a delimiter character to Serial.
//Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}
String HTTPRequestHelper(){
  Serial.println("in HTTP Helper");
    // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    DisplayMessage("connection to server failed");
    Serial.println("connection to server failed");
     WifiHelper.Connect();  
    return "";
  }
  String url = "/TrainsUpdate";
  String data="2";
  String jsonMessage="";
   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 data +"\n");
      //Serial.println("####"+data);
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 10000) {
          Serial.println(">>> Client Timeout !");          
          client.stop();        
        }
      }
       //Serial.println("Reading Switch Result");   
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        jsonMessage = client.readStringUntil('\r');             
      }   
      Serial.println("Response : "+jsonMessage);
      return jsonMessage;
}

/*void InitDisplay()
{
      display.shutdown(false);  // turn on display
      display.setIntensity(8);  // set medium brightness
      display.clearDisplay();   // turn all LED off
      display2.shutdown(false);  // turn on display
      display2.setIntensity(8);  // set medium brightness
      display2.clearDisplay();   // turn all LED off
      delay(100);

}
void DisposeDisplay()
{
      display.clearDisplay();   // turn all LED off  
      display.shutdown(true);  // turn on display
}*/
void DisplayMessage(String _message)
{            
      char Message[500];
      _message.toCharArray(Message,500);
      // Set text to display
     // display.setText(Message);
}
/*void DisplayMessage2(String _message)
{            
      char Message[500];
      _message.toCharArray(Message,500);
      // Set text to display
      display2.setText(Message);
}*/
