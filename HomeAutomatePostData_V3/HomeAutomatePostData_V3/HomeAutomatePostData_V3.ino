/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 * 
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <dht.h>
#include "oled.h"
#include "WifiHelper.h"
#include "MessageBuilder.h"

int photoSensorPin = 0;
double humiditySensorData=0;
double temperatureC=0;
const char* host = "iothomeautomate.azurewebsites.net";
int DeviceId=0;
int PostDataCount=0;
int delayBetweenSend=5000;
int SecondMultiplier=1;// 30;
#define dht_apin 16 // Analog Pin sensor is connected to

dht DHT;

void setup() {

    // put your setup code here, to run once:
    Serial.begin(115200);
     delay(10);
      pinMode(16,OUTPUT); 
      pinMode(5,OUTPUT); 
      pinMode(4,OUTPUT);   
       
     WifiHelper.Connect();    

    oled.DisplayMessage("Connected to Wifi", 1, 0, 3);
    delay(2000);
    
    //if you get here you have connected to the WiFi
    Serial.println("***********************Connected to WiFi*********************** ");
    oled.DisplayMessage("Measuring Temperature and Humidity", 1, 0, 3);
    delay(4000);
}


void loop() {
  //Setting ESP Blue LED High to Blink
  digitalWrite(LED_BUILTIN, HIGH);   
 
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
   Serial.println("Should send after defined time. Current Count");
   Serial.print(PostDataCount);
  if(PostDataCount < SecondMultiplier){   
     digitalWrite(LED_BUILTIN, HIGH);
    PostDataCount=PostDataCount+1;
    delay(delayBetweenSend); // Second     
    digitalWrite(LED_BUILTIN, LOW);    
    
    oled.DisplayMessage(MessageBuilder.DelayHumidityMessage(humiditySensorData), 1, 0, 3);
    delay(1000);  
    
    oled.DisplayMessage(MessageBuilder.DelayTemperatureMessage(temperatureC), 1, 0, 3);
    delay(1000);
    String delayMessage="";
    delayMessage= delayMessage+" Next Collection: "+String(PostDataCount - SecondMultiplier)+"s";
    oled.DisplayMessage(delayMessage, 1, 0, 3);
    return;
   }
   else{
     PostDataCount=0;     
    }
  if (!client.connect(host, httpPort)) {
    oled.DisplayMessage("Connection to Cloud Failed", 3, 20, 3);
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
    String url = "/api/IOTListener/PostSensorData";
    Serial.print("Requesting URL: ");
    Serial.println(url);       
    String response = sendSensorData(client,  url);
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

String sendSensorData(WiFiClient client, String url)
    {   
      CollectSensorData();//Collection before send
      String line="NULL";  
      String data=MessageBuilder.BuildPostMessage(DeviceId, humiditySensorData,temperatureC); 
      /*String data="[";
        data= data+"{'DeviceId':"+DeviceId+",'SensorId':1,";
        data= data+"'Value':'"+humiditySensorData+"'},";
        data= data+"{'DeviceId':"+DeviceId+",'SensorId':2,";
        data= data+"'Value':'"+temperatureC+"'},";
        data= data+"]";*/        
        
        oled.DisplayMessage("Humidity :"+String(humiditySensorData)+"Temp : "+String(temperatureC), 1, 0, 3);        
        delay(4000);
      Serial.println("####"+data);
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
          return "Error in connection";
        }
      }
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        line = client.readStringUntil('\r');       
        Serial.print(line);        
      }
      return line;
    }

    void CollectSensorData(){
       DHT.read11(dht_apin);
      Serial.print("Current humidity = ");
      Serial.print(DHT.humidity);
      Serial.print("%  ");
      Serial.print("temperature = ");
      Serial.print(DHT.temperature); 
      Serial.println("C  ");
      humiditySensorData=DHT.humidity;
      temperatureC=DHT.temperature;
    }
 
