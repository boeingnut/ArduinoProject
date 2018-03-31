#include <Wire.h>
#include <OLED_SSD1306.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "DHT.h"

OLED_SSD1306 OLED;
float data=0;
float temp1=0;
float hum1=0;
char str[6];

DHT dht;
RF24 radio(9, 10);//CE,CSN
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };

void setup(void) 
{
  Serial.begin(9600);
  dht.setup(8);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipes[1]);
  radio.startListening();
  OLED.begin(0x3C);
  OLED.clearscreen();
  OLED.Char_F8x16(0,0,"T&F Lab");
  OLED.Char_F8x16(0,25,"T:");
  OLED.Char_F8x16(90,20,"o");
  OLED.Char_F8x16(100,25,"C");
  OLED.Char_F8x16(0,50,"H:");
  OLED.Char_F8x16(90,50,"%RH");

}

void loop(void)
{
  temp1 = dht.getTemperature();
  hum1 = dht.getHumidity();
  data = 1000000+ 10000*temp1 + 10*hum1;
  radio.write(&data, sizeof(data));
  OLED.Char_F8x16(30,25,dtostrf(temp1,5,2,str));
  OLED.Char_F8x16(30,50,dtostrf(hum1,5,2,str));
  Serial.println(data);
  delay(2000);
}
