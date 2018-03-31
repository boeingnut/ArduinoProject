/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header
 		Pin 4 used here for consistency with other Arduino examples
 
 created  24 Nov 2010
 modified 9 Apr 2012 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>


RTC_DS1307 rtc;
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;
String y;
String m;
String d;
String h;
String mi;
String s;
int tm=0;
int oldtm = 61;

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color

#define DEV_ID1 0b10010000 >> 1
#define DEV_ID2 0b10011110 >> 1

File dataFile;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  Wire.begin();
  rtc.begin();
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

void loop()
{
  DateTime now = rtc.now();
  y = String(now.year(), DEC);
  m = String(now.month(), DEC);
  d = String(now.day(), DEC);
  h = String(now.hour(), DEC);
  mi = String(now.minute(), DEC);
  s = String(now.second(), DEC);
  tm = now.day();
  oldtm;
  if (tm != oldtm)
  {
  dataFile.close();
  String temp = "";
  temp.concat(y);
  temp.concat(m);
  temp.concat(d);
  //temp.concat(h);
  //temp.concat(h);
  temp.concat(".txt");
  char filename[temp.length()+1];
  temp.toCharArray(filename, sizeof(filename));
  Serial.println(filename);
  dataFile = SD.open(filename, FILE_WRITE);
  oldtm = tm;
  }
  else {
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
  if (now.second() == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Temp1 =");
    // print the number of seconds since reset:
  
    int8_t firstByte1;
    int8_t secondByte1;
    float temp1 = 0;
    delay(100);                                // give time for measurement
 
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
    delay(100);                                // give time for measurement
 
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
    
    String dataString = String(y + "/" + m + "/" + d + "  " + h + ":" + mi + ":" + s + "  ");
    dataFile.print(dataString);
    dataFile.print(temp1);
    dataFile.print("  ");
    dataFile.println(temp2);
    // print to the serial port too:
    Serial.print(dataString);
    Serial.print(temp1);
    Serial.print("  ");
    Serial.println(temp2);
    // The following line will 'save' the file to the SD card after every
    // line of data - this will use more power and slow down how much data
    // you can read but it's safer! 
    // If you want to speed up the system, remove the call to flush() and it
    // will save the file only every 512 bytes - every time a sector on the 
    // SD card is filled with data.
    dataFile.flush();
    delay(2000);
  }
  }
  // Take 1 measurement every 500 milliseconds
}









