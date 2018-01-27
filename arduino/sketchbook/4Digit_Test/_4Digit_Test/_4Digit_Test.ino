#include "MeMCore.h"
#include "TM1637.h"

// For use on MeBot Port 1:
#define CLK 12//pins definitions for TM1637 and can be changed to other ports       
#define DIO 11
TM1637 tm1637(CLK,DIO);
MeBuzzer Buzzer;

void setup()
{
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}

int Counter=9999;

void loop() {
  tm1637.point(POINT_ON);
  tm1637.display(0,(int)(Counter/1000));
  tm1637.display(1,(int)((Counter/100)%10)); 
  tm1637.display(2,(int)((Counter/10)%10));
  tm1637.display(3,(int)(Counter%10));

  Buzzer.tone(800, 100);
  delay(400);
  
  tm1637.point(POINT_OFF);
  tm1637.display(0,(int)(Counter/1000));
  tm1637.display(1,(int)((Counter/100)%10)); 
  tm1637.display(2,(int)((Counter/10)%10));
  tm1637.display(3,(int)(Counter%10));

  delay(500);
  if (Counter > 0) Counter--;  
}
