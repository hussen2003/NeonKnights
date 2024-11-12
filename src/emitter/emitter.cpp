#include "Arduino.h"
#include "emitter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <./LCD_Screen/LCD_TFT.h>
#include <./reciever/espNowReceiver.h>

Emitter::Emitter() {};
// // LCD_TFT lcd;
// // extern EspNowReceiver espNowReceiver;

// // const int pwmPin = 25; // Replace with your desired PWM pin
// // const int ledPin = 13;
// // const int frequency = 38000; // 38kHz frequency
// // const int buttonPin = 32;
// // const int reload_pin = 33;

// void Emitter ::setup()
// {

//     //espNowReceiver.setup();
//     // lcd.setup();

//     // ledcSetup(0, frequency, 8); // Channel 0, frequency, resolution (8 bits)
//     // ledcAttachPin(pwmPin, 0);   // Attach the PWM pin to the LEDC channel

//     // lcd.tft.setTextSize(1);
//     // lcd.tft.setTextColor(ST77XX_WHITE);
//     // lcd.tft.setCursor(0, 0);
//     // pinMode(pwmPin, OUTPUT);
//     // pinMode(ledPin, OUTPUT);
//     // digitalWrite(ledPin, LOW);
//     // pinMode(buttonPin, INPUT_PULLUP);
//     // pinMode(reload_pin, INPUT_PULLUP);
// }

// //int sensor_Value;

// void Emitter::loop()
// {
//     //espNowReceiver.loop();
//     // if (digitalRead(buttonPin) == LOW) // Button pressed
//     // {

//     //     // Send shot signal
//     //     tone(pwmPin, frequency); // Generate a 38kHz tone
//     //     delay(100);              // Keep tone for 100ms
//     //     noTone(pwmPin);          // Stop the tone
//     //     delay(500);              // Debounce delay

//     //     // Immediately check the receiver value
//     //     //sensor_Value = espNowReceiver.getButtonValue();

//     //     // Display shot status
//     //     lcd.tft.fillScreen(ST77XX_BLACK); // Clear the screen before printing
//     //     lcd.tft.setCursor(0, 0);
//     //     lcd.tft.setTextSize(1);
//     //     lcd.tft.setTextColor(ST77XX_WHITE);
//     //     lcd.tft.println("EMITTER SHOT!!!");

//     //     if (digitalRead(buttonPin) == 0) // Check if shot hit
//     //     {
//     //         lcd.tft.println("SHOT HIT!!");
//     //     }
//     //     else
//     //     {
//     //         lcd.tft.println("SHOT MISSED!");
//     //     }

//     //     // Serial print for debugging
//     //     // Serial.print("Receiver Value: ");
//     //     // Serial.println(sensor_Value);
//     // }

//     // // Serial.print("Receiver Value: ");
//     // // Serial.println(sensor_Value);
// }

const int button = 32;
const int led = 13;
const int haptic = 26;
const int pwmPin = 25;
const int frequency = 38000; // 38kHz frequency

int sensor_Value;

LCD_TFT lcd;
extern EspNowReceiver espNowReceiver;

void Emitter::setup()
{
    Serial.begin(115200);
    pinMode(button, INPUT_PULLUP);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    pinMode(haptic, OUTPUT);

    espNowReceiver.setup();
    lcd.setup();

    ledcSetup(0, frequency, 8); // Channel 0, frequency, resolution (8 bits)
    ledcAttachPin(pwmPin, 0);   // Attach the PWM pin to the LEDC channel

    lcd.tft.setTextSize(1);
    lcd.tft.setTextColor(ST77XX_WHITE);
    lcd.tft.setCursor(0, 0);
    //     // pinMode(pwmPin, OUTPUT);
    //     // pinMode(ledPin, OUTPUT);
    //     // digitalWrite(ledPin, LOW);
    //     // pinMode(buttonPin, INPUT_PULLUP);
    //     // pinMode(reload_pin, INPUT_PULLUP);
}

void Emitter::loop()
{
    espNowReceiver.loop();
    if (digitalRead(button) == LOW)
    {
        digitalWrite(led, HIGH);
        digitalWrite(haptic, HIGH);
        Serial.println("button pressed!");

        // Send shot signal
        tone(pwmPin, frequency); // Generate a 38kHz tone
        delay(100);              // Keep tone for 100ms
        noTone(pwmPin);          // Stop the tone
        delay(500);              // Debounce delay

        // Immediately check the receiver value
        sensor_Value = espNowReceiver.getButtonValue();

        // Display shot status
        lcd.tft.fillScreen(ST77XX_BLACK); // Clear the screen before printing
        lcd.tft.setCursor(0, 0);
        lcd.tft.setTextSize(1);
        lcd.tft.setTextColor(ST77XX_WHITE);
        lcd.tft.println("EMITTER SHOT!!!");

        if (sensor_Value == 0) // Check if shot hit
        {
            lcd.tft.println("SHOT HIT!!");
        }
        else
        {
            lcd.tft.println("SHOT MISSED!");
        }
    }
    else
    {
        Serial.println("button released!");
        digitalWrite(led, LOW);
        digitalWrite(haptic, LOW);
    }

    //     //espNowReceiver.loop();
    //     // if (digitalRead(buttonPin) == LOW) // Button pressed
    //     // {

    //     //     // Send shot signal
    //     //     tone(pwmPin, frequency); // Generate a 38kHz tone
    //     //     delay(100);              // Keep tone for 100ms
    //     //     noTone(pwmPin);          // Stop the tone
    //     //     delay(500);              // Debounce delay

    //     //     // Immediately check the receiver value
    //     //     //sensor_Value = espNowReceiver.getButtonValue();

    //     //     // Display shot status
    //     //     lcd.tft.fillScreen(ST77XX_BLACK); // Clear the screen before printing
    //     //     lcd.tft.setCursor(0, 0);
    //     //     lcd.tft.setTextSize(1);
    //     //     lcd.tft.setTextColor(ST77XX_WHITE);
    //     //     lcd.tft.println("EMITTER SHOT!!!");

    //     //     if (digitalRead(buttonPin) == 0) // Check if shot hit
    //     //     {
    //     //         lcd.tft.println("SHOT HIT!!");
    //     //     }
    //     //     else
    //     //     {
    //     //         lcd.tft.println("SHOT MISSED!");
    //     //     }

    //     //     // Serial print for debugging
    //     //     // Serial.print("Receiver Value: ");
    //     //     // Serial.println(sensor_Value);
    //     // }

    //     // // Serial.print("Receiver Value: ");
    //     // // Serial.println(sensor_Value);
    // }
}