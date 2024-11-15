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
    bool hasGameStarted;
    char color1[10];
    char color2[10]; // Color for Team 2
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
int originalhealth = 0;
int health = 0;

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




void resetbullets ()
{
    int x = 5;
    int y = 10;

    for(int i = 0; i < 12; i++)
    {
        LcdTFT.tft.fillRect(x,y,10,5,ST7735_YELLOW);
        LcdTFT.tft.fillCircle(x+10,y+2.5,2.5,ST7735_YELLOW);
        y += 10;
    }

    LcdTFT.tft.fillRect(76, 50, 20, 10, ST7735_BLACK);
    LcdTFT.tft.setCursor(30,50);
    LcdTFT.tft.printf("Bullets:%d",12);
}

void resethearts ()
{
    int x = 120;
    int y = 10;

    for(int i = 0; i < 10; i++)
    {
        LcdTFT.tft.fillCircle(x-2,y,2,ST7735_RED);
        LcdTFT.tft.fillCircle(x+2,y,2,ST7735_RED);
        LcdTFT.tft.fillTriangle(x-4,y,x+4,y,x,y+5,ST7735_RED);
        y += 12;
    }

    LcdTFT.tft.fillRect(70, 80, 30, 10, ST7735_BLACK);
    LcdTFT.tft.setCursor(30,80);
    LcdTFT.tft.printf("Health:%d",originalhealth);
}

void deletebullets ()
{
    int deleted = 12 - bullets;
    int y2 = 10*deleted;

    LcdTFT.tft.fillRect(5,10,13,y2,ST7735_BLACK);

    LcdTFT.tft.fillRect(76,50,20,10,ST7735_BLACK);
    LcdTFT.tft.setCursor(30,50);
    LcdTFT.tft.printf("Bullets:%d",bullets);
}

void deletehearts ()
{  
    int y2 = (float)120*(1-((float)health/(float)originalhealth));

    LcdTFT.tft.fillRect(116,8,124,y2-3,ST7735_BLACK);

    LcdTFT.tft.fillRect(70,80,30,10,ST7735_BLACK);
    LcdTFT.tft.setCursor(30,80);
    LcdTFT.tft.printf("Health:%d",originalhealth);
}




void Emitter::setup()
{
    Serial.begin(115200);
    EspNowGunReceiverSetup();
    pinMode(trigger, INPUT_PULLUP);
    pinMode(reload, INPUT_PULLUP);
    pinMode(haptic1, OUTPUT);
    pinMode(haptic2, OUTPUT);

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

}

void updatehealthinfo()
{
    if (id == 1)
    {
        health = RecievedData.health1;
        originalhealth = RecievedData.originalHealth1;
    }
    else if (id == 2)
    {
        health = RecievedData.health2;
        originalhealth = RecievedData.originalHealth2;
    }
}

void Emitter::loop()
{
    updatehealthinfo();
    deletehearts();
    if (health <= 0)
    {
        SetColor(HIGH, LOW, LOW);
        while (health <= 0)
        {
            updatehealthinfo();
            deletehearts();
            Serial.println(health);
            // take info from main hub
        }
        resethearts();
    }
    setGunColor();

    // button pressed

    //&& (digitalRead(reload) == HIGH)
    if ((digitalRead(trigger) == LOW) && (bullets != 0) )
    {
       digitalWrite(haptic1, HIGH);
       digitalWrite(haptic2, HIGH);
       Serial.println("3");

       // take away bullets
       bullets--;
       deletebullets();

       // Start PWM signal on the PWM pin at 38kHz
       ledcWriteTone(ledChannel, frequency);
       delay(100); // Send signal for 100ms

       // Stop the PWM signal
       ledcWrite(ledChannel, 0); // Set duty cycle to 0 to stop the signal
       delay(100);               // Debounce delay

       digitalWrite(haptic1, LOW);
       digitalWrite(haptic2, LOW);
    }

    // if reloading
    if (digitalRead(reload) == HIGH)
    {
        Serial.println("4");

        // delay till done reloading
        while(digitalRead(reload) == HIGH)
        {
            updatehealthinfo();
            deletehearts();
            Serial.println("5");
        }
        //update bullets to 12
        bullets = 12;
        resetbullets();
    }
}