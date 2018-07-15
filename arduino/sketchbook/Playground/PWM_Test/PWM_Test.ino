void setup()
{
  pinMode(11, OUTPUT);
}

void loop()
{
  int i;
  
  for(i = 0; i < 256; i++) {
    analogWrite(11, i);
    delay(10);
  }
  for(;i >= 0; i--) {
    analogWrite(11, i);
    delay(10);
  }
/*  analogWrite(11, 10);
  delay(500);
  analogWrite(11, 240);
  delay(500);*/
}

