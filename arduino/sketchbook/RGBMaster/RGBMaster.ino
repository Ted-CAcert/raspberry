#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(0);        // Write to address 0
  Wire.write(x);         
  Wire.write(255-x);         
  Wire.endTransmission();    // stop transmitting

  x += 5;
  if (x > 255) x = 0;
  delay(500);
}
