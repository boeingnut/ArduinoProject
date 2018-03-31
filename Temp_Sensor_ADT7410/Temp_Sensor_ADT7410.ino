#include <genieArduino.h>
#include <Wire.h>


//Define ADT7410 address and Register Addresses. ***This assumes A0 and A1 are grounded.***
#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

// RAW 16-bit, signed data from ADT7410.
// (tempReading / 128) for positive temps in Celcius.
// ((tempReading - 65536) / 128) for negative temps in Celcius.
// For Farenheight ((ABOVERESULT * 1.8) + 32).
int tempReading = 0;

// Define values in final form. They are in FLOAT form for best precision, use int if you don't need decimal.
float finalTempF = 0.0000;
int TempC;
float finalTempC = 0.0000;

void setup() {
  genieSetup(9600);
  Wire.begin();
  // Initialize the ADT7410.
  ADT7410INIT();
  genieWriteObj(10,0,0);
}

void loop() {
  ADT7410GetTemp();
  TempC = (tempReading/128.0);
  finalTempC = (tempReading/128.0)*100;
  delay(50);
  genieWriteObj(15,0,finalTempC);
  genieWriteObj(18,0,TempC);
  //Serial.println("T = ");
  //Serial.println(finalTempC);
  // Doesn't need to be here. Just keeps my LCD from going nuts.
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
