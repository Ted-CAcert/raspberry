
const int PinRotaryA=2;
const int PinRotaryB=0;
const int PinPush=3;

volatile int Counter = 0;
volatile int LastTick = 0;

void pinCount()
{
/*  if (Counter < 255) Counter++;
  else Counter = 0;
*/ 
  if ((digitalRead(PinRotaryA) == LOW ) && (millis()-LastTick > 1)) {
    LastTick = millis();
    if (digitalRead(PinRotaryB) == LOW) {
      if (Counter < 255) Counter++;
    } else {
      if (Counter > 0) Counter--;
    }
  }
}

// This function is not part of the library for ATTiny...
// ATTiny seems to support interrupts only at logical pin 2 (PB2)
int digitalPinToInterrupt(int logicalPin) {
  return (logicalPin == 2 ? 0 : -1);
}

int led1 = 1;
int led2 = 4;

void setup() {
  digitalWrite(PinRotaryA, HIGH);
  digitalWrite(PinRotaryB, HIGH);
  digitalWrite(PinPush, HIGH);

  attachInterrupt(digitalPinToInterrupt(PinRotaryA), pinCount, FALLING);
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  analogWrite(led2, Counter);
  analogWrite(led1, 255-Counter);
  delay(100);
}

