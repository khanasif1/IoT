#ifndef WifiHelper_h
#define WifiHelper_h

class WifiHelperClass
{
  public:
    WifiHelperClass();
    void Connect();  
    int registerDevice();
    String sendSensorData();
};

extern WifiHelperClass WifiHelper;
#endif

