int led = 13;

int sig[] = {
  10, 10, 10, 30, 30, 30, 10, 10, 10, 0 };
int pause[] = {
  10, 10, 30, 10, 10, 30, 10, 10, 200, 0 };
  
int i;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  i = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(sig[i]*10);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(pause[i]*10);               // wait for a second
  
  i++;
  if (sig[i] == 0) {
    i = 0;
  }
}
