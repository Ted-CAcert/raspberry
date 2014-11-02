#include <EEPROM.h>

int addr = 0;

void setup()
{
  int i;
/*  
  for(i = 0; i < 512; i++) {
    EEPROM.write(i*2, i&255);
    EEPROM.write(i*2+1, i>>8);
  }*/
  EEPROM.write(1023, EEPROM.read(1023)+1);
  Serial.begin(9600);
  Serial.println("");
  Serial.println("===================================================");
}

void loop()
{
  int Byte;
  int i;
/*  
  for(i = 0; i < 512; i++) {
    EEPROM.write(i*2, i&255);
    EEPROM.write(i*2+1, i>>8);
  }*/
  Serial.println("");
  for(i = 0; i < 1024; i++) {
    if (i%16 == 0) Serial.println("");
    Byte=EEPROM.read(i);
    if (Byte < 16) Serial.print(" 0");
    else Serial.print(" ");
    Serial.print(Byte, HEX);
  }
  Serial.println("");
  delay(10000);
}
