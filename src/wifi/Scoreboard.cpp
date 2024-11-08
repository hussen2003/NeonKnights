#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Scoreboard.h"
#include "SPIFFS.h"

Scoreboard::Scoreboard() {};

// Replace with your network credentials
const char* Ssid     = "NEONKNIGHTS";
const char* Password = "neonknights";

// Create an AsyncWebServer object on port 80
AsyncWebServer Server(80);

String Gamemode = "";

String Team1Name = "";
String Team1Color = "";
String Player1Name = "";
String Player1Health = "";
String Player1Damage = "";

String Team2Name = "";
String Team2Color = "";
String Player2Name = "";
String Player2Health = "";
String Player2Damage = "";

// Function to return HTML content
String getHTML() {
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
                .container {
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
                .team{
                    background-color: rgb(0, 0, 0);
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
            </style>
            <script>
                function confirmgame() {
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
            </script>
        </head>
        <body>
            <div class="container">
                <h1>Neon Knights Laser Tag</h1>

                <label for="gamemode" style="font-size: 24px;">Select Game Mode: </label>
                <select id="gamemode">
                    <option value="deathmatch">Team Deathmatch</option>
                    <option value="elemination">Team Elimination</option>
                    <option value="freeforall">Free For All</option>
                </select>

                <div class="teambox">
                    <div class="team">
                        <h2>Team 1</h2>

                        <div class="teamitem">
                            <label for="team1name">Enter Team Name: </label>
                            <input type="text" id="team1name" placeholder="Team Name">
                        </div>
                        <div class="teamitem">
                            <label for="team1color">Select Team Color: </label>
                            <select id="team1color">
                                <option value="red">Red</option>
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

                    <div class="team">
                        <h2>Team 2</h2>

                        <div class="teamitem">
                            <label for="team2name">Enter Team Name: </label>
                            <input type="text" id="team2name" placeholder="Team Name">
                        </div>
                        <div class="teamitem">
                            <label for="team2color">Select Team Color: </label>
                            <select id="team2color">
                                <option value="blue">Blue</option>
                                <option value="red">Red</option>
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
        </body>
        </html>
    )rawliteral";
    return html;
}


void Scoreboard::setup() {
    // Start the Serial communication
    Serial.begin(115200);

    // Set up the access point
    WiFi.softAP(Ssid, Password);
    Serial.println("Access Point started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    // Serve HTML page on root URL
    Server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getHTML());
    });

    if (!SPIFFS.begin(true))
    {
        Serial.println("Failed to mount SPIFFS");
        return;
    }
    // serve backgroung image
    Server.serveStatic("/Laser.jpg", SPIFFS, "/Laser.jpg");

    // Handle form submission
    Server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("gamemode", true)) Gamemode = request->getParam("gamemode", true)->value();
        if (request->hasParam("team1name", true)) Team1Name = request->getParam("team1name", true)->value();
        if (request->hasParam("team1color", true)) Team1Color = request->getParam("team1color", true)->value();
        if (request->hasParam("player1name", true)) Player1Name = request->getParam("player1name", true)->value();
        if (request->hasParam("player1health", true)) Player1Health = request->getParam("player1health", true)->value();
        if (request->hasParam("player1damage", true)) Player1Damage = request->getParam("player1damage", true)->value();
        if (request->hasParam("team2name", true)) Team2Name = request->getParam("team2name", true)->value();
        if (request->hasParam("team2color", true)) Team2Color = request->getParam("team2color", true)->value();
        if (request->hasParam("player2name", true)) Player2Name = request->getParam("player2name", true)->value();
        if (request->hasParam("player2health", true)) Player2Health = request->getParam("player2health", true)->value();
        if (request->hasParam("player2damage", true)) Player2Damage = request->getParam("player2damage", true)->value();

        Serial.println("Game setup confirmed:");
        request->send(200, "text/plain", "Data received");
    });

    // Start server
    Server.begin();
}

void Scoreboard::loop()
{
    // Nothing to do here
    Serial.println("Gamemode: " + Gamemode);
    Serial.println("Team 1 - Name: " + Team1Name + ", Color: " + Team1Color + ", Player Name: " + Player1Name + ", Health: " + Player1Health + ", Damage: " + Player1Damage);
    Serial.println("Team 2 - Name: " + Team2Name + ", Color: " + Team2Color + ", Player Name: " + Player2Name + ", Health: " + Player2Health + ", Damage: " + Player2Damage);
    delay(5000);
}