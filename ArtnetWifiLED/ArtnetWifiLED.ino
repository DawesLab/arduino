/*
  This example will receive multiple universes via Artnet and control a strip of ws2811 leds via
  Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <Adafruit_NeoPixel.h>

//Wifi settings
const char* ssid = "arrialive";
const char* password = "soundboard";
IPAddress ip(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Artnet settings
ArtnetWifi artnet;
const int myUniverse = 0; // CHANGE FOR YOUR SETUP

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  if (universe == myUniverse) {
    Serial.print("got data: ");
    Serial.print(data[399]);
    Serial.print(" and ");
    Serial.println(data[400]);
    analogWrite(14,data[399]); // channel 400 for now.
  }
}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin();

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}
