#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}
// dev1 MAC Address: E4:65:B8:76:88:B0
// dev2 MAC Address: MAC Address: FC:B4:67:72:7C:94
void loop()
{
  // Do Nothing
}