#include "Arduino.h"
int increasebutton = 3;
int decreasebutton = 4;
int LED1 = 5;
int LED2 = 6;
boolean LED1state = LOW;
boolean LED2state = LOW;
boolean buttonstate = LOW;

void setup()
{
  pinMode(increasebutton, INPUT);
  pinMode(decreasebutton, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LED1state);
  digitalWrite(LED2, LED2state);
}

boolean debouncing(boolean button)
{
  boolean reading = digitalRead(button);
  if (buttonstate != reading)
  {
    delay(5);
    reading = digitalRead(button);
  }
  return (reading);
}

void loop()
{
  int increasebuttonstate = debouncing(increasebutton);
  int decreasebuttonstate = debouncing(decreasebutton);
  if (buttonstate == LOW && increasebuttonstate == HIGH)
  {  
    LED1state = !LED1state;
  }
  else if (buttonstate == LOW && decreasebuttonstate == HIGH) 
  {
    LED2state = !LED2state;
  }
  else
  {
    LED1state = LED1state;
    LED2state = LED2state;
  }
  digitalWrite(LED1, LED1state);
  digitalWrite(LED2, LED2state);  
}
