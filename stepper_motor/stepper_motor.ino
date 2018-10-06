#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(200, 2);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, DOUBLE);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, DOUBLE);
}

AccelStepper stepper1(forwardstep1, backwardstep1); // use functions to step


void setup()
{  
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Stepper test!");
  
    stepper1.setMaxSpeed(200.0);
    stepper1.setAcceleration(100.0);
    stepper1.moveTo(2400);
    
}
void loop()
{
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
        Serial.println("reverse!");
        stepper1.moveTo(-stepper1.currentPosition());
    Serial.println("Stepper go!");
    stepper1.run();
}
