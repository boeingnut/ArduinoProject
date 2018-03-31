#include <Wire.h>
#include <OLED_SSD1306.h>
OLED_SSD1306 OLED;

void setup() 
{
OLED.begin(0x3C);
OLED.clearscreen();
Serial.begin(9600);
Serial.println("TEST OLED");
OLED.Char_F8x16(0,0,"TEST OLED");
OLED.Char_F6x8(0,25,"1234567890");
OLED.Char_F8x16(0,50,"SSD1306");
}

void loop() 
{
  
}
   
  
  


