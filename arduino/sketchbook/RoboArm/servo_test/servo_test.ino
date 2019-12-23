#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int curServo = 150;
int LastServo = 0;
int LastSwitch = 0;
int curStatus = 0;

int MinVal=1790;
int MaxVal=2240;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);

  digitalWrite(LED_BUILTIN, LOW);
  LastSwitch = millis();
  curServo = (MinVal+MaxVal)/2;
}

void loop() {
  int Val;

  if (curServo != LastServo) {
    Serial.print("Cur Value: ");
    Serial.println(curServo);
    for(int i=0;i<16;i++) {
      pwm.setPWM(i,0,curServo);
    }
    LastServo=curServo;
  }

  Val = analogRead(A0);
  if (Val > 1000) {
    // Click
    curServo = (MinVal+MaxVal)/2;
  } else if (Val > 600) {
    curServo += (Val-600)/10;
  } else if (Val < 400) {
    curServo -= (400-Val)/10;
  }
  if (curServo < MinVal) curServo = MinVal;
  if (curServo > MaxVal) curServo = MaxVal;

  if (millis() < LastSwitch) LastSwitch = 0;
  if (millis() - LastSwitch > 1000) {
    LastSwitch = millis();
    if (curStatus == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      curStatus=1;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      curStatus=0;
    }
  }
  delay(30);
}
