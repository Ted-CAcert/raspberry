/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */

int led1 = 7;
int led2 = 6;
int led3 = 5;
int WarningLevel = 375;
int ShutdownLevel = 360;
int LastValue = 10000;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT);     
  pinMode(led3, OUTPUT);     
  digitalWrite(led1, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  int NumSamples=100;
  int IntervalMsec=10000;
  int i;
  long int sensorValue;
  // read the input on analog pin 0:
  sensorValue = 0;
  digitalWrite(led2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led3, LOW);    // turn the LED off by making the voltage LOW
  for(i = 0;i < NumSamples; i++) {
    if (i%12 == 0 && LastValue <= WarningLevel && LastValue > ShutdownLevel) {
      digitalWrite(led2, LOW);
    }
    if (i%4 == 0 && LastValue <= ShutdownLevel) {
      digitalWrite(led3, LOW);
    }
    sensorValue += analogRead(A0);
    delay(IntervalMsec/NumSamples);        // delay in between reads for stability
    digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
    if ((i+6)%12 == 0 && LastValue <= WarningLevel && LastValue > ShutdownLevel) {
      digitalWrite(led2, HIGH);
    }
    if ((i+2)%4 == 0 && LastValue <= ShutdownLevel) {
      digitalWrite(led3, HIGH);
    }
  }
  sensorValue /= NumSamples;
  LastValue = sensorValue;
  
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  // print out the value you read:
  Serial.println(sensorValue);
}
