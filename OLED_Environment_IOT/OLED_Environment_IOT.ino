#include <Wire.h>
#include <OLED_SSD1306.h>
#include "DHT.h"

OLED_SSD1306 OLED;
float t=0;
float h=0;
char str[6];
DHT dht;
#define SSID "IOT"
#define PASS "environment"

void setup() 
{
  delay(100);
  dht.setup(4);
  delay(500);
  Serial1.begin(9600);
  Serial.begin(9600);
  OLED.begin(0x3C);
  OLED.clearscreen();
  OLED.Char_F8x16(0,0,"T&F Lab");
  OLED.Char_F8x16(0,25,"T:");
  OLED.Char_F8x16(90,20,"o");
  OLED.Char_F8x16(100,25,"C");
  OLED.Char_F8x16(0,50,"H:");
  OLED.Char_F8x16(90,50,"%RH");
  Serial1.println("AT+RST");
  delay(1000);
  Serial1.println("AT+CWMODE=1");
  delay(1000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial1.println(cmd);
  Serial.println(cmd);
  delay(10000);
  if(Serial1.find("OK")){
    Serial.println("Connected");
  }else{
    Serial.println("Not Connected");
  }
  
}

void loop() 
{
  t = dht.getTemperature();
  h = dht.getHumidity();
  
  OLED.Char_F8x16(30,25,dtostrf(t,5,2,str));
  OLED.Char_F8x16(30,50,dtostrf(h,5,2,str));
  char buf1[16];
  String strtemp = dtostrf(t, 5, 2, buf1);
  char buf2[16];
  String strhum = dtostrf(h, 5, 2, buf2);

   
  String cm = "AT+CIPSTART=\"TCP\",\"";
  cm += "192.168.1.2";
  cm += "\",2055";
  Serial1.println(cm);
  Serial.println(cm);
  delay(5000);
  if(Serial1.find("Error"))
  {
    Serial.println("Error");
  }
  
  String getStr ="Temp=";
  getStr += String(strtemp);
  getStr +="Humid=";
  getStr += String(strhum);
  getStr += "\r\n\r\n";

  // send data length
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);
  Serial1.println(cmd);
  delay(5000);
  if(Serial1.find(">"))
  {
    Serial1.println(getStr);
    Serial.println(getStr);
  }
  else
  {
    Serial1.println("AT+CIPCLOSE");
  }
    
  delay(10000);
}
   
  
  


