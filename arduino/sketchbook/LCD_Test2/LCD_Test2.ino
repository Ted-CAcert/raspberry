void setup()
{
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); 
}

void loop()
{
  digitalWrite(8, HIGH); 
  delayMicroseconds(1);
  digitalWrite(8, LOW); 
  
  delay(1000);
}
