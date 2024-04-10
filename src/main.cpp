#include <Arduino.h>
#include <wifiAP.h>
#include <getMacAddress.h>


void setup() {
    getMacAddress();
    setupWiFi();
}

void loop() {
    loopWiFi();
}