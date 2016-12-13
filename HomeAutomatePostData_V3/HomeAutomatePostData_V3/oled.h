#ifndef oled_h
#define oled_h

class oledClass
{
  public:
    oledClass();
    void DisplayMessage(String Message, int Size, int CurserX, int CurserY);  
};

extern oledClass oled;
#endif

