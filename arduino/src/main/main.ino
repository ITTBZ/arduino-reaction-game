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
String tempCode = "";

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

  Serial.println("Connected to: " + WiFi.SSID() + " as " + WiFi.localIP());

  // TODO Generate tempCode
}

void setLightColor(int r, int g, int b) {
  pixels->clear();
  
  for(int i = 0 ; i < PIXEL_COUNT; i++) {
    pixels->setPixelColor(i, pixels->Color(r, g, b));
  }

  pixels->show();
}

enum GameState {
  loggedOut,
  idle,
  btnNotReady,
  btnReady
};
GameState game_state = loggedOut;

unsigned int reaction_accepted_since = 0; // This value inicates since when the leds are glowing green and the program accepts an input

void loop() {
  switch(game_state) {
    case loggedOut:
      setLightColor(150, 150, 25);
      delay(1000);
      client.get("/identity?code=" + tempCode);
    
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();

      if(statusCode == 200) {
        apiToken = response;
        game_state = idle;
        Serial.println(response);
      }
      break;
    case idle:
      setLightColor(0, 0, 150);
      break;
    case btnNotReady:
      setLightColor(150, 0, 0);
      delay(random(5000, 12000));
      game_state = btnReady;
      reaction_accepted_since = millis();
      break;
    case btnReady:
      setLightColor(0, 150, 0);
      break;
  }
}

static unsigned long last_interrupt_time = 0;
bool is_button_pressed = false;
void buttonPressedAction() {

  // We have to debounce the signals, to ensure that we only accept 1 signal per button press.
  unsigned long current_time = millis();
  if (current_time - last_interrupt_time < BUTTON_INTERRUPT_DELAY) {
    return;
  }

  last_interrupt_time = current_time;

  if(is_button_pressed) {
    is_button_pressed = false;
    return;
  }
  
  is_button_pressed = true;

  // Now we can continue with the normal code execution
  switch(game_state) {
    case idle:
      game_state = btnNotReady;
      break;
    
    case btnNotReady:
      // User pressed too early - This doesn't work that way, since the main thread is blocked by the "delay(random....)"
      game_state = idle;
      break;
    
    case btnReady:
      game_state = idle;
      int reaction_time = current_time - reaction_accepted_since;
      Serial.println(reaction_time);

      String postData = "{\"score\": " + reaction_time + "}";
      client.beginRequest();
      client.post("/games");
      client.sendHeader("Content-Type", "application/json");
      client.sendHeader("Content-Length", postData.length());
      client.sendHeader("Authorization", "Bearer " + apiToken);
      client.beginBody();
      client.print(postData);
      client.endRequest();
      // POST REQUEST
      break;
  }
}