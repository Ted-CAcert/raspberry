int InPin = 1;
int OutPin = 4;
int ClockPin = 3;

void setup() {
  pinMode(OutPin, OUTPUT);
  pinMode(ClockPin, OUTPUT);
  analogReference(DEFAULT);
}

void loop() {
  int CurTick = 0;
  int CurVoltage;
  
  while(CurTick < 10) {
    if (CurTick == 0) {
      digitalWrite(ClockPin, HIGH);
    } else if (CurTick == 1) {
      digitalWrite(ClockPin, LOW);      
    }
    
    CurVoltage = analogRead(InPin);
    analogWrite(OutPin, CurVoltage/4);
    CurTick++;
    delay(100);
  }
}
