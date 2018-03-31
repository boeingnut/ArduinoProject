#include <SPI.h>
#include <SD.h>

File datafile;

int bytedata=-1;
char linea[300] = "";
int conta=0;
 
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

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
  
  bytedata=Serial1.read();         // Read a byte of the serial port
   if (bytedata == -1) {           // See if the port is empty yet
     delay(10); 
   } else {
     // note: there is a potential buffer overflow here!
     linea[conta]=bytedata;        // If there is serial port data, it is put in the buffer
     conta++; 
     if (bytedata==13){            // If the received byte is = to 13, end of transmission
       // note: the actual end of transmission is <CR><LF> (i.e. 0x13 0x10)
       //digitalWrite(ledPin, LOW); 
       // The following for loop starts at 1, because this code is clowny and the first byte is the <LF> (0x10) from the previous transmission.
        datafile = SD.open("data.txt", FILE_WRITE);
        if (! datafile) {
            Serial.println("error opening datalog.txt");
            // Wait forever since we cant write data
            while (1) ;
        } 
        //Serial.println("opening data.txt");

       for (int j=0;j<conta;j++)
         {
             Serial.print(linea[j]);
             datafile.print(linea[j]); 
         }
       datafile.flush();
       
       datafile.close();
//       Serial.println("");
       conta=0;                    // Reset the buffer
       for (int i=0;i<300;i++){    //  
         linea[i]=' ';             
       }                 
     }
   }
   
}  
  
  



   

