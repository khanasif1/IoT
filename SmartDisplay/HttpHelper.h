#ifndef HttpHelper_h
#define HttpHelper_h

class HttpHelperClass
{
  public:
    HttpHelperClass();
    String GetHttp(char* host,String url); 
    String DisplayType(char* host,String url);   
};

extern HttpHelperClass HttpHelper;
#endif

