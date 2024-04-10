#include <WiFi.h>

void getMacAddress()
{
  Serial.begin(115200);
  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  delay(1000);
}
// dev1 MAC Address: E4:65:B8:76:88:B0
// dev2 MAC Address: FC:B4:67:72:7C:94
