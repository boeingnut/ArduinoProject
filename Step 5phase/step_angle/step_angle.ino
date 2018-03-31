/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int old_angle=0;
int new_angle=0;
int d_angle=0;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(12, OUTPUT);     
  pinMode(11, OUTPUT);     
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() 
{
  
  if (Serial.available()>0) 
  {
    new_angle = Serial.parseInt();
    d_angle = new_angle - old_angle;
    Serial.print("Set Angle = ");
    Serial.println(new_angle);
    Serial.print("Change Angle = ");
    Serial.println(d_angle);
    Serial.print("Old Angle = ");
    Serial.println(old_angle);
     
  if(d_angle>0)
  {
    int i = 0;
    while (i<d_angle)
    {
      digitalWrite(11, LOW); 
      digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1);               // wait for a second
      digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
      delay(1);
      i++;
    }
  }
  if(d_angle<0)
  {
    int j = 0;
    d_angle=-1*d_angle;
    while (j<d_angle)
    {
      digitalWrite(12, LOW); 
      digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1);               // wait for a second
      digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
      delay(1);
      j++;
    }  
  }
  }
  old_angle = new_angle;
}
