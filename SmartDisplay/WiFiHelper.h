#ifndef WifiHelper_h
#define WifiHelper_h

class WifiHelperClass
{
  public:
    WifiHelperClass();
    void Connect();  
    void reset();
    int registerDevice();
    String sendSensorData();
};

extern WifiHelperClass WifiHelper;
#endif

