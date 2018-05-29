/* 
 *  TinyRGBSlave has been tested on a ATTiny85 at 8MHz.
 *  In theory it should also run on an ATTiny45, needing about 2k program memory and maybe 100 Bytes of RAM.
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
 *  Each command consists of 4 bytes:
 *  Byte 0:  PWM value for PB1 (red)
 *  Byte 1:  PWM value for PB0 (green)
 *  Byte 2:  PWM value for PB4 (blue)
 *  Byte 3:  Duration and flags
 *  
 *  After a reset the first command is read from EEPROM addresses 0-3 and executed. Asuming it is not the final step, 
 *  the next step is read from EEPROM addresses 4-7 and so on, up to the size of the EEPROM minus one command.
 *  So on an ATTiny85 (512 Bytes of EEPROM) you can store a total of 127 commands.
 *  
 *  Byte 3 of a command is a bitmask:
 *  If Bit 7 (MSB) is set, the RGB values are interpolated during the duration, so they slowly approach the value set in 
 *  the next command.
 *  Bit 6-4 select the unit of the duration:
 *    000: 10ths of seconds
 *    001: seconds
 *    010: tens of seconds
 *    011: hundreds of seconds
 *    100: thousends of seconds
 *    101 to 111: for future use
 *  Bit 3-0 set the duration of the command as the amount of units (set in bits 2-0).
 *    
 *  If byte 3 is set to 0xFF this step is skipped and the program starts again at address 0
 *  
 *  Examples:
 *  Byte 3 of (decimal) 1 to 15 sets a constant RGB value for 0.1 to 1.5 seconds
 *  Byte 3 of 1 011 0110 = 0xB6 = 182 sets the RGB value changing from the value given in this command to the value given in the next command 
 *    during 600 seconds
 *  
 *  
 *  You can write your program into the EEPROM by sending messages like this on the TWI/I2C bus:
 *  <DeviceID of ATTiny> <Program Step Index> <Red-Value> <Green Value> <Blue Value> <Duration&Flags>
 *  
 *  Device ID 0 is the broadcast ID, so every device will execute the transmitted message
 *  
 *  Initial DeviceID is 8, the DeviceID may be changed by sending the new DeviceID as the red value of Program Step Index 255.
 *  Valid DeviceIDs are from 8 to 119, the command to set an invalid deviceid is ignored.
 *  
 *  If you write Program Step Index 254, the Red value will be used as the next PSI to be executed.
 *  
 *  The Program Step Index ranges from 0 to 126 for an ATTiny85 (or from 0 to 62 on an ATTiny45). Program Step Index 255 sets the DeviceID.
 *  
 *  After one message is sent the device needs about 15 ms to write the data to the EEPROM, during this time it will not accept another
 *  message!
 */
#include <EEPROM.h>
#include "tedi2c.h"

typedef struct {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  union {
    uint8_t FlagDuration;
    struct {
      unsigned Amount:4;
      unsigned Unit:3;
      unsigned Flag:1;
    };
  };
} PROGRAM_STEP;

#define RED_PIN 1
#define GREEN_PIN 0
#define BLUE_PIN 4

//#define DEBUG

// 7 bit slave I2C address
int SLAVE_ADDR;

volatile uint8_t NewAddr=0;
volatile uint8_t NewPSI=0xFF;

PROGRAM_STEP CurStep, NextStep;

uint8_t MaxPSI; // Maximum Program Step Index, depends on EEPROM size
uint8_t CurPSI = 0; // Step to load next
unsigned long StartTick=0, EndTick=0;

void setup() {
  int i;

  MaxPSI = (EEPROM.length()/4) - 1;
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Look for slave address in the top 4 addresses of the EEPROM
  for(i = EEPROM.length()-1; i >= EEPROM.length()-4; i--) {
    SLAVE_ADDR = EEPROM.read(i);
    if (SLAVE_ADDR > 7 && SLAVE_ADDR < 0x78) break;
  }
  // No valid address found ==> use 0x08
  if (SLAVE_ADDR <= 7 || SLAVE_ADDR >= 0x78) SLAVE_ADDR = 0x08;
  
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
}

#ifdef DEBUG
void debug_flip() {
  static char Status = 0;

  Status ^= 1;
  if (Status) analogWrite(BLUE_PIN, 64);
  else analogWrite(BLUE_PIN, 0);
}
#else
void debug_flip() { }
#endif

void loop() {
  int i,j;

  if (NewAddr > 7 && NewAddr < 0x78) {
    SLAVE_ADDR = NewAddr;
    for(i = EEPROM.length()-1; i >= EEPROM.length()-4; i--) {
      EEPROM.update(i, NewAddr);
      if (EEPROM.read(i) == NewAddr) break;
      // Memory cell is defect, try to set it to an invalid address
      EEPROM.update(i, 0);
      if (EEPROM.read(i) > 7 && EEPROM.read(i) < 0x78) {
        EEPROM.update(i, 0xFF);
      }
    }
    
    Wire.end();
    // listen to new address
    Wire.begin(SLAVE_ADDR);
    Wire.onReceive(receiveEvent);

    NewAddr = 0;
  }

  if (NewPSI != 0xFF) {
    CurPSI = NewPSI;
    NewPSI = 0xFF;
    EndTick = 0;
  }
  
  // Check for next step
  if (millis() >= EndTick) {
    CurStep.Red = EEPROM.read(CurPSI*4);
    CurStep.Green = EEPROM.read(CurPSI*4+1);
    CurStep.Blue = EEPROM.read(CurPSI*4+2);
    CurStep.FlagDuration = EEPROM.read(CurPSI*4+3);
    if (CurStep.FlagDuration == 0xff) {
      CurPSI = 0;      
      CurStep.Red = EEPROM.read(CurPSI*4);
      CurStep.Green = EEPROM.read(CurPSI*4+1);
      CurStep.Blue = EEPROM.read(CurPSI*4+2);
      CurStep.FlagDuration = EEPROM.read(CurPSI*4+3);
    }
    
    CurPSI++;
    if (CurPSI > MaxPSI) CurPSI = 0;
    
    NextStep.Red = EEPROM.read(CurPSI*4);
    NextStep.Green = EEPROM.read(CurPSI*4+1);
    NextStep.Blue = EEPROM.read(CurPSI*4+2);
    NextStep.FlagDuration = EEPROM.read(CurPSI*4+3);
    
    StartTick = millis();
    switch(CurStep.Unit) {
      case 0:
        EndTick = StartTick + CurStep.Amount*100;
        break;

      case 1:
        EndTick = StartTick + CurStep.Amount*1000;
        break;

      case 2:
        EndTick = StartTick + CurStep.Amount*10000;
        break;

      case 3:
        EndTick = StartTick + CurStep.Amount*100000;
        break;

      default:
        EndTick = StartTick + CurStep.Amount*2000;
        break;
    }
    // Handle overflow...
    if (EndTick < StartTick) EndTick = 0xFFFFFFFF;
  }

  // Set RGB Values
  if (CurStep.Flag == 0 || CurStep.FlagDuration == 0xff) {
    // Simple thing: Just constant
    analogWrite(RED_PIN, CurStep.Red);
    analogWrite(GREEN_PIN, CurStep.Green);
#ifndef DEBUG    
    analogWrite(BLUE_PIN, CurStep.Blue);
#endif    
    if (EndTick - millis() > 100) {
      delay(100); // Just check every for changes every 100ms
    } else {
      delay(EndTick - millis());
    }
  } else {
    // Interpolated...
    float CurVal;
    unsigned long len;

    len = EndTick - StartTick;
    if (len < 1) len = 1;

    // To avoi
    // CurVal = CurStep.Red + (NextStep.Red-CurStep.Red)*(millis()-StartTick)/len;
    CurVal = (millis()-StartTick);
    CurVal /= len;
    CurVal *= (NextStep.Red-CurStep.Red);
    CurVal += CurStep.Red;    
    analogWrite(RED_PIN, CurVal);
    CurVal = (millis()-StartTick);
    CurVal /= len;
    CurVal *= (NextStep.Green-CurStep.Green);
    CurVal += CurStep.Green;    
    analogWrite(GREEN_PIN, CurVal);
#ifndef DEBUG    
    CurVal = CurStep.Blue + (NextStep.Blue-CurStep.Blue)*(millis()-StartTick)/len;    
    CurVal = (millis()-StartTick);
    CurVal /= len;
    CurVal *= (NextStep.Blue-CurStep.Blue);
    CurVal += CurStep.Blue;    
    analogWrite(BLUE_PIN, CurVal);
#endif    

    if (len > 25500) delay(100);
    else delay(1+(len>>8));
  }
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  uint16_t addr;
  
  if (howMany <= 0) return;
  if (howMany > 5) howMany = 5;

  // read Program Step Index
  addr = Wire.read();
  if (addr == 0xFF && howMany > 1) {
    // new device addr
    NewAddr = Wire.read();
    if (NewAddr > 7 && NewAddr < 0x78) {
      // Nothing to do here, the new address is set in the next loop()   
    } else {
      NewAddr = 0;
    }
  } else if (addr == 0xFE && howMany > 1) {
    NewPSI = Wire.read();
    if (NewPSI > MaxPSI) NewPSI = 0xFF; // No way!
  } else {
    // Program Step
    addr *= 4; // Translate to EEPROM address
    if (addr >= EEPROM.length()) return;
    
    howMany--;
    while (howMany) {
      EEPROM.update(addr, Wire.read());
      addr++;
      howMany--;
    }
  }    
}

