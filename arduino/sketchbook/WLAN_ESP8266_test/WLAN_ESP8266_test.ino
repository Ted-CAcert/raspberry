#include <SoftwareSerial.h>

SoftwareSerial mySerial = SoftwareSerial(3, 4);

void setup() {
  mySerial.begin(9600);
  mySerial.println("WLAN_ESP8266_Test");
  
  Serial.begin(115200);
}

void loop() {
  int c;
  
  while(Serial.available()) {
    mySerial.write(Serial.read());
  }
  
  while(mySerial.available()) {
    c = mySerial.read();
    Serial.write(c);
  }
}
