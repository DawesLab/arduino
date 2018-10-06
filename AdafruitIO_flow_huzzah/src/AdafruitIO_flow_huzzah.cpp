#include <Arduino.h>

/***************************************************
 *
 * AMCDawes Adafruit HUZZAH wifi flow sensor for home use
 * connections:
 * pin 2 - flow sensor input
 *
 * Pushes data to io.adafruit.com
 *
 * Based on code Written by Tony DiCola for Adafruit Industries.
 * MIT license, all text above must be included in any redistribution
 ****************************************************/

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//#include "DHT.h"

// WiFi parameters for work
//#define WLAN_SSID       "Boxer Legacy"
//#define WLAN_PASS       "HeartofOak"
// WiFi parameters for home
#define WLAN_SSID       "E6WE1"
#define WLAN_PASS       "1F90243B66"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "amcdawes"
#define AIO_KEY         "71b4f2552899427e061700af696513c380ad1668"

// Functions
void connect();

void flowPulse();

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// which pin to use for reading the sensor? can use any pin!
#define FLOWSENSORPIN 2

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ***************************************/
Adafruit_MQTT_Publish flowrateFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/flowrate");
Adafruit_MQTT_Publish litersFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/liters");
/*************************** Sketch Code ************************************/

// Pulse counting details and interrupt:
// count how many pulses!
volatile uint16_t pulses = 0;
volatile uint16_t countedPulses = 0;
// and use that to calculate a flow rate
volatile float flowrate;

volatile float liters = 0;
volatile float mls = 0;

unsigned long startMillis = 0;
unsigned long endMillis = 0;
unsigned long elapsedMillis = 0;

// Interrupt is called on falling edges to count pulses.

void flowPulse() {
  pulses++;
}

void setup() {

  // Init sensor
  //dht.begin();

  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();


  attachInterrupt(digitalPinToInterrupt(FLOWSENSORPIN), flowPulse, FALLING);

}

void loop() {

  startMillis = millis();
  // wait for 10 seconds
  delay(10000);

  endMillis = millis();
  countedPulses = pulses;
  pulses = 0;  //RESET pulse count
  elapsedMillis = endMillis - startMillis; // TODO check for overflow of millis!

  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }

  // if a plastic sensor use the following calculation
  // SensorFrequency (Hz) = 7.5 * Q (Liters/min)
  // Liters = Q * time elapsed (seconds) / 60 (seconds/minute)
  // Liters = (Frequency (Pulses/second) / 7.5) * time elapsed (seconds) / 60
  // Liters = Pulses / (7.5 * 60)
  liters = countedPulses;
  liters /= 7.5;
  liters /= 60.0;
  mls = liters*1000;
  Serial.print("Pulses: "); Serial.println(countedPulses);
  flowrate = (float)liters * 1000 / (float)elapsedMillis; // flowrate in l/s
  Serial.print("Volume (ml): "); Serial.println(mls);
  Serial.print("Flowrate (l/s): "); Serial.println(flowrate);

  // Publish data
  if (! flowrateFeed.publish(flowrate))
    Serial.println(F("Failed to publish flowrate"));
  else
    Serial.println(F("flowrate published!"));

  if (! litersFeed.publish(liters))
    Serial.println(F("Failed to publish liters"));
  else
    Serial.println(F("liters published!"));
}

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
