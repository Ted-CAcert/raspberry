#include <MeMCore.h>

MeBuzzer buzzer;

void setup() {
  buzzer.tone(1000, 100);
  delay(50);
  buzzer.tone(2000, 100);
  delay(50);
  buzzer.tone(1500, 100);
  delay(50);
  buzzer.tone(1000, 50);
  delay(50);
  buzzer.tone(1000, 50);
  delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:

}
