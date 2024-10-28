#include "Arduino.h"
#include "reciever.h"
#include <./emitter/espNowSender.h>

extern EspNowSender espNowSender;
Reciever::Reciever() {};

//int pd = 25;     // digital photodiode pin
int sensor = 34; // readings from sensor to analog pin
int ledPin = 12;

void Reciever :: setup()
{
    espNowSender.setup();
    Serial.begin(115200);
    pinMode(sensor, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

}

void Reciever :: loop()
{
    espNowSender.loop();
    int sensorValue = analogRead(sensor);
    //Serial.println(sensorValue);
    if (sensorValue == 0)
    {
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }
    delay(100);
}