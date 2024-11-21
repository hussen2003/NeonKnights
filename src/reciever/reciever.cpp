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
    char gameMode[20];
    bool hasGameStarted;
    char color1[10];
    char color2[10]; // Color for Team 2
    int receiverChoice;
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
    //Serial.print("\r\nLast Packet Send Status:\t");
    // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    // Serial.println("SENDING DATA TO MAIN HUB");
                            
    // Serial.println();
}
// callback when data is received



void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    // Serial.println("RECIEVED DATA FROM MAIN HUB");
    // Serial.println();
    // Serial.print("Bytes received: ");
    // Serial.println(len);
    Serial.print("Game Started: ");
    Serial.println(receivedData.hasGameStarted ? "Yes" : "No");
    // Serial.print("Color 1: ");
    // Serial.println(receivedData.color1);
    // Serial.print("Color 2: ");
    // Serial.println(receivedData.color2);
    // Serial.print("Health1: ");
    // Serial.println(receivedData.health1);
    // Serial.print("OriginalHealth1: ");
    // Serial.println(receivedData.originalHealth1);
    // Serial.print("Health2: ");
    // Serial.println(receivedData.health2);
    // Serial.print("OriginalHealth2: ");
    // Serial.println(receivedData.originalHealth2);
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
    //Serial.printf("Data ID sent: %d\n", myData.id);



    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK)
    {
        // Serial.println("Sent with success");
    }
    else
    {
        // Serial.println("Error sending the data");
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

const int haptic = 23;

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

    pinMode(haptic, OUTPUT);
    digitalWrite(haptic, LOW);

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
        
            if (strcmp(receivedData.color1, "blue") == 0)
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
         
            if (strcmp(receivedData.color2, "blue") == 0)
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

unsigned long lastHitTime = 0;           // Time when the last hit was detected
const unsigned long debounceDelay = 200; // Time in milliseconds to ignore other inputs
bool hitDetected = false;
// Function to handle a detected hit
void handleHit(int led_R, int led_B, int led_G)
{
    hitDetected = true;
    lastHitTime = millis(); // Record the time of the hit

    digitalWrite(led_R, HIGH);
    digitalWrite(led_B, LOW);
    digitalWrite(led_G, LOW);

    digitalWrite(haptic, HIGH);
    delay(100); // Short haptic feedback duration
    digitalWrite(haptic, LOW);

    hitDetected = true;     // Mark a hit as detected
    lastHitTime = millis(); // Record the time of the hit
}

void highlightReceiver()
{
    // Reset all receivers to white initially
    setColor(HIGH, HIGH, HIGH);

    // Check if the correct receiver is hit
    if ((receivedData.receiverChoice == 1 && digitalRead(reciever1) == 0) ||
        (receivedData.receiverChoice == 2 && digitalRead(reciever2) == 0) ||
        (receivedData.receiverChoice == 3 && digitalRead(reciever3) == 0) ||
        (receivedData.receiverChoice == 4 && digitalRead(reciever4) == 0))
    {
        // Correct zone hit: Flash all LEDs green for a noticeable time
        setColor(LOW, HIGH, LOW);   // Green
        delay(200);                 // Keep the green flash visible
        setColor(HIGH, HIGH, HIGH); // Reset to white
    }
    else if (digitalRead(reciever1) == 0 || digitalRead(reciever2) == 0 ||
             digitalRead(reciever3) == 0 || digitalRead(reciever4) == 0)
    {
        // Wrong zone hit: Flash all LEDs red for a noticeable time
        setColor(HIGH, LOW, LOW);   // Red
        delay(200);                 // Keep the red flash visible
        setColor(HIGH, HIGH, HIGH); // Reset to white
    }
    else
    {
        // Highlight the specified receiver (no hit detected)
        switch (receivedData.receiverChoice)
        {
        case 1:
            digitalWrite(led1_R, LOW);
            digitalWrite(led1_G, HIGH);
            digitalWrite(led1_B, LOW); // Receiver 1: Green
            break;
        case 2:
            digitalWrite(led2_R, LOW);
            digitalWrite(led2_G, HIGH);
            digitalWrite(led2_B, LOW); // Receiver 2: Green
            break;
        case 3:
            digitalWrite(led3_R, LOW);
            digitalWrite(led3_G, HIGH);
            digitalWrite(led3_B, LOW); // Receiver 3: Green
            break;
        case 4:
            digitalWrite(led4_R, LOW);
            digitalWrite(led4_G, HIGH);
            digitalWrite(led4_B, LOW); // Receiver 4: Green
            break;
        default:
            // No specific receiver highlighted
            setColor(HIGH, HIGH, HIGH);
            break;
        }
    }
}

int Originalhealth = 100;
int Health = 100;
int LastHealth = -1; // Initialize to an impossible value

void updateHealthinfo()
{
    if (myData.id == 1)
    {
        if (receivedData.originalHealth1 > 0)
        {
            Health = receivedData.health1;
            Originalhealth = receivedData.originalHealth1;
        }
        else
        {
            //Serial.println("Error: Received invalid originalHealth1");
        }
    }
    else if (myData.id == 2)
    {
        if (receivedData.originalHealth2 > 0)
        {
            Health = receivedData.health2;
        }
        else
        {
            //Serial.println("Error: Received invalid originalHealth2");
        }
    }
    if (Health == Originalhealth && Health != LastHealth)
    {
        LastHealth = Health;
    }
}

void Reciever::setup()
{
    Serial.begin(115200);
    setupRecieversAndLed();
    espNowSetup();
    setColor(HIGH,HIGH, HIGH);
}

void Reciever::loop()
{
    espNowLoop();

    if (!receivedData.hasGameStarted)
    {
        setColor(HIGH, HIGH, HIGH); // Default LED color when the game hasn't started
        return;
    }
    Serial.println(receivedData.gameMode);

    // Check game mode
    if (strcmp(receivedData.gameMode, "freeforall") == 0)
    {
        // Logic specific to free-for-all
        setVestColor();
        updateHealthinfo();

        // Handle health changes
        if (Health != LastHealth)
        {
            LastHealth = Health;
        }

        // Handle zero health
        while (Health == 0)
        {
            setColor(HIGH, LOW, LOW); // Indicate dead state (red LEDs)
            updateHealthinfo();
        }

        // Check for hits only in free-for-all
        if (millis() - lastHitTime > debounceDelay)
        {
            hitDetected = false;
        }

        if (!hitDetected)
        {
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
                handleHit(led1_R, led1_B, led1_G);
            }
            else if (reciever2Value == 0)
            {
                handleHit(led2_R, led2_B, led2_G);
            }
            else if (reciever3Value == 0)
            {
                handleHit(led3_R, led3_B, led3_G);
            }else if (reciever4Value == 0)
            {
                handleHit(led4_R, led4_B, led4_G);
            }
        }
    }
    else if (strcmp(receivedData.gameMode, "targetpractice") == 0)
    {
        if (millis() - lastHitTime > debounceDelay)
        {
            hitDetected = false;
        }

        if (!hitDetected)
        {
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
                handleHit(led1_R, led1_B, led1_G);
            }
            else if (reciever2Value == 0)
            {
                handleHit(led2_R, led2_B, led2_G);
            }
            else if (reciever3Value == 0)
            {
                handleHit(led3_R, led3_B, led3_G);
            }
            else if (reciever4Value == 0)
            {
                handleHit(led4_R, led4_B, led4_G);
            }
        }
        // Logic specific to target practice
        highlightReceiver(); // Only highlight receiver in target practice mode
    }
    else
    {
        // Default behavior for undefined or new modes
        setColor(HIGH, HIGH, HIGH); // Default to white LEDs
    }
}
