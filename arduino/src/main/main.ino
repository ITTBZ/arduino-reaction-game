#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <Wire.h>
#include "rgb_lcd.h";

rgb_lcd lcd;

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
String tempCode = "";

enum GameState {
  UNAUTHORIZED = 0,
  IN_MENU = 1,
  IN_GAME_WAITING = 2,
  IN_GAME_GO = 3
};

GameState gameState = UNAUTHORIZED;
unsigned int goStartedAt = 0; // This value indicates the timestamp since when the gameState is IN_GAME_GO

static unsigned long buttonLastInterruptedAt = 0;
bool buttonIsBeingPressed = false;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 1);
  lcd.setRGB(255, 255, 255);
  lcd.print("Connecting...");
  
  pixels = new Adafruit_NeoPixel(PIXEL_COUNT, LED_PIN, PIXEL_FORMAT);
  pixels->begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressedAction, CHANGE);

  while ( wifiStatus != WL_CONNECTED) {
    Serial.println("Connecting...");
    wifiStatus = WiFi.begin(ssid, pass);
  }

  Serial.print("Connected to " + String(WiFi.SSID()) + " as ");
  Serial.println(WiFi.localIP());

  randomSeed(analogRead(0)); // random values fix
  tempCode = generateTempCode(6);
  lcd.clear();
  lcd.print("Code: "+ tempCode);
}

void loop() {
  switch(gameState) {

    case UNAUTHORIZED: {
      // The user isn't logged in yet. Show yellow light
      setLightColor(150, 150, 25);

      delay(1000);

      // Check if there is an identity linked to the temp code.
      client.get("/identity?code=" + tempCode);
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();

      if(statusCode == 200) {
        apiToken = response;
        gameState = IN_MENU;
        lcd.clear();
        lcd.print("Logged in!");
      }
      break;
    }
      
    case IN_MENU: {
      setLightColor(0, 0, 150);
      break;
    }
      
    case IN_GAME_WAITING: {
      setLightColor(150, 0, 0);

      delay(random(5000, 12000));
      
      goStartedAt = millis();
      gameState = IN_GAME_GO;
      break;
    }
      
    case IN_GAME_GO: {
      setLightColor(0, 150, 0);
      break;
    }
  }
}

unsigned long betterrandom(unsigned long minVal, unsigned long maxVal) {
  return minVal + random() % (maxVal - minVal);
}

String generateTempCode(int stringLength) {
  String letters[]= {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
  String randString = "";
  for(int i = 0; i < stringLength; i++) {
    int randVal = betterrandom(0, 36);
    randString = randString + letters[randVal];
  }
  return randString;
}

void setLightColor(int r, int g, int b) {
  pixels->clear();
  for(int i = 0 ; i < PIXEL_COUNT; i++) {
    pixels->setPixelColor(i, pixels->Color(r, g, b));
  }
  pixels->show();
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
    case IN_MENU: {
      // User is in menu and starts game by pressing the button.
      gameState = IN_GAME_WAITING;
      break;
    }
    
    /*case IN_GAME_WAITING: {
      // User ingame and pressed too early - This doesn't work that way, since the main thread is blocked by the "delay(random....) above"
      gameState = IN_MENU;
      break;
    }*/
    
    case IN_GAME_GO: {
      gameState = IN_MENU;

      int reactionTime = currentTime - goStartedAt;

      String postData = "{\"score\": " + String(reactionTime) + "}";
      client.beginRequest();
      client.post("/games");
      client.sendHeader("Content-Type", "application/json");
      client.sendHeader("Content-Length", postData.length());
      client.sendHeader("Authorization", "Bearer " + apiToken);
      client.beginBody();
      client.print(postData);
      client.endRequest();

      //lcd.print("Score: " + String(reactionTime));
      break;
    }
      
    default: {
      break;
    }
  }
}
