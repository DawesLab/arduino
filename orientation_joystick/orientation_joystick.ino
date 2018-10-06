// An orientation-based flight system
// Based on PJRC prop controller and teensy 3.2
// Code below comes from a mashup of the NXPMotionSense - Orientation example
// and the Teensy - USB Joystick example

#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>

NXPMotionSense imu;
NXPSensorFusion filter;

const int numButtons = 16;  // 16 for Teensy

void setup() {
  Serial.begin(9600);
  imu.begin();
  filter.begin(100); // 100 is the expected rate in measurements per second
  
  Joystick.useManualSend(true);
  for (int i=0; i<numButtons; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  Serial.println("Begin Complete Joystick Test");
}

byte allButtons[numButtons];
byte prevButtons[numButtons];
int angle=0;

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = ((filter.getRoll() / 180) + 1) * 500;
    pitch = ((filter.getPitch() / 180) +1 ) * 500;
    heading = (filter.getYaw() * 2.8);

    Joystick.X(roll);
    Joystick.Y(pitch);
    Joystick.Z(heading);

    Joystick.send_now();
    
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
  }
}
