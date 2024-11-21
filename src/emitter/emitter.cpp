#include "Arduino.h"
#include "emitter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <./LCD_Screen/LCD_TFT.h>
#include <esp_now.h>
#include <WiFi.h>

int id = 1;

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

input_data RecievedData;

esp_now_peer_info_t peerGunInfo;

void OnGunDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    memcpy(&RecievedData, incomingData, sizeof(RecievedData));
    Serial.println("RECIEVED DATA FROM MAIN HUB");
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Game Started: ");
    Serial.println(RecievedData.hasGameStarted ? "Yes" : "No");
    Serial.print("Color 1: ");
    Serial.println(RecievedData.color1);
    Serial.print("Color 2: ");
    Serial.println(RecievedData.color2);
    Serial.print("Health1: ");
    Serial.println(RecievedData.health1);
    Serial.print("OriginalHealth1: ");
    Serial.println(RecievedData.originalHealth1);
    Serial.print("Health2: ");
    Serial.println(RecievedData.health2);
    Serial.print("OriginalHealth2: ");
    Serial.println(RecievedData.originalHealth2);
}

LCD_TFT LcdTFT;

Emitter::Emitter() {};
const int trigger = 32;
const int reload = 33;
const int led_R = 27;
const int led_G = 14;
const int led_B = 13;
const int haptic1 = 26;
const int haptic2 = 4;

int bullets = 12;
int originalhealth = 100;
int health = 100;

const int pwmPin = 25;
const int frequency = 38000; // 38kHz frequency
const int ledChannel = 0;    // Channel 0 for LEDC PWM
const int resolution = 8;    // 8-bit resolution (0-255)

void SetColor(int red, int green, int blue)
{
    // Set all LEDs to the specified color
    digitalWrite(led_R, red);
    digitalWrite(led_G, green);
    digitalWrite(led_B, blue);
}

void setGunColor()
{
    if (RecievedData.hasGameStarted)
    {
        // Check which ID we are dealing with
        if (id == 1 && strlen(RecievedData.color1) > 0)
        {
            // Set color based on RecievedData.color1
            if (strcmp(RecievedData.color1, "blue") == 0)
            {
                SetColor(LOW, LOW, HIGH); // Set LEDs to blue
            }
            else if (strcmp(RecievedData.color1, "green") == 0)
            {
                SetColor(LOW, HIGH, LOW); // Set LEDs to green
            }
            else if (strcmp(RecievedData.color1, "yellow") == 0)
            {
                SetColor(HIGH, HIGH, LOW); // Set LEDs to yellow
            }
            else if (strcmp(RecievedData.color1, "purple") == 0)
            {
                SetColor(HIGH, LOW, HIGH); // Set LEDs to purple
            }
            else if (strcmp(RecievedData.color1, "cyan") == 0)
            {
                SetColor(LOW, HIGH, HIGH); // Set LEDs to cyan
            }
        }
        else if (id == 2 && strlen(RecievedData.color2) > 0)
        {
            // Set color based on RecievedData.color2
            if (strcmp(RecievedData.color2, "blue") == 0)
            {
                SetColor(LOW, LOW, HIGH); // Set LEDs to blue
            }
            else if (strcmp(RecievedData.color2, "green") == 0)
            {
                SetColor(LOW, HIGH, LOW); // Set LEDs to green
            }
            else if (strcmp(RecievedData.color2, "yellow") == 0)
            {
                SetColor(HIGH, HIGH, LOW); // Set LEDs to yellow
            }
            else if (strcmp(RecievedData.color2, "purple") == 0)
            {
                SetColor(HIGH, LOW, HIGH); // Set LEDs to purple
            }
            else if (strcmp(RecievedData.color2, "cyan") == 0)
            {
                SetColor(LOW, HIGH, HIGH); // Set LEDs to cyan
            }
        }
    }
}

void EspNowGunReceiverSetup()
{
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_AP_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    Serial.println("ESP-NOW started");

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnGunDataRecv));
}

int lastHealth = -1; // Initialize to an impossible value

void resetbullets()
{
    int x = 5;
    int y = 10;

    // Draw all bullets
    for (int i = 0; i < 12; i++)
    {
        LcdTFT.tft.fillRect(x, y, 10, 5, ST7735_YELLOW);            // Bullet body
        LcdTFT.tft.fillCircle(x + 10, y + 2.5, 2.5, ST7735_YELLOW); // Bullet head
        y += 10;
    }

    // Draw static label for bullets
    LcdTFT.tft.fillRect(76, 50, 20, 10, ST7735_BLACK); // Clear old count
    LcdTFT.tft.setCursor(30, 50);
    LcdTFT.tft.setTextColor(ST7735_WHITE);
    LcdTFT.tft.printf("Bullets: %d", bullets);
}

void resethearts()
{
    int x = 120;                            // Fixed x-coordinate for hearts
    int y = 10;                             // Starting y-coordinate
    int heartsToDraw = originalhealth / 10; // Calculate the total number of hearts

    // Draw the required number of hearts
    for (int i = 0; i < heartsToDraw; i++)
    {
        int yPosition = y + (i * 12);                                                              // Position each heart vertically
        LcdTFT.tft.fillCircle(x - 2, yPosition, 2, ST7735_RED);                                    // Left circle of heart
        LcdTFT.tft.fillCircle(x + 2, yPosition, 2, ST7735_RED);                                    // Right circle of heart
        LcdTFT.tft.fillTriangle(x - 4, yPosition, x + 4, yPosition, x, yPosition + 5, ST7735_RED); // Heart triangle
    }

    // Update the health display
    LcdTFT.tft.fillRect(70, 80, 40, 10, ST7735_BLACK); // Clear old health value area
    LcdTFT.tft.setCursor(30, 80);
    LcdTFT.tft.setTextColor(ST7735_WHITE);
    LcdTFT.tft.printf("Health:%d", originalhealth);
}

void deletebullets()
{
    int deleted = 12 - bullets;
    int y = 10 * deleted;

    // Clear only the bullets that were "deleted"
    LcdTFT.tft.fillRect(5, 10, 13, y, ST7735_BLACK);

    // Update the bullet count
    LcdTFT.tft.fillRect(76, 50, 20, 10, ST7735_BLACK); // Clear old count
    LcdTFT.tft.setCursor(30, 50);
    LcdTFT.tft.setTextColor(ST7735_WHITE);
    LcdTFT.tft.printf("Bullets: %d", bullets);
}

void deletehearts()
{
    if (originalhealth == 0)
    {
        //Serial.println("Error: originalhealth is 0!");
        return; // Prevent division by zero
    }

    // Calculate the number of hearts to be "removed" based on health percentage
    int heartsToDelete = 10 - (health * 10) / originalhealth; // Calculate how many hearts to delete
    int x = 120;                                              // x-coordinate for hearts
    int y = 10;                                               // Starting y-coordinate for hearts

    // Delete the required number of hearts
    for (int i = 0; i < heartsToDelete; i++)
    {
        int yPosition = y + (i * 12);                                   // Position for each heart
        LcdTFT.tft.fillRect(x - 4, yPosition - 2, 9, 10, ST7735_BLACK); // Clear the heart area
    }

    // Update the health display
    LcdTFT.tft.fillRect(70, 80, 40, 10, ST7735_BLACK); // Clear old health value area
    LcdTFT.tft.setCursor(30, 80);
    LcdTFT.tft.setTextColor(ST7735_WHITE);
    LcdTFT.tft.printf("Health:%d", health);
}

void Emitter::setup()
{
    Serial.begin(115200);
    EspNowGunReceiverSetup();
    pinMode(trigger, INPUT_PULLUP);
    pinMode(reload, INPUT_PULLUP);
    pinMode(haptic1, OUTPUT);
    pinMode(haptic2, OUTPUT);
    pinMode(led_R, OUTPUT);
    pinMode(led_G, OUTPUT);
    pinMode(led_B, OUTPUT);

    // Configure LEDC for PWM generation (Channel 0, frequency 38kHz, 8-bit resolution)
    ledcSetup(ledChannel, frequency, resolution);

    // Attach PWM pin to the LEDC channel
    ledcAttachPin(pwmPin, ledChannel);

    LcdTFT.setup();
    LcdTFT.tft.fillScreen(ST7735_BLACK);
    LcdTFT.tft.printf("     Neon Knights");
    LcdTFT.tft.printf("          Laser Tag");

    resethearts();
    resetbullets();
    SetColor(HIGH, HIGH, HIGH);
}

void updatehealthinfo()
{
    if (id == 1)
    {
        if (RecievedData.originalHealth1 > 0)
        {
            health = RecievedData.health1;
            originalhealth = RecievedData.originalHealth1;
        }
        else
        {
            //Serial.println("Error: Received invalid originalHealth1");
        }
    }
    else if (id == 2)
    {
        if (RecievedData.originalHealth2 > 0)
        {
            health = RecievedData.health2;
            originalhealth = RecievedData.originalHealth2;
        }
        else
        {
            //Serial.println("Error: Received invalid originalHealth2");
        }
    }
    if (health == originalhealth && health != lastHealth)
    {
        resethearts();
        lastHealth = health;
    }
}

unsigned long lastReloadTime = 0;
const unsigned long debounceDelay = 200; // 200ms debounce time

void Emitter::loop()
{
    Serial.printf("Color: %s \n", RecievedData.color2);
    if (!RecievedData.hasGameStarted)
    {
        SetColor(HIGH, HIGH, HIGH); // Default LED color when the game hasn't started
        return;
    }
    setGunColor();
    updatehealthinfo();

    // Only update the display when health changes
    if (health != lastHealth)
    {
        deletehearts();
        lastHealth = health; // Update the last health value
    }
    while (health == 0)
    {
        SetColor(HIGH, LOW, LOW);
        updatehealthinfo();
    }
    // Button press logic
    if ((digitalRead(trigger) == LOW) && (bullets != 0) && digitalRead(reload) == LOW)
    {
        Serial.println("TRIGGER PRESSED");
        digitalWrite(haptic1, HIGH);
        digitalWrite(haptic2, HIGH);

        bullets--;
        deletebullets();

        ledcWriteTone(ledChannel, frequency);
        delay(100); // Send signal for 100ms
        ledcWrite(ledChannel, 0);
        delay(100);
        Serial.println("EMITTER SHOT");

        digitalWrite(haptic1, LOW);
        digitalWrite(haptic2, LOW);
    }

    // Reload logic
    if (digitalRead(reload) == HIGH && millis() - lastReloadTime > debounceDelay)
    {
        lastReloadTime = millis(); // Update the last reload time
        bullets = 12;
        resetbullets();
    }
}
