#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)

  Wire.setClock(31000); // Max. frequency for 8MHz slave is 50 kHz, but 31 kHz leaves a bit reserve
/*    
  // Set the deviceID by writing new ID in address 3
  Wire.beginTransmission(8); // transmit to device #8 (0 is broadcast)
  Wire.write(3);        // Write to address 3, Device ID
  Wire.write(8);         
  Wire.endTransmission();    // stop transmitting
*/  
}

byte x = 50;

void loop() {
  Wire.setClock(31000); // Max. frequency for 8MHz slave is 50 kHz, but 31 kHz leaves a bit reserve
    
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(0);        // Write to address 0
  Wire.write(x);         
  Wire.write(x);         
  Wire.write(x);         
  Wire.endTransmission();    // stop transmitting

  if (x == 50) x = 200;
  else x = 50;

  delay(500);
}
