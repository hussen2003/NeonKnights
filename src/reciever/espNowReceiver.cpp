#include "espNowReceiver.h"
#include <esp_now.h>
#include <WiFi.h>

#define greenLedPin 17
#define redLedPin 16

EspNowReceiver::EspNowReceiver() {};

typedef struct struct_message {
    int button_value;
} struct_message;

struct_message my_data;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&my_data, incomingData, sizeof(my_data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Button Value: ");
  Serial.println(my_data.button_value);

}
 
void EspNowReceiver::setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  // Turn on the red LED by default
  digitalWrite(redLedPin, HIGH);
}
void EspNowReceiver::loop() {
  if(my_data.button_value == 0)
  {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }
  if(my_data.button_value == 1)
  {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
  }
}