#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
short int curServo[10];
short int newServo[10];
int CurChannel = 0;

int MinVal=1790;
int MaxVal=2240;

short int parking[] = { 1880, 1860, 2220, 2050, 1980, 1830, 0, 0, 0, 0 };
short int wave[] = { 2200, 2000, 2000, 2200, 1800, 2100, 0, 0, 0, 0 };
unsigned char speedval[] = { 10, 4, 4, 5, 15, 10, 10, 10, 10, 10 };

// LED Pulse
unsigned long LastSwitch = 0;
int curStatus = 0;

// Serial Command Interface
char CmdBuffer[30];
byte BufIDX = 0;

void setup() {
  int i;
  
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(9600);
  Serial.print("Arm Test 1 ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  pwm.begin();
  pwm.setPWMFreq(50);

  digitalWrite(LED_BUILTIN, LOW);
  LastSwitch = millis();
  for(i=0;i<6;i++) {
    if (parking[i] > 0) {
      newServo[i] = parking[i];
      curServo[i] = parking[i]+1;
    }
    pwm.setPWM(i,0,curServo[i]);
    Serial.print(newServo[i]);
    Serial.print("\t");
  }
  Serial.print("\r\n");
}

int HandleCommand() {
  int Ret = 0;
  
  if (CmdBuffer[0] == 'c') {
    if (CmdBuffer[1]-'0' >= 0 && CmdBuffer[1]-'0' < 10) {
      CurChannel = CmdBuffer[1]-'0';
      Serial.print("CurChannel: ");
      Serial.println(CurChannel);
    }
  } else if (isdigit(CmdBuffer[0])) {
    int NewVal = atoi(CmdBuffer);

    if (NewVal >= MinVal && NewVal <= MaxVal) {
      newServo[CurChannel] = NewVal;
      Ret = 1;      
    }
  } else if (CmdBuffer[0] == 'p') {
    int i;
    for(i=0;i<6;i++) {
      if (parking[i] > 0) {
        newServo[i] = parking[i];
      }
    }
    Ret = 1;   
  } else if (CmdBuffer[0] == 'w') {
    int i;
    for(i=0;i<6;i++) {
      if (parking[i] > 0) {
        newServo[i] = wave[i];
      }
    }
    Ret = 1;   
  }
  for(int i=0;i<6;i++) {
    Serial.print(newServo[i]);
    Serial.print("\t");
  }
  Serial.print("\r\n");

  return Ret;
}
void loop() {
  int Val;
  int DoUpdate; 

  DoUpdate = 0;

  while(Serial.available()) {
    int CurChar = Serial.read();
    
    if (CurChar > 0) {
      if (CurChar == '\r' || CurChar == '\n') {
        Serial.print("\r\n"); // Echo
        if (BufIDX > 0) {
          CmdBuffer[BufIDX] = 0;
          DoUpdate = HandleCommand();
          BufIDX = 0;
        }
      } else {
        Serial.write(CurChar); // Echo
        CmdBuffer[BufIDX] = CurChar;
        BufIDX++;
      }

      if (BufIDX >= sizeof(CmdBuffer)-1) {
        Serial.print("\r\n");
        Serial.print("Buffer overflow, max ");
        Serial.write(sizeof(CmdBuffer)-1);
        Serial.println(" characters!");
        BufIDX = 0;
      }
    }
  }

  for(int i=0;i<6;i++) {
    if (newServo[i] < curServo[i]) {
      if (curServo[i]-newServo[i] > speedval[i]) {
        curServo[i] -= speedval[i];
      } else {
        curServo[i] = newServo[i];
      }
      pwm.setPWM(i,0,curServo[i]);
    } else if (newServo[i] > curServo) {
      if (newServo[i]-curServo[i] > speedval[i]) {
        curServo[i] += speedval[i];
      } else {
        curServo[i] = newServo[i];
      }
      pwm.setPWM(i,0,curServo[i]);
    }
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
