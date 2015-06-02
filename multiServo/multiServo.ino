#include <Servo.h>
#define SERVOS 2
int servoPins[SERVOS] = {7,8};

Servo myservo[SERVOS];

void setup()
{
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  for(int i=0; i < SERVOS; i++)
    myservo[i].attach(servoPins[i]);
}

void loop()
{
  serviceSerial();
}

void serviceSerial()
{
  static int pos = 0;
  
  if ( Serial.available()) {
    char ch = Serial.read();
    
    if( isDigit(ch) )
      pos = (pos * 10) + (ch - '0');
    else if(ch == 'a') {
      //set control pin high
      digitalWrite(6,HIGH);
      myservo[0].write(pos);
      digitalWrite(6, LOW);
      //set control pin low
      Serial.print("on channel ");
      Serial.print(ch - 'a');
      Serial.print(" going to: ");
      Serial.println(pos);
      pos = 0;
    }
  }
}
