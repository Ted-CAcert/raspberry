/* Ein sehr kleines Dimmer-Programm */

/* Festlegen der Pin-Nummern */
const int OutLED=4;
const int InTaster=2;

/* Variable für momentane Helligkeit */
int Helligkeit=128;

/* Diese Prozedur wird beim Einschalten einmal aufgerufen */
void setup()
{
  pinMode(InTaster, INPUT_PULLUP);
  pinMode(OutLED, OUTPUT);
}

/* Diese Prozedur wird ständig neu aufgerufen */
void loop()
{
  if (digitalRead(InTaster) == LOW) {
    Helligkeit++;
    if (Helligkeit > 255) Helligkeit=0;
  }
  analogWrite(OutLED, Helligkeit);
  delay(10);
}

