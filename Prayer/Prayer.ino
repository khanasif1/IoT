#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <ArduinoJson.h>
#include "WifiHelper.h"
#include "HttpHelper.h"

long apiCallCounter = 0;
char* host = "iotv2readaladhan.azurewebsites.net";


void setup() { 
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    delay(100);
    Serial.println("***********************Connected Started*********************** ");
    WifiHelper.Connect();        
    delay(500); 
    Serial.println("***********************Connected to WiFi*********************** ");
}

void loop() {
      
      if(apiCallCounter == 0){
        Serial.println("First run pulling salat time ");
        DynamicJsonBuffer jsonBuffer(2000);
        
        //String urlDisplayType ="/GetIsPrayerTime?datetime=null";//2018-02-19%2016%3A49
        String urlDisplayType ="/GetIsPrayerTest";
        String jsonMessage =HttpHelper.GetHttp(host,urlDisplayType);
        
        jsonBuffer.clear();       
        JsonObject& rootDisplay = jsonBuffer.parseObject(jsonMessage);                           
        String isItTime = rootDisplay["State"];
        String salatTime = rootDisplay["SalatTime"];                           
        Serial.println("Salat time "+String(salatTime));
        jsonBuffer.clear();
        apiCallCounter=millis();
        if(isItTime=="true"){
            Serial.println("Enable switch");
            digitalWrite(13, HIGH);
            Serial.println("Delay 3min 30 sec");// sets the digital pin 13 on
            delay(219000);                 // waits Adhan Audio to complete
            Serial.println("Disable switch");
            digitalWrite(13, LOW); 
       }else{
            Serial.println("Disable switch");
            digitalWrite(13, LOW);       // sets the digital pin 13 on        
        }
      }
      else if(millis()- apiCallCounter>20000){
        Serial.println("Getting Salat time ");
        DynamicJsonBuffer jsonBuffer(2000);
        
        //String urlDisplayType ="/GetIsPrayerTime?datetime=null";
        String urlDisplayType ="/GetIsPrayerTest";
        String jsonMessage =HttpHelper.GetHttp(host,urlDisplayType);
        
        jsonBuffer.clear();       
        JsonObject& rootDisplay = jsonBuffer.parseObject(jsonMessage);                           
        String isItTime = rootDisplay["State"];                   
        String salatTime = rootDisplay["SalatTime"];                           
        Serial.println("Salat time "+String(salatTime));
        jsonBuffer.clear();        
        apiCallCounter=millis();
        if(isItTime=="true"){
            Serial.println("Enable switch");
            digitalWrite(13, HIGH);       // sets the digital pin 13 on
            Serial.println("Delay 3min 30 sec");// sets the digital pin 13 on
            delay(219000);                 // waits Adhan Audio to complete
            Serial.println("Disable switch");
            digitalWrite(13, LOW); 
       }else{
            Serial.println("Disable switch");
            digitalWrite(13, LOW);       // sets the digital pin 13 on        
        }        
      }
}
