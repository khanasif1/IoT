/*DS3231   Arduino
  
  VCC     VCC
  GND     GND
  SCL     A5
  SDA     A4
*/

// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"
// *** SPFD5408 change -- End

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET 0 // Can alternately just connect to Arduino's reset pin
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };



// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD,LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

// -- Setup
String jsonMessage = "";         // a string to hold incoming data
boolean IsMessageComplete = false;  // whether the string is complete
int i=0;

String G_Hours;String G_Minutes;String G_Seconds;String G_AMPM;String G_Date; String G_Temp;

void setup(void) {
  
  Serial.begin(9600);
   Wire.begin();
   rtc.begin();
  DateTime dt(2017, 04, 02, 12, 59, 30, 0);
  rtc.setDateTime(dt);
  progmemPrintln(PSTR("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  progmemPrintln(PSTR("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  progmemPrintln(PSTR("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  tft.reset(); 
  // Code changed to works 
  
  tft.begin(0x9341); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial

  // *** SPFD5408 change -- End

  progmemPrintln(PSTR("Benchmark                Time (microseconds)"));

 /* progmemPrint(PSTR("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);
*/
  progmemPrint(PSTR("Text                     "));
  //Serial.println(testText(jsonMessage));
  delay(3000);
  progmemPrintln(PSTR("Done!"));

  SetStartupMessage();

}
void progmemPrint(const char *str) {
  char c;
  while(c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}
void loop(void) { 
     PrintTime();  
}

void PrintTime(){
   
    DateTime now = rtc.now(); 
    Serial.print(String(now.year())+'/'+String(now.month())+'/'+String(now.date())+' '+String(now.hour())+':'+String(now.minute())+':'+String(now.second())+' '+weekDay[now.dayOfWeek()]);
    Serial.println(); 
    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature()); //read registers and display the temperature
    Serial.println((char)223);
    String Temp=String(rtc.getTemperature());
    //SetErrorMessage("In DS3231");
    String Hours = "";
    String AMPM = "";
    if(now.hour()>12){
      Serial.println("In 12"+String(now.hour())); 
      int _hour = now.hour() - 12;
      Hours = FormatText(_hour);  
      AMPM = "pm";     
    }else{
      Serial.println("In 12"+String(now.hour())); 
      int _hour = now.hour();
      Hours = FormatText(_hour);             
      AMPM="am";
    } 
    String Minutes = FormatText(now.minute());
    String Seconds = FormatText(now.second());    
    String Day = FormatText(now.date());
    String Month = FormatText(now.month());
    String Year = FormatText(now.year());
    SetDateTime(Hours,Minutes,Seconds,Day,Month,Year,Temp,AMPM);
    delay(1000);
   
}
  
void serialEvent() {  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
        if (inChar != '$') {
          Serial.print(inChar);
          jsonMessage += inChar;   
          }else{
            IsMessageComplete=true;
          }
     }
   if(IsMessageComplete){
   Serial.println("Serial Message"+jsonMessage);
   
    const size_t BUFFER_SIZE =
      JSON_OBJECT_SIZE(9);    // the root object has 9 elements
      /*+ JSON_OBJECT_SIZE(5)  // the "address" object has 5 elements
      + JSON_OBJECT_SIZE(2)  // the "geo" object has 2 elements
      + JSON_OBJECT_SIZE(3)  // the "company" object has 3 elements
      + MAX_CONTENT_SIZE;    // additional space for strings*/
   DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
   
   JsonObject& root = jsonBuffer.parseObject(jsonMessage);
        String timeVal = root[String("Time")];
        String DateTime = root[String("DateTime")];
        String Hours = root[String("Hours")];
        String Minutes = root[String("Minutes")];
        String Seconds = root[String("Seconds")];
        String HourPeriod = root[String("HourPeriod")];
        String Day = root[String("Day")];
        String Month = root[String("Month")];
        String Year = root[String("Year")];
        Serial.println(timeVal+"-**"+DateTime+"-**"+Hours+"-**"+Minutes+"-**"+Seconds);
        if(Hours!="" && Minutes !="" && Seconds!="" && Day!="" && Month!="" && Year!=""){
          if(Hours.length()==1){Hours="0"+Hours;}
          if(Minutes.length()==1){Minutes="0"+Minutes;}
          if(Seconds.length()==1){Seconds="0"+Seconds;}
          if(Day.length()==1){Day="0"+Day;}
          if(Month.length()==1){Month="0"+Month;}            
          //SetDateTime(Hours,Minutes,HourPeriod,Day,Month,Year);              
          }else{
           Serial.println("Not able to read Json. Executing RESET.....");             
           Serial.end();
           delay(2000);
           Serial.begin(9600);
           delay(4000);
           if(Serial.available()){
            Serial.println("Serial reset COMPLETE.....");             
            }
          }
        jsonMessage="";
        IsMessageComplete=false;
    }else{
     Serial.println("Still Reading");  
    } 

}


unsigned long SetDateTime(String _Hours,String _Minutes,String _Seconds,String _Day,String _Month,String _Year, String _Temp, String _AMPM) {
    Serial.print("in TFT:"+_Hours+" "+_Minutes+" "+_Seconds+" "+_Day);
     unsigned long start = micros();     

    if(_Day !="" && _Month != "" && _Year !=""){  
       String FullDate=_Day+"-"+_Month+"-"+_Year;
       if(G_Date!=FullDate){
         CleanText(0, 0, 4, G_Date);
         G_Date = FullDate;
         tft.setCursor(0, 0);
          tft.setTextColor(WHITE);  
          tft.setTextSize(4);
          tft.println(FullDate);    
        }
     }
        
    if(_Hours !=""){
      if(G_Hours != _Hours){
        CleanText(0, 60, 8, G_Hours);
        G_Hours = _Hours;
        tft.setCursor(0, 60);
        tft.setTextColor(RED);
        tft.setTextSize(8);
        tft.println(_Hours+":");      
      }       
    }

    if(_Minutes != ""){       
      if(G_Minutes != _Minutes){  
       CleanText(130, 60, 8, G_Minutes);
       G_Minutes = _Minutes;
       tft.setCursor(130, 60);
       tft.setTextColor(RED);
       tft.setTextSize(8);
       tft.println(_Minutes+" ");    
      }       
    }
    if(_Seconds != ""){         
      if(G_Seconds != _Seconds){        
        CleanText(225, 60, 5, G_Seconds);
        G_Seconds=_Seconds;
        Serial.println("Setting Sec"+_Seconds);
        tft.setCursor(225, 60);
        tft.setTextColor(RED);
        tft.setTextSize(5);
        tft.println(_Seconds);
      }
    }
   if(_Seconds != ""){         
    if(G_AMPM != _AMPM){        
        CleanText(225, 100, 3, G_AMPM);
        G_AMPM = _AMPM;     
        Serial.println("AM/PM");
        tft.setCursor(225, 100);
        tft.setTextColor(RED);
        tft.setTextSize(3);
        tft.println(_AMPM);
     }
   }
   
   if(_Temp != ""){         
      if(G_Temp != _Temp){        
        CleanText(0, 140, 3, G_Temp);
        G_Temp=_Temp;
        Serial.println("Setting Temp"+_Temp);
        tft.setCursor(0, 140);
        tft.setTextColor(GREEN);
        tft.setTextSize(3);       
        tft.println(_Temp);
        tft.drawChar(90, 140, 247, GREEN, GREEN, 2);
      }
    }
  
  return micros() - start;
}

String FormatText(int _number){

    if(_number < 10){
      Serial.println("In <9"+String(_number));
      return "0"+String(_number);
    }else{
      Serial.println("In 10"+String(_number));
      return String(_number);
    }            
}
void CleanText(int _x, int _y,int _size, String _TexttoClean )
{
      Serial.println("Clean Text");
      tft.setCursor(_x, _y);
      tft.setTextColor(BLACK);
      tft.setTextSize(_size);
      tft.println(_TexttoClean);   
  }
unsigned long SetErrorMessage(String _message) {
  Serial.println("Error Message");
  unsigned long start = micros();
  tft.setRotation(1); 
   tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(RED);
  tft.setTextSize(5);
  tft.println(_message);    
  return micros() - start;
}
unsigned long SetStartupMessage() {
  Serial.println("Startup Message");
  unsigned long start = micros();
  tft.setRotation(1); 
  tft.fillScreen(BLACK);
  tft.setCursor(40, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.println("IOT Clock");    
  tft.setCursor(0, 80);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.println("Clock Init.....");   
  //delay(5000);
  tft.setRotation(1);
  tft.fillScreen(BLACK); 
  return micros() - start;
}
