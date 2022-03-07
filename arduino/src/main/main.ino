#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int LED_PIN = 6;
int BUTTON_PIN = 3;
int BUTTON_INTERRUPT_DELAY = 20;
int PIXEL_COUNT = 60;

// Neopixel config
int PIXEL_FORMAT = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel *pixels;

// Initialize WiFi network credentials
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, "195.201.132.250", 3000);
int wifiStatus = WL_IDLE_STATUS;

String apiToken = "";
String tempCode = "AEIOU1";

enum GameState {
  UNAUTHORIZED = 0,
  IN_MENU = 1,
  IN_GAME_WAITING = 2,
  IN_GAME_GO = 3
};

GameState gameState;
unsigned int goStartedAt = 0; // This value indicates the timestamp since when the gameState is IN_GAME_GO

static unsigned long buttonLastInterruptedAt = 0;
bool buttonIsBeingPressed = false;

void setup() {
  Serial.begin(9600);
  
  pixels = new Adafruit_NeoPixel(PIXEL_COUNT, LED_PIN, PIXEL_FORMAT);
  pixels->begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressedAction, CHANGE);

  while ( wifiStatus != WL_CONNECTED) {
    Serial.println("Connecting...");
    wifiStatus = WiFi.begin(ssid, pass);
  }

  //Serial.println("Connected to: " + WiFi.SSID() + " as " + WiFi.localIP());
  Serial.println("Connected!");
  gameState = GameState.UNAUTHORIZED;

  // TODO Generate tempCode
}

void setLightColor(int r, int g, int b) {
  pixels->clear();
  for(int i = 0 ; i < PIXEL_COUNT; i++) {
    pixels->setPixelColor(i, pixels->Color(r, g, b));
  }
  pixels->show();
}

void loop() {
  switch(gameState) {

    case GameState.UNAUTHORIZED:
      // The user isn't logged in yet. Show yellow light
      setLightColor(150, 150, 25);

      delay(1000);

      // Check if there is an identity linked to the temp code.
      client.get("/identity?code=" + tempCode);
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();

      if(statusCode == 200) {
        Serial.println(response);
        apiToken = response;
        gameState = GameState.IN_MENU;
      }
      break;
      
    case GameState.IN_MENU:
      setLightColor(0, 0, 150);
      break;
      
    case GameState.IN_GAME_WAITING:
      setLightColor(150, 0, 0);

      delay(random(5000, 12000));
      
      goStartedAt = millis();
      gameState = GameState.IN_GAME_GO;
      break;
      
    case GameState.IN_GAME_GO:
      setLightColor(0, 150, 0);
      break;
  }
}


void buttonPressedAction() {

  // We have to debounce the signals, to ensure that we only accept 1 signal per button press.
  unsigned long currentTime = millis();
  if (currentTime - buttonLastInterruptedAt < BUTTON_INTERRUPT_DELAY) {
    return;
  }

  buttonLastInterruptedAt = currentTime;
  if(buttonIsBeingPressed) {
    buttonIsBeingPressed = false;
    return;
  }
  
  buttonIsBeingPressed = true;

  switch(gameState) {
    case GameState.IN_MENU:
      // User is in menu and starts game by pressing the button.
      gameState = GameState.IN_GAME_WAITING;
      break;
    
    case GameState.IN_GAME_WAITING:
      // User ingame and pressed too early - This doesn't work that way, since the main thread is blocked by the "delay(random....) above"
      gameState = GameState.IN_MENU;
      break;
    
    case GameState.IN_GAME_GO:
      gameState = GameState.IN_MENU;

      int reactionTime = currentTime - goStartedAt;
      Serial.println(reactionTime);

      String postData = "{\"score\": " + String(reactionTime) + "}";
      client.beginRequest();
      client.post("/games");
      client.sendHeader("Content-Type", "application/json");
      client.sendHeader("Content-Length", postData.length());
      client.sendHeader("Authorization", "Bearer " + apiToken);
      client.beginBody();
      client.print(postData);
      client.endRequest();
      break;
      
    default:
      break;
  }
}
