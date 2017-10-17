#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include "WifiHelper.h"
#include "MessageBuilder.h"
#include <ArduinoJson.h>

//const char* host = "iothome.azurewebsites.net";
const char* host = "iotv2readgtfsnsw.azurewebsites.net";
int i=0;
 DynamicJsonBuffer jsonBuffer;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("***********************Connected Started*********************** ");
  WifiHelper.Connect();      
  //if you get here you have connected to the WiFi
   Serial.println("***********************Connected to WiFi*********************** ");
    pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  // put your main code here, to run repeatedly:
  i++;
  //Serial.println("Hi From NodeMCU"+String(i));
  delay(10000);

  //Serial.print("connecting to ");
  //Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
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
        for (auto& request : requests) {
           String type = request["Stop_headsign"];
           const char* value = request["Arrival_time"];
          Serial.println(">>"+String(value));    
        }
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
