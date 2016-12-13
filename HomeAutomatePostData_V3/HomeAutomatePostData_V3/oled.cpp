#include "Arduino.h"
#include "oled.h"
//OLED LIB
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//OLED INIT
#define OLED_SDA   D7  //MOSI -- D1
#define OLED_SCL   D5  //CLK  -- D0
#define OLED_DC    D4  //     --DC
#define OLED_CS    12  // no need of connecting, just use some pin number
#define OLED_RESET D3  //RES  --RES
Adafruit_SSD1306 display(OLED_SDA,OLED_SCL, OLED_DC, OLED_RESET, OLED_CS);

oledClass::oledClass(){
      display.begin(SSD1306_SWITCHCAPVCC);   // since i am using adafruit library, i have to display their logo
      //display.begin(bitmap_112283585);
      display.display();
      delay(500);
}

void oledClass::DisplayMessage(String Message, int Size, int CurserX, int CurserY){
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(Size);
        display.setCursor(CurserX,CurserY);
        display.print(Message);
        display.display();
  }
oledClass oled=oledClass();

