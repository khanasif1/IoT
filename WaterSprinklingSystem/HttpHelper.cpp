#include "Arduino.h"
#include "HttpHelper.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include "WifiHelper.h"
HttpHelperClass::HttpHelperClass(){
}
   
String HttpHelperClass::GetHttp(char* host,String url){
      Serial.println("in HTTP Helper");
        // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      
      if (!client.connect(host, httpPort)) {
        //DisplayMessage("connection to server failed");
        Serial.println("connection to server failed");
         WifiHelper.Connect();  
        //return "";
      }
      Serial.println("Is Connected");
      //String url = "/GetIOTTicker?id=1";
      String data="2";
      String jsonMessage="";
       client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     //"Connection: close\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Content-Length: " + data.length() + "\r\n" +
                     "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                     data +"\n");
         Serial.println("Get Request executed");
          unsigned long timeout = millis();
          while (client.available() == 0) {
            if (millis() - timeout > 40000) {
              Serial.println(">>> Client Timeout !");          
              client.stop();        
            }
          }
           // Skip HTTP headers
          char endOfHeaders[] = "\r\n\r\n";
          if (!client.find(endOfHeaders)) {
            Serial.println(F("Invalid response"));
            return "";
          }
          Serial.println("Reading HTTP Result");   
          // Read all the lines of the reply from server and print them to Serial
          while(client.available()){
            jsonMessage =jsonMessage+ client.readStringUntil('\r');
          }   
          Serial.println("Response : "+jsonMessage);
          return jsonMessage;
  }
  String HttpHelperClass::PostSecureHttp(char* host,String url,String id){
      Serial.println("in HTTP Helper");
      Serial.println("Host"+ String(host)+" *** url"+url);
      
        // Use WiFiClient class to create TCP connections
      WiFiClientSecure client;
      const int httpPort = 443;
      
      if (!client.connect(host, httpPort)) {
        //DisplayMessage("connection to server failed");
        Serial.println("connection to server failed");
         WifiHelper.Connect(); 
        //return "";
      }else Serial.println("connected to server...");
      
      Serial.println("Is Connected");      
      
      String data="{\"Location\": \""+id+"\", \"Temperature\":\"2year\" }";
      String jsonMessage="";      
       client.print(String("POST ")+url+" HTTP/1.1\r\n"+
       "Host: IOTV2EventHubs-ns.servicebus.windows.net\r\n"+
       "Authorization: SharedAccessSignature sr=https%3a%2f%2fiotv2eventhubs-ns.servicebus.windows.net%2fiotv2messageevents%2fmessages&sig=ZKCxOLzTo131zbEGpuE8fmFZeAyufVWcs%2fatJ%2fo3oME%3d&se=1587800082&skn=RootManageSharedAccessKey\r\n"+
       "Content-Type: application/atom+xml;type=entry;charset=utf-8\r\n" +       
       "Content-Length: " + data.length() +"\r\n"+
       "\r\n"+
       data+"\n");
         Serial.println("Get Request executed");
          unsigned long timeout = millis();
          while (client.available() == 0) {
              Serial.println(">>> Client missing!");          
            if (millis() - timeout > 40000) {
              Serial.println(">>> Client Timeout !");          
              client.stop();        
            }
          }
          delay(5000);
          Serial.println("Reading HTTP Result");   
          // Read all the lines of the reply from server and print them to Serial
          while(client.available()){          
            jsonMessage =jsonMessage+ client.readString();                        
          }   
          Serial.println("Response : "+jsonMessage);          
          return jsonMessage;   
  }
  
HttpHelperClass HttpHelper=HttpHelperClass();

