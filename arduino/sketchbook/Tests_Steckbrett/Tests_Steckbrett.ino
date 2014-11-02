float tf = 1.5;

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 8;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(14400);
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(7, INPUT);  
  pinMode(6, INPUT);  
  analogReference(INTERNAL);  
}

// the loop routine runs over and over again forever:
void loop() {
  int i;
  long int sensorValue1;
  long int sensorValue2;
  const int NumSamples = 1000;
  
//  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led, LOW); 
  
  sensorValue1 = 0;
  sensorValue2 = 0;
  for(i = 0; i < NumSamples; i++) {
    sensorValue1 += analogRead(A0);
    sensorValue2 += analogRead(A1);
    delay(7);               
  }
  Serial.print(sensorValue1/NumSamples);
  Serial.print("\t");
  Serial.print(sensorValue2/NumSamples);
  Serial.print("\t");
  Serial.print(digitalRead(7));
  Serial.print("\t");
  Serial.println(digitalRead(6));  

}
