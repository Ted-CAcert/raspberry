/*
  Blink variation
 
  This example code is in the public domain.
 */

#include <SoftwareSerial.h>
/* Software Serial on MISO/MOSI-Pins of ATtiny85, so I can easily use the Buspirate to check it... 
 * MOSI-Pin: 0
 * MISO-Pin: 1
*/
//SoftwareSerial mySerial(0, 1);
 
int led1 = 3;
int led2 = 4;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  //pinMode(led1, OUTPUT);     
  //pinMode(led2, OUTPUT);
//  mySerial.begin(9600);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void pulse() {
  int i;
  for(i = 0; i < 256; i++) {
    analogWrite(0, i);
    analogWrite(1, i);
    analogWrite(2, i);
    analogWrite(3, i);
    analogWrite(4, i);
    delay(4);
  }
  for(i = 255; i >= 0; i--) {
    analogWrite(0, i);
    analogWrite(1, i);
    analogWrite(2, i);
    analogWrite(3, i);
    analogWrite(4, i);
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
  /*mySerial.print("A");
  blink();
  mySerial.print("B"); */
}
