#include "Arduino.h"
#include "CryptoMessageHelper.h"
#include <ArduinoJson.h>
CryptoMessageHelperClass::CryptoMessageHelperClass(){
}
   
String CryptoMessageHelperClass::GetMessage(String rawMessage){
        DynamicJsonBuffer jsonBuffer(2000);
        jsonBuffer.clear();
        JsonObject& root = jsonBuffer.parseObject(rawMessage);
        JsonArray& requests = root["root"];
        String _cryptoDisplayMessage;
        for (auto& request : requests) {
        String _cryptomessageHeader=request["Coin"];
        _cryptoDisplayMessage=_cryptoDisplayMessage+_cryptomessageHeader;
        String _cryptomessageValue = request["BuyPrice"];
        _cryptoDisplayMessage=_cryptoDisplayMessage+":"+_cryptomessageValue;
        String _cryptomessageStat = request["CoinStats"];
          if(_cryptomessageStat.indexOf("-")>=0){
            Serial.println("***down**** "+_cryptomessageStat+" "+_cryptomessageStat.indexOf("-"));
            _cryptoDisplayMessage=_cryptoDisplayMessage+_cryptomessageStat+"\x19";
          }else{
            Serial.println("***up****  "+_cryptomessageStat+" "+_cryptomessageStat.indexOf("-"));
            _cryptoDisplayMessage=_cryptoDisplayMessage+_cryptomessageStat+"\x18";
          }
        }
        Serial.println(_cryptoDisplayMessage);
        return _cryptoDisplayMessage;
  }
 
CryptoMessageHelperClass CryptoMessageHelper=CryptoMessageHelperClass();

