#include <Arduino.h>
//#include <./oldwifi/wifiSAP.h>
#include <getMacAddress.h>
//#include <./oldwifi/wifiScan.h>
#include <./wifi/mainHub.h>
// #include <./reciever/espNowReceiver.h>
// #include <./emitter/espNowSender.h>
#include <./emitter/emitter.h>
#include <./reciever/reciever.h>
#include <./LCD_Screen/LCD_TFT.h>
#include <./LCD_Screen/menu.h>


//  Main hub: FC:B4:67:74:4B:E0
//  Vest 1: D0:EF:76:15:4E:20
//  Gun 2:

GetMacAddress getMacAddress;
// WifiSAP wifiSAP;
// WifiScan wifiScan;
MainHub mainHub;
//EspNowReceiver espNowReceiver;
//EspNowSender espNowSender;
LCD_TFT lcdTFT;
menu Menu;
Emitter emitter;
Reciever reciever;

void setup() {
  //mainHub.setup();
  //emitter.setup();
  reciever.setup();
}

void loop() {
  //mainHub.loop();
  //emitter.loop();
  reciever.loop();
}


