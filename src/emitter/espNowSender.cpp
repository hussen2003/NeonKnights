#include "espNowSender.h"
#include <esp_now.h>
#include <WiFi.h>

EspNowSender::EspNowSender() {};

#define buttonPin 0
#define greenLEDPin = 27 

int button_value;
uint8_t mac_address [] = {0xD0, 0xEF, 0x76, 0x15, 0x22, 0xB8};

// mac address of receiver
//uint8_t mac_address [] = {0xFC, 0xB4, 0x67, 0x72, 0x7C, 0x94};

typedef struct message
{
  int button_value;
 
} message;

message myData;

esp_now_peer_info_t peerInfo;

//callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.printf("\r\nPacket send status: \t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Failure");
}

void EspNowSender::setup()
{
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK)
  {
    Serial.println("ESP_NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, mac_address, sizeof(peerInfo.peer_addr));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if(esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("ESP_NOW add peer failed");
    return;
  }
}

void EspNowSender::loop()
{
  int sensor_val = analogRead(34);
  // if(sensor_val > 0)
  // {
  //   myData.button_value = 0;
  // }
  // else if (sensor_val == 0)
  // {
  //   myData.button_value = 1;
  // }

  myData.button_value = sensor_val;

  esp_err_t result = esp_now_send(mac_address, (uint8_t *) &myData, sizeof(myData));

  if(result == ESP_OK)
  {

    Serial.println("ESP_NOW send success");
    Serial.print("Receiver data: ");
    Serial.println(myData.button_value);
  }else{
    Serial.println("ESP_NOW send failed");
  }

}

