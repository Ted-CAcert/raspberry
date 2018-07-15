#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

void setup()
{
  mySerial.begin(9600);
}

void loop()
{
  mySerial.write(0x55);
  delay(100);
}
