#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ArduinoJson.h>
#include "HttpHelper.h"
#include "HealthCheckHelper.h"


HealthCheckHelperClass::HealthCheckHelperClass(){
}
  String HealthCheckHelperClass::PingHealth(String id){
    Serial.println("Start health check ping");
    char* hostHub = "IOTV2EventHubs-ns.servicebus.windows.net";
    String url="/iotv2messageevents/messages";
    Serial.println("End health check ping");
    return HttpHelper.PostSecureHttp(hostHub,url,id);    
  }

HealthCheckHelperClass HealthCheckHelper=HealthCheckHelperClass();

