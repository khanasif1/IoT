#include "Arduino.h"
#include "HttpHelper.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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
          Serial.println("Reading HTTP Result");   
          // Read all the lines of the reply from server and print them to Serial
          while(client.available()){
            jsonMessage = client.readStringUntil('\r');             
          }   
          Serial.println("Response : "+jsonMessage);
          return jsonMessage;
  }
  
HttpHelperClass HttpHelper=HttpHelperClass();

