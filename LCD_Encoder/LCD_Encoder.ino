#include <Bounce.h>
#include <LiquidCrystal.h>
#include <Encoder.h>

Encoder enc(2,3);
LiquidCrystal lcd(12, 8, 7, 6, 5, 4);
Bounce bouncer = Bounce( 13, 15);

void setup() {
  // set up the LCD's number of columns and rows: 
  pinMode(13, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  lcd.begin(16, 2);
}

long encposition  = 0;
int pos = 0;
int Led1,Led2,Led3;


void loop() 
{
  long encpos;
  if (pos < 0)
  {
    enc.write(16);
  }
  else if (pos > 4)
  {
    enc.write(0);
  }
  encpos = enc.read();
  pos = encpos/4;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Light Control");
  encposition = pos;
  if (encposition == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("LED1");
    bouncer.update ();
    if (bouncer.read() == 1)
    {
      lcd.clear();
      enc.write(4*Led1);
      for (int i = 0; i<500; i++)
      {
      Led1 = enc.read()/4;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brigthness LED1");
      lcd.setCursor(0, 1);
      lcd.print("Level");
      lcd.setCursor(6, 1);
      lcd.print(Led1);
      analogWrite(11, Led1);
      delay(50);
      bouncer.update ();
      if (bouncer.read() == 1)
      {
        break;
      }
      }
      enc.write(0);
    }
    else
    {
    }    
  }
  else if (encposition == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("LED2");
    bouncer.update ();
    if (bouncer.read() == 1)
    {
      lcd.clear();
      enc.write(4*Led2);
      for (int i = 0; i<500; i++)
      {
      Led2 = enc.read()/4;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brigthness LED2");
      lcd.setCursor(0, 1);
      lcd.print("Level");
      lcd.setCursor(6, 1);
      lcd.print(Led2);
      analogWrite(10, Led2);
      delay(50);
      bouncer.update ();
      if (bouncer.read() == 1)
      {
        break;
      }
      }
      enc.write(4);
    }
    else
    {
    }
  }
  else if (encposition == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print("LED3");
    bouncer.update ();
    if (bouncer.read() == 1)
    {
      lcd.clear();
      enc.write(4*Led3);
      for (int i = 0; i<500; i++)
      {
      Led3 = enc.read()/4;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brigthness LED3");
      lcd.setCursor(0, 1);
      lcd.print("Level");
      lcd.setCursor(6, 1);
      lcd.print(Led3);
      analogWrite(9, Led3);
      delay(50);
      bouncer.update ();
      if (bouncer.read() == 1)
      {
        break;
      }
      }
      enc.write(8);
    }
    else
    {
    }
  }
  else if (encposition == 3)
  {
    lcd.setCursor(0, 1);
    lcd.print("On all LEDs");
    bouncer.update ();
    if (bouncer.read() == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brigthness");
      lcd.setCursor(0, 1);
      lcd.print("Level Max");
      lcd.setCursor(6, 1);
      analogWrite(9, 255);
      analogWrite(10, 255);
      analogWrite(11, 255);
      
      enc.write(12);
    }
    else
    {
    }
  }
  else if (encposition == 4)
  {
    lcd.setCursor(0, 1);
    lcd.print("Off all LEDs");
    bouncer.update ();
    if (bouncer.read() == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brigthness");
      lcd.setCursor(0, 1);
      lcd.print("Level off");
      lcd.setCursor(6, 1);
      analogWrite(9, 0);
      analogWrite(10, 0);
      analogWrite(11, 0);
      
      enc.write(16);
    }
    else
    {
    }
  }
  encposition = pos;
  delay(50);
}
