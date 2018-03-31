#include <SPI.h>
#include <SD.h>

File datafile;

void setup() {
  Serial.begin(9600);
  //Serial1.begin(9600);

  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:    
    
    while (1) ;
  }
  Serial.println("card initialized.");
  
}

void loop() {
  String dat=Serial.readString();
  Serial.println(dat);// Read a byte of the serial port
  datafile.close();
  datafile = SD.open("data.txt", FILE_WRITE);
  if (! datafile) {
    //Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
    } 
  datafile.println(dat);
  datafile.flush();        
}

