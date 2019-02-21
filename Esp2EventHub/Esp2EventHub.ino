#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ArduinoJson.h>


char* host = "IOTV2EventHubs-ns.servicebus.windows.net";
//char* host = "iotv2readcryptocurrency.azurewebsites.net";
WiFiManager wifiManager;
void setup() {
  Serial.println("***********************Setup Begins*********************** ");
  Serial.begin(9600);
  delay(100);  
  Serial.println("***********************Connected Started*********************** ");
   wifiManager.autoConnect("AutoConnectAP");     
  delay(500); 
  Serial.println("***********************Connected to WiFi*********************** ");
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
      DynamicJsonBuffer jsonBuffer(2000);
        String urlDisplayType ="/iotv2messageevents/publishers/sender/messages";
        String jsonMessageDisplay =GetHttp(host,urlDisplayType);        
        jsonBuffer.clear();       
        //JsonObject& rootDisplay = jsonBuffer.parseObject(jsonMessageDisplay);                           
        //long displayType = rootDisplay["DisplayType"];                           
        //Serial.println("Display Type "+String(displayType));
        //jsonBuffer.clear();
        delay(5000);
}

String GetHttp(char* host,String url){
      Serial.println("in HTTP Helper");
        // Use WiFiClient class to create TCP connections
      WiFiClientSecure client;
      const int httpPort = 443;
      
      if (!client.connect(host, httpPort)) {
        //DisplayMessage("connection to server failed");
        Serial.println("connection to server failed");
         wifiManager.autoConnect("AutoConnectAP");  
        //return "";
      }else Serial.println("connected to server...");
      
      Serial.println("Is Connected");      
      String data="{\"Location\": \"Arduino\", \"Temperature\":\"2year\" }";
      String jsonMessage="";      
       client.print(String("POST ")+"/iotv2messageevents/messages HTTP/1.1\r\n"+
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
            jsonMessage = client.readString();                        
          }   
          Serial.println("Response : "+jsonMessage);          
          return jsonMessage;          
  }
