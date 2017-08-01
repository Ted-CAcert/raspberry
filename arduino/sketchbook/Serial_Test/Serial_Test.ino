/* Serial Control test */
#define CMDBUFSIZE 50

int inByte = 0;         // incoming serial byte
char CmdLine[CMDBUFSIZE];
int CmdByte = 0;

int value[5];
char *ValStrings[] = {
  "Sensor 1",
  "Sensor 2",
  "Sensor 3",
  "Sensor 4",
  NULL
};

void setup()
{
  Serial.begin(9600);
  
  Serial.println("\r\nSerial_Test Ready.");
  CmdLine[CmdByte] = 0;
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);
}

void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    // Echo
    Serial.write(inByte);
    if (inByte == '\r') {
      Serial.print("\n");
      if (strcmp(CmdLine, "list") == 0) {
        int i;
        
        for(i = 0; i < 4; i++) {
          Serial.print(ValStrings[i]);
          Serial.print(": ");
          Serial.println(value[i]);
        }
      } else if (strncmp(CmdLine, "set ",4) == 0) {
        do {
          int i;
          int ValIndex;
          int NewVal;          
          String TheCmd;
          
          TheCmd = String(CmdLine).substring(4);
          TheCmd.trim();
          i = TheCmd.indexOf(' ');
          if (i <= 0) {
            Serial.println("set Syntax Error");
            break;
          }
          ValIndex = TheCmd.substring(0, i).toInt();
          TheCmd = TheCmd.substring(i+1);
          NewVal = TheCmd.toInt();
          
          if (ValIndex < 1 || ValIndex > 4) {
            Serial.print("Index out of bounds: ");
            Serial.println(ValIndex);
            break;
          }
          value[ValIndex-1] = NewVal;
          if (ValIndex <= 3) {
            analogWrite(8+ValIndex, NewVal);
          }
          Serial.println("set received");
        } while(0);
      } else {
        Serial.println("Unknown command!");
      }
      CmdByte = 0;
    } else if (inByte == '\n') {
      // Do nothing.
    } else {
      if (CmdByte >= CMDBUFSIZE-1) {
        Serial.println("");
        Serial.println("Buffer overflow!");
        CmdByte = 0;
      } else {
        CmdLine[CmdByte] = inByte;
        CmdByte++;
      }
    }
    CmdLine[CmdByte] = 0;
  }
}

