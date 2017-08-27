/* Using an ATmega 328 on a breadboard with minimal circutry at 3.3V
 * ESP 8266 in an ESP-01 module connected to physical pins 2 and 3 (hardware USART RXD and TXD)
 * Sofware serial on physical pins 5 and 6 talking to a FTDI232 USB serial as
 * control/debug port
 *  
 * In my tests with 8 MHz internal oscillator communication with ESP8266 did not work. 
 * If the ESP did send more than one byte only the first byte was received OK, an other were garbled.
 *  
 * With an external 12 MHz ceramic resonator receiving works fine. It looks like 8 MHz is to slow for
 * 115.2 kBaud on hardware USART?
 * Software serial did work at 19200 Baud at 8 MHz and 115.2 kBaud at 12 MHz.
 * 
 */

#include <SoftwareSerial.h>

#define INBUF_SIZE 100

SoftwareSerial mySerial = SoftwareSerial(3, 4);

enum {
  Reset,
  Init,
  Idle,
  DataWait
} Status;
long LastStatusChange;

int cmd_i = 0;
char Buf[20];
int out_i = 0;
int in_w = 0;
int in_r = 0;
char InBuf[INBUF_SIZE];
unsigned long LastDataTime = 0;


void setup() {
//  mySerial.begin(9600);
//  mySerial.begin(19200); // Maximum for 8 MHz clock?
//  mySerial.begin(38400);
//  mySerial.begin(57600);
  mySerial.begin(115200); // 12 MHz clock seems to support 115 kBaud
  
  mySerial.println("\r\nWLAN_ESP8266_Test");
  mySerial.print(__DATE__);
  mySerial.print(" ");
  mySerial.println(__TIME__);
    
  Serial.begin(115200);
  
  Status = Reset;
  LastStatusChange = millis();
  Serial.println("AT+RST");
  LastDataTime = millis();
}

void HandleCommand(char *Buf) {
  if (strcmp(Buf, "r") == 0) {
    Serial.println("AT+RST");
    LastDataTime = millis();
  } else if (strcmp(Buf, "v?") == 0) {
    Serial.println("AT+GMR");
    LastDataTime = millis();
  } else if (strcmp(Buf, "e0") == 0) {
    Serial.println("ATE0");
    LastDataTime = millis();
  } else if (strcmp(Buf, "e1") == 0) {
    Serial.println("ATE1");
    LastDataTime = millis();
  } else if (strcmp(Buf, "t") == 0) {
    Serial.println("AT+CIPSNTPTIME?");
    LastDataTime = millis();
  } else if (strcmp(Buf, "0") == 0) {
    Serial.println("AT");
    LastDataTime = millis();
  } else if (strcmp(Buf, "?") == 0) {    
    Serial.println("AT+CIPMUX?");
    LastDataTime = millis();
  } else if (strcmp(Buf, "1") == 0) {    
    Serial.println("AT+CIPSTART=\"TCP\",\"192.168.178.41\",80,60");
    LastDataTime = millis();
  } else if (strcmp(Buf, "2") == 0) {    
    Serial.println("AT+CIPSEND=71\r\n");
    LastDataTime = millis();
  } else if (strcmp(Buf, "3") == 0) {    
    Serial.println("GET /data.html?Timestap=20170820T232500&Id=123456789&Val=123123.44345");
    LastDataTime = millis();
  } else if (strcmp(Buf, "9") == 0) {    
    Serial.println("AT+CIPCLOSE");
    LastDataTime = millis();
  } else {
    mySerial.println("Unknown WLAN command");
    mySerial.println(Buf);
  }
}

void WLANListenLoop() {
  int c;
  long utics;

  utics = micros();
  do {
    while (Serial.available()) {
      InBuf[in_w++] = Serial.read();
      if (in_w >= INBUF_SIZE) in_w = 0;
      utics=micros();
      LastDataTime = millis();
    }
    if (utics > micros()) utics=micros();
  } while(micros()-utics < 500);
}

void CmdListenLoop() {
  int c;
  
  while (mySerial.available()) {
    c = mySerial.read();
    if (c == '\n' || c == '\r') {
      if (cmd_i > 0) {
        mySerial.println("");
        Buf[cmd_i] = 0;
        HandleCommand(Buf);
        cmd_i = 0;
      } // else do nothing
    } else {
      Buf[cmd_i] = c;
      if (cmd_i < 19) cmd_i++;
      mySerial.write(c);
    }
  }
}

void WLANCheckData() {
  while(in_w != in_r) {
    mySerial.write(InBuf[in_r]);
    in_r++;
    if (in_r >= INBUF_SIZE) in_r = 0;
  }
}

void loop() {
  WLANListenLoop();
  WLANCheckData();
  CmdListenLoop();
}
