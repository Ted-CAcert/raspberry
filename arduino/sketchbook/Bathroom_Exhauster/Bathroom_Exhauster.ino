/*
 * Bathroom exhauster control
 * 
 * Turns on the exhauster on high humidity (and maybe other conditions)
 * 
 */

#include <SoftwareSerial.h>
#include <Wire.h>

// Note that I2C uses Pins 0 and 2
const int StatusLED = 4; // Should support PWM
const int ActionPin = 3; 
const bool InvertAction = false; // false means that HIGH on ActionPin turns on the exhauster
const int TxPin = 1; // Output pin for serial debug info
const int DummyPin = 9; // FixMe: Pin used for serial input, since SoftwareSerial needs an RX pin
const int SerialBaud = 115200;
const int SensorAddress = 0x40;

struct _DataBuf {
  float HumidityPercent;
  float TempCelsius;
};

SoftwareSerial mySerial(DummyPin, TxPin);

uint8_t DeviceID;

uint8_t checkID()
{
  uint8_t ID_1;

  // Check device ID
  Wire.beginTransmission(SensorAddress);
  Wire.write(0xFC);
  Wire.write(0xC9);
  Wire.endTransmission();

  Wire.requestFrom(SensorAddress,1);

  ID_1 = Wire.read();

  return(ID_1);
}

void setup() {
  
  pinMode(StatusLED, OUTPUT);
  pinMode(ActionPin, OUTPUT);
  Wire.begin();
  DeviceID = checkID();

  switch(DeviceID) {
    case 0x15:
      mySerial.println("Si7021");
      break;

    case 0x32:
      mySerial.println("HTU21D");
      break;

    default:
      mySerial.print("Unknown ID ");
      mySerial.println(DeviceID, HEX);
  }
}

bool DoCommand(int TheCmd, int Numbytes)
{
  int counter = 0;
  
  Wire.beginTransmission(SensorAddress);
  Wire.write(0xF5); // Rel. Humidity, no hold
  Wire.endTransmission();

  delay(100);

  Wire.requestFrom(SensorAddress, 3);
  
  while(Wire.available() < 3) {
    delay(1);
    counter++;
    if (counter > 100) {
      return false;
    }
  }

  return true;
}

bool FetchData(struct _DataBuf *pBuffer)
{
  uint16_t Result;
  unsigned int msb;
  unsigned int lsb;

  if (!DoCommand(0xF5, 3)) return false;

  msb = Wire.read();
  lsb = Wire.read();

  lsb &= 0xFC;
  Result = msb << 8 | lsb;

  pBuffer->HumidityPercent = (125.0*Result/65536)-6;


  if (!DoCommand(0xE0, 2)) return false;
  
  msb = Wire.read();
  lsb = Wire.read();

  lsb &= 0xFC;
  Result = msb << 8 | lsb;

  pBuffer->TempCelsius = (175.72*Result/65536)-46.85;

  return false;
}

void loop() {
  struct _DataBuf MyBuf;

  if (DeviceID != 0x15 && DeviceID != 0x32) {
    mySerial.print("Warning: Unknown DeviceID ");
    mySerial.println(DeviceID, HEX);
  }
  if (FetchData(&MyBuf)) {
    mySerial.print("Hum: ");
    mySerial.print(MyBuf.HumidityPercent);
    mySerial.print("%, Temp: ");
    mySerial.print(MyBuf.TempCelsius);
    mySerial.println("°C");
  } else {
    mySerial.println("FetchData failed");
  }
  delay(5000);
}
