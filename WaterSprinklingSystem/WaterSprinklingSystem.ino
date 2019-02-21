#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <WiFiClientSecure.h>
#include <Streaming.h> 
#include <ArduinoJson.h>
#include "WifiHelper.h"
#include "HttpHelper.h"

long apiScheduler = millis();
long serverhour=0;
long servermin=0;
char* host = "iotv2datetimeread.azurewebsites.net";

void setup() {
  Serial.println("***********************Setup Begins*********************** ");
  Serial.begin(9600);
  delay(100);  
  Serial.println("***********************Connected Started*********************** ");
  WifiHelper.Connect();
  delay(500); 
  Serial.println("***********************Connected to WiFi*********************** ");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
 if(millis()- apiScheduler>300000){
        Serial.println("Getting display type");
        DynamicJsonBuffer jsonBuffer(2000);
        String urlDisplayType ="/GetTime?locationId=India%20Standard%20Time";
        String jsonMessageDisplay =HttpHelper.GetHttp(host,urlDisplayType);
        jsonBuffer.clear();       
        JsonObject& rootDisplay = jsonBuffer.parseObject(jsonMessageDisplay);                           
        serverhour = rootDisplay["hour"];
        servermin = rootDisplay["minutes"];                           
        Serial.println("hr: "+String(serverhour)+ " min: "+String(servermin));
        if(serverhour== 18 && servermin== 00){
          
          }
        jsonBuffer.clear();        
        apiScheduler=millis();
      }
}
