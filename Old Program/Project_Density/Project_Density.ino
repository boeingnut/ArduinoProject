#include "Barometer.h"
#include <Wire.h>
#include "DHT.h"
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;

float temperature;
float pressure;
File myFile;
File datalog;
String Data;
long i = 0;
int date = 1;

//Parameter for Calibration services
float offT;
float offTl;
float offH;
float offP;
float Sample;

//Date Time
String y;
String m;
String d;
String h;
String mi;
String s;

//Sensors
Barometer myBarometer;
DHT dht;

void setup(){
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  myBarometer.init();
  dht.setup(2);
  
  Wire.begin();
  rtc.begin();
  
  pinMode(10, OUTPUT);
  if (!SD.begin(4)) {
    return;
  }
  
  myFile = SD.open("Cal.txt");
  if (myFile) 
  {
    Serial.println("Cal.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Data = Data + char(myFile.read());	
    }
    // close the file:
    myFile.close();
  } 
  else 
  {
    while(1){}	// if the file didn't open, print an error:
  }
  offT = Data.substring(9,15).toFloat();
  offTl = Data.substring(26,32).toFloat();
  offH = Data.substring(43,49).toFloat();
  offP = Data.substring(60,67).toFloat();
  Sample = Data.substring(74,80).toFloat();
}

void loop()
{
  lcd.clear();
  DateTime now = rtc.now();
  y = String(now.year(), DEC);
  m = String(now.month(), DEC);
  d = String(now.day(), DEC);
  h = String(now.hour(), DEC);
  mi = String(now.minute(), DEC);
  s = String(now.second(), DEC);
  
  if (i>3600)
  {
    date ++;
    i = 0;
  }
  if (i <= 3600)
  {
  datalog.close();
  String temp = "HOUR";
  temp.concat(date);
  temp.concat(".txt");
  char filename[temp.length()+1];
  temp.toCharArray(filename, sizeof(filename));
  datalog = SD.open(filename, FILE_WRITE);
  }
  if (i == 0)
  {
    datalog.println("T(s)  T_A(c)  T_L(c)  H(%RH)  P(mbar)");
  }
  if (! datalog) 
  {
    while (1) ;
  }
  
  //Read Environment
  temperature = (myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT())+dht.getTemperature())/2; //Get the temperature, bmp085ReadUT MUST be called first
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP())*0.01;//Get the temperature
  float humidity = dht.getHumidity();
  
  lcd.setCursor(0,0);
  lcd.print("Temp Air: ");
  lcd.setCursor(10,0);
  lcd.print(temperature+offT, 2);
  lcd.setCursor(16,0);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("T Liquid: ");
  lcd.setCursor(10,1);
  lcd.print(temperature+offTl, 2);
  lcd.setCursor(16,1);
  lcd.print("C");

  lcd.setCursor(0,2);
  lcd.print("Humidity: ");
  lcd.setCursor(10,2);
  lcd.print(humidity+offH, 2);
  lcd.setCursor(16,2);
  lcd.print("%RH");

  lcd.setCursor(0,3);
  lcd.print("Pressure: ");
  lcd.setCursor(10,3);
  lcd.print(pressure+offP, 1);
  lcd.setCursor(16,3);
  lcd.print("mbar");
  
  
  datalog.print(i);
  datalog.print("  ");
  datalog.print(temperature+offT, 2);
  datalog.print("  ");
  datalog.print(temperature+offTl, 2);
  datalog.print("  ");
  datalog.print(humidity+offH, 2);
  datalog.print("  ");
  datalog.println(pressure+offP, 1);
  
  i = i + (Sample/1000);
  
  delay(Sample); //wait a second and get values again.
  datalog.flush();
    
}


