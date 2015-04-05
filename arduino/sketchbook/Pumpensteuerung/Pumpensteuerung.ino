#include <LiquidCrystal.h>
#include <OneWire.h>
#include <EEPROM.h>
#include <PString.h>

#define MODE_AUTO 0
#define MODE_ON 1
#define MODE_OFF 2
#define MODE_T_ON 3
#define MODE_T_OFF 4

//LiquidCrystal lcd(2, 3, 4,  8, 9, 10, 11, 12, 13, 5, 6);
LiquidCrystal lcd(8, 7, 6,   4, 3, 12, 13);
OneWire ow(9);

const int PinRotaryA=2;
const int PinRotaryB=10;
// Produktiv
const int PinPush=11;
// Für Tests wegen Kollistion mit MOSI-Signal
//const int PinPush=0;
const int PinBacklight=5;
const int PinPumpe=1;
const long SampleMSecs = 10000;

// Wanted temperatore in Celsius. Currently this cannot be changed, ToDo: extend Menu
int TargetTemp = 60;

volatile int Counter = 0;
volatile unsigned long LastTick = 0;

int LastCount = 0;
unsigned long LastActionTick = 0;
unsigned long LastPumpChange;
unsigned long LastPumpRun;
unsigned long LastLoop = 0;
int WantedPumpState = 0;
int LastPush = 0;
unsigned long RemainingTemp = 0;

struct Status_Struct {
  byte CurMode;
  byte KesselSensor[8];
  byte WasserSensor[8];
  uint8_t crc8;
} Status;

int CurMenu = 0;
int CurItem1;
int CurItem2;
int CurItem3;

const int NumMenu1=6;
const char *Menu1[] = {
  "AUTO",
  "EIN",
  "AUS",
  "Temp. EIN",
  "Temp. AUS",
  "EINSTELLUNGEN",
  NULL
};

const int NumMenu2=2;
const char Menu2[][16] = {
  { 'W', 'A', 'S', 'S', 'E', 'R', 'F', -11, 'H', 'L', 'E', 'R', 0 },
  { 'K', 'E', 'S', 'S', 'E', 'L', 'F', -11, 'H', 'L', 'E', 'R', 0 },
  { 0 }
};

const int NumMenu2_34=24;
const char *Menu2_34[] = {
  "1 Stunde",
  "2 Stunden",
  "3 Stunden",
  "4 Stunden",
  "5 Stunden",
  "6 Stunden",
  "7 Stunden",
  "8 Stunden",
  "9 Stunden",
  "10 Stunden",
  "11 Stunden",
  "12 Stunden",
  "13 Stunden",
  "14 Stunden",
  "15 Stunden",
  "16 Stunden",
  "17 Stunden",
  "18 Stunden",
  "19 Stunden",
  "20 Stunden",
  "21 Stunden",
  "22 Stunden",
  "23 Stunden",
  "24 Stunden",
  NULL
};

byte SensorList[5][8];
int NumSensors = 0;

void pinCount()
{
  if ((digitalRead(PinRotaryA) == LOW) && (millis()-LastTick > 10)) {
    LastTick = millis();
    if (digitalRead(PinRotaryB) == LOW)
      Counter++;
    else
      Counter--;
  }
}

void StoreStatus(uint16_t Offset)
{
  unsigned int i;
  uint8_t *pStatus = (uint8_t *)&Status;
    
  if (Offset < 2) {
    // Lese Offset aus EEPROM
    Offset = ((uint16_t)EEPROM.read(0))*256 + EEPROM.read(1);
  }
  // Generiere Prüfsumme
  Status.crc8 = OneWire::crc8(pStatus, sizeof(Status)-sizeof(Status.crc8));
  
  for(i = 0; i < sizeof(Status); i++) {
    EEPROM.write(Offset+i, pStatus[i]);
  }
}

void InitStatus(uint16_t Offset)
{
  int i;
  uint16_t OldOffset;
  
  OldOffset = ((uint16_t)EEPROM.read(0))*256 + EEPROM.read(1);
  if (Offset != OldOffset) {
    if (1 < Offset) {
      EEPROM.write(0,(byte)(Offset>>8));
      EEPROM.write(1,(byte)(Offset&0xFF));
    } else {
      Offset = OldOffset;
    }
  }
  
  Status.CurMode = MODE_OFF; // Keine Sensoren konfiguriert!
  for(i = 0; i < 8; i++) {
    Status.KesselSensor[i] = 0;
    Status.WasserSensor[i] = 0;
  }
  StoreStatus(Offset);
}

void LoadStatus()
{
  uint16_t Offset = 0xFFFF;

  Offset = ((uint16_t)EEPROM.read(0))*256 + EEPROM.read(1);
  if (Offset == 0xFFFF) {
    //Neu initialisieren
    InitStatus(2);
    
    lcd.setCursor(0, 3);    
    lcd.print("Initialisiert!");
  } else {
    unsigned int i;
    uint8_t *pStatus = (uint8_t *)&Status;
    uint8_t crc;
        
    for(i = 0; i < sizeof(Status); i++) {
      pStatus[i] = EEPROM.read(Offset+i);
    }
    crc = OneWire::crc8(pStatus, sizeof(Status)-sizeof(Status.crc8));
    if (crc != Status.crc8) {
      lcd.setCursor(0, 3);    
      lcd.print("CRC: ");
      lcd.print(crc, HEX);
      lcd.print(" soll ");
      lcd.print(Status.crc8, HEX);
      
      InitStatus(Offset+sizeof(Status));
    }
  }
  /* No temporary modes after reboot */
  if (Status.CurMode == MODE_T_ON || Status.CurMode == MODE_T_OFF) Status.CurMode = MODE_AUTO;
}

void setup()
{
    lcd.begin(20, 4);
    pinMode(PinPumpe, OUTPUT);
    digitalWrite(PinRotaryA, HIGH);
    digitalWrite(PinRotaryB, HIGH);
    digitalWrite(PinPush, HIGH);
    digitalWrite(PinPumpe, LOW);
    LastPumpChange = millis();
    LastPumpRun = millis();
    analogWrite(PinBacklight, 255);
    attachInterrupt(0, pinCount, CHANGE);
    LoadStatus();
}

void OutputStatusLine(float TempW, float TempK)
{
  char Buffer[21];
  PString TheLine(Buffer, sizeof(Buffer));
  
  if (TempW <= -100) {
    TheLine.print("????");
  } else {
    if (TempW < 10 && TempW >= 0) TheLine.print(" ");
    TheLine.print(TempW, 1);
  } 
  TheLine.write(0xDF);
  TheLine.print("C");
  while(TheLine.length() < 7) TheLine.print(" ");
  switch(Status.CurMode) {
    case MODE_AUTO:
      TheLine.print("AUTO");
      break;
      
    case MODE_ON:
      TheLine.print("EIN");
      break;
      
    case MODE_OFF:
      TheLine.print("AUS");
      break;

    case MODE_T_ON:
      TheLine.print("TEIN");
      break;
      
    case MODE_T_OFF:
      TheLine.print("TAUS");
      break;
      
    default:
      TheLine.print("????");
  }
  while(TheLine.length() < 12) TheLine.print(" ");
  if (digitalRead(PinPumpe) == HIGH) {
    TheLine.print("* ");
  } else {
    TheLine.print("  ");
  }
  
  if (TempK <= -100) {
    TheLine.print("????");
  } else {
    if (TempK < 10 && TempK >= 0) TheLine.print(" ");
    TheLine.print(TempK, 1);
  } 
  TheLine.write(0xDF);
  TheLine.print("C");
  
  lcd.setCursor(0, 0);    
  lcd.print(TheLine);
}

int CmpAddr(byte addr1[8], byte addr2[8])
{
  int Ret = 0;
  int i;
  
  for(i = 0; Ret == 0 && i < 8; i++) {
    if (addr1[i] > addr2[i]) Ret = 1;
    else if (addr1[i] < addr2[i]) Ret = -1;
  }
  
  return Ret;
}

void PrintMenu()
{
  char Buffer[21];
  PString TheLine(Buffer, sizeof(Buffer));
  
  switch(CurMenu)
  {
    case 3:
      if (NumSensors == 0) {
        TheLine = "Kein Sensor";
        CurItem3 = 0;
      } else if (CurItem3 == NumSensors) {
        TheLine = "<<<";
      } else {
        int i;
        
        for(i = 0; i < 8; i++) {
          TheLine.print(SensorList[CurItem3][i], HEX);
        }
        if (CmpAddr(SensorList[CurItem3], Status.WasserSensor) == 0) {
          TheLine.print("  W");
        } else if (CmpAddr(SensorList[CurItem3], Status.KesselSensor) == 0) {
          TheLine.print("  K");
        }
      }
      while(TheLine.length() < 20) TheLine.print(" ");
      lcd.setCursor(0, 3);
      lcd.print(TheLine);
      
    case 2:
      if (CurItem1 == 3 || CurItem1 == 4) {
        if (Menu2_34[CurItem2] != NULL) {
          TheLine = Menu2_34[CurItem2];
          if (RemainingTemp > 0) {
            if (CurItem2 == (int)(RemainingTemp/3600000)) {
              TheLine.print(" *");
            }
          }
        } else {
          TheLine = "<<<";
        }
      } else {
        if (Menu2[CurItem2][0] != 0) {
          TheLine = Menu2[CurItem2];
        } else {
          TheLine = "<<<";
        }
      }
      while(TheLine.length() < 20) TheLine.print(" ");
      lcd.setCursor(0, 2);
      lcd.print(TheLine);
      
    case 1:
      if (Menu1[CurItem1] != NULL) {
        TheLine = Menu1[CurItem1];
        if (CurItem1 == Status.CurMode) TheLine += " *";
      } else {
        TheLine = "<<<";
      }
      while(TheLine.length() < 20) TheLine.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(TheLine);
      break;
  }
  if (CurMenu < 2) {
    lcd.setCursor(0, 2);
    lcd.print("                    ");
  }
  if (CurMenu < 3) {
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  }
}

void HandleMenu1(int SelChange)
{
  if (SelChange > 0) {
    if (CurItem1 < NumMenu1) CurItem1++;
  } else if (SelChange < 0) {
    if (CurItem1 > 0) CurItem1--;
  }
}

void HandleMenu2(int SelChange)
{
  if (CurItem1 == 3 || CurItem1 == 4) {
    if (SelChange > 0) {
      if (CurItem2 < NumMenu2_34) CurItem2++;
    } else if (SelChange < 0) {
      if (CurItem2 > 0) CurItem2--;
    }
  } else {
    if (SelChange > 0) {
      if (CurItem2 < NumMenu2) CurItem2++;
    } else if (SelChange < 0) {
      if (CurItem2 > 0) CurItem2--;
    }
  }
}

void HandleMenu3(int SelChange)
{
  if (SelChange > 0) {
    if (CurItem3 < NumSensors) CurItem3++;
  } else if (SelChange < 0) {
    if (CurItem3 > 0) CurItem3--;
  }
}

void ScanOneWireBus()
{
  byte address[8];
  int i=0;
  byte tmp = 0;
 
  NumSensors = 0;
  while (NumSensors < 5 && ow.search(address))  {
    tmp = 0;
    switch(address[0]) {
      case 0x10:
        //0x10 = DS18S20
      case 0x28:
        //0x28 = DS18B20
        tmp = 1;
        break;

      default:
        tmp = 0;
        break;      
    }

    //display the address, if tmp is ok
    if (tmp == 1)
    {
      if (OneWire::crc8(address, 7) == address[7]) {
        //all is ok, display it
        for (i=0;i<8;i++) {
          SensorList[NumSensors][i] = address[i];
        }
        NumSensors++;
      }
    }//end if tmp
  }//end while
}

void HandleMenu(int SelChange)
{
  if (digitalRead(PinPush) == LOW && LastPush == 0) {
    LastPush = 1;
    if (CurMenu == 0) {
      CurMenu = 1;
      CurItem1 = 0;
    } else if (CurMenu == 1) {
      switch(CurItem1) {
        case 0:
          Status.CurMode = 0;
          StoreStatus(0);
          break;
        case 1:
          Status.CurMode = 1;
          StoreStatus(0);
          break;
        case 2:
          Status.CurMode = 2;
          StoreStatus(0);
          break;
        case 3:
        case 4:
        case 5:
          CurMenu = 2;
          CurItem2 = 0;
          break;
        default:
          CurMenu = 0;
          break;
      }
    } else if (CurMenu == 2) {
      if (CurItem1 == 3 || CurItem1 == 4) {
        if (CurItem2 < NumMenu2_34) {
          RemainingTemp = (CurItem2+1) * 36000000;
          Status.CurMode = CurItem1;
          /* Do not StoreStatus for temorary modes */
        }
        CurMenu = 1;
        CurItem1 = 0;
      } else {
        switch(CurItem2) {
          case 0:
          case 1:
            ScanOneWireBus();
            CurMenu = 3;
            CurItem3 = 0;
            break;
            
          default:
            CurMenu = 1;
            break;
        }
      }
    } else if (CurMenu == 3) {
      if (CurItem3 == NumSensors) {
        CurMenu = 2;
      } else {
        int i;
        
        if (CurItem2 == 0) {
          for(i = 0; i < 8; i++)
            Status.WasserSensor[i] = SensorList[CurItem3][i];
          StoreStatus(0);          
        } else if (CurItem3 == 1) {
          for(i = 0; i < 8; i++)
            Status.KesselSensor[i] = SensorList[CurItem3][i];
          StoreStatus(0);          
        }
      }
    }
    LastActionTick = millis();
  } else if (SelChange != 0) {
    if (CurMenu == 0) {
      CurMenu = 1;
      CurItem1 = 0;
    }
    LastActionTick = millis();
    
    switch(CurMenu) {
      case 1:
        HandleMenu1(SelChange);
        break;
        
      case 2:
        HandleMenu2(SelChange);
        break;
        
      case 3:
        HandleMenu3(SelChange);
    }    
  }
  
  if (CurMenu != 0) {
    PrintMenu();
  }
  
  if (CurMenu != 0 && millis()-LastActionTick > 60000) {
    int Line;
    
    CurMenu = 0;
    
    for(Line = 1; Line < 3; Line++) {
      lcd.setCursor(0, Line);
      lcd.print("                    ");
    }
  }
  if (digitalRead(PinPush) == HIGH) {
    LastPush = 0;
  }
}

void SetBacklight()
{
  if (millis()-LastActionTick < 20000) {
    analogWrite(PinBacklight, 255);    
  } else if (millis()-LastActionTick < 60000) {
    analogWrite(PinBacklight, 255-(millis()-LastActionTick)/250);
  } else {
    analogWrite(PinBacklight, 0);
  }
}

void StartConversion(byte addr[8])
{
  ow.reset();
  ow.select(addr);
  ow.write(0x44, 1);
}

float ReadTemperature(byte addr[8]) 
{
  byte data[12];
  int i;
  int HighByte, LowByte, TReading, SignBit;
  
  ow.reset();
  ow.select(addr);
  ow.write(0xBE);
  
  for(i = 0; i < 9; i++) {
    data[i] = ow.read();
  }

  if (OneWire::crc8(data, 8) == data[8]) {
    LowByte = data[0];
    HighByte = data[1];
    TReading = (HighByte<<8)+LowByte;
    SignBit = TReading & 0x8000;
    if (SignBit) {
      TReading = (TReading ^ 0xffff)+1;
    }
    
    if (SignBit) TReading = -1*TReading;
  } else {
    // CRC mismatch ==> return -100
    TReading = -200;
  }
  return ((float)TReading)/2;
}

// Calculate if the pump should be active or not, sets WantedPumpState
void CalcPump(float TempW, float TempK)
{
  switch(Status.CurMode) {
    case MODE_AUTO:
      if (TempW > 0 && TempK > 0) {
        if (TempW > TargetTemp) {
          // Water already warm enough
          WantedPumpState = 0;
        } else if (TempK > TempW+5) {
          // Temp. in heating is significantly warmer than water
          WantedPumpState = 1;
          LastPumpRun = millis();
        } else {
          // Every 15 minutes: start pump for 10 seconds to get heater temp to sensor
          if ((WantedPumpState == 1) && (millis() - LastPumpRun > 900000)) {
            LastPumpRun = millis();
            WantedPumpState = 0;
          } else if ((WantedPumpState == 1) && (millis() - LastPumpRun > 890000)) {
            WantedPumpState = 1;
            // Do not set LastPumpRun here
          } else {
            WantedPumpState = 0;
          }
        }
      } else {
        // No complete sensor readings ==> Off
        WantedPumpState = 0;
      }
      break;
      
    case MODE_ON:
      WantedPumpState = 1;
      LastPumpRun = millis();
      break;
      
    case MODE_OFF:
      WantedPumpState = 0;
      break;
      
    case MODE_T_ON:
      if (LastLoop > 0 && LastLoop < millis()) {
        unsigned long Delta = millis() - LastLoop;
        if (Delta >= RemainingTemp) {
          RemainingTemp = 0;
          Status.CurMode = MODE_AUTO;
        } else {
          WantedPumpState = 1;
          RemainingTemp -= Delta;
        }
      }
      LastLoop = millis();
      break;
      
    case MODE_T_OFF:
      if (LastLoop > 0 && LastLoop < millis()) {
        unsigned long Delta = millis() - LastLoop;
        if (Delta >= RemainingTemp) {
          RemainingTemp = 0;
          Status.CurMode = MODE_AUTO;
        } else {
          WantedPumpState = 0;
          RemainingTemp -= Delta;
        }
      }
      LastLoop = millis();
      break;
  }
}

// Set pump activity according to WantedPumpState
// Do not change pump activity if last change is less then 5 seconds before,
// just to make sure the motor is not damaged...
void SetPump()
{
  if (millis() - LastPumpChange > 5000) {
    if (digitalRead(PinPumpe) == HIGH && WantedPumpState == 0) {
      digitalWrite(PinPumpe, LOW);
      LastPumpChange = millis();
    } else if (digitalRead(PinPumpe) == LOW && WantedPumpState == 1) {
      digitalWrite(PinPumpe, HIGH);
      LastPumpChange = millis();
    }
  }
}

float TempW = -100, TempK = -100;
unsigned long LastConvStart = 0;
unsigned long LastConvEnd = 1;
void loop()
{
  int SelChange;
  
  if (millis() < LastPumpChange) LastPumpChange = 0;
  if (millis() < LastTick) LastTick = 0;
  if (millis() < LastPumpRun) LastPumpRun = 0;
  if (millis() < LastActionTick) LastActionTick = 0;
  if (millis() < LastConvStart) LastConvStart = 0;
  if (millis() < LastConvEnd) LastConvEnd = 1;
  
  SetBacklight();
    
  if (millis() > LastConvEnd + SampleMSecs && LastConvStart < LastConvEnd) {
    if (Status.WasserSensor[0] > 0) StartConversion(Status.WasserSensor);
    if (Status.KesselSensor[0] > 0) StartConversion(Status.KesselSensor);
    LastConvStart = millis();
    if (LastConvStart <= LastConvEnd) LastConvStart = LastConvEnd+1;
  }

  if (LastConvStart > LastConvEnd && LastConvStart + 800 < millis()) {
    LastConvEnd = millis();
    if (Status.WasserSensor[0] > 0) {
      TempW = ReadTemperature(Status.WasserSensor);
    } else {
      TempW = -100;
    }
    if (Status.KesselSensor[0] > 0) {
      TempK = ReadTemperature(Status.KesselSensor);
    } else {
      TempK = -100;
    }
  }
  OutputStatusLine(TempW, TempK);
    
  noInterrupts();
  SelChange = Counter;
  if (Counter != 0) {
    Counter = 0;
  }
  interrupts();
  HandleMenu(SelChange);
  
  CalcPump(TempW, TempK); // Calculate if the pump should be active or not, sets WantedPumpState
  SetPump(); // Set pump activity according to WantedPumpState
  
  delay(100);      
}
