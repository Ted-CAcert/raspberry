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

#define INBUF_SIZE 500
#define LINEBUF_SIZE 100
#define OUTBUF_SIZE 100

void LogByte(char c);
void LogString(const char *str);

SoftwareSerial mySerial = SoftwareSerial(3, 4);

/* Global Status */
enum {
  Reset,
  Init,
  Idle,
  Processing, // Processing received data, do not call processing loop again
  DataWait // Waiting for specific data
} Status;

/* Status of all TCP connections */
enum {
  ConnClosed,
  ConnIdle
} ConnStatus[5];

long LastStatusChange;

int cmd_i = 0;
char Buf[20]; // Buffer for commands incoming from serial line
int out_i = 0;
int in_w = 0;
int in_r = 0;
int out_w = 0;
int out_r = 0;
char InBuf[INBUF_SIZE]; // Buffer for incoming data from WLAN modile
char LineBuf[LINEBUF_SIZE]; // Buffer for one \r terminated line of incoming data from WLAN module
char OutBuf[OUTBUF_SIZE]; // Buffer for debugging data to be sent via serial line when idle.
                          // Sending data via serial line while WLAN data is pending may lead to lost data....
unsigned long LastDataTime = 0;


void setup() {
  int i;
  
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

  for(i = 0; i < 5; i++) {
    ConnStatus[i] = ConnIdle;
  }
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
  } else if (strcmp(Buf, "m?") == 0) {    
    Serial.println("AT+CIPMUX?");
    LastDataTime = millis();
  } else if (strcmp(Buf, "m0") == 0) {    
    Serial.println("AT+CIPMUX=0");
    LastDataTime = millis();
  } else if (strcmp(Buf, "m1") == 0) {    
    Serial.println("AT+CIPMUX=1");
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

  /* Soft-AP Tests */
  } else if (strcmp(Buf, "s?") == 0) {    
    Serial.println("AT+CWMODE_CUR?");
    LastDataTime = millis();
  } else if (strcmp(Buf, "s0") == 0) {    
    Serial.println("AT+CWMODE_CUR=3");
    LastDataTime = millis();
  } else if (strcmp(Buf, "s1") == 0) {    
    Serial.println("AT+CWSAP_CUR=\"ORKUS\",\"12345678\",5,0");
    LastDataTime = millis();
  } else if (strcmp(Buf, "s2") == 0) {    
    Serial.println("AT+CIPSERVER=1,80");
    LastDataTime = millis();
  } else if (strcmp(Buf, "s3") == 0) {
    Serial.println("AT+CIPSEND=0,106");
  } else if (strcmp(Buf, "s4") == 0) {
    const char *Result="<html><head><title>TEST ESP8266</title></head><body><h1>Test ESP8266</h1><p>Dummy Text</p></body></html>";    
    Serial.println(Result);
    LastDataTime = millis();
  } else if (strcmp(Buf, "s9") == 0) {
    Serial.println("AT+CIPCLOSE=0");
  } else {
    mySerial.println("Unknown WLAN command");
    mySerial.println(Buf);
  }
}

/*
 * Call with Wait_usec 500 to 1000 if you expect a bigger data block which you want to receive 
 * as a whole. Call with 0 if you just want to clear the receiver queue.
 */
void WLANListenLoop(unsigned long Wait_usec) {
  unsigned long utics;
  char c;

  utics = micros();
  do {
    while (Serial.available()) {
      c = Serial.read();
      InBuf[in_w++] = c;
      if (in_w >= INBUF_SIZE) in_w = 0;
      if (c == '\r' && Status != Processing) {
        Status = Processing;
        WLANCheckData();
        Status = DataWait;
      }
      utics=micros();
      LastDataTime = millis();
    }
    if (utics > micros()) utics=micros();
  } while(micros()-utics < Wait_usec);
  if (Wait_usec > 100) Status = Idle;
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

void HandleLine() {
  LogString(LineBuf);
  LogString("|\r\n");
}

bool WLANCheckData() {  
  int i = 0;
  bool LineComplete = false;
  int Save_in_r = in_r;
  
  while(in_w != in_r) {
    LogByte(InBuf[in_r]);
    if (InBuf[in_r] == '\n') {
      // Do nothing, just ignore \n
    } else if (InBuf[in_r] == '\r') {
      // End of command line, break loop
      in_r++;
      if (in_r >= INBUF_SIZE) in_r = 0;
      LineBuf[i] = '\0';
      LineComplete = true;
      break;
    } else {
      LineBuf[i++] = InBuf[in_r];
    }
    
    in_r++;
    if (in_r >= INBUF_SIZE) in_r = 0;

    if (i >= LINEBUF_SIZE-1) {
      // Ugh... Buffer overflow. This is not good... treat as line break?
      LineBuf[i] = '\0';
      LineComplete = true;
      break;
    }
  }

  if (LineComplete) {
    WLANListenLoop(0); // Just store pending WLAN data in InBuf
    HandleLine();
  }

  return (LineComplete);
}

void LogByte(char c) {
  OutBuf[out_w++] = c;
  if (out_w >= OUTBUF_SIZE) out_w = 0;
  if (out_w == out_r) {
    // Buffer overflow...
    out_r++;
    if (out_r >= OUTBUF_SIZE) out_r = 0;
  }
}

void LogString(const char *str) {
  while(*str != '\0') {
    LogByte(*str);
    str++;
  }
}


void SendDebugData() {
  while(out_w != out_r) {
    mySerial.write(OutBuf[out_r++]);
    if (out_r >= OUTBUF_SIZE) out_r = 0;
  }
}
void loop() {
  WLANListenLoop(500);
  SendDebugData();
  CmdListenLoop();
}
