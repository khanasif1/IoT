#include "Arduino.h"
#include "WifiHelper.h"
#include <WiFiManager.h> 

int _DeviceId=0;
WifiHelperClass::WifiHelperClass(){
}
   //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
void WifiHelperClass::Connect(){
     //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
     wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
  }

void WifiHelperClass::reset(){
   if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
   }
   
  }
 int WifiHelperClass::registerDevice(){
  
  }
 String WifiHelperClass::sendSensorData(){
  
  }  
  
WifiHelperClass WifiHelper=WifiHelperClass();

