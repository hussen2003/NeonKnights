#include "Arduino.h"
#include "emitter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <./LCD_Screen/LCD_TFT.h>
//#include <./reciever/espNowReceiver.h>

Emitter::Emitter() {};
const int trigger = 32;
const int led = 13;
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
    pinMode(led, OUTPUT);
    pinMode(haptic1, OUTPUT);
    pinMode(haptic2, OUTPUT);

    // Configure LEDC for PWM generation (Channel 0, frequency 38kHz, 8-bit resolution)
    ledcSetup(ledChannel, frequency, resolution);

    // Attach PWM pin to the LEDC channel
    ledcAttachPin(pwmPin, ledChannel);
}

void Emitter::loop()
{
    if (digitalRead(trigger) == LOW)
    {
        digitalWrite(led, HIGH);
        digitalWrite(haptic1, HIGH);
        digitalWrite(haptic2, HIGH);

        Serial.println("button pressed!");

        // Start PWM signal on the PWM pin at 38kHz
        ledcWriteTone(ledChannel, frequency);
        delay(100); // Send signal for 100ms

        // Stop the PWM signal
        ledcWrite(ledChannel, 0); // Set duty cycle to 0 to stop the signal
        delay(100);               // Debounce delay
    }
    else
    {
        digitalWrite(led, LOW);
        digitalWrite(haptic1, LOW);
        digitalWrite(haptic2, LOW);
    }
}