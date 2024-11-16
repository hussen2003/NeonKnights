#include "Arduino.h"
#include "reciever.h"
#include <esp_now.h>
#include <WiFi.h>

Reciever::Reciever() {}

// RECEIVER'S MAC Address
// uint8_t broadcastAddress[] = {0xFC, 0xB4, 0x67, 0x74, 0x4B, 0xE0};
uint8_t broadcastAddress[] = {0xfc, 0xB4, 0x67, 0x72, 0x7c, 0x94};

// Structure to send data (same as before)
typedef struct struct_message
{
    int id = 2; // must be unique for each sender board
    int reciever1Value;
    int reciever2Value;
    int reciever3Value;
    int reciever4Value;
} struct_message;

// Structure to receive game-related data
typedef struct input_data
{
    bool hasGameStarted;
    char color1[10];
    char color2[10]; // Color for Team 2
    int health1;
    int originalHealth1;
    int health2;
    int originalHealth2;
} input_data;

// Create instances of structs
struct_message myData;
input_data receivedData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// callback when data is received

int Originalhealth = 100;
int Health = 100;

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Game Started: ");
    Serial.println(receivedData.hasGameStarted ? "Yes" : "No");
    Serial.print("Color 1: ");
    Serial.println(receivedData.color1);
    Serial.print("Color 2: ");
    Serial.println(receivedData.color2);
}

void espNowSetup()
{
    // Init Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register for Send callback
    esp_now_register_send_cb(OnDataSent);

    // Register for Receive callback
    esp_now_register_recv_cb(OnDataRecv);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
}

void espNowLoop()
{
    Serial.printf("Data ID sent: %d\n", myData.id);



    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }
    delay(100);
}

const int reciever1 = 39;
const int reciever2 = 34;
const int reciever3 = 35;
const int reciever4 = 32;

const int led1_R = 33;
const int led1_G = 25;
const int led1_B = 26;

const int led2_R = 27;
const int led2_G = 14;
const int led2_B = 13;

const int led3_R = 4;
const int led3_G = 16;
const int led3_B = 17;

const int led4_R = 18;
const int led4_G = 19;
const int led4_B = 21;

void setupRecieversAndLed()
{

    // Set receivers as input
    pinMode(reciever1, INPUT_PULLUP);
    pinMode(reciever2, INPUT_PULLUP);
    pinMode(reciever3, INPUT_PULLUP);
    pinMode(reciever4, INPUT_PULLUP);

    // Set RGB LED pins as output and initialize to LOW
    pinMode(led1_R, OUTPUT);
    pinMode(led1_G, OUTPUT);
    pinMode(led1_B, OUTPUT);

    pinMode(led2_R, OUTPUT);
    pinMode(led2_G, OUTPUT);
    pinMode(led2_B, OUTPUT);

    pinMode(led3_R, OUTPUT);
    pinMode(led3_G, OUTPUT);
    pinMode(led3_B, OUTPUT);

    pinMode(led4_R, OUTPUT);
    pinMode(led4_G, OUTPUT);
    pinMode(led4_B, OUTPUT);

    // Initialize all RGB pins to LOW
    digitalWrite(led1_R, LOW);
    digitalWrite(led1_G, LOW);
    digitalWrite(led1_B, LOW);

    digitalWrite(led2_R, LOW);
    digitalWrite(led2_G, LOW);
    digitalWrite(led2_B, LOW);

    digitalWrite(led3_R, LOW);
    digitalWrite(led3_G, LOW);
    digitalWrite(led3_B, LOW);

    digitalWrite(led4_R, LOW);
    digitalWrite(led4_G, LOW);
    digitalWrite(led4_B, LOW);
}

void setColor(int red, int green, int blue)
{
    // Set all LEDs to the specified color
    digitalWrite(led1_R, red);
    digitalWrite(led1_G, green);
    digitalWrite(led1_B, blue);

    digitalWrite(led2_R, red);
    digitalWrite(led2_G, green);
    digitalWrite(led2_B, blue);

    digitalWrite(led3_R, red);
    digitalWrite(led3_G, green);
    digitalWrite(led3_B, blue);

    digitalWrite(led4_R, red);
    digitalWrite(led4_G, green);
    digitalWrite(led4_B, blue);
}

void setVestColor()
{
    if (receivedData.hasGameStarted)
    {
        // Check which ID we are dealing with
        if (myData.id == 1 && strlen(receivedData.color1) > 0)
        {
            if (strcmp(receivedData.color1, "red") == 0)
            {
                setColor(HIGH, LOW, LOW); // Set LEDs to blue
            }
            // Set color based on receivedData.color1
            else if (strcmp(receivedData.color1, "blue") == 0)
            {
                setColor(LOW, LOW, HIGH); // Set LEDs to blue
            }
            else if (strcmp(receivedData.color1, "green") == 0)
            {
                setColor(LOW, HIGH, LOW); // Set LEDs to green
            }
            else if (strcmp(receivedData.color1, "yellow") == 0)
            {
                setColor(HIGH, HIGH, LOW); // Set LEDs to yellow
            }
            else if (strcmp(receivedData.color1, "purple") == 0)
            {
                setColor(HIGH, LOW, HIGH); // Set LEDs to purple
            }
            else if (strcmp(receivedData.color1, "cyan") == 0)
            {
                setColor(LOW, HIGH, HIGH); // Set LEDs to cyan
            }
        }
        else if (myData.id == 2 && strlen(receivedData.color2) > 0)
        {
            // Set color based on receivedData.color2
            if (strcmp(receivedData.color2, "red") == 0)
            {
                setColor(HIGH, LOW, LOW); // Set LEDs to blue
            }
            else if (strcmp(receivedData.color2, "blue") == 0)
            {
                setColor(LOW, LOW, HIGH); // Set LEDs to blue
            }
            else if (strcmp(receivedData.color2, "green") == 0)
            {
                setColor(LOW, HIGH, LOW); // Set LEDs to green
            }
            else if (strcmp(receivedData.color2, "yellow") == 0)
            {
                setColor(HIGH, HIGH, LOW); // Set LEDs to yellow
            }
            else if (strcmp(receivedData.color2, "purple") == 0)
            {
                setColor(HIGH, LOW, HIGH); // Set LEDs to purple
            }
            else if (strcmp(receivedData.color2, "cyan") == 0)
            {
                setColor(LOW, HIGH, HIGH); // Set LEDs to cyan
            }
        }
    }
}

void Reciever::setup()
{
    Serial.begin(115200);
    setupRecieversAndLed();
    espNowSetup();
}


void Reciever::loop()
{
    espNowLoop();

    int reciever1Value = digitalRead(reciever1);
    int reciever2Value = digitalRead(reciever2);
    int reciever3Value = digitalRead(reciever3);
    int reciever4Value = digitalRead(reciever4);

    myData.reciever1Value = reciever1Value;
    myData.reciever2Value = reciever2Value;
    myData.reciever3Value = reciever3Value;
    myData.reciever4Value = reciever4Value;

    if (reciever1Value == 0)
    {
        digitalWrite(led1_R, HIGH);
        digitalWrite(led1_B, LOW);
        digitalWrite(led1_G, LOW);
        delay(100);
    }
    if (reciever2Value == 0)
    {
        digitalWrite(led2_R, HIGH);
        digitalWrite(led2_B, LOW);
        digitalWrite(led2_G, LOW);
        delay(100);
    }
    if (reciever3Value == 0)
    {
        digitalWrite(led3_R, HIGH);
        digitalWrite(led3_B, LOW);
        digitalWrite(led3_G, LOW);
        delay(100);
    }
    if (reciever4Value == 0)
    {
        digitalWrite(led4_R, HIGH);
        digitalWrite(led4_B, LOW);
        digitalWrite(led4_G, LOW);
        delay(100);
    }

    setVestColor();
}
