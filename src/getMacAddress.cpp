#include "getMacAddress.h"

GetMacAddress::GetMacAddress() {}

void GetMacAddress::setup() {
  Serial.begin(115200);
  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  delay(1000);
}

