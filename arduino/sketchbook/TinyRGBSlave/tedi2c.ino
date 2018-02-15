/*
 * This is very basic and not portable...
 */

TedWire Wire = TedWire();

void (*USI_TWI_On_Slave_Receive)(int);

uint8_t TWI_RxBuf[TWI_RX_BUFFER_SIZE];

static unsigned char TWI_slaveAddress;

static volatile uint8_t TWI_RxHead;
static volatile uint8_t TWI_RxTail;

static volatile uint8_t TWI_curByte;
static volatile uint8_t TWI_curBit;

static volatile uint8_t *pInRegister;
static volatile uint8_t *pOutRegister;
static volatile uint8_t *pModeRegister;
static uint8_t bitSDA;
static uint8_t bitSCL;


#define TWISTATUS_STOP 0
#define TWISTATUS_START 1
#define TWISTATUS_SEND 2
#define TWISTATUS_REQUEST 3
static volatile uint8_t TWI_Status;

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

uint8_t TWI_ReadByte() {
  // ToDo...
  TWI_Status = TWISTATUS_STOP;
  return 0;  
}

void TWI_SendACK() {
  // ToDo...
}

void TWI_SendNACK() {
  // ToDo...
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
      if ((TWI_Status = TWISTATUS_START) && (CurByte == 0 || CurByte>>1 == TWI_slaveAddress)) {
        unsigned char tmpRxHead;
        unsigned char tmpTxTail; // Temporary variables to store volatiles
        
        // CurByte == 0 is broadcast...
        if (CurByte & 1) {
          TWI_Status = TWISTATUS_SEND;
          // Ignoring this at the moment...
        } else {
          TWI_Status = TWISTATUS_REQUEST;
          TWI_SendACK();
          while(TWI_Status == TWISTATUS_SEND) {
            CurByte = TWI_ReadByte();
            
            tmpRxHead = (TWI_RxHead + 1) & TWI_RX_BUFFER_MASK;
            if (TWI_RxTail != tmpRxHead) {
              TWI_RxHead            = tmpRxHead;
              TWI_RxBuf[TWI_RxHead] = CurByte;
              TWI_SendACK();
            } else // If the buffer is full then:
            {
              TWI_SendNACK();
              break;
            }
          }
          if (TWI_RxHead != TWI_RxTail && USI_TWI_On_Slave_Receive) {
            USI_TWI_On_Slave_Receive(USI_TWI_Data_In_Receive_Buffer());
          }
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
    *pOutRegister &= ~(bitSDA | bitSCL);
    SREG = oldSREG;
  }  

  attachInterrupt(digitalPinToInterrupt(PIN_SDA), TWI_ISR, CHANGE);
}

void TedWire::end(void) {
  detachInterrupt(digitalPinToInterrupt(PIN_SDA));
}

int TedWire::available(void) {
  return 0;
}

int TedWire::read(void) {
  return 0;
}

// sets function called on slave write
void TedWire::onReceive( void (*function)(int) ) {
  USI_TWI_On_Slave_Receive = function;
}

