#include "espNowReceiver.h"
#include <esp_now.h>
#include <WiFi.h>
#include <./LCD_Screen/LCD_TFT.h>

EspNowReceiver::EspNowReceiver() {};

typedef struct struct_message {
  int button_value;
} struct_message;

struct_message my_data;
int sensor_value;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  delay(100); // Optional delay, can be removed
  memcpy(&my_data, incomingData, sizeof(my_data));

  // Update sensor_value when new data is received
  sensor_value = my_data.button_value;

  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Sensor Value: ");
  // Serial.println(sensor_value);
  // Serial.println();
}

int EspNowReceiver::getButtonValue()
{
  return sensor_value;
}

void EspNowReceiver::setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);


}
void EspNowReceiver::loop() {
  
}

