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
          for (auto& request : requests) {
             String Stop_headsign = request["Stop_headsign"];
             String Arrival_time = request["Arrival_time"];
             String Departure_time = request["Departure_time"];
             String ArriveDelay = request["ArriveDelay"];
             String DepartDelay = request["DepartDelay"];           
             
            Serial.println(">>"+String(Stop_headsign)+">>"+String(Arrival_time)+">>"+String(Departure_time)+">>"+String(ArriveDelay)+">>"+String(DepartDelay));  
           _gtfsDisplayMessage =_gtfsDisplayMessage + String(Stop_headsign)+"Arrival Time:"
                                +String(Arrival_time)+"Departure Time :"+String(Departure_time)
                                +"Arrive Delay:"+String(ArriveDelay)+"Depart Delay:"+String(DepartDelay);
           
          } 
          return _gtfsDisplayMessage;
  }
GTFSMessageHelperClass GTFSMessageHelper=GTFSMessageHelperClass();

