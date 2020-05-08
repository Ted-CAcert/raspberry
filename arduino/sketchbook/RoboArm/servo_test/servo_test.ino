#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int curServo[6];
int LastServo = 0;
int LastSwitch = 0;
int curStatus = 0;

int MinVal=1790;
int MaxVal=2240;

void setup() {
  int i;
  
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);

  digitalWrite(LED_BUILTIN, LOW);
  LastSwitch = millis();
  curServo[0] = (MinVal+MaxVal)/2;
  curServo[1] = (MinVal+MaxVal)/2;
  curServo[2] = (MinVal+MaxVal)/2;
  curServo[3] = (MinVal+MaxVal)/2;
  curServo[4] = (MinVal+MaxVal)/2;
  curServo[5] = (MinVal+MaxVal)/2;
}

void loop() {
  int Val;

  for(int i=0;i<6;i++) {
    pwm.setPWM(i,0,curServo[i]);
  }

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
