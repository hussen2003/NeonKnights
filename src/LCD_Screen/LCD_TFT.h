#ifndef LCD_TFT_H
#define LCD_TFT_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

class LCD_TFT {
  public:
    LCD_TFT();
    void setup();
    void loop();
    Adafruit_ST7735 tft;
};

#endif