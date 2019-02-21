#ifndef HealthCheckHelper_h
#define HealthCheckHelper_h

class HealthCheckHelperClass
{
  public:
    HealthCheckHelperClass(); 
    String PingHealth(String id);   
};

extern HealthCheckHelperClass HealthCheckHelper;
#endif

