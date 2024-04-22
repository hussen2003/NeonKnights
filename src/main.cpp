#include <Arduino.h>
#include <wifiSAP.h>
#include <getMacAddress.h>
#include <wifiScan.h>
#include <./reciever/espNowReceiver.h>
#include <./emitter/espNowSender.h>
#include <./LCD_Screen/LCD_Screen.h>


GetMacAddress getMacAddress;
WifiSAP wifiSAP;
WifiScan wifiScan;
EspNowReceiver espNowReceiver;
EspNowSender espNowSender;
LCD_Screen lcdScreen;


void setup() {
  // getMacAddress.setup();
  // wifiScan.setup();
  // wifiSAP.setup();
  // espNowSender.setup();
  // espNowReceiver.setup();
  lcdScreen.setup();
}

void loop() {
  // wifiScan.loop();
  // wifiSAP.loop();
  // espNowSender.loop();
  // espNowReceiver.loop();
  
}