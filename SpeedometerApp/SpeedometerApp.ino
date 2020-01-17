
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <ArduinoJson.h>
#include <Wire.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET 0 // Can alternately just connect to Arduino's reset pin

const int pingPin = 13; // Yello Trigger Pin of Ultrasonic Sensor
const int echoPin = 12; //Green Echo Pin of Ultrasonic Sensor


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

String G_inch;String G_cms;

void setup(void) {
  
  Serial.begin(9600);
   Wire.begin();
   //rtc.begin();
  //DateTime dt(2017, 04, 02, 12, 59, 30, 0);
  //rtc.setDateTime(dt);
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
     Distance();     
}
void Distance(){
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   //inches=10;
   //cm=22;
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   ShowDistance(String(inches),String(cm));
   delay(100);
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}


 void ShowDistance(String _inch,String _cms) {
    Serial.print("in TFT:"+_inch+" "+_cms);
   
    if(_inch !=""){
      if(G_inch != _inch){
        CleanText(0, 50, 8, G_inch+"in");
        G_inch = _inch;
        tft.setCursor(0, 50);
        tft.setTextColor(RED);
        tft.setTextSize(8);
        tft.println(_inch+"in");      
      }       
    }

    if(_cms != ""){       
      if(G_cms != _cms){  
       CleanText(0, 140, 8, G_cms+"cm");
       G_cms = _cms;
       tft.setCursor(0, 140);
       tft.setTextColor(RED);
       tft.setTextSize(8);
       tft.println(_cms+"cm");    
      }       
    }   
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

unsigned long SetStartupMessage() {
  Serial.println("Startup Message");
  unsigned long start = micros();
  tft.setRotation(1); 
  tft.fillScreen(BLACK);
  tft.setCursor(40, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.println("DSense");    
  tft.setCursor(0, 80);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.println("Sensor Starting");   
  //delay(5000);
  tft.setRotation(1);
  tft.fillScreen(BLACK);   
  return micros() - start;
}
