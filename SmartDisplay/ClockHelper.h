#ifndef ClockHelper_h
#define ClockHelper_h

class ClockHelperClass
{
  public:
    ClockHelperClass(); 
    void DisplayTime(int h,int m,int s,int pinCS, int numberOfHorizontalDisplays, int numberOfVerticalDisplays);   
};

extern ClockHelperClass ClockHelper;
#endif

