#include "wifiScan.h"
#include <WiFi.h>

WifiScan::WifiScan() {};

#define greenLedPin 17
#define redLedPin 16

void WifiScan::setup() 
{
  Serial.begin(115200);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
}

void WifiScan::loop() 
{
  Serial.println("scanning available networks...");
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
  int n = WiFi.scanNetworks();
  if(n!=0)
  {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    Serial.print(n);
    Serial.println(" networks found.");
    for(int i = 0; i < n; ++i)
    {
      Serial.print("network "); Serial.print(i+1); Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" ("); Serial.print(WiFi.RSSI(i)); Serial.print(")");
      Serial.println(WiFi.encryptionType(i) == WIFI_AUTH_OPEN? " open " : " ***");
      delay(50);
    }
  }
  else{
    Serial.println("no networks found");
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
  }
  Serial.println("\n------------------------------------------\n");
  delay(5000);
}