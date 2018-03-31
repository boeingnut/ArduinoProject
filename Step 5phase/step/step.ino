/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int i =0;
double angle;
int a = 0;
int wait1;
int wait2;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(12, OUTPUT);     
  pinMode(11, OUTPUT);     
  pinMode(10, INPUT);     
  pinMode(9, INPUT);     
  pinMode(8, INPUT);     
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() 
{
  a = analogRead(A0);
  wait1 = map(a,553,1023,10,1);
  wait2 = map(a,0,470,1,10);
  if(analogRead(A0)>553)
 {
  digitalWrite(11, LOW); 
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(wait1);               // wait for a second
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
  delay(wait1);
  i++;
 }
  if(analogRead(A0)<470)
 {
  digitalWrite(12, LOW); 
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(wait2);               // wait for a second
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
  delay(wait2);
  i--;
 }
  if(digitalRead(8)==HIGH)
 {
  digitalWrite(12, LOW); 
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
  angle = i*0.024;
  Serial.println(angle);
  delay(100);
 }// wait for a second
}
