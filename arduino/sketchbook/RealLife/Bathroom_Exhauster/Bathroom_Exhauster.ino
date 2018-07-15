/*
   Bathroom exhauster control

   Turns on the exhauster on high humidity (and maybe other conditions)

*/

#include "TinySerial.h"
#include <USIWire.h>

// Note that I2C uses Pins 0 and 2
const int StatusLED = 4; // Should support PWM
const int ActionPin = 3;
const bool InvertAction = false; // false means that HIGH on ActionPin turns on the exhauster
const int TxPin = 1; // Output pin for serial debug info
const int SerialBaud = 19200; // Heuristically this was the highest rate working with 8MHz int. clock
const int SensorAddress = 0x40;
const int Threshold = 80;

struct _DataBuf {
  float HumidityPercent;
  float TempCelsius;
};
TinySerial mySerial(TxPin);

uint8_t DeviceID;

uint8_t checkID()
{
  uint8_t ID_1;

  // Check device ID
  Wire.beginTransmission(SensorAddress);
  Wire.write(0xFC);
  Wire.write(0xC9);
  Wire.endTransmission();

  Wire.requestFrom(SensorAddress, 1);

  ID_1 = Wire.read();

  return (ID_1);
}

void setup() {

  pinMode(StatusLED, OUTPUT);
  pinMode(ActionPin, OUTPUT);
  mySerial.begin(SerialBaud);
  mySerial.println("Bathroom_Exhauster");
  mySerial.print(__DATE__);
  mySerial.print(" ");
  mySerial.println(__TIME__);
  Wire.begin();
  DeviceID = checkID();

  switch (DeviceID) {
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
  digitalWrite(StatusLED, HIGH);
  digitalWrite(ActionPin, LOW);
}

bool DoCommand(int TheCmd, int Numbytes)
{
  int counter = 0;

  Wire.beginTransmission(SensorAddress);
  Wire.write(TheCmd); // Rel. Humidity, no hold
  Wire.endTransmission();

  delay(100);

  Wire.requestFrom(SensorAddress, Numbytes);

  while (Wire.available() < Numbytes) {
    delay(1);
    counter++;
    if (counter > 100) {
      mySerial.print("Timeout waiting for data, available ");
      mySerial.print(Wire.available());
      mySerial.print(", wanted ");
      mySerial.println(Numbytes);
      return false;
    }
  }

  return true;
}

bool FetchData(struct _DataBuf *pBuffer)
{
  uint16_t Result;
  unsigned char msb;
  unsigned char lsb;
  unsigned char crc;

  if (!DoCommand(0xF5, 3)) return false;

  msb = Wire.read();
  lsb = Wire.read();
  crc = Wire.read();

#ifdef DEBUG
  mySerial.print("Hum Raw  0x");
  mySerial.print(msb, HEX);
  if (lsb < 16) mySerial.print("0");
  mySerial.print(lsb, HEX);
  mySerial.print(", CRC: 0x");
  mySerial.println(crc, HEX);
#endif

  lsb &= 0xFC;
  Result = msb << 8 | lsb;

  pBuffer->HumidityPercent = ((125.0 * Result) / 65536) - 6;

  if (!DoCommand(0xE0, 2)) return false;
  //  if (!DoCommand(0xF3, 3)) return false;

  msb = Wire.read();
  lsb = Wire.read();
  //  crc = Wire.read();
#ifdef DEBUG
  mySerial.print("Temp Raw 0x");
  mySerial.print(msb, HEX);
  if (lsb < 16) mySerial.print("0");
  mySerial.print(lsb, HEX);
  //  mySerial.print(", CRC: 0x");
  //  mySerial.println(crc, HEX);
#endif
  lsb &= 0xFC;
  Result = msb << 8 | lsb;

  pBuffer->TempCelsius = ((175.72 * Result) / 65536) - 46.85;

  return true;
}

void HandleAction(struct _DataBuf *pBuffer)
{
  if (pBuffer->HumidityPercent >= Threshold) {
    digitalWrite(ActionPin, InvertAction ? LOW : HIGH);
  } else {
    digitalWrite(ActionPin, InvertAction ? HIGH : LOW);
  }
}

void ShowStatusLED(char Ratio, int NumSeconds)
{
  int NumDone = 0;

  if (Ratio > 100) Ratio = 100;

  while(NumDone < NumSeconds) {
    NumDone++;
    if (Ratio < 0) {
      digitalWrite(StatusLED, HIGH);
      delay(250);
      digitalWrite(StatusLED, LOW);
      delay(250);
      digitalWrite(StatusLED, HIGH);
      delay(250);
      digitalWrite(StatusLED, LOW);
      delay(250);
    } else {
      analogWrite(StatusLED, 255);
      delay(500);
      analogWrite(StatusLED, (255*Ratio)/100);
      delay(500);
    }
  }
}
void loop() {
  struct _DataBuf MyBuf;
  char Ratio;

  digitalWrite(StatusLED, LOW);
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

    HandleAction(&MyBuf);
    if (MyBuf.HumidityPercent >= Threshold) {
      Ratio = 100;
    } else {
      Ratio = 90  - (Threshold - MyBuf.HumidityPercent)*3;
      if (Ratio < 1) Ratio = 1;
    }
  } else {
    mySerial.println("FetchData failed");
    Ratio = -1;
  }

  ShowStatusLED(Ratio, 60);
}
