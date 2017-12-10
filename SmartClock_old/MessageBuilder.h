#ifndef MessageBuilder_h
#define MessageBuilder_h

class MessageBuilderClass
{
  public:
    MessageBuilderClass();
    String BuildRegistrationMessage();
    String BuildPostMessage(int DeviceId, double humiditySensorData,double temperatureC);
    String DelayHumidityMessage(double humiditySensorData);
    String DelayTemperatureMessage(double temperatureC);
};

extern MessageBuilderClass MessageBuilder;
#endif

