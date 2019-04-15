#include "Arduino.h"
#include "ClockHelper.h"
#include "Adafruit_GFX.h"
#include "Max72xxPanel.h"

ClockHelperClass::ClockHelperClass(){
}
  void ClockHelperClass::DisplayTime(int h,int m,int s,int pinCS, int numberOfHorizontalDisplays, int numberOfVerticalDisplays){
    Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);    
      matrix.setIntensity(13);
  matrix.setRotation(0, 1);        // 1 
  matrix.setRotation(1, 1);        // 2 
  matrix.setRotation(2, 1);        // 3 
  matrix.setRotation(3, 1);        // 4 
    matrix.fillScreen(LOW);
    int y = (matrix.height() - 8) / 2;
    
    if(s & 1){matrix.drawChar(14, y, (String(":"))[0], HIGH, LOW, 1);}
    else{matrix.drawChar(14, y, (String(" "))[0], HIGH, LOW, 1);}
        String hour1;String hour2 ;
          String min1; String min2;
        
        if(h>9){
           if(h>9 && h<20){
             hour1 = String (h);
             hour2 = String (h - 10);
           }
           else{
             hour1 = String (h);
             hour2 = String (h - 20);           
           }
          }else{          
           hour1 = String (0);
           hour2 = String (h);
          }
          
          if(m>9){
            if(m>9 &&m<20){
             min1 = String (m);
             min2 = String (m - 10);            
            }else if(m>19 &&m<30){
             min1 = String (m);
             min2 = String (m - 20);            
            }else if(m>29 &&m<40){
             min1 = String (m);
             min2 = String (m - 30);            
            }else if(m>39 &&m<50){
             min1 = String (m);
             min2 = String (m - 40);            
            }else if(m>49 &&m<60){
             min1 = String (m);
             min2 = String (m - 50);            
            }else{
             min1 = String (m);
             min2 = String (m);                          
            }
          }else{
           min1 = String (0);
           min2 = String (m);
          }
          String sec1 = String (3);
          String sec2 = String (3);
          int xh = 2;
          int xm = 19;
      //    int xs = 28;
      
          matrix.drawChar(xh, y, hour1[0], HIGH, LOW, 1);
          matrix.drawChar(xh+6, y, hour2[0], HIGH, LOW, 1);
          matrix.drawChar(xm, y, min1[0], HIGH, LOW, 1);
          matrix.drawChar(xm+6, y, min2[0], HIGH, LOW, 1);
          //matrix.drawChar(xs, y, sec1[0], HIGH, LOW, 1);
          //matrix.drawChar(xs+6, y, sec2[0], HIGH, LOW, 1);  
    matrix.write();
  }

ClockHelperClass ClockHelper=ClockHelperClass();

