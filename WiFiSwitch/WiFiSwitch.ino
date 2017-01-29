#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include "WifiHelper.h"
#include "MessageBuilder.h"


const char* host = "iothome.azurewebsites.net";
int DeviceId=0;
void setup() {
   // put your setup code here, to run once:
    Serial.begin(115200);
     delay(10);         
     WifiHelper.Connect();      
    //if you get here you have connected to the WiFi
    Serial.println("***********************Connected to WiFi*********************** ");
   

}

void loop() {
  //Setting ESP Blue LED High to Blink
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");    
    //Setting ESP Blue LED Low if Web Request Fail
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }
   if(DeviceId==0)
  {
     // We now create a URI for the request
    String url = "/api/IOTListener/RegisterSensor";
    Serial.println("Requesting URL: ");
    Serial.println(url);
    registerDevice(client, url);
  }
  else
  {
    Serial.println("Start Switch Data");
    String url = "/api/IOTListener/GetSwitchStatus/"+String(DeviceId);
    Serial.println(url);
    getSwitchData(client, url);  
    Serial.println("Stop Switch Data");
   }
   
  client.stop();
  Serial.println();
  Serial.println("closing connection");
  digitalWrite(LED_BUILTIN, LOW);
}
void registerDevice(WiFiClient client, String url)
{
    String data= MessageBuilder.BuildRegistrationMessage();
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
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
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        DeviceId=line.toInt();
        Serial.print(DeviceId);
      }
    }
    void getSwitchData(WiFiClient client, String url)
    { 
      Serial.println("In Switch");      
      String data="2";  
     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 data +"\n");
      Serial.println("####"+data);
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
       Serial.println("Reading Switch Result");   
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        
        Serial.println("********************");
        Serial.println(line);
        Serial.println("********************");        
      }
      //return line;
    }
