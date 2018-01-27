#include "tools.h"

/****************************************************
 * 
 * Ted7Seg
 * 
 ****************************************************/
static const int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,
                                 0x66,0x6d,0x7d,0x07,
                                 0x7f,0x6f,0x77,0x7c,
                                 0x39,0x5e,0x79,0x71,//0~9,A,b,C,d,E,F
                                 0x40}; // -

 
Ted7Seg::Ted7Seg(int ClkPin, int DioPin)
 : TM1637(ClkPin, DioPin)
{
  init();
  set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}

Ted7Seg::Ted7Seg()
 : TM1637(12, 11)
{ 
  init();
  set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}

void Ted7Seg::displayNumber(short TheValue)
{
  int8_t DispData[4];
  
  if (TheValue >= 0) {
    DispData[0] = (int)(TheValue/1000)%10;
  } else {
    TheValue = -1*TheValue;
    DispData[0] = 16;
  }
  DispData[1] = (int)(TheValue/100)%10;
  DispData[2] = (int)(TheValue/10)%10;
  DispData[3] = (int)(TheValue)%10;

  display(DispData);
}

void Ted7Seg::display(int8_t DispData[])
{
  int8_t SegData[4];
  uint8_t i;
  for(i = 0;i < 4;i ++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for(i=0;i < 4;i ++)
  {
    writeByte(SegData[i]);
  }
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}

void Ted7Seg::displayRaw(int8_t DispData[])
{
  uint8_t i;

  start();
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for(i=0;i < 4;i ++)
  {
    writeByte(DispData[i]);
  }
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}

void Ted7Seg::coding(int8_t DispData[])
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  for(uint8_t i = 0;i < 4;i ++)
  {
    if(DispData[i] == 0x7f)DispData[i] = 0x00;
    else DispData[i] = TubeTab[DispData[i]] + PointData;
  }
}
int8_t Ted7Seg::coding(int8_t DispData)
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  if(DispData == 0x7f) DispData = 0x00 + PointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + PointData;
  return DispData;
}

/****************************************************
 * 
 * TedGyro
 * 
 ****************************************************/

TedGyro::TedGyro()
 : ITG3200()
{
  Heading = 0;
  LastTime = 0;
}

void TedGyro::init()
{
  ITG3200::init();
}

/*    
 *     Read all registers at once:
 *     1D   GYRO_XOUT_H
 *     1E   GYRO_XOUT_L
 *     1F   GYRO_YOUT_H
 *     20   GYRO_YOUT_L
 *     21   GYRO_ZOUT_H
 *     22   GYRO_ZOUT_L
 */
void TedGyro::getXYZ(short *x, short *y, short *z)
{
  union {
    struct { unsigned char L; unsigned char H; };
    short Val;
  } Buf;
  char i;
  
  Wire.requestFrom(GYRO_ADDRESS, 6, ITG3200_GX_H, 1, true);
  for(i = 0; i < 3; i++) {
    if (Wire.available() > 1) {
      Buf.H = Wire.read();
      Buf.L = Wire.read();

      switch(i) {
        case 0:
          if (x) *x = Buf.Val+x_offset;
          break;

        case 1:
          if (y) *y = Buf.Val+y_offset;
          break;

        case 4:
          if (z) *z = Buf.Val+z_offset;
          break;
      }
    }
  }
}

void TedGyro::zeroCalibrate(unsigned int samples, unsigned int sampleDelayMS) 
{
  int16_t x_offset_temp = 0;
  int16_t y_offset_temp = 0;
  int16_t z_offset_temp = 0;
  short x,y,z;
  x_offset = 0;
  y_offset = 0;
  z_offset = 0;
  getXYZ(&x,&y,&z);
  for (int i = 0;i < samples;i++){
    delay(sampleDelayMS);
    getXYZ(&x,&y,&z);
    x_offset_temp += x;
    y_offset_temp += y;
    z_offset_temp += z;
  }

  x_offset = abs(x_offset_temp)/samples;
  y_offset = abs(y_offset_temp)/samples;
  z_offset = abs(z_offset_temp)/samples;
  if(x_offset_temp > 0)x_offset = -x_offset;
  if(y_offset_temp > 0)y_offset = -y_offset;
  if(z_offset_temp > 0)z_offset = -z_offset;
}

double TedGyro::integrate()
{
  short CurY;
  unsigned long CurTime = millis();
  double Delta;

  getXYZ(NULL, &CurY, NULL);
  if (CurY < 10 && CurY > -10) CurY = 0; // Try to avoid drift... Not ideal but (hopefully) better than nothing.
  
  if (LastTime == 0) {
    LastY = CurY;
  } else if (CurTime == LastTime) {
    LastY = (LastY+CurY)>>1;
  } else {
    unsigned long TimeDiff;
    if (CurTime < LastTime) {
      TimeDiff = (0xFFFFFFFF-LastTime)+CurTime;
    } else {
      TimeDiff = CurTime - LastTime;
    }

    // Some lines to get the types correct... :-)
    Delta = (LastY+CurY);
    Delta *= TimeDiff;
    Delta /= 28400; // Nominally this should be 28750, empirically I need a bit less...
    Heading += Delta;
    if (Heading > 180) Heading -= 360;
    if (Heading <= -180) Heading += 360;


    LastY = CurY;
  }
  
  LastTime = CurTime;

  return Heading;
}

