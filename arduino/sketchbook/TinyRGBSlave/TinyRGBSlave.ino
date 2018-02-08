#include "tedi2c.h"

// 7 bit slave I2C address
const int SLAVE_ADDR = 0x8;
// current register address for read/write
volatile uint8_t addr = 0;

volatile uint8_t RGBVals[3];
volatile uint8_t NewAddr;

void setup() {
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);

  RGBVals[0] = 10;
  RGBVals[1] = 150;

  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
}

void loop() {
  analogWrite(1, RGBVals[0]);
  analogWrite(4, RGBVals[1]);
  delay(10);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (howMany <= 0) return;

  // read register address
  addr = Wire.read();
  addr = addr & 0x3; // Allowed addresses 0-3
  
  howMany--;

  // write the rest to the register, if this was a write request
  while (howMany) {
    if (addr < 3) {
      RGBVals[addr] = Wire.read();
    } else {
      NewAddr = Wire.read();
    }
    addr++; // set next addr
    if (addr >= 4) addr = 0; // start at 0 on register end
    howMany--;
  }
  // clear rx buffer
  while (Wire.available()) Wire.read();
}

