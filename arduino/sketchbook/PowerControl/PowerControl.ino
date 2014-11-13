/* Factor not necessary when build.f_cpu in boards.txt is configured correctly? */
//const float tf = 1.5; // 1.5 for 8MHz
const float tf=1.0;

const int pinPowerControl = 8; // PB0
const int pinShutdownInit = 7; // PD7
const int pinLifeTick = 6; // PD6
const int pinArmPowerCheck = 5; // PD5
const int pinBatteryVoltage = 0; // PC0

const int PowerStatus_ON = 1;
const int PowerStatus_InShutdown = 2;
const int PowerStatus_OFF = 3;

const long DefLifetickWait = 10000/tf; 
const long BootLifetickWait = 300000/tf; // Only relevant if WatchdogArmCount == 0
long LifetickWait = DefLifetickWait;
const long ShutdownWait = 60000/tf;
const long UndervoltageWait = 30000/tf;
const long RestartWait = 300000/tf;

const int ShutdownVoltage = 830; // Reading lower than this will initiate shutdown
const int StartupVoltage = 870;  // Reading higher than this will allow startup

// WatchdogArmCount may be > 0 in development environment...
//const int WatchdogArmCount = 5;
const int WatchdogArmCount = 0;

unsigned long LastChange;
int LastLevel;
int NumChanges;
int PowerStatus;
unsigned long LastStatusChange;

unsigned long LastUnderVoltage;
unsigned long LastStartupVoltage;
boolean PowerCheckArmed;

int AnalogValue[6];


void setup() {
  pinMode(pinPowerControl, OUTPUT);
  digitalWrite(pinPowerControl, LOW);
  pinMode(pinShutdownInit, OUTPUT);
  digitalWrite(pinShutdownInit, HIGH);
  pinMode(pinLifeTick, INPUT_PULLUP);
  pinMode(pinArmPowerCheck, INPUT_PULLUP);
  
  analogReference(INTERNAL);  
  
  LastChange = 0;
  NumChanges = 0;
  LastLevel=HIGH;
  PowerStatus=PowerStatus_ON;
  LastStatusChange=0;
  LastUnderVoltage=0;
  LastStartupVoltage=0;
  PowerCheckArmed=false;

  if (WatchdogArmCount == 0) {
    LifetickWait = BootLifetickWait;
    LastChange=millis();
  }
  Serial.begin(9600*tf);
  Serial.println("Started.");
}

void CheckLifetick()
{
  unsigned long CurMilli;
  int CurLevel;
  
  CurMilli = millis();
  if (CurMilli < LastChange) {
    LastChange = 0;
  }
  
  CurLevel = digitalRead(pinLifeTick);
  if (CurLevel!=LastLevel && CurMilli-LastChange>100) {
    LastLevel = CurLevel;
    LastChange = CurMilli;
    LifetickWait = DefLifetickWait;
    if (NumChanges < WatchdogArmCount*2) NumChanges++;
    if (CurLevel) {
      int i;
      for(i = 0; i < 5; i++) {
        Serial.print(AnalogValue[i]);
        Serial.print("\t");
      }
      Serial.println(AnalogValue[5]);
    }

    if (WatchdogArmCount > 0 && NumChanges == WatchdogArmCount) {
      Serial.println("Watchdog armed.");
    }
  }
}

void InitShutdown()
{
  digitalWrite(pinShutdownInit, LOW);
}

void PowerOff()
{
  digitalWrite(pinPowerControl, HIGH);
  digitalWrite(pinShutdownInit, HIGH);
}

void PowerOn()
{
  digitalWrite(pinPowerControl, LOW);
  digitalWrite(pinShutdownInit, HIGH); // Zur Sicherheit
}

void DoPowerStatus()
{
  unsigned long CurMilli;
  boolean DoShutdown;
  boolean DoStartup;
  
  CurMilli = millis();
  if (CurMilli < LastStatusChange) {
    LastStatusChange = 0;
  }
  if (CurMilli < LastChange) {
    LastChange = 0;
  }
  if (CurMilli < LastUnderVoltage) {
    LastUnderVoltage = 0;
  }
 
  DoShutdown=false;
  switch(PowerStatus) {
    case PowerStatus_ON:
      if ((NumChanges >= WatchdogArmCount) && (CurMilli - LastChange > LifetickWait)) {
        Serial.println("Shutdown: Missing Lifetick");
        DoShutdown=true;
      } else if (PowerCheckArmed && LastUnderVoltage>0 && CurMilli - LastUnderVoltage > UndervoltageWait) {
        Serial.println("Shutdown: Undervoltage");
        DoShutdown=true;
      }
      if (DoShutdown) {
        PowerStatus = PowerStatus_InShutdown;
        LastStatusChange = CurMilli;
        delay(50);
        InitShutdown();
      }
      break;
      
    case PowerStatus_InShutdown:
      if (CurMilli - LastStatusChange > ShutdownWait) {
        PowerStatus = PowerStatus_OFF;
        LastStatusChange = CurMilli;
        Serial.println("Power Off.");
        delay(50);
        PowerOff();
      }
      break;
      
    case PowerStatus_OFF:
      if (CurMilli - LastStatusChange > RestartWait) {
        DoStartup = false;
        if (PowerCheckArmed) {
          if (LastStartupVoltage > 0 && CurMilli - LastStartupVoltage > RestartWait) DoStartup = true;
        } else {
          DoStartup = true;
        }
        if (DoStartup) {
          PowerOn();
          if (WatchdogArmCount == 0) {
            LifetickWait = BootLifetickWait;
            LastChange=millis();
          }
          PowerStatus = PowerStatus_ON;
          LastStatusChange = CurMilli;
          NumChanges = 0;
          delay(50);
          Serial.println("Power ON.");
        }
      }
      break;
  } 
}

void CheckVoltage()
{ 
  /* One sample (of each channel) takes about 6 millisec */
  const int NumSamples = 10;
  
  unsigned long CurMilli;
  long SampleValue[6];
  int i, CurSample;
  
  /* Check if battery voltage should be watched */
  if (digitalRead(pinArmPowerCheck) == LOW) {
    if (!PowerCheckArmed) {
      delay(100);
      if (digitalRead(pinArmPowerCheck) == LOW) {
        PowerCheckArmed = true;
        Serial.println("PowerCheck armed.");
      }
    }
  } else {
    if (PowerCheckArmed) {
      delay(100);
      if (digitalRead(pinArmPowerCheck) == HIGH) {
        PowerCheckArmed = false;
        Serial.println("Powercheck disarmed.");
      }
    }
  }
  
  /* Do analog samples */  
  for(i = 0; i < 6; i++) SampleValue[i] = 0;
  for(CurSample = 0; CurSample < NumSamples; CurSample++) {
    for(i = 0; i < 6; i++) {
       SampleValue[i] += analogRead(i);
       delay(1);
    }
  }
  for(i = 0; i < 6; i++) {
    AnalogValue[i] = SampleValue[i]/NumSamples;
  }
  
  /* Record battery power status */
  CurMilli = millis();
  if (CurMilli == 0) CurMilli = 1;
  if (CurMilli < LastChange) {
    LastUnderVoltage = 0;
    LastStartupVoltage = 0;
  }

  if (PowerCheckArmed) {
    if (AnalogValue[pinBatteryVoltage] < ShutdownVoltage && LastUnderVoltage == 0) {
      LastUnderVoltage = CurMilli;
    }
    if (AnalogValue[pinBatteryVoltage] >= ShutdownVoltage && LastUnderVoltage > 0) {
      LastUnderVoltage = 0;
    }
    
    if (AnalogValue[pinBatteryVoltage] >= StartupVoltage && LastStartupVoltage == 0) {
      LastStartupVoltage = CurMilli;
    }
    if (AnalogValue[pinBatteryVoltage] < StartupVoltage && LastStartupVoltage > 0) {
      LastStartupVoltage = 0;
    }    
  } else {
    LastUnderVoltage = 0;
    LastStartupVoltage = 0;
  }
}

void loop() {
  CheckLifetick();
  
  DoPowerStatus();
  
  CheckVoltage();
}
