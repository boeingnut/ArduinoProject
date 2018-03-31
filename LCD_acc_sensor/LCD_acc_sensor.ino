#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
}

void loop() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Value = ");
  lcd.setCursor(0, 1);
  lcd.print(analogRead(0)); 
  lcd.setCursor(5, 1);
  lcd.print(analogRead(1)); 
  lcd.setCursor(10, 1);
  lcd.print(analogRead(2)); 
  delay(100);
}
