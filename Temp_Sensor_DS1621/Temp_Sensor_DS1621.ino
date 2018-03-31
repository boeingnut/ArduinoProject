/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color

#define DEV_ID1 0b10010000 >> 1
#define DEV_ID2 0b10011110 >> 1

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  Wire.begin();
  Wire.beginTransmission(DEV_ID1);             // connect to DS1621 (#0)
  Wire.write(0xAC);                            // Access Config
  Wire.write(0x02);                            // set for continuous conversion
  Wire.beginTransmission(DEV_ID1);             // restart
  Wire.write(0xEE);                            // start conversions
  Wire.endTransmission();
  
  Wire.begin();
  Wire.beginTransmission(DEV_ID2);             // connect to DS1621 (#0)
  Wire.write(0xAC);                            // Access Config
  Wire.write(0x02);                            // set for continuous conversion
  Wire.beginTransmission(DEV_ID2);             // restart
  Wire.write(0xEE);                            // start conversions
  Wire.endTransmission();
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("Temp1 =");
  // print the number of seconds since reset:
  
  int8_t firstByte1;
  int8_t secondByte1;
  float temp1 = 0;
  delay(1000);                                // give time for measurement
 
  Wire.beginTransmission(DEV_ID1);
  Wire.write(0xAA);                            // read temperature command
  Wire.endTransmission();
  Wire.requestFrom(DEV_ID1, 2);		// request two bytes from DS1621 (0.5 deg. resolution)
  firstByte1 = Wire.read();		        // get first byte
  secondByte1 = Wire.read();		// get second byte
 
  temp1 = firstByte1;
 
  if (secondByte1)			        // if there is a 0.5 deg difference
      temp1 += 0.5;
  lcd.setCursor(8, 0);
  lcd.print(temp1);
  
  lcd.setCursor(0, 1);
  lcd.print("Temp2 =");
  // print the number of seconds since reset:
  
  int8_t firstByte2;
  int8_t secondByte2;
  float temp2 = 0;
  delay(1000);                                // give time for measurement
 
  Wire.beginTransmission(DEV_ID2);
  Wire.write(0xAA);                            // read temperature command
  Wire.endTransmission();
  Wire.requestFrom(DEV_ID2, 2);		// request two bytes from DS1621 (0.5 deg. resolution)
  firstByte2 = Wire.read();		        // get first byte
  secondByte2 = Wire.read();		// get second byte
 
  temp2 = firstByte2;
 
  if (secondByte2)			        // if there is a 0.5 deg difference
      temp2 += 0.5;
  lcd.setCursor(8, 1);
  lcd.print(temp2);

  Serial.print("Temp1 = ");
  Serial.print(temp1);
  Serial.print(", Temp2 = ");
  Serial.println(temp2);

  
}
