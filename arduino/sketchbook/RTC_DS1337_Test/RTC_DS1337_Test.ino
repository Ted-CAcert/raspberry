#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial = SoftwareSerial(3, 4);

void setup() {
  Wire.begin();
  //Wire.SetClock(100000);
  mySerial.begin(9600);
  mySerial.println("RTC_DS1337_Test");

  Wire.beginTransmission(0x68);
  Wire.write(0xE);
  
  Wire.write(0x00);
  Wire.write(0x00);
  
  Wire.endTransmission();
  
}

void loop() {
  int i;
  int Seconds, Minutes, Hours, Day, Month, Year;
  
  Wire.requestFrom(0x68, 16);
  
  i = 0;
  Seconds = Minutes = Hours = Day = Month = Year = 0;
  while(Wire.available()) {
    int c = Wire.read();
    
    if (c < 16) mySerial.print("0");
    mySerial.print(c, HEX);
    mySerial.print(" ");
    
    switch(i) {
      case 0:
        Seconds = (c>>4)*10+(c&0xf);
        break;

      case 1:
        Minutes = (c>>4)*10+(c&0xf);
        break;

      case 2:
        if (c & 0x40) {
          // AM/PM
          Hours = ((c>>4)&0x1)*10+(c&0xf);
          if (c & 0x20) {
            Hours += 12;
          }
        } else {
          // 24 Hours
          Hours = ((c>>4)&0x3)*10+(c&0xf);
        }
        break;
      
      // case 3: Day of week
      
      case 4:
        Day = (c>>4)*10+(c&0xf);
        break;

      case 5: // Month + Century-Bit
        Month = ((c>>4)&0x1)*10+(c&0xf);
        if (c & 0x80) {
          Year = 2000;
        } else {
          Year = 1900;
        }
        break;

      case 6:
        Year += (c>>4)*10+(c&0xf);
        break;
    }
    i++;
  }
  mySerial.print("\r\n");
  mySerial.print(Year); 
  if (Month < 10) mySerial.print("-0"); else mySerial.print ("-"); mySerial.print(Month);
  if (Day < 10) mySerial.print("-0"); else mySerial.print ("-"); mySerial.print(Day);
  if (Hours < 10) mySerial.print(" 0"); else mySerial.print (" "); mySerial.print(Hours);
  if (Minutes < 10) mySerial.print(":0"); else mySerial.print (":"); mySerial.print(Minutes);
  if (Seconds < 10) mySerial.print(":0"); else mySerial.print (":"); mySerial.println(Seconds);
  
  Wire.beginTransmission(0x68);
  Wire.write(0xE);
  
  Wire.write(0x00);
  Wire.write(0x00);
  
  Wire.endTransmission();
  

  delay(1000);
}
