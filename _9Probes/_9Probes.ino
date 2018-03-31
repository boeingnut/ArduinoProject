#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

DHT dht1;
DHT dht2;
DHT dht3;
DHT dht4;
DHT dht5;
DHT dht6;
DHT dht7;
DHT dht8;
DHT dht9;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()
{
  lcd.begin(20,4);
  lcd.backlight();
  
  
  Serial.begin(9600);
  
}

void loop()
{
  dht1.setup(22);
  dht2.setup(24);
  dht3.setup(26);
  dht4.setup(28);
  dht5.setup(30);
  dht6.setup(32);
  dht7.setup(34);
  dht8.setup(36);
  dht9.setup(38);' 
  float h1 = 0;
  float t1 = 0;
  h1 = dht1.getHumidity();
  t1 = dht1.getTemperature();
  
  float h2 = 0;
  float t2 = 0;
  h2 = dht2.getHumidity();
  t2 = dht2.getTemperature();
  
  float h3 = 0;
  float t3 = 0;
  h3 = dht3.getHumidity();
  t3 = dht3.getTemperature();
  
  float h4 = 0;
  float t4 = 0;
  h4 = dht4.getHumidity();
  t4 = dht4.getTemperature();
  
  float h5 = 0;
  float t5 = 0;
  h5 = dht5.getHumidity();
  t5 = dht5.getTemperature();
  
  float h6 = 0;
  float t6 = 0;
  h6 = dht6.getHumidity();
  t6 = dht6.getTemperature();
  
  float h7 = 0;
  float t7 = 0;
  h7 = dht7.getHumidity();
  t7 = dht7.getTemperature();
  
  float h8 = 0;
  float t8 = 0;
  h8 = dht8.getHumidity();
  t8 = dht8.getTemperature();
  
  float h9 = 0;
  float t9 = 0;
  h9 = dht9.getHumidity();
  t9 = dht9.getTemperature();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Incubator Temp");
  lcd.setCursor(0,1);
  lcd.print(t1,1);
  lcd.setCursor(9,1);
  lcd.print(t2,1);
  lcd.setCursor(16,1);
  lcd.print(t3,1);
  lcd.setCursor(0,2);
  lcd.print(t4,1);
  lcd.setCursor(9,2);
  lcd.print(t5,1);
  lcd.setCursor(16,2);
  lcd.print(t6,1);
  lcd.setCursor(0,3);
  lcd.print(t7,1);
  lcd.setCursor(9,3);
  lcd.print(t8,1);
  lcd.setCursor(16,3);
  lcd.print(t9,1);
  delay(10000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Incubator Humid");
  lcd.setCursor(0,1);
  lcd.print(h1,1);
  lcd.setCursor(9,1);
  lcd.print(h2,1);
  lcd.setCursor(16,1);
  lcd.print(h3,1);
  lcd.setCursor(0,2);
  lcd.print(h4,1);
  lcd.setCursor(9,2);
  lcd.print(h5,1);
  lcd.setCursor(16,2);
  lcd.print(h6,1);
  lcd.setCursor(0,3);
  lcd.print(h7,1);
  lcd.setCursor(9,3);
  lcd.print(h8,1);
  lcd.setCursor(16,3);
  lcd.print(h9,1);
  delay(10000);
  
}
