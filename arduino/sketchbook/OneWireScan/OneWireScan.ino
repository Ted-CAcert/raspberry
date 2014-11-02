#include <OneWire.h>
 
OneWire ow(2);
 
void setup(void)
{
  Serial.begin(9600);
  lookUpSensors();
}
 
void lookUpSensors()
{
  byte address[8];
  int i=0;
  byte ok = 0, tmp = 0;
 
  Serial.println("--Suche gestartet--");
  while (ow.search(address))
  {
    tmp = 0;
    switch(address[0]) {
      case 0x10:
        //0x10 = DS18S20
        Serial.print("Device is a DS18S20 : ");
        tmp = 1;
        break;
        
      case 0x28:
        //0x28 = DS18B20
        Serial.print("Device is a DS18B20 : ");
        tmp = 1;
        break;

      case 0x06:
        Serial.print("Device is a DS1993 4K NVRAM : ");
        tmp = 1;
        break;
        
      default:
        Serial.print("Unknown Device : ");
        Serial.println(address[0],HEX);
        tmp = 0;
        break;      
    }

    //display the address, if tmp is ok
    if (tmp == 1)
    {
      if (OneWire::crc8(address, 7) != address[7])
      {
        Serial.println("but it doesn't have a valid CRC!");
      }
      else
      {
        //all is ok, display it
        for (i=0;i<8;i++)
        {
          if (address[i] < 9)
          {
            Serial.print("0");
          }
          Serial.print("0x");
          Serial.print(address[i],HEX);
          if (i<7)
          {
            Serial.print(", ");
          }
        }
        Serial.println("");
        ok = 1;
      }
    }//end if tmp
  }//end while
  if (ok == 0)
  {
    Serial.println("Keine Sensoren gefunden");
  }
  Serial.println("--Suche beendet--");
}
 
void loop(void)
{
  //do nothing :)
}

