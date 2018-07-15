#include <LiquidCrystal.h>

//LiquidCrystal lcd(2, 3, 4,  8, 9, 10, 11, 12, 13, 5, 6);
LiquidCrystal lcd(6, 7, 8,   12, 13, 3, 4);
const int PinRotaryA=2;
const int PinRotaryB=10;
const int PinPush=11;
const int PinBacklight=5;

volatile int Counter = 0;
volatile unsigned long LastTick = 0;
int LastCount = 0;

void pinCount()
{
  if ((digitalRead(PinRotaryA) == LOW) && (millis()-LastTick > 10)) {
    LastTick = millis();
    if (digitalRead(PinRotaryB) == LOW)
      Counter++;
    else
      Counter--;
  }
}

void setup()
{
    lcd.begin(20, 4);
    lcd.print("Hallo, Albrecht!");
    digitalWrite(PinRotaryA, HIGH);
    digitalWrite(PinRotaryB, HIGH);
    digitalWrite(PinPush, HIGH);
    analogWrite(PinBacklight, 255);
    attachInterrupt(0, pinCount, CHANGE);
}

void loop()
{
  int i;
  
  for(i = 10; i >= 0; i--) {
    if (millis() < LastTick) LastTick = 0;
    
/*    lcd.setCursor(0, 1);
    if (i < 10) lcd.print(" ");
    lcd.print(i);
    delay(500);
*/
    
    if (Counter != LastCount) {
      lcd.setCursor(0, 2);
      lcd.print(Counter);
      lcd.print(" ");
      LastCount = Counter;
    }
/*    if (digitalRead(PinPush) == LOW && Counter >= 0 && Counter < 256) {
      analogWrite(PinBacklight, Counter);
    }*/
    delay(10);
    
  }
/*  lcd.setCursor(0, 1);
  lcd.print("***BUMM***");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("          ");*/
  
}
