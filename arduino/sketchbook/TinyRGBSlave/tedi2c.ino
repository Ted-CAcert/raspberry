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

#define TWISTATUS_STOP 0
#define TWISTATUS_START 1
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

void TWI_ISR()
{
  
}

TedWire::TedWire() {
}

void TedWire::begin(uint8_t address) {
  Flush_TWI_Buffers();
  TWI_Status = TWISTATUS_STOP;

  TWI_slaveAddress = address;

  USI_TWI_On_Slave_Receive = 0;
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

