#include "MeMCore.h"
#include "tools.h"

Ted7Seg Seg;
TedGyro gyro;
MeBuzzer Buzzer;
MeIR ir;
MeDCMotor MotorL(M1);
MeDCMotor MotorR(M2);

int moveSpeed = 200;


void get_ir_command();
void StopAll();

void setup() {
  Buzzer.tone(400, 200);
  Serial.begin(9600);
  Seg.displayNumber(0);
  gyro.init();
  gyro.zeroCalibrate(200,10);//sample 200 times to calibrate and it will take 200*10ms
  Buzzer.tone(800, 200);
}

void loop() {
  short Heading;

  Heading = (short)gyro.integrate();
  Seg.displayNumber(Heading);
  get_ir_command();
  delay(10); 
}

void Forward()
{
  MotorL.run(-moveSpeed);
  MotorR.run(moveSpeed);
}

void Backward()
{
  MotorL.run(moveSpeed); 
  MotorR.run(-moveSpeed);
}

void TurnLeft()
{
  MotorL.run(moveSpeed*0.8);
  MotorR.run(moveSpeed*0.8);
}

void TurnRight()
{
  MotorL.run(-moveSpeed*0.8);
  MotorR.run(-moveSpeed*0.8);
}

void StopAll() {
  MotorL.run(0);
  MotorR.run(0);  
}

void get_ir_command()
{
  static long time = millis();
  
  if(ir.decode())
  {
    uint32_t value = ir.value;
    switch ((value >> 16) & 0xff)
    {
      case IR_BUTTON_A:
      case IR_BUTTON_B:
      case IR_BUTTON_C:
      case IR_BUTTON_D:
      case IR_BUTTON_E:
      case IR_BUTTON_F:
        break;
      case IR_BUTTON_UP:
        break;
      case IR_BUTTON_DOWN:
        break;
      case IR_BUTTON_RIGHT:
        TurnRight();
        break;
      case IR_BUTTON_LEFT:
        TurnLeft();
        break;
      case IR_BUTTON_SETTING:
        break;
      case IR_BUTTON_9:
        break;
      case IR_BUTTON_8:
        break;
      case IR_BUTTON_7:
        break;
      case IR_BUTTON_6:
        break;
      case IR_BUTTON_5:
        break;
      case IR_BUTTON_4:
        break;
      case IR_BUTTON_3:
        break;
      case IR_BUTTON_2:
        break;
      case IR_BUTTON_1:
        break;
    }
    time = millis();
  } else if (millis() - time > 120) {
    StopAll();
  }
}

