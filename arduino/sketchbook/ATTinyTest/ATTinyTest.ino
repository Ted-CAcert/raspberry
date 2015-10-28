/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
int led1 = 3;
int led2 = 4;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT);     
}

void pulse() {
  int i;
  for(i = 0; i < 256; i++) {
    analogWrite(led2, i);
    analogWrite(led1, 255-i);
    delay(4);
  }
  for(i = 255; i >= 0; i--) {
    analogWrite(led2, i);
    analogWrite(led1, 255-i);
    delay(4);
  }
}

void blink()
{
    digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
    digitalWrite(led2, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(led1, HIGH);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second
    digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second  
}
// the loop routine runs over and over again forever:
void loop() {
  pulse();
  blink();
}
