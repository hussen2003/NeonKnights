#ifndef MENU_H
#define MENU_H

#include "LCD_TFT.h"


class menu{
  public:
    menu();
    void setup();
    void loop();
    private:
      LCD_TFT lcd_TFT;
};

#endif // MENU_H