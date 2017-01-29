#include "Arduino.h"
#include "MessageBuilder.h"



MessageBuilderClass::MessageBuilderClass(){
}
   
String MessageBuilderClass::BuildRegistrationMessage(){
   String data="{";
        data= data+"'Name':'WiFiSwitch',";
        data= data+"'Description':'Test',";
        data= data+"'Vendor' : 'ESP8266',";
        data= data+"'IsActive':true,";
        data= data+"'CreateDate':'2017-01-26T14:21:51.3883469+11:00',";
        data= data+"'UpdateDate':'1799-01-01T00:00:00'";
        data= data+"}";
        return data;  
  }
String MessageBuilderClass::BuildPostMessage(int DeviceId, double humiditySensorData,double temperatureC){
        String data="[";
        data= data+"{'DeviceId':"+DeviceId+",'SensorId':1,";
        data= data+"'Value':'"+humiditySensorData+"'},";
        data= data+"{'DeviceId':"+DeviceId+",'SensorId':2,";
        data= data+"'Value':'"+temperatureC+"'},";
        data= data+"]";
        return data;
  }
 String MessageBuilderClass:: DelayHumidityMessage(double humiditySensorData){
        String delayMessage="";
        if(humiditySensorData !=0){
           delayMessage=delayMessage +" Humidity :"+String(humiditySensorData);
          }else{
           delayMessage=delayMessage +" Humidity : Not Collected";
          }
          return delayMessage;
  }
  String MessageBuilderClass:: DelayTemperatureMessage(double temperatureC){
        
        String delayMessage="";
        if(temperatureC !=0){
           delayMessage= delayMessage+" Temp : "+String(temperatureC);
          }else{
           delayMessage=delayMessage +" Temp : Not Collected";        
          }
        return delayMessage;
 }
 
MessageBuilderClass MessageBuilder=MessageBuilderClass();

