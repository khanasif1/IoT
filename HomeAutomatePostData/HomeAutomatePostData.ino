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

const char* host = "iothomeautomate.azurewebsites.net";
String DeviceId="";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  delay(5000);


  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
 
  if(DeviceId=="")
  {
     // We now create a URI for the request
    String url = "/api/IOTListener/RegisterSensor";
    Serial.print("Requesting URL: ");
    Serial.println(url);
    registerDevice(client, url);
  }
  else
  {
     String url = "/api/IOTListener/PostSensorData";
     String response= SendData(client, url);
    }
  client.stop();
  Serial.println();
  Serial.println("closing connection");
}

bool registerDevice(WiFiClient client, String url)
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
          return false;
        }
      }
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        DeviceId=line;
        Serial.print(line);
      }
      if(DeviceId!="0")
      {return true;}
      else{ return false;}
  }
  String SendData(WiFiClient client, String url)
  {
    String line;
     String data="{";
        data= data+"'SensorId':'"+DeviceId+"',";
        data= data+"'Value':'Test',";
        data= data+"}";
   
    Serial.print("Requesting URL: ");
    Serial.println(url);
    
    Serial.print("Device Id"+ DeviceId );
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n");
                         unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return "Error in connecting";
        }
      }
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        line = client.readStringUntil('\r');       
        Serial.print(line);
        
      }
      return line;
    
  }

