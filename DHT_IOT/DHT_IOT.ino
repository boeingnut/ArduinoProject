#include <SoftwareSerial.h>
#include "DHT.h"

DHT dht;
String apiKey = "IZ1AUINCY57LRRK6";
#define SSID "IOT"
#define PASS "environment"
// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX

void setup()
{
  Serial.begin(9600);
  
  ser.begin(9600);
  
  // reset ESP8266
  dht.setup(2); // data pin 2
  ser.println("AT+RST");
  
  ser.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  ser.println(cmd);
  Serial.println(cmd);
  delay(10000);
  if(ser.find("OK")){
    Serial.println("Connected");
  }else{
    Serial.println("Not Connected");
  }
}

void loop()
{
  float humidity = 0;
  float temperature = 0;
  for(int i = 0; i < 10; i++) 
  {
      delay(dht.getMinimumSamplingPeriod());
      humidity += dht.getHumidity();
      temperature += dht.getTemperature();
  }
  
  char buf1[16];
  String strtemp = dtostrf(temperature/10, 4, 1, buf1);
  char buf2[16];
  String strhum = dtostrf(humidity/10, 4, 1, buf2);
  
  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "192.168.1.2";//"184.106.153.149"; // api.thingspeak.com
  cmd += "\",2055";
  ser.println(cmd);
  Serial.println(cmd);
  
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
   // prepare GET string
  //String getStr = "GET /update?api_key=";
  //getStr += apiKey;
  String getStr ="Temp=";
  getStr += String(strtemp);
  getStr +="Humid=";
  getStr += String(strhum);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);
  if(ser.find(">")){
    ser.print(getStr);
    Serial.println(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(strhum);
  Serial.print("\t\t");
  Serial.println(strtemp);
  delay(55000);
}

