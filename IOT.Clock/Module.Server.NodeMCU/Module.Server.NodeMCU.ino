#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include "WifiHelper.h"
#include "MessageBuilder.h"
#include <ArduinoJson.h>

const char* host = "iothome.azurewebsites.net";
int i=0;
 DynamicJsonBuffer jsonBuffer;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WifiHelper.Connect();      
  //if you get here you have connected to the WiFi
   Serial.println("***********************Connected to WiFi*********************** ");
   PostLogs("WIFI COnnected");
}



void loop() {
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
    Serial.println("connection to clock server failed");
    //WifiHelper.Connect();
    return;
  }

  PostLogs("Start server requets to get time");
  String url = "/api/ArduinoDisplay/GetCurrentTime";
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
          PostLogs("Client Timeout");
          client.stop();        
        }
      }
       //Serial.println("Reading Switch Result");   
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        jsonMessage = client.readStringUntil('\r');             
      }
        //Serial.print("********************");
        //Serial.println(state);
        //Serial.print("********************");       
        JsonObject& root = jsonBuffer.parseObject(jsonMessage);
        String timeVal = root[String("Time")];
        String DateTime = root[String("DateTime")];
        String Hours = root[String("Hours")];
        String Minutes = root[String("Minutes")];
        String Seconds = root[String("Seconds")];
        Serial.println(jsonMessage+"$");
        PostLogs("JSON Message :"+jsonMessage);
        //Serial.println("DateTime:"+DateTime);
        //Serial.print(Hours);
        //Serial.print(Minutes);
        //Serial.println(Seconds);
        //Serial.println("$");
}
void PostLogs(String logs){

   // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    
    if (!client.connect(host, httpPort)) {
      Serial.println("connection to clock server failed");
      //WifiHelper.Connect();
      return;
    }
     String url="/api/ArduinoDisplay/StreamingLogs";
     String line="";
     String data="{'Logs':'"+logs+"',";
        data= data+"'Device':'IOT Clock ESP8266',";
        data= data+"'DeviceId':'1',";
        data= data+"'DeviceName':'IOT Clock'}";
      

     client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +                 
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 data + "\n");  
  }
