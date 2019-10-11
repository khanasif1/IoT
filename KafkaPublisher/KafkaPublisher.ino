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
#include <ArduinoJson.h>
#include "WifiHelper.h"
#include "HttpHelper.h"


char* host = "40.126.229.114";
const int httpPort = 80;
String url = "/KafkaPost";

int photocellPin = 0; 
int photocellReading; 
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
       Serial.println("In Loop");                       
       
       photocellReading = analogRead(photocellPin);
       Serial.println("Light : "+photocellReading);
        
       long randNumber = random(0, 500); 
       String data="{";
        data= data+"'_id':'0',";
        data= data+"'_message':'"+randNumber+"',";
        data= data+"'_timestamp':'2016-10-07T14:21:51.3883469+11:00',";
        data= data+"}";        
      
      Serial.println("Data : "+data);
      WiFiClient client;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      Serial.println("API Connected. Start POST");  
      /*client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 data + "\n");                 
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
     while(client.available()){
        String line = client.readStringUntil('\r');        
        Serial.print("Post Response : /c"+line);
      }*/
}
