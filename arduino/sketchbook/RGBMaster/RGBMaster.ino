#include <Wire.h>

/* RGB Master
 * currently this is mostly an interface between serial ASCII and TWI bus. 
 * Format of command line:
 * <DeviceID> <Program Step Index> <Red-Value> [ <Green-Value> [ <Blue-Value> [ <Duration&Flags> ]]]
 * DeviceID 0 is broadcast, normal device IDs start are from 8 to 119
 * The Program Step Index ranges from 0 to 126 for an ATTiny85 (or from 0 to 62 on an ATTiny45). Program Step Index 255 sets the DeviceID.
 */

char CmdBuffer[30];
byte BufIDX = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("RGB Master ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
}

void HandleCommand() {
  byte Val[6];
  
  int CurInt = 0;
  byte CurIDX;
  byte NumVals = 0;
  

  for(CurIDX = 0; CurIDX < BufIDX; CurIDX++) {
    if (isDigit(CmdBuffer[CurIDX])) {
      CurInt *= 10;
      CurInt += CmdBuffer[CurIDX] - '0';
    } else {
      if (CurInt > 255) {
        Serial.print("Value ");
        Serial.write(NumVals+1);
        Serial.println(" too big, max. 255");
        return;
      }
      if (NumVals >= sizeof(Val)) {
        Serial.println("Too many values, max. 4");
        return;
      }
      Val[NumVals] = CurInt;
      NumVals++;
      CurInt=0;
    }
  }
  Val[NumVals] = CurInt;
  NumVals++;
  CurInt=0;

  if (NumVals < 3) {
    Serial.println("Too few values, syntax is <DevID> <PSI> <Val1> ...");
    return;
  }
  
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.setClock(31000); // Max. frequency for 8MHz slave is 50 kHz, but 31 kHz leaves a bit reserve
  Wire.beginTransmission(Val[0]); // DeviceID (0 is broadcast)
  for(CurIDX = 1; CurIDX < NumVals; CurIDX++) {
    Wire.write(Val[CurIDX]);
    Serial.print(Val[CurIDX]);
    Serial.print(" ");
  }  
  Wire.endTransmission();    // stop transmitting
  Serial.println("OK");
}

void loop() {
  if (Serial.available()) {
    int CurChar = Serial.read();
    
    if (CurChar > 0) {
      if (isDigit(CurChar)) {
        Serial.write(CurChar); // Echo
        CmdBuffer[BufIDX] = CurChar;
        BufIDX++;
      } else if (CurChar == '\r' || CurChar == '\n') {
        Serial.print("\r\n"); // Echo
        if (BufIDX > 0) {
          HandleCommand();
          BufIDX = 0;
        }
      } else if (isWhitespace(CurChar)) {
        if (BufIDX > 0 && CmdBuffer[BufIDX-1] != '  ') {
          Serial.write(' '); // Echo
          CmdBuffer[BufIDX] = ' ';
          BufIDX++;
        } // Ignore leading spaces
      } // Ignore everything else

      if (BufIDX >= sizeof(CmdBuffer)-1) {
        Serial.print("\r\n");
        Serial.print("Buffer overflow, max ");
        Serial.write(sizeof(CmdBuffer)-1);
        Serial.println(" characters!");
        BufIDX = 0;
      }
    }
  }

  delay(1);
}
