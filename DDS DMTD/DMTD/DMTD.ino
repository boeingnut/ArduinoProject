#include <Bounce.h>
#include <LiquidCrystal.h>
#include <Encoder.h>
#include <stdio.h>
#include <dds.h>

Encoder enc(2,3);
LiquidCrystal lcd(12, 8, 7, 6, 5, 4);
Bounce bouncer = Bounce( 13, 15);
dds dds(DDS9834, 11, 9, 10, 70000000LL);

void setup() {
  // set up the LCD's number of columns and rows: 
  pinMode(13, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  lcd.begin(16, 2);
  dds.calibrate(0.0); 
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
}

long encposition  = 0;
int pos = 0;


void loop() 
{
  long encpos;
  if (pos < 0)
  {
    enc.write(0);
  }
  else if (pos > 9)
  {
    enc.write(36);
  }
  encpos = enc.read();
  pos = encpos/4;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DDS Frequency");
  encposition = pos;
  if (encposition == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("10 000 000 Hz");
    dds.setfrequency(10000000LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 0);
  }    
  else if (encposition == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("10 000 001 Hz");
    dds.setfrequency(10000001LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 1);
  }
  else if (encposition == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print("10 000 010 Hz");
    dds.setfrequency(10000010LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 2);
  }
  else if (encposition == 3)
  {
    lcd.setCursor(0, 1);
    lcd.print("10 000 100 Hz");
    dds.setfrequency(10000100LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 3);
  }
  else if (encposition == 4)
  {
    lcd.setCursor(0, 1);
    lcd.print("10 001 000 Hz");
    dds.setfrequency(10001000LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 4);
  }
  else if (encposition == 5)
  {
    lcd.setCursor(0, 1);
    lcd.print("5 000 000 Hz");
    dds.setfrequency(5000000LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 5);
  }
  else if (encposition == 6)
  {
    lcd.setCursor(0, 1);
    lcd.print("5 000 001 Hz");
    dds.setfrequency(5000001LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 6);
  }
  else if (encposition == 7)
  {
    lcd.setCursor(0, 1);
    lcd.print("5 000 010 Hz");
    dds.setfrequency(5000010LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 7);
  }
  else if (encposition == 8)
  {
    lcd.setCursor(0, 1);
    lcd.print("5 000 100 Hz");
    dds.setfrequency(5000100LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 8);
  }
  else if (encposition == 9)
  {
    lcd.setCursor(0, 1);
    lcd.print("5 001 000 Hz");
    dds.setfrequency(5001000LL);
    do
    {
      encpos = enc.read();
      pos = encpos/4;
      encposition = pos;
      delay(100);
    }while (encposition == 9);
  }
  encposition = pos;
}



