const int PinXA=4;
const int PinXB=5;
const int PinYA=2;
const int PinYB=3;

void setup() {
  Serial.begin(9600);

  pinMode(PinXA, OUTPUT);
  pinMode(PinXB, OUTPUT);
  pinMode(PinYA, OUTPUT);
  pinMode(PinYB, OUTPUT);
}

int DelayB=1;
int LowTime=1;

int WaitX = 0;
int WaitY = 0;

int WaitTime(int Data) {
  if (abs(Data-350)<100) {
    return 150;
  } else if (abs(Data-350)<200) {
    return 50;
  } else if (abs(Data-350)<250) {
    return 20;
  } else if (abs(Data-350)<300) {
    return 10;
  } else {
    return 2;
  }
}

void loop() {
  int Data0, Data1;
  
  Data0 = analogRead(A0);
  Data1 = analogRead(A1);
//  Serial.print(Data0);
//  Serial.print("\t");
//  Serial.println(Data1);

  if (WaitX > 0) WaitX--;
  if (WaitY > 0) WaitY--;
  
  if (WaitX < 1) {
    if (Data0 <= 300) {
      digitalWrite(PinXA, LOW);
      delay(DelayB);
      digitalWrite(PinXB, LOW);
      delay(LowTime);
      digitalWrite(PinXA, HIGH);
      delay(DelayB);
      digitalWrite(PinXB, HIGH);
    } else if (Data0 >= 400) {
      digitalWrite(PinXB, LOW);
      delay(DelayB);
      digitalWrite(PinXA, LOW);
      delay(LowTime);
      digitalWrite(PinXB, HIGH);
      delay(DelayB);
      digitalWrite(PinXA, HIGH);
    }
    WaitX = WaitTime(Data0);
  }

  if (WaitY < 1) {
    if (Data1 <= 300) {
      digitalWrite(PinYA, LOW);
      delay(DelayB);
      digitalWrite(PinYB, LOW);
      delay(LowTime);
      digitalWrite(PinYA, HIGH);
      delay(DelayB);
      digitalWrite(PinYB, HIGH);
    } else if (Data1 >= 400) {
      digitalWrite(PinYB, LOW);
      delay(DelayB);
      digitalWrite(PinYA, LOW);
      delay(LowTime);
      digitalWrite(PinYB, HIGH);
      delay(DelayB);
      digitalWrite(PinYA, HIGH);
    }
    WaitY = WaitTime(Data1);
  }

  delay(1);
}
