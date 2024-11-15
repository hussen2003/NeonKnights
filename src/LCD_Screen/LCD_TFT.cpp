#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "LCD_TFT.h"

// Define the pins for the TFT display
#define TFT_CS 19
#define TFT_RST 17
#define TFT_DC 16
#define TFT_SCLK 18
#define TFT_MOSI 23

LCD_TFT::LCD_TFT() : tft(TFT_CS, TFT_DC, TFT_RST) {}


void LCD_TFT::setup()
{
  // Initialize the display
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST77XX_BLACK);

  // // Display some text
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 0);

  // // Draw a rectangle
  //tft.drawRect(10, 10, 50, 50, ST77XX_RED);
}

void LCD_TFT::loop()
{
  // Nothing to do here
}
