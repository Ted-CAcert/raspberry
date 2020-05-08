int Spannung = 0;
 
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);

}

void loop() {
  delay(200);

  Serial.println("1");
  while(digitalRead(D1) == HIGH) {
    delay(10);
  }
  Serial.println("2");
  delay(1000);
  Serial.println("3");

  while(digitalRead(D1) == LOW) {
    delay(10);
  }

  if (Spannung == 0) {
    Spannung=1;
    digitalWrite(D2, HIGH);
    Serial.println("Ein");
  } else {
    Spannung=0;
    digitalWrite(D2, LOW);
    Serial.println("Aus");
  }
}
