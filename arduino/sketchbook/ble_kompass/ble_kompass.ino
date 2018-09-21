/* Compass Test */


// Reference the I2C Library
//#include <Wire.h>
// Reference the HMC5883L Compass Library
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#include "BluefruitConfig.h"
#include <HMC5883L.h>

// Store our compass as a variable.
HMC5883L compass;
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// Record any errors that may occur in the compass.
// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];

int error = 0;

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  // Initialize the serial port.
  Serial.begin(9600);
delay(10000);
  Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.

  Serial.println("Constructing new HMC5883L");
    
  Serial.println("Setting scale to +/- 0.88 Ga");
  error = compass.setScale(0.88); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.getErrorText(error));
  
  Serial.println("Setting measurement mode to continous.");
  error = compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.getErrorText(error));

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Teddys BLE 1" )) ) {
    error(F("Could not set device name?"));
  }
}

void loop() {
  union {
    uint8_t buffer[6];
    struct {
      int16_t X;
      int16_t Z;
      int16_t Y;
    } Val;
  } b;
  
  
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(3);
  Wire.endTransmission();
  
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.requestFrom(HMC5883L_ADDRESS, 6);

  if(Wire.available() == 6)
  {
    for(uint8_t i = 0; i < 6; i+=2)
    {
      b.buffer[i+1] = Wire.read();
      b.buffer[i] = Wire.read();
    }
  }
  Wire.endTransmission();
  
  ble.print(b.Val.X);
  ble.print("\t");
  ble.print(b.Val.Y);
  ble.print("\t");
  ble.print(b.Val.Z);
  ble.print("\n");
  
  delay(5000);
}
