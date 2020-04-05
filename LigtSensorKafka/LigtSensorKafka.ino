#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <WiFiClientSecure.h>
#include <Streaming.h> 
#include <TimeLib.h>
#include <ArduinoJson.h>
#include "WifiHelper.h"


char* host = "40.126.229.114";

void setup() {
  Serial.begin(9600);
  delay(500); 
  Serial.println("***********************Setup Begins*********************** ");
 
 
  Serial.println("***********************Connected Started*********************** ");
  WifiHelper.Connect();      
  delay(500); 
  Serial.println("***********************Connected to WiFi*********************** ");
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
      Serial.println("In Loop");                            
                    WiFiClient client;
                    const int httpPort = 80;
                    Serial.println("Getting time");
                    DynamicJsonBuffer jsonBufferTime(2000);
                    String url ="/KafkaPost";
                    delay(4000);
                    String data="[";
                          data= data+"{'_id':0,";
                          data= data+"'_message':222,";
                          data= data+"'_timestamp':'2019-10-01T08:40:13.066Z'";                          
                          data= data+"]";
                    Serial.println("####"+data);


                    if (!client.connect(host, httpPort)) {                      
                      Serial.println("connection failed");    
                      //Setting ESP Blue LED Low if Web Request Fail
                      digitalWrite(LED_BUILTIN, LOW);
                      return;
                    }
                    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                               "Host: " + host + "\r\n" +                 
                               "Content-Type: application/json\r\n" +
                               "Content-Length: " + data.length() + "\r\n" +
                               "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                               data + "\n"); 
                   unsigned long timeout = millis();                    
                   while (client.available() == 0) {
                    if (millis() - timeout > 5000) {
                      Serial.println(">>> Client Timeout !");
                      client.stop();                      
                    }
                  }
                  
                  // Read all the lines of the reply from server and print them to Serial
                  while(client.available()){
                    String line = client.readStringUntil('\r');       
                    Serial.print(line);        
                  }
}
