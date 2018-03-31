#include <nmea.h>
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "RTClib.h"
#include <SoftwareSerial.h>


//Setup Real Time Clock
RTC_DS1307 rtc;

//Setup NMEA protocal for GPS
NMEA gps(ALL);

SoftwareSerial GPS(2, 3);

////////////////////////////////////////////////////////////////////
double temp_act,hum_act,press_act;

//Parameter for SD Card 
String h;
String mi;
String s;


File dataFile;


void setup()
{
  //Open Serial for Communication with Ground Station
  Serial.begin(9600);
  //Open Serial for Communication with GPS
  GPS.begin(9600);
  //SD Card Chip Selector Pin
  pinMode(4, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  //Serial.println("card initialized.");
  
  //Start Real Time Clock
  rtc.begin();
  //rtc.adjust(DateTime(__DATE__, __TIME__));
}
//-------------------------------------------------------------------------------------------
void loop()
{
  //Read GPS
  char check = 'A';
  while (1){
    if (GPS.available() > 0) {
    // read incoming character from GPS and feed it to NMEA type object
    if (gps.decode(GPS.read())) {
      // full sentence received
      check = gps.terms();
    }
    
  }
  else if (check == gps.terms())
  {
      break;
  }
  }  
  delay(500);

  //Acquire DateTime
  DateTime now = rtc.now();
  h = String(now.hour(), DEC);
  mi = String(now.minute(), DEC);
  s = String(now.second(), DEC);
  //Create File for Each day
  dataFile.close();
  String temp = "Data.txt";
  char filename[temp.length()+1];
  temp.toCharArray(filename, sizeof(filename));
    //Serial.println(filename);
  dataFile = SD.open(filename, FILE_WRITE);
  
  //Write Data to File
  String dataString = String(h + ":" + mi + ":" + s + "  ");
  dataFile.print(dataString);
  dataFile.print("Position : ");
  dataFile.print(gps.gprmc_latitude(),6);
  dataFile.print(" , ");
  dataFile.print(gps.gprmc_longitude(),6);    
  dataFile.print(" T = ");
  dataFile.print(temp_act,2);
  dataFile.print(" H = ");
  dataFile.print(hum_act,2);
  dataFile.print(" P = ");
  dataFile.println(press_act);
  dataFile.flush();

  //Send Data to Ground Station
  delay(100);
  Serial.println(dataString);
  Serial.print("Position: ");
  delay(100);
  Serial.print(gps.gprmc_latitude(),6);
  Serial.print(" , ");
  Serial.println(gps.gprmc_longitude(),6);
  delay(100);
  Serial.print("Temperature = ");
  Serial.println(temp_act,2);
  Serial.print("Humidity = ");
  Serial.println(hum_act,2); 
  Serial.print("Pressure = ");
  Serial.println(press_act);
  Serial.println("");
  Serial.println("");
  
  
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------


