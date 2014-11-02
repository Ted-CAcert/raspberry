const int PinRed = 11;
const int PinGreen = 10;
const int PinBlue = 9;

void setup()
{
  pinMode(PinBlue, OUTPUT);
  pinMode(PinGreen, OUTPUT);
  pinMode(PinRed, OUTPUT);
  
  analogWrite(PinBlue, 0);
  analogWrite(PinGreen, 0);
  analogWrite(PinRed, 0);
}

void ColorPulse()
{
    int r, g, b;
  int dr, dg, db;
  int i;
  int j;
  
  r = g = b = 0;
  dr = dg = db = 1;
  while(1) {
    for(i = 0; i < 4; i++) {
      for(j = 0; j < 4; j++) {
        b += db;
        if (b >= 255 && db == 1) db = -1;
        if (b <= 0 && db == -1) db = 1;
        analogWrite(PinBlue, b);
        delay(10);
      }
      g += dg;
      if (g >= 255 && dg == 1) dg = -1;
      if (g <= 0 && dg == -1) dg = 1;
      analogWrite(PinGreen, g);
    }
    r += dr;
    if (r >= 255 && dr == 1) dr = -1;
    if (r <= 0 && dr == -1) dr = 1;
    analogWrite(PinRed, r);
/*
    for(r = 0; r < 256; r++) {
      analogWrite(11, r);
      for(g = 0; g < 256; g++) {
        analogWrite(10, g);
        for(b = 0; b < 256; b++) {
          analogWrite(9, b);
          delay(10);
        }
      }
    }
*/    
  }
}

void ColorPulseLow()
{
    int r, g, b;
  float dr, dg, db;
  int i;
  int j;
  int StepsUp = 50;
  int StepsDown=50;

  r = random(0, 128);
  g = random(0, 128);
  b = random(0, 128);
  
  dr = ((float)r)/StepsUp;
  dg = ((float)g)/StepsUp;
  db = ((float)b)/StepsUp;  
  for(i = 0; i < StepsUp; i++)
  {
    analogWrite(PinRed, dr*i);
    analogWrite(PinGreen, dg*i);
    analogWrite(PinBlue, db*i);
    delay(10);
  }
  
  dr = ((float)r)/StepsDown;
  dg = ((float)g)/StepsDown;
  db = ((float)b)/StepsDown;
  for(i = StepsDown-1; i >= 0 ; i--)
  {
    analogWrite(PinRed, dr*i);
    analogWrite(PinGreen, dg*i);
    analogWrite(PinBlue, db*i);
    delay(10);
  }
  
  delay(5000);
}

void ColorFlash()
{
  while(1) {
    analogWrite(PinRed, 255);
    delay(10);
    analogWrite(PinRed, 0);
    
    delay(300);
    
    analogWrite(PinGreen, 255);
    delay(10);
    analogWrite(PinGreen, 0);

    delay(150);
    
    analogWrite(PinGreen, 255);
    delay(10);
    analogWrite(PinGreen, 0);

    delay(150);
    
    analogWrite(PinBlue, 255);
    delay(10);
    analogWrite(PinBlue, 0);
    
    delay(300);   
  } 
}

void loop()
{
//  ColorPulse();
  ColorPulseLow();
}


