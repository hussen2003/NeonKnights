#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <esp_now.h>
#include "mainHub.h"
#include "SPIFFS.h"

MainHub::MainHub() {};

// List of vests' MAC Addresses
uint8_t vestMacAddresses[4][6] = {
    {0xD0, 0xEF, 0x76, 0x14, 0xf6, 0x1C},  // First vest MAC address
    {0xD0, 0xEF, 0x76, 0x15, 0x37, 0x84},  // Second vest MAC address (example)
    {0xD0, 0xEF, 0x76, 0x14, 0xfa, 0x10},  // first gun mac
    {0xD0, 0xEF, 0x76, 0x15, 0x62, 0xBC}}; // second gun MAC address

typedef struct input_data
{
    bool hasGameStarted;
    char color1[10]; // Color for Team 1
    char color2[10]; // Color for Team 2
    int health1;
    int originalHealth1;
    int health2;
    int originalHealth2;
} input_data;

input_data gameData;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message
{
    int id; // 1: team1; 2:team2
            //   int kills;
            //   int Health;
            //   int deaths;
    int reciever1Value;
    int reciever2Value;
    int reciever3Value;
    int reciever4Value;
} struct_message;

extern struct_message myData;

esp_now_peer_info_t peerInfoHub[4]; // Array to hold peer information for multiple vests

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

// Create an array with all the structures
struct_message boardsStruct[2] = {board1, board2};

// callback function that will be executed when data is received
void OnDataRecvHub(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    char macStr[18];
    // Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    // Serial.println(macStr);

    memcpy(&myData, incomingData, sizeof(myData));
    // Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

    // Update the structures with the new incoming data
    if (myData.id == 1)
    {
        // Update Player 1's board
        board1.reciever1Value = myData.reciever1Value;
        board1.reciever2Value = myData.reciever2Value;
        board1.reciever3Value = myData.reciever3Value;
        board1.reciever4Value = myData.reciever4Value;
    }
    else if (myData.id == 2)
    {
        // Update Player 2's board
        board2.reciever1Value = myData.reciever1Value;
        board2.reciever2Value = myData.reciever2Value;
        board2.reciever3Value = myData.reciever3Value;
        board2.reciever4Value = myData.reciever4Value;
    }
}

// Callback function when data is sent
void OnDataSentHub(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Replace with your network credentials
const char *Ssid = "TEST";
const char *Password = "neonknights";

// Create an AsyncWebServer object on port 80
AsyncWebServer Server(80);
AsyncWebSocket ws("/ws");

String Gamemode = "";

int Player1Health;
int Player2Health;
int Player1Damage;
int Player2Damage;
int Player1InitialHealth; // Initial health value for Player 1
int Player2InitialHealth; // Initial health value for Player 2
String Team1Name = "";
String Team1Color = "";
String Player1Name = "";

String Team2Name = "";
String Team2Color = "";
String Player2Name = "";

int Player1Kills = 0;
int Player1Deaths = 0;
double Player1kd = 0.0;

int Player2Kills = 0;
int Player2Deaths = 0;
double Player2kd = 0.0;

String getteamcolor(int team)
{
    int red = 0;
    int green = 0;
    int blue = 0;

    if (team == 1)
    {
        // Set colors
        if (Team1Color == "blue")
        {
            int blue = 255;
        }
        else if (Team1Color == "green")
        {
            int green = 255;
        }
        else if (Team1Color == "yellow")
        {
            int red = 255;
            int green = 255;
        }
        else if (Team1Color == "purple")
        {
            int red = 255;
            int blue = 255;
        }
        else if (Team1Color == "cyan")
        {
            int green = 255;
            int blue = 255;
        }
    }
    else if (team == 2)
    {
        // Set colors
        if (Team2Color == "blue")
        {
            int blue = 255;
        }
        else if (Team2Color == "green")
        {
            int green = 255;
        }
        else if (Team2Color == "yellow")
        {
            int red = 255;
            int green = 255;
        }
        else if (Team2Color == "purple")
        {
            int red = 255;
            int blue = 255;
        }
        else if (Team2Color == "cyan")
        {
            int green = 255;
            int blue = 255;
        }
    }
    return "rgba(" + String(red) + ", " + String(green) + ", " + String(blue) + ", 0.6)";
}

// Function to return HTML content
String getHTML()
{
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Neon Knights Game Setup</title>
            <style>
                html, body {
                    height: 100%;
                    margin: 0;
                    padding: 0;
                    font-family: Arial, sans-serif;
                    text-align: center;
                    display: flex;
                    align-items: center;
                    justify-content: center;
                    color: white;
                }
                body {
                    background-image: url('Laser.jpg'); /* Replace with the path to your image */
                    background-size: cover;
                    background-position: center;
                    background-repeat: no-repeat;
                }
                h1 {
                    font-size: 50px;
                    height: 100%;
                    margin-top: 0;
                    margin-bottom: 20px;
                    font-family: Arial, sans-serif;
                    text-align: center;
                    display: flex;
                    align-items: center;
                    justify-content: center;
                    color: white;
                }
                .gamesetup {
                    background-color: rgba(0, 0, 0, 0.6); /* Translucent black */
                    padding: 30px;
                    border-radius: 10px;
                    width: 100%;
                    max-width: 3500px;
                    box-sizing: border-box;
                    backdrop-filter: blur(10px); /* Applies blur to the background */
                    box-shadow: 0px 4px 20px rgba(255, 255, 255, 0.3); /* Optional shadow for depth */
                    color: white;
                }
                .scoreboard {
                
                    background-color: rgba(0, 0, 0, 0.6); /* Translucent black */
                    padding: 30px;
                    border-radius: 10px;
                    width: 100%;
                    max-width: 3500px;
                    box-sizing: border-box;
                    backdrop-filter: blur(10px); /* Applies blur to the background */
                    box-shadow: 0px 4px 20px rgba(255, 255, 255, 0.3); /* Optional shadow for depth */
                    color: white;
                }
                h2 {
                    margin: 0;
                    font-size: 30px;
                    font-family: Arial, sans-serif;
                    text-align: center;
                    display: flex;
                    color: white;
                }
                .teambox {
                    padding: 20px;
                    max-width: 1000px;
                    box-sizing: border-box;
                    display: flex;
                    flex-direction: row;  /* Keep elements in a row */
                    justify-content: space-evenly;  /* Center-align the teams containers */
                    align-items: center;
                    width: 100%;
                    gap: 50px;  /* Add gap between the teams containers */
                }
                .team1{
                    background-color: )rawliteral" +
                  getteamcolor(1) + R"rawliteral(;
                    padding: 20px; 
                    border-radius: 10px;
                    width: 100%;
                    backdrop-filter: blur(10px); /* Applies blur to the background */
                    color: white;
                    display: flex;           /* Make it a flex container */
                    flex-direction: column;  /* Arrange children vertically */
                    align-items: center;     /* Center align items horizontally */
                }
                .team2{
                    background-color: )rawliteral" +
                  getteamcolor(2) + R"rawliteral(;
                    padding: 20px; 
                    border-radius: 10px;
                    width: 100%;
                    backdrop-filter: blur(10px); /* Applies blur to the background */
                    color: white;
                    display: flex;           /* Make it a flex container */
                    flex-direction: column;  /* Arrange children vertically */
                    align-items: center;     /* Center align items horizontally */
                }
                .teamitem{
                    display: flex;
                    flex-direction: row;  /* Stack label and input/select side by side */
                    align-items: flex-start; /* Left-align items */
                    justify-content: flex-start; /* Align items to the left */
                    width: 100%;
                    margin-top: 15px;
                    margin-bottom: 0px;
                }
                button {
                    font-size: 20px;
                    padding: 10px;
                    margin: 0px;
                    border-radius: 10px;
                }
                .hidden {
                    display: none;
                }
                .statbox{
                    display: flex;
                    flex-direction: row;  /* Stack label and input/select side by side */
                    align-items: flex-start; /* Left-align items */
                    justify-content: flex-start; /* Align items to the left */
                    width: 100%;
                    margin-top: 15px;
                    margin-bottom: 0px;
                }
                .stat {
                    color: white;
                    border-color: 30px, white;
                    min-width: 100px;
                    max-width: 100px;
                }
            </style>
            <script>
                let socket = new WebSocket("ws://" + window.location.hostname + "/ws");

                socket.onmessage = function(event) {
                    let data = JSON.parse(event.data);
                    
                    // Update team and player elements based on received data
                    document.getElementById("t1n").textContent = data.t1n;
                    document.getElementById("p1n").textContent = data.p1n;
                    document.getElementById("player1score").textContent = data.player1score;
                    document.getElementById("player1kills").textContent = data.player1kills;
                    document.getElementById("player1deaths").textContent = data.player1deaths;

                    document.getElementById("t2n").textContent = data.t2n;
                    document.getElementById("p2n").textContent = data.p2n;
                    document.getElementById("player2score").textContent = data.player2score;
                    document.getElementById("player2kills").textContent = data.player2kills;
                    document.getElementById("player2deaths").textContent = data.player2deaths;
                };

                socket.onclose = function(event) {
                    console.log("WebSocket closed");
                };
                
                window.onload = function() {
                    document.querySelector('.scoreboard').classList.add('hidden');
                };

                function confirmgame() {
                    document.querySelector('.gamesetup').classList.add('hidden');
                    document.querySelector('.scoreboard').classList.remove('hidden');

                    const formData = new FormData();
                    formData.append("gamemode", document.getElementById("gamemode").value);
                    formData.append("team1name", document.getElementById("team1name").value);
                    formData.append("team1color", document.getElementById("team1color").value);
                    formData.append("player1name", document.getElementById("player1name").value);
                    formData.append("player1health", document.getElementById("player1health").value);
                    formData.append("player1damage", document.getElementById("player1damage").value);
                    formData.append("team2name", document.getElementById("team2name").value);
                    formData.append("team2color", document.getElementById("team2color").value);
                    formData.append("player2name", document.getElementById("player2name").value);
                    formData.append("player2health", document.getElementById("player2health").value);
                    formData.append("player2damage", document.getElementById("player2damage").value);

                    fetch("/submit", {
                        method: "POST",
                        body: formData
                    }).then(response => response.text()).then(data => console.log("Data submitted:", data));
                }

                function endgame() {
                    document.querySelector('.gamesetup').classList.remove('hidden');
                    document.querySelector('.scoreboard').classList.add('hidden');

                    fetch("/endgame", {
                        method: "POST"
                    }).then(response => response.text()).then(data => console.log("Game ended:", data));
                }
            </script>
        </head>
        <body>
            <div class="gamesetup">
                <h1>Neon Knights Laser Tag</h1>

                <label for="gamemode" style="font-size: 24px;">Select Game Mode: </label>
                <select id="gamemode">
                    <option value="freeforall">Free For All</option>
                    <option value="targetpractice">Target Practice</option>
                </select>

                <div class="teambox">
                    <div class="team1">
                        <h2>Team 1</h2>

                        <div class="teamitem">
                            <label for="team1name">Enter Team Name: </label>
                            <input type="text" id="team1name" placeholder="Team Name">
                        </div>
                        <div class="teamitem">
                            <label for="team1color">Select Team Color: </label>
                            <select id="team1color">
                                <option value="blue">Blue</option>
                                <option value="green">Green</option>
                                <option value="yellow">Yellow</option>
                                <option value="purple">Purple</option>
                                <option value="cyan">Cyan</option>
                            </select>
                        </div>

                        <div class="teamitem">
                            <label for="player1name">Enter Player Name: </label>
                            <input type="text" id="player1name" placeholder="Player Name">
                        </div>
                        <div class="teamitem">
                            <label for="player1health">Enter Players Health: </label>
                            <input type="text" id="player1health" placeholder="Player Health">
                        </div>
                        <div class="teamitem">
                            <label for="player1damage">Enter Player Damage: </label>
                            <input type="text" id="player1damage" placeholder="Player Damage">
                        </div>
                    </div>

                    <div class="team2">
                        <h2>Team 2</h2>

                        <div class="teamitem">
                            <label for="team2name">Enter Team Name: </label>
                            <input type="text" id="team2name" placeholder="Team Name">
                        </div>
                        <div class="teamitem">
                            <label for="team2color">Select Team Color: </label>
                            <select id="team2color">
                                <option value="blue">Blue</option>
                                <option value="green">Green</option>
                                <option value="yellow">Yellow</option>
                                <option value="purple">Purple</option>
                                <option value="cyan">Cyan</option>
                            </select>
                        </div>

                        <div class="teamitem">
                            <label for="player2name">Enter Player Name: </label>
                            <input type="text" id="player2name" placeholder="Player Name">
                        </div>
                        <div class="teamitem">
                            <label for="player2health">Enter Players Health: </label>
                            <input type="text" id="player2health" placeholder="Player Health">
                        </div>
                        <div class="teamitem">
                            <label for="player2damage">Enter Player Damage: </label>
                            <input type="text" id="player2damage" placeholder="Player Damage">
                        </div>
                    </div>
                </div>

                <button onclick="confirmgame()">Confirm Game Setup</button>
            </div>

            <div class="scoreboard">
    <h1>Neon Knights Laser Tag</h1>
    <div class="teambox">

        <div class="team1" style="background-color: ")rawliteral" + getteamcolor(1) + R"rawliteral(;>
            <div class="stat" id="t1n">
                <!-- Insert Team 1 Name -->
                )rawliteral" +
                  Team1Name + R"rawliteral(
            </div>

            <div class="statbox">
                <div class="stat">
                    PlayerName
                </div>
                <div class="stat">
                    Kills
                </div>
                <div class="stat">
                    Deaths
                </div>
                <div class="stat">
                    K/D
                </div>
            </div>

            <div class="statbox">
                <div class="stat" id="p1n">
                    <!-- Insert Player 1 Name -->
                    )rawliteral" +
                  Player1Name + R"rawliteral(
                </div>
                <div class="stat" id="player1kills">
                    <!-- Insert Player 1 Kills -->
                    )rawliteral" +
                  Player1Kills + R"rawliteral(
                </div>
                <div class="stat" id="player1deaths">
                    <!-- Insert Player 1 Deaths -->
                    )rawliteral" +
                  Player1Deaths + R"rawliteral(
                </div>
                <div class="stat" id="kd">
                    <!-- Insert Player 1 Score -->
                    )rawliteral" +
                  Player1kd + R"rawliteral(
                </div>
            </div>
        </div>

        <div class="team2"  style="background-color: ")rawliteral" + getteamcolor(1) + R"rawliteral(;>
            <div class="stat" id="t2n">
                <!-- Insert Team 2 Name -->
                )rawliteral" +
                  Team2Name + R"rawliteral(
            </div>

            <div class="statbox">
                <div class="stat">
                    PlayerName
                </div>
                <div class="stat">
                    Kills
                </div>
                <div class="stat">
                    Deaths
                </div>
                <div class="stat">
                    Score
                </div>
            </div>

            <div class="statbox">
                <div class="stat"  id="p2n">
                    <!-- Insert Player 2 Name -->
                    )rawliteral" +
                  Player2Name + R"rawliteral(
                </div>
                <div class="stat" id="player2kills">
                    <!-- Insert Player 2 Kills -->
                    )rawliteral" +
                  Player2Kills + R"rawliteral(
                </div>
                <div class="stat" id="player2deaths">
                    <!-- Insert Player 2 Deaths -->
                    )rawliteral" +
                  Player2Deaths + R"rawliteral(
                </div>
                <div class="stat" id="kd">
                    <!-- Insert Player 2 Score -->
                    )rawliteral" +
                  Player2kd + R"rawliteral(
                </div>
            </div>
        </div>
    </div>
    <button onclick="endgame()">End Game</button>
</div>
</body>
</html>

    )rawliteral";
    return html;
}

void UpdateWebpage()
{
    Serial.println("UPDATE WEBPAGE!!!");
    
    String json = "{";
    json += "\"t1n\":\"" + Team1Name + "\",";
    json += "\"p1n\":\"" + Player1Name + "\",";
    json += "\"player1kills\":" + String(Player1Kills) + ",";
    json += "\"player1deaths\":" + String(Player1Deaths) + ",";
    //json += "\"player1kd\":" + String((float)Player1Kills / max(1, Player1Deaths)) + ",";
    json += "\"t2n\":\"" + Team2Name + "\",";
    json += "\"p2n\":\"" + Player2Name + "\",";
    json += "\"player2kills\":" + String(Player2Kills) + ",";
    json += "\"player2deaths\":" + String(Player2Deaths) + ",";
    //json += "\"player2kd\":" + String((float)Player2Kills / max(1, Player2Deaths));
    json += "}";

    Serial.println(json);
    ws.textAll(json);
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.println("WebSocket client connected");
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.println("WebSocket client disconnected");
    }
}

void EspNowSetup()
{
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the callback for receiving data
    esp_now_register_recv_cb(OnDataRecvHub);

    // Add peers (vests) to the list
    for (int i = 0; i < 4; i++)
    {
        memcpy(peerInfoHub[i].peer_addr, vestMacAddresses[i], 6);
        peerInfoHub[i].channel = 0;
        peerInfoHub[i].encrypt = false;

        if (esp_now_add_peer(&peerInfoHub[i]) != ESP_OK)
        {
            Serial.print("Failed to add peer: ");
            for (int j = 0; j < 6; j++)
            {
                Serial.print(vestMacAddresses[i][j], HEX);
                if (j < 5)
                    Serial.print(":");
            }
            Serial.println();
        }
    }

    // Register the callback for sending data
    esp_now_register_send_cb(OnDataSentHub);
}

void EspNowLoop()
{
    // Send board data to each vest
    for (int i = 0; i < 4; i++)
    {
        esp_err_t result = esp_now_send(vestMacAddresses[i], (uint8_t *)&myData, sizeof(myData));

        if (result == ESP_OK)
        {
            // Serial.printf("Board data sent successfully to vest %d\n", i + 1);
        }
        else
        {
            // Serial.printf("Error sending board data to vest %d\n", i + 1);
        }
    }
    delay(100);
}

void sendGameData()
{
    gameData.health1 = Player1Health;
    gameData.health2 = Player2Health;

    for (int i = 0; i < 4; i++) // Iterate through all MAC addresses
    {
        esp_err_t result = esp_now_send(vestMacAddresses[i], (uint8_t *)&gameData, sizeof(gameData));
        if (result == ESP_OK)
        {
            // Serial.print("Game data sent to vest ");
            // Serial.println(i + 1);
        }
        else
        {
            // Serial.print("Error sending to vest ");
            // Serial.println(i + 1);
        }
    }
    delay(100);
}

void MainHub::setup()
{
    Serial.begin(115200);

    // Set up WiFi in Station Mode
    WiFi.mode(WIFI_AP_STA);

    // Static IP configuration
    IPAddress local_IP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_IP, gateway, subnet);

    // Set up the access point
    WiFi.softAP(Ssid, Password);
    delay(5000); // Wait for AP to stabilize
    Serial.println("Access Point started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    Serial.println("Connected clients:");
    Serial.println(WiFi.softAPgetStationNum()); // Number of connected clients
    // Serve HTML page on root URL
    Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/html", getHTML());
    });

    if (!SPIFFS.begin(true))
    {
        Serial.println("Failed to mount SPIFFS");
        return;
    }
    // serve backgroung image
    Server.serveStatic("/Laser.jpg", SPIFFS, "/Laser.jpg");

    // WebSocket setup
    ws.onEvent(onWsEvent);
    Server.addHandler(&ws);

    // Handle form submission
    Server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("gamemode", true)) 
            Gamemode = request->getParam("gamemode", true)->value();
        if (request->hasParam("team1name", true)) 
            Team1Name = request->getParam("team1name", true)->value();
        if (request->hasParam("team1color", true)) 
        {
            Team1Color = request->getParam("team1color", true)->value();
            strcpy(gameData.color1, Team1Color.c_str()); // Store Team 1 color in the global variable
        }
        if (request->hasParam("player1name", true)) 
            Player1Name = request->getParam("player1name", true)->value();
        if (request->hasParam("player1health", true))
        {
            Player1Health = request->getParam("player1health", true)->value().toInt();        // Convert to int
            Player1InitialHealth = request->getParam("player1health", true)->value().toInt(); // Convert to int
            gameData.originalHealth1 = Player1InitialHealth;
        }  
        if (request->hasParam("player1damage", true)) 
            Player1Damage = request->getParam("player1damage", true)->value().toInt();  // Convert to int
        if (request->hasParam("team2name", true)) 
            Team2Name = request->getParam("team2name", true)->value();
        if (request->hasParam("team2color", true)) 
        {
            Team2Color = request->getParam("team2color", true)->value();
            strcpy(gameData.color2, Team2Color.c_str()); // Store Team 2 color in the global variable
        }
        if (request->hasParam("player2name", true)) 
            Player2Name = request->getParam("player2name", true)->value();
        if (request->hasParam("player2health", true)) 
        {
            Player2Health = request->getParam("player2health", true)->value().toInt(); // Convert to int
            Player2InitialHealth = request->getParam("player2health", true)->value().toInt(); // Convert to int
            gameData.originalHealth2 = Player2InitialHealth;
        }
            
        if (request->hasParam("player2damage", true)) 
            Player2Damage = request->getParam("player2damage", true)->value().toInt();  // Convert to int

    
        Serial.println("Game setup confirmed:");
        Serial.print("Gamemode: " + Gamemode);
        Serial.print("Team 1 color: " + Team1Color);
        Serial.print("Team 2 color: " + Team2Color);
        Serial.println();

        // Send a response back to the client
        request->send(200, "text/plain", "Data received");
    });

    Server.on("/endgame", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        gameData.hasGameStarted = false; // Set the game end flag
        Gamemode = "";
        Player1Kills = 0;
        Player1Deaths = 0;
        Player1kd = 0.0;

        Player2Kills = 0;
        Player2Deaths = 0;
        Player2kd = 0.0;
        Serial.println("Game has ended");
        request->send(200, "text/plain", "Game ended"); // Send confirmation response

    });

    // Start server
    Server.begin();
    EspNowSetup();
}

// int hitCounter = 0;           // Tracks successful hits
// int missCounter = 0;          // Tracks misses
// double accuracy = 0.0;        // Tracks the accuracy percentage
// unsigned long timerStart = 0; // Start time for 10-second timer
// int currentTarget = 0;        // Tracks which receiver (1, 2, or 3) is the current target
// bool isTargetHit = false;     // Tracks if the current target has been hit
void updateKD()
{
    if (Player1Deaths != 0)
    {
        Player1kd = Player1Kills / Player1Deaths;
    }
    else
    {
        Player1kd = Player1Kills;
    }

    if (Player2Deaths != 0)
    {
        Player2kd = Player2Kills / Player2Deaths;
    }
    else
    {
        Player2kd = Player2Kills;
    }
}

void MainHub::loop()
{
    // EspNowLoop(); // Handles sending and receiving via ESP-NOW
    if (Gamemode == "freeforall")
    {
        sendGameData(); // Send game data to vests
        gameData.hasGameStarted = true;

        // Handle Player 1's board data (vest 1)
        if (board1.reciever1Value == 0 || board1.reciever2Value == 0 || board1.reciever3Value == 0 || board1.reciever4Value == 0)
        {
            // Player 1 is hit (any receiver value is 0)
            Player1Health -= Player2Damage;
        }

        // Handle Player 2's board data (vest 2)
        if (board2.reciever1Value == 0 || board2.reciever2Value == 0 || board2.reciever3Value == 0 || board2.reciever4Value == 0)
        {
            // Player 2 is hit (any receiver value is 0)
            Player2Health -= Player1Damage;
        }

        // Check if Player 1 has died
        if (Player1Health <= 0)
        {
            Player1Health = 0; // Make sure health doesn't go negative
            Player2Kills++;    // Player 2 gets a kill
            Player1Deaths++;   // Player 1 has died
            // sendColorToVest(1, "white"); // Player 1 is dead, update Player 1's vest color to white
            Team1Color = "red";
            sendGameData();
            UpdateWebpage();
            delay(5000); // Wait for 5 seconds before respawning Player 1
            // updateKD();
            //  Respawn Player 1 by resetting their health
            Player1Health = Player1InitialHealth; // Restore original health for Player 1
            // sendColorToVest(1, "green");          // Change Player 1's vest color back to green
        }

        // Check if Player 2 has died
        if (Player2Health <= 0)
        {
            Player2Health = 0; // Make sure health doesn't go negative
            Player1Kills++;    // Player 1 gets a kill
            Player2Deaths++;   // Player 2 has died
            // updateKD();
            //  sendColorToVest(2, "white"); // Player 2 is dead, update Player 2's vest color to white
            Team2Color = "red";
            sendGameData();
            UpdateWebpage();
            delay(5000); // Wait for 5 seconds before respawning Player 2

            // Respawn Player 2 by resetting their health
            Player2Health = Player2InitialHealth; // Restore original health for Player 2
            // sendColorToVest(2, "green");          // Change Player 2's vest color back to green
        }
        // Update the webpage with the latest stats

        // Debugging: Print received values
        Serial.printf("Vest 1 Reciever 1: %d Vest 2 Reciever 1: %d\n", board1.reciever1Value, board2.reciever1Value);
        Serial.printf("Vest 1 Reciever 2: %d Vest 2 Reciever 2: %d\n", board1.reciever2Value, board2.reciever2Value);
        Serial.printf("Vest 1 Reciever 3: %d Vest 2 Reciever 3: %d\n", board1.reciever3Value, board2.reciever3Value);
        Serial.printf("Vest 1 Reciever 4: %d Vest 2 Reciever 4: %d\n", board1.reciever4Value, board2.reciever4Value);

        Serial.println();
        // Debugging output

        Serial.println("Player 1 Health: " + String(Player1Health));
        Serial.println("Player 2 Health: " + String(Player2Health));
        Serial.println("Player 1 Kills: " + String(Player1Kills));
        Serial.println("Player 1 Deaths: " + String(Player1Deaths));
        Serial.println("Player 2 Kills: " + String(Player2Kills));
        Serial.println("Player 2 Deaths: " + String(Player2Deaths));
    }

    // if (Gamemode == "targetpractice")
    // {

    //     // If a target has not been selected, randomly choose a target (1, 2, or 3)
    //     if (currentTarget == 0 || isTargetHit)
    //     {
    //         currentTarget = random(1, 4); // Randomly select 1, 2, or 3
    //         Serial.println("New target: Receiver " + String(currentTarget));
    //         isTargetHit = false;   // Reset the target hit flag
    //         timerStart = millis(); // Reset the timer for the new target
    //     }

    //     // Check if the randomly selected receiver was hit
    //     if ((currentTarget == 1 && board1.reciever1Value == 0) ||
    //         (currentTarget == 2 && board1.reciever2Value == 0) ||
    //         (currentTarget == 3 && board1.reciever3Value == 0))
    //     {
    //         hitCounter++; // Target was hit
    //         Serial.println("Target hit!");
    //         isTargetHit = true; // Mark the target as hit
    //     }

    //     // Check if the timer has exceeded 10 seconds or another receiver was hit
    //     if (millis() - timerStart >= 10000 ||
    //         (board1.reciever1Value == 0 && currentTarget != 1) ||
    //         (board1.reciever2Value == 0 && currentTarget != 2) ||
    //         (board1.reciever3Value == 0 && currentTarget != 3))
    //     {

    //         if (!isTargetHit)
    //         { // If the target was not hit in time or another receiver was hit
    //             missCounter++;
    //             Serial.println("Target missed!");
    //         }

    //         // After 10 seconds or a miss, reset and pick a new target
    //         currentTarget = 0; // Reset target to select a new one
    //     }

    //     // Calculate accuracy
    //     if (hitCounter + missCounter > 0)
    //     {
    //         accuracy = ((double)hitCounter / (hitCounter + missCounter)) * 100;
    //     }

    //     // Display stats for debugging
    //     Serial.println("Hits: " + String(hitCounter) + " | Misses: " + String(missCounter) + " | Accuracy: " + String(accuracy, 2) + "%");

    //     // Optional: Update webpage with hit/miss stats and accuracy
    //     UpdateWebpage();
    // }
}
