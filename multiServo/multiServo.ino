#include <Servo.h>
#define SERVOS 2
int servoPins[SERVOS] = {7,8};

Servo myservo[SERVOS];

void setup()
{
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
      pos = pos * 10 + ch - '0';
    else if(ch >= 'a' && ch <= 'a'+ SERVOS)
      myservo[ch - 'a'].write(pos);
  }
}
