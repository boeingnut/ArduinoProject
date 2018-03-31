#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

DHT dht1;
DHT dht2;
DHT dht3;
DHT dht4;
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int chipSelect = 10;
String y;
String m;
String d; 
String h;
String mi;
String s;
int tm=0;
int oldtm = 61;

File dataFile;

void setup()
{
  dht1.setup(6);
  dht2.setup(5);
  dht3.setup(4);
  dht4.setup(3);
  Serial.begin(9600);
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    //Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  //Serial.println("card initialized.");
  
  rtc.begin();
  //rtc.adjust(DateTime(__DATE__, __TIME__));
  lcd.begin(20,4);
  lcd.backlight();
  
}

void loop()
{
  float h1 = 0;
  float t1 = 0;
  h1 = dht1.getHumidity();
  t1 = dht1.getTemperature();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sens1  T:");
  lcd.setCursor(9,0);
  lcd.print(t1,1);
  lcd.setCursor(14,0);
  lcd.print("H:");
  lcd.setCursor(16,0);
  lcd.print(h1,1);
  
  float h2 = 0;
  float t2 = 0;
  h2 = dht2.getHumidity();
  t2 = dht2.getTemperature();
  
  lcd.setCursor(0,1);
  lcd.print("Sens2  T:");
  lcd.setCursor(9,1);
  lcd.print(t2,1);
  lcd.setCursor(14,1);
  lcd.print("H:");
  lcd.setCursor(16,1);
  lcd.print(h2,1);
  
  float h3 = 0;
  float t3 = 0;
  h3 = dht3.getHumidity();
  t3 = dht3.getTemperature()-1;
  
  lcd.setCursor(0,2);
  lcd.print("Sens3  T:");
  lcd.setCursor(9,2);
  lcd.print(t3,1);
  lcd.setCursor(14,2);
  lcd.print("H:");
  lcd.setCursor(16,2);
  lcd.print(h3,1);
  
  float h4 = 0;
  float t4 = 0;
  h4 = dht4.getHumidity();
  t4 = dht4.getTemperature()-1;
  
  lcd.setCursor(0,3);
  lcd.print("Sens4  T:");
  lcd.setCursor(9,3);
  lcd.print(t4,1);
  lcd.setCursor(14,3);
  lcd.print("H:");
  lcd.setCursor(16,3);
  lcd.print(h4,1);
    
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
  //Serial.println(filename);
  dataFile = SD.open(filename, FILE_WRITE);
  oldtm = tm;
  }
  else {
  if (! dataFile) {
    //Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
  
  String dataString = String(y + "/" + m + "/" + d + "  " + h + ":" + mi + ":" + s + "  ");
  dataFile.print(dataString);
  dataFile.print(t1);
  dataFile.print("  ");
  dataFile.print(h1);
  dataFile.print("  ");
  dataFile.print(t2);
  dataFile.print("  ");
  dataFile.print(h2);
  dataFile.print("  ");
  dataFile.print(t3);
  dataFile.print("  ");
  dataFile.print(h3);
  dataFile.print("  ");
  dataFile.print(t4);
  dataFile.print("  ");
  dataFile.println(h4);
  // print to the serial port too:
  Serial.print(dataString);
  Serial.print(t1);
  Serial.print("  ");
  Serial.print(h1);
  Serial.print("  ");
  Serial.print(t2);
  Serial.print("  ");
  Serial.print(h2);
  Serial.print("  ");
  Serial.print(t3);
  Serial.print("  ");
  Serial.print(h3);
  Serial.print("  ");
  Serial.print(t4);
  Serial.print("  ");
  Serial.println(h4);
    // The following line will 'save' the file to the SD card after every
    // line of data - this will use more power and slow down how much data
    // you can read but it's safer! 
    // If you want to speed up the system, remove the call to flush() and it
    // will save the file only every 512 bytes - every time a sector on the 
    // SD card is filled with data.
  dataFile.flush();
  delay(60000);
  }
}
