#include <Arduino.h>
#include <./wifi/wifiSAP.h>
#include <getMacAddress.h>
#include <./wifi/wifiScan.h>
#include <./reciever/espNowReceiver.h>
#include <./emitter/espNowSender.h>
#include <./LCD_Screen/LCD_Screen.h>
#include <./LCD_Screen/LCD_TFT.h>
#include <./LCD_Screen/menu.h>


GetMacAddress getMacAddress;
WifiSAP wifiSAP;
WifiScan wifiScan;
EspNowReceiver espNowReceiver;
EspNowSender espNowSender;
LCD_Screen lcdScreen;
LCD_TFT lcdTFT;
menu Menu;


void setup() {
  // getMacAddress.setup();
  // wifiScan.setup();
  // wifiSAP.setup();
  // espNowSender.setup();
  // espNowReceiver.setup();
  //lcdScreen.setup();
  //lcdTFT.setup();
  Menu.setup();
}

void loop() {
  // wifiScan.loop();
  // wifiSAP.loop();
  // espNowSender.loop();
  // espNowReceiver.loop();
  Menu.loop();
}

