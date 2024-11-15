#include "Arduino.h"
#include "emitter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <./LCD_Screen/LCD_TFT.h>
//#include <./reciever/espNowReceiver.h>

LCD_TFT LcdTFT;

Emitter::Emitter() {};
const int trigger = 32;
const int reload = 33;
const int led_R = 27;
const int led_G = 14;
const int led_B = 13;
const int haptic1 = 26;
const int haptic2 = 4;

const int pwmPin = 25;
const int frequency = 38000; // 38kHz frequency
const int ledChannel = 0;    // Channel 0 for LEDC PWM
const int resolution = 8;    // 8-bit resolution (0-255)

void Emitter::setup()
{
    Serial.begin(115200);
    pinMode(trigger, INPUT_PULLUP);
    pinMode(haptic1, OUTPUT);
    pinMode(haptic2, OUTPUT);

    // Configure LEDC for PWM generation (Channel 0, frequency 38kHz, 8-bit resolution)
    ledcSetup(ledChannel, frequency, resolution);

    // Attach PWM pin to the LEDC channel
    ledcAttachPin(pwmPin, ledChannel);

    LcdTFT.setup();
    LcdTFT.tft.fillScreen(ST7735_BLACK);
    LcdTFT.tft.printf("     Neon Knights");
    LcdTFT.tft.printf("          Laser Tag");
}


void resetbullets ()
{
    int x = 5;
    int y = 10;

    for(int i = 0; i < 12; i++)
    {
        LcdTFT.tft.fillRect(x,y,10,5,ST7735_YELLOW);
        LcdTFT.tft.fillCircle(x+10,y+2.5,2.5,ST7735_YELLOW);
        y += 10;
    }

    LcdTFT.tft.setCursor(50,64);
    LcdTFT.tft.printf("Bullets: %d",12);
}

void resethearts ()
{
    int x = 120;
    int y = 10;

    for(int i = 0; i < 10; i++)
    {
        LcdTFT.tft.fillCircle(x-2,y,2,ST7735_RED);
        LcdTFT.tft.fillCircle(x+2,y,2,ST7735_RED);
        LcdTFT.tft.fillTriangle(x-4,y,x+4,y,x,y+5,ST7735_RED);
        y += 12;
    }
}

void deletebullets (int bullets)
{
    int deleted = 12 - bullets;
    int y2 = 10*deleted;

    LcdTFT.tft.fillRect(5,10,13,y2,ST7735_BLACK);

    LcdTFT.tft.setCursor(50,64);
    LcdTFT.tft.printf("Bullets: %d",12);
}

void deletehearts (int originalhealth, int health)
{  
    int y2 = (float)120*(1-((float)health/(float)originalhealth));

    LcdTFT.tft.fillRect(116,8,124,y2-3,ST7735_BLACK);
}

void ledcolor(char color[10])
{
    if (strcmp(color,"red"))
    {
        digitalWrite(led_R, HIGH);
        digitalWrite(led_G, LOW);
        digitalWrite(led_B, LOW);
    }
    else if (strcmp(color,"blue"))
    {
        digitalWrite(led_R, LOW);
        digitalWrite(led_G, LOW);
        digitalWrite(led_B, HIGH);
    }
    else if (strcmp(color, "green"))
    {
        digitalWrite(led_R, LOW);
        digitalWrite(led_G, HIGH);
        digitalWrite(led_B, LOW);
    }
    else if (strcmp(color,"yellow"))
    {
        digitalWrite(led_R, HIGH);
        digitalWrite(led_G, HIGH);
        digitalWrite(led_B, LOW);
    }
    else if (strcmp(color,"purple"))
    {
        digitalWrite(led_R, HIGH);
        digitalWrite(led_G, LOW);
        digitalWrite(led_B, HIGH);
    }
    else if (strcmp(color, "cyan"))
    {
        digitalWrite(led_R, LOW);
        digitalWrite(led_G, HIGH);
        digitalWrite(led_B, HIGH);
    }
}

int bullets = 12;
int originalhealth = 100;
int health = 0;
char color[10] = "blue";
char red[10] = "red";

void Emitter::loop()
{
    // take in info from the main hub
    // update health

   deletehearts(originalhealth,health);
   if (health <= 0)
   {
        ledcolor(red);
        while (health <= 0)
        {
            // take info from main hub
        }
        ledcolor(color);
   }

    // button pressed 
    if ((digitalRead(trigger) == LOW) && (bullets != 0) && (digitalRead(reload) == HIGH))
    {
        digitalWrite(haptic1, HIGH);
        digitalWrite(haptic2, HIGH);

        Serial.println("button pressed!");
        // take away bullets
        bullets--;
        deletebullets(bullets);

        // Start PWM signal on the PWM pin at 38kHz
        ledcWriteTone(ledChannel, frequency);
        delay(100); // Send signal for 100ms

        // Stop the PWM signal
        ledcWrite(ledChannel, 0); // Set duty cycle to 0 to stop the signal
        delay(100);               // Debounce delay

        digitalWrite(haptic1, LOW);
        digitalWrite(haptic2, LOW);
    }

    // if reloading
    if (digitalRead(reload) == LOW)
    {
        // delay till done reloading
        while(digitalRead(reload) == LOW)
        {

        }
        //update bullets to 12
        bullets = 12;
        resetbullets();
    }
}