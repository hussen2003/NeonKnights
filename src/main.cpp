// #include <Arduino.h>
// #include <wifiSAP.h>
// #include <getMacAddress.h>
// #include <wifiScan.h>
// #include <./reciever/espNowReceiver.h>
// #include <./emitter/espNowSender.h>
// #include <./LCD_Screen/LCD_Screen.h>


// GetMacAddress getMacAddress;
// WifiSAP wifiSAP;
// WifiScan wifiScan;
// EspNowReceiver espNowReceiver;
// EspNowSender espNowSender;
// LCD_Screen lcdScreen;


// void setup() {
//   // getMacAddress.setup();
//   // wifiScan.setup();
//   // wifiSAP.setup();
//   // espNowSender.setup();
//   // espNowReceiver.setup();
//   lcdScreen.setup();
// }

// void loop() {
//   // wifiScan.loop();
//   // wifiSAP.loop();
//   // espNowSender.loop();
//   // espNowReceiver.loop();
  
// }

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Define the pins for the TFT display
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2
#define TFT_SCLK   18
#define TFT_MOSI   23

// Create an instance of the display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialize the display
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST77XX_BLACK);

  // Display some text
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 0);
  tft.println("Hello, World!");

  // Draw a rectangle
  tft.drawRect(10, 10, 50, 50, ST77XX_RED);
}

void loop() {
  // Nothing to do here
}
