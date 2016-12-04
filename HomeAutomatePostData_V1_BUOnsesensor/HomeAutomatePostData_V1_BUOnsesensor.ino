/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "iKhan";
const char* password = "Centos0!";
int photoSensorPin = 0;
const char* host = "iothomeautomate.azurewebsites.net";
int DeviceId=0;
int FailCount=0;
void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  SetWifi();

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(LED_BUILTIN, OUTPUT); 
}
void SetWifi()
{
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop() {
   digitalWrite(LED_BUILTIN, HIGH);  
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
   Serial.println("Fail Count :"+String(FailCount));
  if(FailCount > 4){
    Serial.println("Reset WiFi");
    WiFi.disconnect();
    delay(500);
     SetWifi();
     FailCount=0;
     digitalWrite(LED_BUILTIN, LOW);
     return;
  }
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    FailCount=FailCount+1;
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
  String data="{";
        data= data+"'Name':'ClimateManager_HomeMerrylands',";
        data= data+"'Description':'Test',";
        data= data+"'Vendor' : 'ESP8266',";
        data= data+"'IsActive':true,";
        data= data+"'CreateDate':'2016-10-07T14:21:51.3883469+11:00',";
        data= data+"'UpdateDate':'1799-01-01T00:00:00'";
        data= data+"}";
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 data + "\n");
  
      // This will send the request to the server
      /*client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n");*/
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
      Serial.println(analogRead(photoSensorPin));
      int photoSensorData=analogRead(photoSensorPin);
      String line="NULL";   
      String data="{";
        data= data+"'SensorId':"+DeviceId+",";
        data= data+"'Value':'"+photoSensorData+"',";
        data= data+"}";
      Serial.println("####"+data);
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
  
  

