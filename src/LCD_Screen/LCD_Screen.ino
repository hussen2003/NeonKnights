
// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int shootpin = 2;
const int reloadpin = 4;
int shootstate = 0;
int reloadstate = 0;
int x = 3;

void setup() 
{\
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Ammo Count: ");
  lcd.setCursor(12, 0);
  lcd.print(x);
  pinMode(shootpin, INPUT);
  pinMode(reloadpin,  INPUT);
  Serial.println(x);
  Wire.setClock(10000);
}

void loop() {
  shootstate = digitalRead(shootpin);
  reloadstate = digitalRead(reloadpin);
  if(x > 0)
  {
    if(shootstate == HIGH)
    {
      while(shootstate == HIGH)
      {
        shootstate = digitalRead(shootpin);
      }
      x = x-1;
      Serial.println(x);
      lcd.setCursor(12, 0);
      lcd.print(x);
    }
  }
  if(reloadstate == HIGH)
  {
    x = 3;
    lcd.setCursor(0, 0);
    lcd.print("Ammo Count: ");
    lcd.setCursor(12, 0);
    lcd.print(x);
  }
  while(x == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Reload      ");
    reloadstate = digitalRead(reloadpin);
    if(reloadstate == HIGH)
    {
      x = 3;
      lcd.setCursor(0, 0);
      lcd.print("Ammo Count: ");
      lcd.setCursor(12, 0);
      lcd.print(x);
    }
  }
}