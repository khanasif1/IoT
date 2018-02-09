#include "Arduino.h"
#include "GTFSMessageHelper.h"
#include <ArduinoJson.h>

GTFSMessageHelperClass::GTFSMessageHelperClass(){
}
   
String GTFSMessageHelperClass::GetMessage(String rawMessage){  
          String _gtfsDisplayMessage="";
          DynamicJsonBuffer jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(rawMessage);
          JsonArray& requests = root["root"];
          _gtfsDisplayMessage="::Schedule::";
          String _gtfsRaw="";
          for (auto& request : requests) {
             String Stop_headsign = request["Stop_headsign"];
             String Arrival_time = request["Arrival_time"];
             String Departure_time = request["Departure_time"];
             String ArriveDelay = request["ArriveDelay"];
             String DepartDelay = request["DepartDelay"];           
             
            Serial.println(">>"+String(Stop_headsign)+">>"+String(Arrival_time)+">>"+String(Departure_time)+">>"+String(ArriveDelay)+">>"+String(DepartDelay));  
           _gtfsRaw =_gtfsRaw + String(Stop_headsign)+"Arrival Time:"
                                +String(Arrival_time)+"Departure Time :"+String(Departure_time)
                                +"Arrive Delay:"+String(ArriveDelay)+"Depart Delay:"+String(DepartDelay);
          }
          if(_gtfsRaw == ""){
            Serial.println("Could not grab server data");
            _gtfsDisplayMessage="Train schedule will be available soon...";
          } else{
            _gtfsDisplayMessage=_gtfsDisplayMessage+_gtfsRaw;
          }
          return _gtfsDisplayMessage;
  }
GTFSMessageHelperClass GTFSMessageHelper=GTFSMessageHelperClass();

