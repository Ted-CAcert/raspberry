/*
 * This is very basic and not portable...
 * 
 * I'm trying to avoid as much overhead as possible since for a 8MHz ATTiny clock it's very hard
 * to support a 100kHz TWI clock frequency. In fact, using 50 kHz seem to work with occasional dropped
 * transactions. At 31 kHz this seems to work quite reliably.
 * 
 */
 
TedWire Wire = TedWire();

void (*USI_TWI_On_Slave_Receive)(int);

uint8_t TWI_RxBuf[TWI_RX_BUFFER_SIZE];

static unsigned char TWI_slaveAddress;

static volatile uint8_t TWI_RxHead;
static volatile uint8_t TWI_RxTail;

static volatile uint8_t *pInRegister;
static volatile uint8_t *pOutRegister;
static volatile uint8_t *pModeRegister;
static uint8_t bitSDA;
static uint8_t bitSCL;

static uint8_t TWI_curByte;
static uint8_t TWI_curBit;

#define TWISTATUS_STOP 0
#define TWISTATUS_START 1
#define TWISTATUS_SEND 2
#define TWISTATUS_REQUEST 3
#define TWISTATUS_ERROR 0xF
static volatile uint8_t TWI_Status;

#ifdef DEBUG
static uint8_t bit4;
#endif

// This function is not part of the library for ATTiny...
// On ATTiny logical pin 2 (PB2) serves as External Interrupt 0 (INT0),
// which seems to be the interrupt type handled by attachInterrupt
//
// All data pins support the "Pin Change Interrupt" (PCINTx), but I
// don't know how to add a handler for that yet...
int digitalPinToInterrupt(int logicalPin) {
  return (logicalPin == 2 ? 0 : -1);
}



void Flush_TWI_Buffers(void)
{
  TWI_RxTail = 0;
  TWI_RxHead = 0;
}

// Check if there is data in the receive buffer.
unsigned char USI_TWI_Data_In_Receive_Buffer(void)
{
  unsigned char tmpRxTail;          // Temporary variable to store volatile
  tmpRxTail = TWI_RxTail;           // Not necessary, but prevents warnings
  return (TWI_RxHead - tmpRxTail) & TWI_RX_BUFFER_MASK; // Return 0 (FALSE) if the receive buffer is empty.
}

// Doing this inline avoids the performanve overhead of a function call at the cost of bigger code
inline bool WaitLowSCL(uint16_t TimeOut) {
  uint8_t LastState;

  // Each loop will need at least (probably more, I'm just guessing roughly), or 1 us
  LastState = (*pInRegister & bitSDA);
  while(*pInRegister & bitSCL) {      
    if ((*pInRegister & bitSDA) != LastState) {
      // SDA changing while SCL is HIGH is a START or STOP status, so I'm aborting the read
      if (LastState) {
        // In fact this is a "REPEATED START"
        TWI_Status = TWISTATUS_ERROR; // Sorry, we can't handle this...
      } else {
        TWI_Status = TWISTATUS_STOP;          
      }
      return false;
    }
    if (--TimeOut == 0) {
      TWI_Status = TWISTATUS_ERROR;
      return false;
    }
  }
  
  return true;
}

uint8_t TWI_ReadByte() {  

  TWI_curBit = 0;
  TWI_curByte = 0;
  
  // SCK is probably still HIGH wenn we arrive here, so, first wait for LOW SCL
  // Using a rough timeout to avoid getting completely stuck by noise
  while(!(TWI_curBit & 0x08)) {
    if (!WaitLowSCL(1000)) return 0;
    
    // Now wait for SCL to go HIGH again. No timeout checking here, there may be clock stretching,
    // and the idle level of SCL is HIGH ...
    while(!(*pInRegister & bitSCL));

    //SCL is HIGH again, sample SDA now.
    TWI_curByte<<=1;
    if (*pInRegister & bitSDA) {
      TWI_curByte |= 1;
    }
    TWI_curBit++;
  }
  
  return TWI_curByte;  
}

void TWI_SendACK() {
  uint16_t TimeOut;
  uint8_t LastState;

  if (!WaitLowSCL(1000)) return;

  // Now that SCL is LOW configure SDA as output and pull it low
  // Disabling interrupts is not necessary, as we are already in an interrupt handler
  // where interrupts are disabled anyway

  *pModeRegister |= bitSDA;
  *pOutRegister &= ~bitSDA;

  while(!(*pInRegister & bitSCL));

  // Now wait for LOW SCL again before resetting SDA as input
  WaitLowSCL(1000);
  *pModeRegister &= ~bitSDA;
  *pOutRegister &= ~bitSDA; // Disable pull-up, just to be sure  
}

void TWI_SendNACK() {
  // Just do nothing, should count as NACK...
  return;
}


/* This is called on a CHANGE event of PIN_SDA.
   If SDA changes while SCL is HIGH this is either a START event 
   (if SDA goes LOW) or a STOP event (if SDA goes HIGH) of the 
   I2C bus protocol.
*/
void TWI_ISR()
{  
  // Check current status of SCL
    if (*pInRegister & bitSCL) {
    if (*pInRegister & bitSDA) {
      // This is a STOP condition...
      TWI_Status = TWISTATUS_STOP;
    } else {
      // Hmm, what if there was already a START without STOP? Currently ignoring that...
      uint8_t CurByte;
      
     TWI_Status = TWISTATUS_START;
     CurByte = TWI_ReadByte();
      // There might have been a STOP during receiving...
      if ((TWI_Status == TWISTATUS_START) && (CurByte == 0 || CurByte>>1 == TWI_slaveAddress)) {
        unsigned char tmpRxHead;
        unsigned char tmpTxTail; // Temporary variables to store volatiles
        
#ifdef TEDI2C_DEBUG
  *pInRegister = bit4;
#endif
        // CurByte == 0 is broadcast...
        if (CurByte & 1) {
          TWI_Status = TWISTATUS_SEND;
          // Ignoring this at the moment...
        } else {
          TWI_Status = TWISTATUS_REQUEST;
          TWI_SendACK();
          while(TWI_Status == TWISTATUS_REQUEST) {
            CurByte = TWI_ReadByte();

            if (TWI_Status != TWISTATUS_REQUEST) break;
#ifdef TEDI2C_DEBUG
  *pInRegister = bit4;
#endif
            tmpRxHead = (TWI_RxHead + 1) & TWI_RX_BUFFER_MASK;
            if (TWI_RxTail != tmpRxHead) {
              TWI_RxBuf[TWI_RxHead] = CurByte;
              TWI_RxHead            = tmpRxHead;
              TWI_SendACK();
            } else // If the buffer is full then:
            {
              TWI_SendNACK();
              break;
            }
          }
#ifdef TEDI2C_DEBUG
  *pInRegister = bit4;
#endif
          if (TWI_RxHead != TWI_RxTail && USI_TWI_On_Slave_Receive) {
            USI_TWI_On_Slave_Receive(USI_TWI_Data_In_Receive_Buffer());
          }
          Flush_TWI_Buffers();
        }
      }
    }
  }
}

TedWire::TedWire() {
  // This is assuming we are on ATTiny, where we have only pone port B
  // so I'm saving the second set of register pointers
  // Using pointers to (hopefully) save cycles...
  pInRegister = portInputRegister(digitalPinToPort(PIN_SDA));
  pOutRegister = portOutputRegister(digitalPinToPort(PIN_SDA));
  pModeRegister = portModeRegister(digitalPinToPort(PIN_SDA));
  bitSDA = digitalPinToBitMask(PIN_SDA);
  bitSCL = digitalPinToBitMask(PIN_SCL);  

#ifdef DEBUG
  bit4 = digitalPinToBitMask(4);
#endif
}

void TedWire::begin(uint8_t address) {
  Flush_TWI_Buffers();
  TWI_Status = TWISTATUS_STOP;

  TWI_slaveAddress = address;

  // Set both pins to INPUT, adapted from wiring_digital.c
  { 
    uint8_t oldSREG = SREG;
    cli();
    *pModeRegister &= ~(bitSDA | bitSCL);
    *pOutRegister &= ~(bitSDA | bitSCL);  // Disable pullup resistors
    SREG = oldSREG;
  }  

  attachInterrupt(digitalPinToInterrupt(PIN_SDA), TWI_ISR, FALLING);
}

void TedWire::end(void) {
  detachInterrupt(digitalPinToInterrupt(PIN_SDA));
}

int TedWire::available(void) {
  return TWI_RxHead - TWI_RxTail;
}

int TedWire::read(void) {
  int Ret = TWI_RxBuf[TWI_RxTail];

  TWI_RxTail = (TWI_RxTail + 1) & TWI_RX_BUFFER_MASK;
  return Ret;
}

// sets function called on slave write
void TedWire::onReceive( void (*function)(int) ) {
  USI_TWI_On_Slave_Receive = function;
}

