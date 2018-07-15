void setup() {
  int i;
  
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  /* Ruhestellung anfahren! */
  delay(50);
  for(i = 0; i < 100; i++) {
    digitalWrite(5, HIGH);
    delayMicroseconds(600);
    digitalWrite(5, LOW);
    delay(50);
  }
}

// Pulsdauer: 600 - 2600
int Value = 0;
int Inc = 100;

unsigned long LastTick = 0;
void loop() {
  
  digitalWrite(5, HIGH);
  delayMicroseconds(600+Value);
  digitalWrite(5, LOW);

  if (millis()-LastTick > 1000) {
    Value += Inc;
    if (Value >= 2000) {
      Value = 2000;
      Inc = -100;
    } else if (Value <= 0) {
      Value = 0;
      Inc = 100;
    }
    LastTick = millis();
  }
  
  delay(100);
}
