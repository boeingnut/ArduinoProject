#include <Wire.h>
#include <OLED_SSD1306.h>
#include "DHT.h"

OLED_SSD1306 OLED;
float t=0;
float h=0;
char str[6];
DHT dht;

void setup() 
{
  dht.setup(4);
  delay(1000);
  OLED.begin(0x3C);
  OLED.clearscreen();
  OLED.Char_F8x16(0,0,"T&F Lab");
  OLED.Char_F8x16(0,25,"T:");
  OLED.Char_F8x16(90,20,"o");
  OLED.Char_F8x16(100,25,"C");
  OLED.Char_F8x16(0,50,"H:");
  OLED.Char_F8x16(90,50,"%RH");
}

void loop() 
{
  t = dht.getTemperature();
  h = dht.getHumidity();
  
  OLED.Char_F8x16(30,25,dtostrf(t,5,2,str));
  OLED.Char_F8x16(30,50,dtostrf(h,5,2,str));

  delay(500);
}
   
  
  


