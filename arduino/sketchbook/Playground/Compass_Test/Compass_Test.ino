/* Compass Test */


// Reference the I2C Library
#include <Wire.h>
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur in the compass.
int error = 0;

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  // Initialize the serial port.
  Serial.begin(9600);

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
  
  Serial.print(b.Val.X);
  Serial.print("\t");
  Serial.print(b.Val.Y);
  Serial.print("\t");
  Serial.print(b.Val.Z);
  Serial.print("\n");
  
  delay(100);
}
