#include <EEPROM.h>
#include "TinySerial.h"

const char SerialTxPin=1;
TinySerial mySerial(SerialTxPin);

void setup() {
  mySerial.begin(9600);
  mySerial.println(F("EEPROM-Dump"));
  mySerial.print(__DATE__);
  mySerial.print(" ");
  mySerial.println(__TIME__);
}

void loop() {
  int i;

  mySerial.print("\r\n\r\n");
  for(i = 0; i < EEPROM.length(); i++) {
    if (i%16 == 0) {
      mySerial.print("\r\n");
      if (i < 256) {
        mySerial.print("0");
      }
      mySerial.print(i, HEX);
      mySerial.print(": ");
    }
    mySerial.print(EEPROM.read(i), HEX);
    mySerial.print(" ");
  }
  mySerial.print("\r\n");

  delay(10000);
}
