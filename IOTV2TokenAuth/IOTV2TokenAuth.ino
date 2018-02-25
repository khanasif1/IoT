#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include "WifiHelper.h"

//const char* host = "iotv2readgtfsnsw.azurewebsites.net";
const char* host = "iotv2gateway.azurewebsites.net";


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    delay(100);          
    Serial.println("***********************Connected Started*********************** ");
    WifiHelper.Connect();          
    delay(500);
    Serial.println("***********************Connected to WiFi*********************** ");
    pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {

  Serial.println("in loop");
  delay(1000);
    // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  
   if (!client.connect(host, httpPort)) {      
      Serial.println("connection to server failed");
       WifiHelper.Connect();  
      return;
    }

    //String url = "/TrainsUpdate";
    String url = "/restifyPost";
    String data="{\"_CorrelationId\":\"1\",\"_Url\":\"http://iotv2deviceread.azurewebsites.net/AuthenticateDevice\",\"_InboundMessage\":null,\"_InboundPost\":\"{\"Password\":\"test\",\"Token\":\"\",\"DeviceRegistrationId\":\"9A15AC1D-E8FB-4BD7-928C-3CBD987CB21F\"}\"}";
    String jsonMessage="";
    
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    //"Connection: close\r\n" +
                    "AuthToken:{\"Password\":\"test\",\"Token\":\"\",\"DeviceRegistrationId\":\"9A15AC1D-E8FB-4BD7-928C-3CBD987CB21F\"} \r\n" +
                    "Content-Type: application/json\r\n" +
                    "Content-Length: " + data.length() + "\r\n" +
                    "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                    data +"\n");
                    //Serial.println("####"+data);
                    unsigned long timeout = millis();
                    while (client.available() == 0) {
                    if (millis() - timeout > 5000) {
                    Serial.println(">>> Client Timeout !");   
                    client.stop();        
                    }
                   }
      delay(1000);                   
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
    jsonMessage = client.readStringUntil('\r');             
    }  
    Serial.println("Response:"+String(jsonMessage));
    delay(500);
}
