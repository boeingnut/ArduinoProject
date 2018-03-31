// include the servo library
#include <Servo.h>
#include <PID_v1.h>

double Setpoint1, Input1, Output1;
double Setpoint2, Input2, Output2;
Servo myServo1;
Servo myServo2;// create a servo object 
PID myPID1(&Input1, &Output1, &Setpoint1, 0.01, 0.01, 0.01, DIRECT);
PID myPID2(&Input2, &Output2, &Setpoint2, 0.01, 0.01, 0.01, DIRECT);
int const potPin1 = A0; // analog pin used to connect the potentiometer
int const potPin2 = A1;
int potVal1,potVal2;  // variable to read the value from the analog pin 
int angle1,angle2;   // variable to hold the angle for the servo motor 

void setup() 
{
  Input1 = analogRead(A0);
  Setpoint1 = 90;
  Input2 = analogRead(A1);
  Setpoint2 = 90;
  myServo1.attach(9); // attaches the servo on pin 9 to the servo object 
  myServo2.attach(10);
  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
  myPID1.SetSampleTime(100);
  myPID2.SetSampleTime(80);
  //Serial.begin(9600);
}

void loop() {
  //Input = analogRead(A0);
  potVal1 = analogRead(potPin1); // read the value of the potentiometer
  potVal2 = analogRead(potPin2);
  Input1 = map(potVal1, 294, 735, 0, 180);
  Input2 = map(potVal2, 294, 735, 0, 180);
  //Serial.print("Input: ");
  //Serial.print(Input);
  double gap1 = abs(Setpoint1-Input1); //distance away from setpoint
  if(gap1 > 40)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID1.SetTunings(5, 10, 1);
  }
  else if (gap1 < 40 && gap1 > 10)
  {
    myPID1.SetTunings(1, 2, 0.1);
  }
  else
  {
    myPID1.SetTunings(0.2, 0.1, 0);
  }
  
  
  double gap2 = abs(Setpoint2-Input2); //distance away from setpoint
  if(gap2 > 40)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID2.SetTunings(1, 2, 0.2);
  }
  else if (gap2 < 40 && gap2 > 10)
  {
    myPID2.SetTunings(0.2, 0.5, 0.05);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID2.SetTunings(0.05, 0.1, 0);
  }
  myPID1.Compute();
  myPID2.Compute();
  //Serial.print(", Output: ");
  //Serial.println(Output);
  // scale the numbers from the pot 
  angle1 = map(Output1, 0, 255, 180, 0);
  angle2 = map(Output2, 0, 255, 180, 0);
  
  //Serial.print(", angle: ");
  //Serial.println(angle1);
  // set the servo position  
  myServo1.write(angle1);
  myServo2.write(angle2);
  // wait for the servo to get there 
  //delay(20);
}



