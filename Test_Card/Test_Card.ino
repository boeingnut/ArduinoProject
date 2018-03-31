#include <LiquidCrystal.h>
#include <Wire.h>
#include "DHT.h"

#define DHTPIN 10
#define DHTTYPE DHT11
#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(45, 43, 41, 39, 37, 35);

char input;
int tempReading = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  Serial.begin(9600);
  Wire.begin();
  dht.begin();
  ADT7410INIT();
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Test Program");  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  if (Serial.available())
  {
    input = Serial.read();
    switch (input)
    {
      case 'Temp':
      {
        ADT7410GetTemp();
        float t = (tempReading/128.0);
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.setCursor(6, 1);
        lcd.print(t);
        lcd.setCursor(11, 1);
        lcd.print(" *C");
        break;
      }
      case 'Humid':
      {
        float h = dht.readHumidity();
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.println(" %\t");
        lcd.setCursor(0, 1);
        lcd.print("Humid: ");
        lcd.setCursor(7, 1);
        lcd.print(h);
        lcd.setCursor(12, 1);
        lcd.print(" %");
        break;
      }
      case 'R01':
      {  
      lcd.setCursor(0, 1);
      lcd.print("Relay 1-1 On    ");
      digitalWrite(5, HIGH);
      break;
      }
      case 'R02':
      {  
      lcd.setCursor(0, 1);
      lcd.print("Relay 1-2 On    ");
      digitalWrite(4, HIGH);
      break;
      }
      case 'R03':
      {  
      lcd.setCursor(0, 1);
      lcd.print("Relay 1-3 On    ");
      digitalWrite(3, HIGH);
      break;
      }
      case 'R04':
      {  
      lcd.setCursor(0, 1);
      lcd.print("Relay 1-4 On    ");
      digitalWrite(2, HIGH);
      break;
      }
      case 'clear':
      {  
      lcd.setCursor(0, 1);
      lcd.print("All Relay Off   ");
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      break;
      }
    }
  }
}

