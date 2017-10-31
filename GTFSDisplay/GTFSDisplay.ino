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
******************************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include "WifiHelper.h"
#include "MessageBuilder.h"
#include <ArduinoJson.h>
#include <FC16.h>

//const char* host = "iothome.azurewebsites.net";
const char* host = "iotv2readgtfsnsw.azurewebsites.net";
int i=0;
DynamicJsonBuffer jsonBuffer;

const int csPin = D8;      // CS pin used to connect FC16
const int displayCount = 4;   // Number of displays; usually 4 or 8
const int scrollDelay = 30;   // Scrolling speed - pause in ms
FC16 display = FC16(csPin, displayCount);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);  
  InitDisplay();
  DisplayMessage("Connection Started");
  Serial.println("***********************Connected Started*********************** ");
  WifiHelper.Connect();      
  //if you get here you have connected to the WiFi
  DisplayMessage("Connected to WiFi");
   Serial.println("***********************Connected to WiFi*********************** ");
    pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() { 
  
  digitalWrite(LED_BUILTIN, HIGH);
  // put your main code here, to run repeatedly:
  i++;
  
  delay(10000);

  //Serial.print("connecting to ");
  //Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    DisplayMessage("connection to server failed");
    Serial.println("connection to server failed");
     WifiHelper.Connect();  
    return;
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
        if (millis() - timeout > 5000) {
          //Serial.println(">>> Client Timeout !");
          
          client.stop();        
        }
      }
       //Serial.println("Reading Switch Result");   
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        jsonMessage = client.readStringUntil('\r');             
      }
        Serial.print("********************");
        //Serial.println(jsonMessage);
        
        

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(jsonMessage);
        JsonArray& requests = root["root"];
        String _msg="Next Train --> ";
        for (auto& request : requests) {
           String Stop_headsign = request["Stop_headsign"];
           String Arrival_time = request["Arrival_time"];
           String Departure_time = request["Departure_time"];
           String ArriveDelay = request["ArriveDelay"];
           String DepartDelay = request["DepartDelay"];           
           
          Serial.println(">>"+String(Stop_headsign)+">>"+String(Arrival_time)+">>"+String(Departure_time)+">>"+String(ArriveDelay)+">>"+String(DepartDelay));  
          _msg=_msg + String(Stop_headsign)+"   Arrival Time :  "+String(Arrival_time)+"   Departure Time :  "+String(Departure_time)+" Arrive Delay :  "+String(ArriveDelay)+" Depart Delay :  "+String(DepartDelay);
          DisplayMessage( _msg);
          //delay(1000);
        }
        //DisposeDisplay();
        Serial.println("$");    
        
       /*String timeVal = root[String("Stop_headsign")];
        String DateTime = root[String("Arrival_time")];
        String Hours = root[String("Departure_time")];
        String Minutes = root[String("ArriveDelay")];
        String Seconds = root[String("DepartDelay")];
        Serial.println("@@@@@@@@@@@@@@@@@@@@@@");
        Serial.println("DateTime:"+timeVal);
        Serial.print(DateTime);
        Serial.print(Hours);
        Serial.println(Minutes);
        Serial.println("@@@@@@@@@@@@@@@@@@@@@@");*/
        digitalWrite(LED_BUILTIN, LOW);

}
void InitDisplay()
{
      display.shutdown(false);  // turn on display
      display.setIntensity(8);  // set medium brightness
      display.clearDisplay();   // turn all LED off
      delay(100);

}
void DisposeDisplay()
{
      display.clearDisplay();   // turn all LED off  
      display.shutdown(true);  // turn on display
}
void DisplayMessage(String _message)
{            
      char Message[500];
      _message.toCharArray(Message,500);
      // Set text to display
      display.setText(Message);

      for(int i=0;i<1000;i++){
       // Perform scroll
       display.update();
       delay(20);
      }
}
