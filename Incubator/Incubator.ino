#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DHT dht1;
DHT dht2;
DHT dht3;
DHT dht4;
DHT dht5;
DHT dht6;
DHT dht7;
DHT dht8;
DHT dht9;
////////////////////////////////////////////////////////////////
//Setup

//Delay time in second 
int t_delay = 60;
//Offset from the calibration 
float offt1 = 0;
float offt2 = 0;
float offt3 = 0;
float offt4 = 0;
float offt5 = 0;
float offt6 = 0;
float offt7 = 0;
float offt8 = 0;
float offt9 = 0;
float offh1 = 0;
float offh2 = 0;
float offh3 = 0;
float offh4 = 0;
float offh5 = 0;
float offh6 = 0;
float offh7 = 0;
float offh8 = 0;
float offh9 = 0;
////////////////////////////////////////////////////////////////

void setup() 
{
  Serial.begin(9600);
  lcd.begin(20,4);
  lcd.backlight();
  dht1.setup(22);
  dht2.setup(24);
  dht3.setup(26);
  dht4.setup(28);
  dht5.setup(30);
  dht6.setup(32);
  dht7.setup(34);
  dht8.setup(36);
  dht9.setup(38);
}

void loop() 
{
  if (Serial.read() == 'T')
  
  {
  float t1 = dht1.getTemperature()+offt1;
  float h1 = dht1.getHumidity()+offh1;
  float t2 = dht2.getTemperature()+offt2;
  float h2 = dht2.getHumidity()+offh2;
  float t3 = dht3.getTemperature()+offt3;
  float h3 = dht3.getHumidity()+offh3;
  float t4 = dht4.getTemperature()+offt4;
  float h4 = dht4.getHumidity()+offh4;
  float t5 = dht5.getTemperature()+offt5;
  float h5 = dht5.getHumidity()+offh5;
  float t6 = dht6.getTemperature()+offt6;
  float h6 = dht6.getHumidity()+offh6;
  float t7 = dht7.getTemperature()+offt7;
  float h7 = dht7.getHumidity()+offh7;
  float t8 = dht8.getTemperature()+offt8;
  float h8 = dht8.getHumidity()+offh8;
  float t9 = dht9.getTemperature()+offt9;
  float h9 = dht9.getHumidity()+offh9;
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
  Serial.print(h4);
  Serial.print("  ");
  Serial.print(t5);
  Serial.print("  ");
  Serial.print(h5);
  Serial.print("  ");
  Serial.print(t6);
  Serial.print("  ");
  Serial.print(h6);
  Serial.print("  ");
  Serial.print(t7);
  Serial.print("  ");
  Serial.print(h7);
  Serial.print("  ");
  Serial.print(t8);
  Serial.print("  ");
  Serial.print(h8);
  Serial.print("  ");
  Serial.print(t9);
  Serial.print("  ");
  Serial.println(h9);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Incubator Temp");
  lcd.setCursor(0,1);
  lcd.print(t1,1);
  lcd.setCursor(8,1);
  lcd.print(t2,1);
  lcd.setCursor(16,1);
  lcd.print(t3,1);
  lcd.setCursor(0,2);
  lcd.print(t4,1);
  lcd.setCursor(8,2);
  lcd.print(t5,1);
  lcd.setCursor(16,2);
  lcd.print(t6,1);
  lcd.setCursor(0,3);
  lcd.print(t7,1);
  lcd.setCursor(8,3);
  lcd.print(t8,1);
  lcd.setCursor(16,3);
  lcd.print(t9,1);
  delay(10000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Incubator Humid");
  lcd.setCursor(0,1);
  lcd.print(h1,1);
  lcd.setCursor(8,1);
  lcd.print(h2,1);
  lcd.setCursor(16,1);
  lcd.print(h3,1);
  lcd.setCursor(0,2);
  lcd.print(h4,1);
  lcd.setCursor(8,2);
  lcd.print(h5,1);
  lcd.setCursor(16,2);
  lcd.print(h6,1);
  lcd.setCursor(0,3);
  lcd.print(h7,1);
  lcd.setCursor(8,3);
  lcd.print(h8,1);
  lcd.setCursor(16,3);
  lcd.print(h9,1);
  }
}
