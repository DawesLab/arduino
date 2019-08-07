// A WiFi LED strip driver for ArtNet
// Configure ArtNet source to unicast to universe 0 on it's IP.
// set numLeds for strip length and test wifi connectivity via serial port
// should be pingable by hostname

#include <ESP8266WiFi.h>  // Was just Wifi.h, I assume feather needs this instead of arduino wifi
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

//Wifi settings - be sure to replace these with the WiFi network that your computer is connected to

const char* ssid = "ALCGAP";
const char* password = "1f90243b66";
IPAddress ip(192,168,1,200);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// LED Strip
const int numLeds = 288; // Change if your setup has more or less LED's
const int numberOfChannels = numLeds * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
#define DATA_PIN 13 //The data pin that the WS2812 strips are connected to.
CRGB leds[numLeds];

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0;

bool sendFrame = 1;
int previousDataLength = 0;

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;
  WiFi.mode(WIFI_STA); // AMCD added to set to wifi client not ap
  WiFi.hostname("sunrise");
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  WiFi.printDiag(Serial);
  Serial.println("");
  Serial.println(WiFi.macAddress());
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println(" ");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  //Serial.print("Got frame: ");
  //Serial.print(universe);
  //Serial.print(" ");
  //Serial.println(length);
  sendFrame = 1;
  // set brightness of the whole strip 
  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
  }
  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < numLeds)
    {
      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
  }
  previousDataLength = length;
  //Serial.print(leds[0]);
  //Serial.print(leds[1]);
  //Serial.println(leds[2]);
  FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin();
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, numLeds);

  // onDmxFrame will execute every time a packet is received by the ESP32
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}
