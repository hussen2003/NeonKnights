#ifndef ESPNOWRECEIVER_H
#define ESPNOWRECEIVER_H

#include <esp_now.h>

class EspNowReceiver
{
public:
  EspNowReceiver();
  void setup();
  void loop();
  int getButtonValue();
};



#endif