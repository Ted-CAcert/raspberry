/* 
 *  TinyRGBSlave has been tested on a ATTiny85 at 8MHz.
 *  In theory it should also run on an ATTiny25, needing less than 2k program memory and maybe 100 Bytes of RAM.
 *  
 *  The ATTiny is intended to control one strip of RGB LEDs, the RGB values can be set using TWI/I2C bus.
 *  This should allow one "master" (for example one Arduino board, or anything else that can implement an
 *  TWI master at ~30 kHz) to control up to 112 RGB LED strips...
 *  
 *  RGB LEDs must be connected to pins PB0, PB1 and PB4, that are pins 5, 6 and 3 on the PDIP/SOIC-casing.
 *  Pin PB2 (pin 7) is the TWI SDA (data) signal, whereas PB3 is SCL (clock). 
 *  Note that you'll probably need an additional driver transistor and power supply if you want to control more 
 *  than a single LED.
 *  
 *  Since the TWI is implemented in software the maximum TWI clock frequency supported at 8 MHz ATTiny clock is about 
 *  50 kHz, though in my tests I normally did use only 31 kHz ("Wire.setClock(31000);" on an Arduino). 
 *  In theory 100 kHz TWI clock should be supported if the ATTiny is clocked at 20 MHz.
 *  
 *  The device has 4 registers that can be written into by TWI:
 *  0:  PWM value for PB0
 *  1:  PWM value for PB1
 *  2:  PWM value for PB4
 *  3:  New TWI device ID
 *  
 *  The address counter is incremented by 1 after each write, and reset to 0 after reachinc a value > 2.
 *  So, a new device ID may be set together with all PWM values by startung with address 3 and writing four
 *  values (new ID, PWM0, PWM1, PWM4).
 *  
 *  The device ID is stored in the EEPROM and therefor survive a reset or power cycle, all PWM values are 
 *  set to 0 after a reset. (ToDo for next version: Allow the initial PWM values to be stored in EEPROM)
 *  
 *  Reading register contents back is not supported by this software.
 *  
 *  The device always reacts to the broadcast address 0. This is a simple way to reset the device ID (of all 
 *  devices currently connected to the TWI bus!) if you don't know its current value, or to turn off all LEDs 
 *  simultaneously.
 *  
 */
#include <EEPROM.h>
#include "tedi2c.h"

// 7 bit slave I2C address
int SLAVE_ADDR;
// current register address for read/write
volatile uint8_t addr = 0;

volatile uint8_t RGBVals[3];
volatile uint8_t NewAddr = 0;

void setup() {
  int i;
  
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);

  RGBVals[0] = 0;
  RGBVals[1] = 0;
  RGBVals[2] = 0;

  // Look for slave address in EEPROM
  for(i = 0; i < EEPROM.length(); i++) {
    SLAVE_ADDR = EEPROM.read(i);
    if (SLAVE_ADDR > 7 && SLAVE_ADDR < 0x78) break;
  }
  // No address specified ==> use 0x08
  if (SLAVE_ADDR <= 7 || SLAVE_ADDR >= 0x78) SLAVE_ADDR = 0x08;
  
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
}

void loop() {
  int i,j;

  analogWrite(0, RGBVals[0]);
  analogWrite(1, RGBVals[1]);
#ifndef TEDI2C_DEBUG  
  analogWrite(4, RGBVals[2]);
#endif

  if (NewAddr > 7 && NewAddr < 0x78) {
    // Write new adress to EEPROM
    // Valid slave addresses are 0x08-0x77
    int8_t temp;

    for(i = 0; i < EEPROM.length(); i++) {
      temp = EEPROM.read(i);
      if (temp > 7 && temp < 0x78) break;
    }
    if (i < EEPROM.length()) {
      if (temp != NewAddr) {
        EEPROM.write(i, NewAddr);
      }
    } else {
      i--;
      EEPROM.write(i, NewAddr);
    }
    while(EEPROM.read(i) != NewAddr) {
      // Looks like the memory cell is broken, this can happen on EEPROMs. Use the next lower cell
      i--;
      EEPROM.write(i, NewAddr);
    }
    SLAVE_ADDR = NewAddr;
    
    Wire.end();
    // listen to new address
    Wire.begin(SLAVE_ADDR);
    Wire.onReceive(receiveEvent);

    NewAddr = 0;
  }
  
// "Manually" switching bit 4
//  for(i = 0; i < 10; i++) {
//    *pOutRegister2 &= ~bit4;
//    *pOutRegister2 |= bit4;
//  }

// Toggle Bit 4 
//  for(i = 0; i < 21; i++) {
//    *pInRegister2 = bit4;
//  }
  delay(10);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (howMany <= 0) return;
  if (howMany > 4) howMany = 4;

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
    // Address 3 (Slave Address) may only be written if explicitly addressed!
    if (addr >= 3) addr = 0; // start at 0 on register end
    howMany--;
  }
}

