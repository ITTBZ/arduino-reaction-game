#include <SPI.h>
#include <WiFiNINA.h>


// -----[ DEFINE PINS ]-----
int LED_PIN = 6;
int BUTTON_PIN = 3;

//-----[ DEFINE HARDWARE SPECIFIC ]-----//
int BUTTON_INTERRUPT_DELAY = 20;
int PIXEL_COUNT = 60;

//-----[ INIT NEOPIXEL ]-----//
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//-----[ INIT WIFI ]-----//


#include "arduino_secrets.h" 
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

//-----[ REST OF INIT ]-----//
int PIXEL_FORMAT = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel *pixels;
WiFiClient client;


//===========[ run the main setup code ]===========//
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pixels = new Adafruit_NeoPixel(PIXEL_COUNT, LED_PIN, PIXEL_FORMAT);
  pixels->begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressedAction, CHANGE);

  // Wifi things
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
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

//===========[ run the main code ]===========//
void loop() {
  switch(game_state) {
    case loggedOut:
      setLightColor(150, 150, 25);

      String tempCode = "A5HK92";

      if (client.connect("192.168.96.239", 3000)) {
        Serial.println("connected to server");
        // Make a HTTP request:
        client.println("GET /search?q=" + tempCode + " HTTP/1.1");
        client.println("Host: 192.168.96.239");
        client.println("Connection: close");
        client.println();
      }

      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }

      // if the server's disconnected, stop the client:
      if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting from server.");
        client.stop();

        // do nothing forevermore:
        while (true);
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

  // TODO: Show account id on display, if logged in
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
    case loggedOut:
      // User is not logged in... Handle the login on button click for now
      // TODO: Handle login -> This should happen over the fingerprint reader instead of the button press.
      game_state = idle;
      break;

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
      break;
  }
}
