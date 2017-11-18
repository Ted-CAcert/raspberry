/* Pin definitions */
const char PinRotaryA=2;
const char PinRotaryB=0;
const char PinPush=3;
const char led1 = 1;
const char led2 = 4;

volatile char Counter = 0;
volatile unsigned long LastTick = 0; // Last user action. Turn light off after long inactivity

void pinCount()
{
  if ((digitalRead(PinRotaryA) == LOW ) && (millis()-LastTick > 1)) {
    LastTick = millis();
    if (digitalRead(PinRotaryB) == LOW) {
      if (Counter < 126) Counter++;
    } else {
      if (Counter > -126) Counter--;
    }
  }
}

// This function is not part of the library for ATTiny...
// ATTiny seems to support interrupts only at logical pin 2 (PB2)
int digitalPinToInterrupt(int logicalPin) {
  return (logicalPin == 2 ? 0 : -1);
}

void setup() {
  digitalWrite(PinRotaryA, HIGH);
  digitalWrite(PinRotaryB, HIGH);
  digitalWrite(PinPush, HIGH);

  attachInterrupt(digitalPinToInterrupt(PinRotaryA), pinCount, FALLING);
  pinMode(led1, OUTPUT);     
  analogWrite(led1, 100);
  pinMode(led2, OUTPUT);
  analogWrite(led2, 100);
}

enum {
STAT_OFF,
STAT_STARTING, // Button pressed, but not yet released
STAT_ON,
STAT_STOPPING
} Status = STAT_OFF;

unsigned char LightLevel = 0;
char SideMod = 0; /* -10 to 10, Difference in 10%-Steps */
unsigned long PushDownTick =  0;
unsigned long SideModTick = 0;
char CurDirection = 1; // Light level on push goes up (1), down (-1) or is constant (0) 
bool FlagIdle=false;

void TurnOff() {
  LightLevel=0;
  Status=STAT_STOPPING;
  SideMod=0;
  CurDirection = 1;
}

void loop() {
  // Handle timer overflows
  if (LastTick > millis()) LastTick = 0; 
  if (PushDownTick > millis()) PushDownTick = 1;
  if (SideModTick > millis()) SideModTick = 1;
  
  if (Status == STAT_ON && Counter != 0) {
    /* Handle button turning: dimming of one side */
    if (SideModTick == 0 || SideModTick < millis()) {
      SideModTick = 0;
      if (SideMod < 0) {
        SideMod += Counter;
        if (SideMod < -10) SideMod=-10;
        if (SideMod >= 0) {
          SideMod = 0;
          /* Once the sides are equal, wait a second before continuing */
          SideModTick = millis()+1000; // Overflow is no real problem here...
        }
      } else if (SideMod > 0) {
        SideMod += Counter;
        if (SideMod > 10) SideMod = 10;
        if (SideMod <= 0) {
          SideMod = 0;
          SideModTick = millis()+1000; // Overflow is no real problem here...
        }
      } else {
        SideMod += Counter;
        if (SideMod > 10) SideMod=10;
        if (SideMod <-10) SideMod=-10;
      }
    }
  }
  Counter = 0;

  /* Handle pushed button */
  if (digitalRead(PinPush) == LOW) {
    // Button is pressed
    LastTick = millis();
    if (PushDownTick == 0) {
      // Button just pushed
      PushDownTick = millis();
      if (Status == STAT_OFF) {
        Status=STAT_STARTING;
        LightLevel=1;
        CurDirection = 1;
      } else {
        if (LightLevel == 255) CurDirection = -1;
        if (LightLevel == 0) CurDirection = 1; // Should not happen, light level 0 is off, but to be on the safe side...
      }
    } else if (Status == STAT_STARTING || Status == STAT_ON) {
      // Button is held down
      char Factor=10;

      if (millis()-PushDownTick < 1000) {
        Factor=1; // Go slowly during the first second
      }
      
      if (((int)LightLevel) + (int)(CurDirection)*Factor > 255) LightLevel=255;
      else if (((int)LightLevel) + (int)(CurDirection)*Factor < 0) {
        TurnOff();
      } else {
        LightLevel += CurDirection*Factor;
      }
    }
  } else {
    // Button is not pressed
    if (PushDownTick > 0) {
      // Just released the button
      if (Status==STAT_ON) {
        if (millis()-PushDownTick <= 150) {
          // Short push when running: turn off
          TurnOff();
        }
      }
      PushDownTick = 0;
    }
    if (Status==STAT_STARTING) {
      Status=STAT_ON;
    } else if (Status==STAT_STOPPING) {
      Status=STAT_OFF;
    }    
  }

  /* Half light level after 1 hour inactivity, turn off after 2 hours */
  if (Status == STAT_ON) {
    if (millis() - LastTick < 3600000) {
      FlagIdle = false;
    } else if (millis() - LastTick >= 3600000 && millis() - LastTick < 7200000) {
      FlagIdle = true;
    } else {
      TurnOff();
    }
  }
  
  /* Set the light levels */
  if (SideMod == 0) {
    analogWrite(led1, FlagIdle ? LightLevel/2 : LightLevel);
    analogWrite(led2, FlagIdle ? LightLevel/2 : LightLevel);
  } else if (SideMod > 0) {
    analogWrite(led1, FlagIdle ? LightLevel/2 : LightLevel);
    analogWrite(led2, FlagIdle ? (LightLevel*(10-SideMod))/20 : (LightLevel*(10-SideMod))/10);
  } else {
    analogWrite(led2, FlagIdle ? LightLevel/2 : LightLevel);
    analogWrite(led1, FlagIdle ? (LightLevel*(10+SideMod))/20 : (LightLevel*(10+SideMod))/10);
  }

  /* Save a tiny bit of power when turned off... */
  if (Status == STAT_OFF)
    delay(200);
  else
    delay(50);
}

