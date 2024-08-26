#include "menu.h"

#define upButtonPin 25
#define downButtonPin 27
#define selectButtonPin 26
#define ledPin 2

menu::menu() {};

void menu::setup() {
  lcd_TFT.setup();
  pinMode(upButtonPin, INPUT_PULLDOWN);
  pinMode(downButtonPin, INPUT_PULLDOWN);
  pinMode(selectButtonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void menu::loop() {
  int upButtonState = digitalRead(upButtonPin);
  int downButtonState = digitalRead(downButtonPin);
  int selectButtonState = digitalRead(selectButtonPin);

  if(upButtonState == HIGH) { 
    digitalWrite(ledPin, HIGH);
    lcd_TFT.tft.drawRect(10, 10, 50, 50, ST77XX_RED);
  }
  else if(downButtonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    lcd_TFT.tft.drawRect(10, 60, 50, 50, ST77XX_GREEN);
  }
  else if(selectButtonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    lcd_TFT.tft.fillScreen(ST77XX_BLACK);
  }
  else {
    digitalWrite(ledPin, LOW);
  }

}