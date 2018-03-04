#ifndef tedi2c_h
#define tedi2c_h

#define TWI_RX_BUFFER_SIZE (16)
#define TWI_RX_BUFFER_MASK (TWI_RX_BUFFER_SIZE - 1)

//#define TEDI2C_DEBUG

// SDA must be Pin 2, the ony one supported for interrupt
#define PIN_SDA 2
// SCL may be any pin
#define PIN_SCL 3

class TedWire {
  public:
    TedWire();
    void begin(uint8_t);
    void end();
    
    int available(void);
    int read(void);
    
    void onReceive( void (*)(int) );
//    void onRequest( void (*)(void) );
//    uint8_t isActive(void);
};

extern TedWire Wire;


#endif
