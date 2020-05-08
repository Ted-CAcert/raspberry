// https://github.com/esp8266/Arduino/blob/3e7b4b8e0cf4e1f7ad48104abfc42723b5e4f9be/variants/nodemcu/pins_arduino.h

//int ledPin = 16; // D0, On-Board LED
//int ledPin = 5; // D1
int ledPin = D0; // GPIO4, D2
//int ledPin = 0; // D3
//int ledPin = 2; // D4
//int ledPin = 14; // D5
//int ledPin = 12; // D6
//int ledPin = 13; // GPIO13, D7
//int ledPin = 15; // D8

void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);
  
  Serial.println("Setup done");  
  Serial.println();
}

void loop() {
  int Pin;
  int i;
  int Val1=0, Val2=0;

  delay(300);
  digitalWrite(ledPin, LOW);

  digitalWrite(D1, LOW);
  Val1 = analogRead(A0);
  delay(1);
  Val1 += analogRead(A0);
  delay(1);
  Val1 += analogRead(A0);

  digitalWrite(D1, HIGH);
  delay(5);
  Val2 = analogRead(A0);
  delay(1);
  Val2 += analogRead(A0);
  delay(1);
  Val2 += analogRead(A0);
  digitalWrite(D1, LOW);

  Serial.print("Values: ");
  Serial.print(Val1);
  Serial.print("\t");
  Serial.println(Val2);

  delay(300);
  digitalWrite(ledPin, HIGH);
}
