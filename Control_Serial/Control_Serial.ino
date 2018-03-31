#include "DHT.h"
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define Relay11 3
#define Relay12 4
#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

DHT dht(DHTPIN, DHTTYPE);

char input;
int tempReading = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  dht.begin();
  ADT7410INIT();
  
  pinMode(Relay11, OUTPUT);
  pinMode(Relay12, OUTPUT);
}

void loop()
{
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
        break;
      }
    	case 'Humid':
      {
        float h = dht.readHumidity();
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.println(" %\t");
        break;
      }
    	case 'R01':
      {
        digitalWrite(Relay11, HIGH);
        break;
      }
    	case 'R02':
      {
        digitalWrite(Relay12, HIGH);
        break;
      }  
    	case 'clear':
      {
        digitalWrite(Relay11, LOW);
        digitalWrite(Relay12, LOW);
        break;
      }
    }   
  }
}

void ADT7410INIT() {
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

