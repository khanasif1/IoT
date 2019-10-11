#ifndef HttpHelper_h
#define HttpHelper_h

class HttpHelperClass
{
  public:
    HttpHelperClass();
    String GetHttp(char* host,String url); 
    String PostSecureHttp(char* host,String url,String id); 
};

extern HttpHelperClass HttpHelper;
#endif

