=// include the servo library
#include <Servo.h>

Servo myServo1;
Servo myServo2;// create a servo object 
int const potPin1 = A0; // analog pin used to connect the potentiometer
int const potPin2 = A1;
int potVal1,potVal2;  // variable to read the value from the analog pin 
int angle1,angle2;   // variable to hold the angle for the servo motor 

void setup() 
{
  potVal1 = analogRead(A0);
  potVal2 = analogRead(A1);
  myServo1.attach(9); // attaches the servo on pin 9 to the servo object 
  myServo2.attach(10);
  //Serial.begin(9600);
}

void loop() {
  //Input = analogRead(A0);
  for (int i = 0; i < 50; i++)
  {
  potVal1 += analogRead(potPin1); // read the value of the potentiometer
  potVal2 += analogRead(potPin2);
  }
  potVal1 = potVal1/51;
  potVal2 = potVal2/51;
  angle1 = map(potVal1, 294, 735, 180, 0);
  angle2 = map(potVal2, 294, 735, 180, 0);
  //Serial.print("Input: ");
  //Serial.print(Input);
  //Serial.print(", angle: ");
  //Serial.println(angle1);
  // set the servo position  
  myServo1.write(angle1);
  myServo2.write(angle2);
  // wait for the servo to get there 
  //delay(50);
}



