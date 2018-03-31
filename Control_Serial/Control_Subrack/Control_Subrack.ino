#include <Wire.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

DHT dht;

LiquidCrystal lcd(45, 43, 41, 39, 37, 35);

char input;
int tempReading = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  Serial.begin(9600);
  Wire.begin();
  ADT7410INIT();
  dht.setup(10);
      
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
       
  if (Serial.available())
  {
  input = Serial.read();
    switch (input)
    {
      case '0':
      {  
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(3, LOW);
      digitalWrite(2, LOW);
      break;
      }
      case '1':
      {  
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(3, LOW);
      digitalWrite(2, LOW);
      break;
      }
      case '2':
      {  
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(2, LOW);
      break;
      }
      case '3':
      {  
      digitalWrite(2, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(3, LOW);
      break;
      }
      case 'K':
      {  
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      break;
      }
      case '4':
      {  
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(7, LOW);
      digitalWrite(6, LOW);
      break;
      }
      case '5':
      {  
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(7, LOW);
      digitalWrite(6, LOW);
      break;
      }
      case '6':
      {  
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(6, LOW);
      break;
      }
      case '7':
      {  
      digitalWrite(6, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(7, LOW);
      break;
      }
      case 'L':
      {  
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
      }
      case 'a':
      {  
      digitalWrite(25, HIGH);
      digitalWrite(24, LOW);
      digitalWrite(23, LOW);
      digitalWrite(22, LOW);
      break;
      }
      case 'b':
      {  
      digitalWrite(24, HIGH);
      digitalWrite(25, LOW);
      digitalWrite(23, LOW);
      digitalWrite(22, LOW);
      break;
      }
      case 'c':
      {  
      digitalWrite(23, HIGH);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(22, LOW);
      break;
      }
      case 'd':
      {  
      digitalWrite(22, HIGH);
      digitalWrite(24, LOW);
      digitalWrite(23, LOW);
      digitalWrite(25, LOW);
      break;
      }
      case 'M':
      {  
      digitalWrite(25, LOW);
      digitalWrite(24, LOW);
      digitalWrite(23, LOW);
      digitalWrite(22, LOW);
      break;
      }
      case 'e':
      {  
      digitalWrite(29, HIGH);
      digitalWrite(28, LOW);
      digitalWrite(27, LOW);
      digitalWrite(26, LOW);
      break;
      }
      case 'f':
      {  
      digitalWrite(28, HIGH);
      digitalWrite(29, LOW);
      digitalWrite(27, LOW);
      digitalWrite(26, LOW);
      break;
      }
      case 'g':
      {  
      digitalWrite(27, HIGH);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(26, LOW);
      break;
      }
      case 'h':
      {  
      digitalWrite(26, HIGH);
      digitalWrite(28, LOW);
      digitalWrite(27, LOW);
      digitalWrite(29, LOW);
      break;
      }
      case 'N':
      {  
      digitalWrite(29, LOW);
      digitalWrite(28, LOW);
      digitalWrite(27, LOW);
      digitalWrite(26, LOW);
      break;
      }
    }
  ADT7410GetTemp();
  float t = (tempReading/128.0);
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(6, 0);
  lcd.print(t);
  lcd.setCursor(11, 0);
  lcd.print(" *C ");
  
  float h = dht.getHumidity();
  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.setCursor(7, 1);
  lcd.print(h);
  lcd.setCursor(12, 1);
  lcd.print(" % ");
  }
}

void ADT7410INIT() 
{
  //Initialization of the ADT7410 sets the configuration register based on input from the
  //Analog Devices datasheet page 14. 16-bit resolution selected.
  Wire.beginTransmission(B1001000);
  Wire.write(0x03);
  Wire.write(B10000000); 
  Wire.endTransmission();
}

void ADT7410GetTemp()
{
  byte MSB;
  byte LSB;
  // Send request for temperature register.
  Wire.beginTransmission(ADT7410Address);
  Wire.write(ADT7410TempReg);
  Wire.endTransmission();
  // Listen for and acquire 16-bit register address.
  Wire.requestFrom(ADT7410Address,2);
  MSB = Wire.read();
  LSB = Wire.read();
  // Assign global 'tempReading' the 16-bit signed value.
  tempReading = ((MSB << 8) | LSB);
}


