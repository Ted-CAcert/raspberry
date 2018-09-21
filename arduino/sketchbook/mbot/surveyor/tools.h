#ifndef __TOOLS_H
#define __TOOLS_H

#include "ITG3200.h"
#include "TM1637.h"

class Ted7Seg : public TM1637
{
  public:
    Ted7Seg(int ClkPin, int DioPin);
    Ted7Seg();

    void displayNumber(short TheValue);
    void display(int8_t DispData[]);
    void displayRaw(int8_t DispData[]);
    void coding(int8_t DispData[]); 
    int8_t coding(int8_t DispData); 
};

class TedGyro : public ITG3200
{
  private:
    short LastY;
    unsigned long LastTime;
  
  protected:
    int16_t x_offset;
    int16_t y_offset;
    int16_t z_offset;

    double Heading;
    
  public:
    TedGyro();

    void init();
    void getXYZ(short *x, short *y, short *z);
    void zeroCalibrate(unsigned int samples, unsigned int sampleDelayMS);
    short getLastY() { return LastY; };

    double integrate();
};
#endif
