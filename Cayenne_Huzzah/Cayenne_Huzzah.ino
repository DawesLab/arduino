// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

int value = 0;

// WiFi network info.
char ssid[] = "E6WE1";
char wifiPassword[] = "1F90243B66";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "745d1dc0-aed7-11e6-82f1-ed3c04eeee58";
char password[] = "2b8667f9759b42c3463cd6a04d8c1551fefa20b2";
char clientID[] = "a3b84c60-a70c-11e7-b0e9-e9adcff3788e";

unsigned long lastMillis = 0;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();

	//Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 10000) {
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, 100);
    Cayenne.virtualWrite(1, value);
		//Some examples of other functions you can use to send data.
		//Cayenne.celsiusWrite(1, 22.0);
		//Cayenne.luxWrite(2, 700);
		//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_IN(1)
{
  CAYENNE_LOG("input on %u - %s, %s", request.channel, getValue.getId(), getValue.asString());
  value = getValue.asInt();
  analogWrite(14,value);
}

