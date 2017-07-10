const int Lautsprecher=4;
const int InTaster=2;

const int Hoch=500;
const int Tief=1000;

void setup()
{
  pinMode(InTaster, INPUT_PULLUP);
  pinMode(Lautsprecher, OUTPUT);
}

int Wait=Tief;
int Richtung=-1;
int LastTaster=HIGH;
long Dauer=0;
  
void loop()
{
  
  if (digitalRead(InTaster) == LOW) {
    digitalWrite(Lautsprecher, HIGH);
    delayMicroseconds(Wait);
    digitalWrite(Lautsprecher, LOW);
    delayMicroseconds(Wait);
    
    Dauer += Wait;
    if (Dauer > Tief*5) {
      Wait += Richtung*5;
      if (Wait >= Tief) Richtung=-1;
      if (Wait <= Hoch) Richtung=1;
      Dauer = 0;
    }
  } else {
    Wait = Tief;
    Richtung = -1;
    Dauer = 0;
    delay(10);
  }
}

