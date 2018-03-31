#include "Barometer.h"
#include <Wire.h>
#include "DHT.h"
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <SPI.h>
#include <Bounce.h>
#include <Encoder.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Encoder enc(3,7);
Bounce selector = Bounce( 5, 20);
Bounce exitconfig = Bounce( 6, 20);
RTC_DS1307 rtc;

const int chipSelect = 4;
int encposition  = 0;
int pos = 0;

float T;
float Tl;
float pressure;
float humidity;

//File datalog;
//String Data;

int tm=0;
int oldtm = 61;

//Parameter for Calibration services
float offT;
float offTl;
float offH;
float offP;
float Sample;

int n_offT;
int n_offTl;
int n_offH;
int n_offP;
int n_Sample;

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

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  lcd.begin(20,4);
  lcd.backlight();
  lcd.clear();
  myBarometer.init();
  dht.setup(2);
      
  //pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  //if (!SD.begin(chipSelect)) {
  //  while (1) ;
  //}
  //rtc.adjust(DateTime(__DATE__, __TIME__));
  //rtc.now();

}

void loop()
{
  while (exitconfig.read()!=1)
  {
    lcd.setCursor(0,0);
    lcd.print("   System  Config ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    
    int encpos;
    if (pos < 0)
    {
      enc.write(16);
    }
    else if (pos > 4)
    {
      enc.write(0);
    }
    encpos = enc.read();
    pos = encpos/4;
    encposition = pos;
    if (encposition == 0)
    {
      lcd.setCursor(0, 1);
      lcd.print("Temp Air");
      selector.update ();
      if (selector.read() == 1)
      {
        enc.write(0);
        for (int i = 0; i<500; i++)
        {
        n_offT = EEPROM.read(0);  
        offT = (n_offT-127) + enc.read()/4;
        lcd.setCursor(0, 2);
        lcd.print("Offset");
        lcd.setCursor(7, 2);
        lcd.print("      ");
        lcd.setCursor(7, 2);
        lcd.print(offT/10);
        delay(300);
        selector.update ();
        if (selector.read() == 1)
          {
            n_offT = int(offT+127);
            EEPROM.write(0, n_offT);
            delay(50);
            break;
          }
        }
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        enc.write(0);
      }
      else
      {
      }
    }
    else if (encposition == 1)
    {
      lcd.setCursor(0, 1);
      lcd.print("Temp Probe");
      selector.update ();
      if (selector.read() == 1)
      {
        enc.write(0);
        for (int i = 0; i<500; i++)
        {
        n_offTl = EEPROM.read(1);  
        offTl = (n_offTl-127) + enc.read()/4;
        lcd.setCursor(0, 2);
        lcd.print("Offset");
        lcd.setCursor(7, 2);
        lcd.print("      ");
        lcd.setCursor(7, 2);
        lcd.print(offTl/100);
        delay(300);
        selector.update ();
        if (selector.read() == 1)
          {
            n_offTl = int(offTl+127);
            EEPROM.write(1, n_offTl);
            break;
          }
        }
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        enc.write(4);
      }
      else
      {
      }
    }
    else if (encposition == 2)
    {
      lcd.setCursor(0, 1);
      lcd.print("Humidity");
      selector.update ();
      if (selector.read() == 1)
      {
        enc.write(0);
        for (int i = 0; i<500; i++)
        {
        n_offH = EEPROM.read(2);  
        offH = (n_offH-127) + enc.read()/4;
        lcd.setCursor(0, 2);
        lcd.print("Offset");
        lcd.setCursor(7, 2);
        lcd.print("      ");
        lcd.setCursor(7, 2);
        lcd.print(offH/10);
        delay(300);
        selector.update ();
        if (selector.read() == 1)
          {
            n_offH = int(offH+127);
            EEPROM.write(2, n_offH);
            break;
          }
        }
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        enc.write(8);
      }
      else
      {
      }
    }
    else if (encposition == 3)
    {
      lcd.setCursor(0, 1);
      lcd.print("Pressure");
      selector.update ();
      if (selector.read() == 1)
      {
        enc.write(0);
        for (int i = 0; i<500; i++)
        {
        n_offP = EEPROM.read(3);  
        offP = (n_offP-127) + enc.read()/4;
        lcd.setCursor(0, 2);
        lcd.print("Offset");
        lcd.setCursor(7, 2);
        lcd.print("      ");
        lcd.setCursor(7, 2);
        lcd.print(offP/2);
        delay(300);
        selector.update ();
        if (selector.read() == 1)
          {
            n_offP = int(offP+127);
            EEPROM.write(3, n_offP);
            break;
          }
        }
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        enc.write(12);
      }
      else
      {
      }
    }  
    else if (encposition == 4)
    {
      lcd.setCursor(0, 1);
      lcd.print("Sampling Rate");
      selector.update ();
      if (selector.read() == 1)
      {
        enc.write(0);
        for (int i = 0; i<500; i++)
        {
        n_Sample = EEPROM.read(4);  
        Sample = abs((n_Sample-127) + enc.read()/4);
        lcd.setCursor(0, 2);
        lcd.print("Rate");
        lcd.setCursor(7, 2);
        lcd.print("           ");
        lcd.setCursor(7, 2);
        lcd.print((Sample*500) + 1000, 0);
        delay(500);
        selector.update ();
        if (selector.read() == 1)
          {
            n_Sample = int(Sample+127);
            EEPROM.write(4, n_Sample);
            break;
          }
        }
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        enc.write(16);
      }
      else
      {
      }
    }
    exitconfig.update ();
    delay(50);
  }
  
  offT = EEPROM.read(0)-127;
  offTl = EEPROM.read(1)-127;
  offH = EEPROM.read(2)-127;
  offP = EEPROM.read(3)-127;
  Sample = EEPROM.read(4)-127;
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Temp Air: ");
  lcd.setCursor(10,1);
  lcd.print(offT/10, 2);
  lcd.setCursor(19,1);
  lcd.print("C");

  lcd.setCursor(0,0);
  lcd.print("T PROBE : ");
  lcd.setCursor(10,0);
  lcd.print(offTl/100, 2);
  lcd.setCursor(19,0);
  lcd.print("C");

  lcd.setCursor(0,2);
  lcd.print("Humidity: ");
  lcd.setCursor(10,2);
  lcd.print(offH/10, 2);
  lcd.setCursor(17,2);
  lcd.print("%RH");

  lcd.setCursor(0,3);
  lcd.print("Pressure: ");
  lcd.setCursor(10,3);
  lcd.print(offP/2, 1);
  lcd.setCursor(16,3);
  lcd.print("mbar");
  
  delay(2000);
  exitconfig.update ();

  while(exitconfig.read()!=1)
  {
  DateTime now = rtc.now();
  y = String(now.year(), DEC);
  m = String(now.month(), DEC);
  d = String(now.day(), DEC);
  h = String(now.hour(), DEC);
  mi = String(now.minute(), DEC);
  s = String(now.second(), DEC);
  tm = now.day();
  
  delay(50);
  Serial.println("T");
  String templ;
  delay(50);
  templ=Serial.readString();
  Serial.println(templ);
  Tl = templ.toFloat();
  delay(200);
  
  T = (dht.getTemperature()+myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()))/2; //Get the temperature, bmp085ReadUT MUST be called first
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP())*0.01;//Get the temperature
  humidity = dht.getHumidity();
 
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Temp Air: ");
  lcd.setCursor(10,1);
  lcd.print(T+offT/10, 2);
  lcd.setCursor(19,1);
  lcd.print("C");

  lcd.setCursor(0,0);
  lcd.print("T PROBE : ");
  lcd.setCursor(10,0);
  lcd.print(Tl+offTl/100, 3);
  lcd.setCursor(19,0);
  lcd.print("C");

  lcd.setCursor(0,2);
  lcd.print("Humidity: ");
  lcd.setCursor(10,2);
  lcd.print(humidity+offH/10, 2);
  lcd.setCursor(17,2);
  lcd.print("%RH");

  lcd.setCursor(0,3);
  lcd.print("Pressure: ");
  lcd.setCursor(10,3);
  lcd.print(pressure+offP/2, 1);
  lcd.setCursor(16,3);
  lcd.print("mbar");

  //if (tm != oldtm)
  //{
  //datalog.close();
  //String temp = "";
  //temp.concat(y);
  //temp.concat(m);
  //temp.concat(d);
  //temp.concat(".txt");
  //char filename[temp.length()+1];
  //temp.toCharArray(filename, sizeof(filename));
  //datalog = SD.open(filename, FILE_WRITE);
  //}
  //if (! datalog) {
  //  Serial.println("error opening datalog.txt");
  //  // Wait forever since we cant write data
  //  while (1) ;
  //}
  //else
  //{
  //String dataString = String(y + "/" + m + "/" + d + "  " + h + ":" + mi + ":" + s + "  ");
  //datalog.print(dataString);
  //datalog.print("  ");
  //datalog.print("T_A = ");
  //datalog.print("  ");
  //datalog.print(T+offT/10, 2);
  //datalog.print("  ");
  //datalog.print("T_L = ");
  //datalog.print("  ");
  //datalog.print(Tl+offTl/100, 3);
  //datalog.print("  ");
  //datalog.print("H = ");
  //datalog.print("  ");
  //datalog.print(humidity+offH/10, 2);
  //datalog.print("  ");
  //datalog.print("P = ");
  //datalog.print("  ");
  //datalog.println(pressure+offP/2, 1);
    
  //datalog.flush();
  exitconfig.update ();
  delay(Sample*500); //wait a second and get values again.
  //}
  
  }
  lcd.clear();
  delay(2000);
  exitconfig.update();
    
}


