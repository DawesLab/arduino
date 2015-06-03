#include <Servo.h>
#define SERVOS 2
int servoPins[SERVOS] = {7,8};

Servo myServo[SERVOS];
int positionA = 150;
int positionB = 50;
String stringA = "Now in position A";
String stringB = "Now in position B";

void setup()
{
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  for(int i=0; i < SERVOS; i++)
    myServo[i].attach(servoPins[i]);
}

void loop()
{
  serviceSerial();
}

void serviceSerial()
{
  int pos = 0;
  String script;
  
  if ( Serial.available()) 
  {
    char ch = Serial.read();
    
    if (ch == 'a'||ch == 'A')
    {
     script = stringA;
     digitalWrite (6, HIGH);
     myServo[0].write(positionA);
     delay(500);
     digitalWrite(6, LOW);
    }
    else if (ch == 'b'||ch == 'B')
    {
     script = stringB;
     digitalWrite (6, HIGH);
     myServo[0].write(positionB);
     delay(500);
     digitalWrite(6, LOW);
    }
    
    if (ch == 'a'||ch == 'A'||ch == 'b'||ch == 'B')
    {
      Serial.println(script);
    }
  }
}
