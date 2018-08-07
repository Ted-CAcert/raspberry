#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
const int AnzahlPixel = 16;
const int DatenPin = 5;
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(AnzahlPixel, DatenPin);
unsigned long Starttick;
int Laufzeit;

void setup() {
  int Zaeler;
  pixel.begin();
  for(Zaeler=0;Zaeler>AnzahlPixel;Zaeler++)
  {
    pixel.setPixelColor(Zaeler, pixel.Color(0,0,0));
  }

  Starttick=millis();
  randomSeed(analogRead(4));
  Laufzeit = random(15000, 21000);
  setup2();
}
int counter = 0;

void loop2() {counter =(counter++);};

int Warte = 20;

void DoPixel(int SchrittNr) {
  if (SchrittNr == 1) {
  pixel.setPixelColor(0, pixel.Color(64,0,0));
pixel.setPixelColor(1, pixel.Color(0,64,0));
pixel.setPixelColor(2, pixel.Color(0,0,64));
pixel.setPixelColor(3, pixel.Color(64,0,0));
pixel.setPixelColor(4, pixel.Color(0,64,0));
pixel.setPixelColor(5, pixel.Color(0,0,64));
pixel.setPixelColor(6, pixel.Color(64,0,0));
pixel.setPixelColor(7, pixel.Color(0,64,0));
pixel.setPixelColor(8, pixel.Color(0,0,64));
pixel.setPixelColor(9, pixel.Color(64,0,0));
pixel.setPixelColor(10, pixel.Color(0,64,0));
pixel.setPixelColor(11, pixel.Color(0,0,64));
pixel.setPixelColor(12, pixel.Color(64,0,0));
pixel.setPixelColor(13, pixel.Color(0,64,0));
pixel.setPixelColor(14, pixel.Color(0,0,64));
pixel.setPixelColor(15, pixel.Color(64,0,0));

pixel.show();
  } else if (SchrittNr == 2) {
pixel.setPixelColor(0, pixel.Color(0,64,0));
pixel.setPixelColor(1, pixel.Color(0,0,64));
pixel.setPixelColor(2, pixel.Color(64,0,0));
pixel.setPixelColor(3, pixel.Color(0,64,0));
pixel.setPixelColor(4, pixel.Color(0,0,64));
pixel.setPixelColor(5, pixel.Color(64,0,0));
pixel.setPixelColor(6, pixel.Color(0,64,0));
pixel.setPixelColor(7, pixel.Color(0,0,64));
pixel.setPixelColor(8, pixel.Color(64,0,0));
pixel.setPixelColor(9, pixel.Color(0,64,0));
pixel.setPixelColor(10, pixel.Color(0,0,64));
pixel.setPixelColor(11, pixel.Color(64,0,0));
pixel.setPixelColor(12, pixel.Color(0,64,0));
pixel.setPixelColor(13, pixel.Color(0,0,64));
pixel.setPixelColor(14, pixel.Color(64,0,0));
pixel.setPixelColor(15, pixel.Color(0,64,0));
pixel.show();

  } else if (SchrittNr == 3) {

pixel.setPixelColor(0, pixel.Color(0,0,64));
pixel.setPixelColor(1, pixel.Color(64,0,0));
pixel.setPixelColor(2, pixel.Color(0,64,0));
pixel.setPixelColor(3, pixel.Color(0,0,64));
pixel.setPixelColor(4, pixel.Color(64,0,0));
pixel.setPixelColor(5, pixel.Color(0,64,0));
pixel.setPixelColor(6, pixel.Color(0,0,64));
pixel.setPixelColor(7, pixel.Color(64,0,0));
pixel.setPixelColor(8, pixel.Color(0,64,0));
pixel.setPixelColor(9, pixel.Color(0,0,64));
pixel.setPixelColor(10, pixel.Color(64,0,0));
pixel.setPixelColor(11, pixel.Color(0,64,0));
pixel.setPixelColor(12, pixel.Color(0,0,64));
pixel.setPixelColor(13, pixel.Color(64,0,0));
pixel.setPixelColor(14, pixel.Color(0,64,0));
pixel.setPixelColor(15, pixel.Color(0,0,64));
pixel.show();

  } else if (SchrittNr == 4) {

pixel.setPixelColor(0, pixel.Color(64,0,0));
pixel.setPixelColor(1, pixel.Color(0,64,0));
pixel.setPixelColor(2, pixel.Color(0,0,64));
pixel.setPixelColor(3, pixel.Color(64,0,0));
pixel.setPixelColor(4, pixel.Color(0,64,0));
pixel.setPixelColor(5, pixel.Color(0,0,64));
pixel.setPixelColor(6, pixel.Color(64,0,0));
pixel.setPixelColor(7, pixel.Color(0,64,0));
pixel.setPixelColor(8, pixel.Color(0,0,64));
pixel.setPixelColor(9, pixel.Color(64,0,0));
pixel.setPixelColor(10, pixel.Color(0,64,0));
pixel.setPixelColor(11, pixel.Color(0,0,64));
pixel.setPixelColor(12, pixel.Color(64,0,0));
pixel.setPixelColor(13, pixel.Color(0,64,0));
pixel.setPixelColor(14, pixel.Color(0,0,64));
pixel.setPixelColor(15, pixel.Color(64,0,0));

pixel.show();

  } else if (SchrittNr == 5) {
pixel.setPixelColor(0, pixel.Color(0,64,0));
pixel.setPixelColor(1, pixel.Color(0,0,64));
pixel.setPixelColor(2, pixel.Color(64,0,0));
pixel.setPixelColor(3, pixel.Color(0,64,0));
pixel.setPixelColor(4, pixel.Color(0,0,64));
pixel.setPixelColor(5, pixel.Color(64,0,0));
pixel.setPixelColor(6, pixel.Color(0,64,0));
pixel.setPixelColor(7, pixel.Color(0,0,64));
pixel.setPixelColor(8, pixel.Color(64,0,0));
pixel.setPixelColor(9, pixel.Color(0,64,0));
pixel.setPixelColor(10, pixel.Color(0,0,64));
pixel.setPixelColor(11, pixel.Color(64,0,0));
pixel.setPixelColor(12, pixel.Color(0,64,0));
pixel.setPixelColor(13, pixel.Color(0,0,64));
pixel.setPixelColor(14, pixel.Color(64,0,0));
pixel.setPixelColor(15, pixel.Color(0,64,0));
pixel.show();

  } else if (SchrittNr == 6) {
pixel.setPixelColor(0, pixel.Color(0,0,64));
pixel.setPixelColor(1, pixel.Color(64,0,0));
pixel.setPixelColor(2, pixel.Color(0,64,0));
pixel.setPixelColor(3, pixel.Color(0,0,64));
pixel.setPixelColor(4, pixel.Color(64,0,0));
pixel.setPixelColor(5, pixel.Color(0,64,0));
pixel.setPixelColor(6, pixel.Color(0,0,64));
pixel.setPixelColor(7, pixel.Color(64,0,0));
pixel.setPixelColor(8, pixel.Color(0,64,0));
pixel.setPixelColor(9, pixel.Color(0,0,64));
pixel.setPixelColor(10, pixel.Color(64,0,0));
pixel.setPixelColor(11, pixel.Color(0,64,0));
pixel.setPixelColor(12, pixel.Color(0,0,64));
pixel.setPixelColor(13, pixel.Color(64,0,0));
pixel.setPixelColor(14, pixel.Color(0,64,0));
pixel.setPixelColor(15, pixel.Color(0,0,64));
pixel.show();

  } else if (SchrittNr == 7) {
pixel.setPixelColor(0, pixel.Color(64,0,0));
pixel.setPixelColor(1, pixel.Color(0,64,0));
pixel.setPixelColor(2, pixel.Color(0,0,64));
pixel.setPixelColor(3, pixel.Color(64,0,0));
pixel.setPixelColor(4, pixel.Color(0,64,0));
pixel.setPixelColor(5, pixel.Color(0,0,64));
pixel.setPixelColor(6, pixel.Color(64,0,0));
pixel.setPixelColor(7, pixel.Color(0,64,0));
pixel.setPixelColor(8, pixel.Color(0,0,64));
pixel.setPixelColor(9, pixel.Color(64,0,0));
pixel.setPixelColor(10, pixel.Color(0,64,0));
pixel.setPixelColor(11, pixel.Color(0,0,64));
pixel.setPixelColor(12, pixel.Color(64,0,0));
pixel.setPixelColor(13, pixel.Color(0,64,0));
pixel.setPixelColor(14, pixel.Color(0,0,64));
pixel.setPixelColor(15, pixel.Color(64,0,0));

pixel.show();

  } else if (SchrittNr == 8) {
pixel.setPixelColor(0, pixel.Color(0,64,0));
pixel.setPixelColor(1, pixel.Color(0,0,64));
pixel.setPixelColor(2, pixel.Color(64,0,0));
pixel.setPixelColor(3, pixel.Color(0,64,0));
pixel.setPixelColor(4, pixel.Color(0,0,64));
pixel.setPixelColor(5, pixel.Color(64,0,0));
pixel.setPixelColor(6, pixel.Color(0,64,0));
pixel.setPixelColor(7, pixel.Color(0,0,64));
pixel.setPixelColor(8, pixel.Color(64,0,0));
pixel.setPixelColor(9, pixel.Color(0,64,0));
pixel.setPixelColor(10, pixel.Color(0,0,64));
pixel.setPixelColor(11, pixel.Color(64,0,0));
pixel.setPixelColor(12, pixel.Color(0,64,0));
pixel.setPixelColor(13, pixel.Color(0,0,64));
pixel.setPixelColor(14, pixel.Color(64,0,0));
pixel.setPixelColor(15, pixel.Color(0,64,0));
pixel.show();

  } else if (SchrittNr == 9) {
pixel.setPixelColor(0, pixel.Color(0,0,64));
pixel.setPixelColor(1, pixel.Color(64,0,0));
pixel.setPixelColor(2, pixel.Color(0,64,0));
pixel.setPixelColor(3, pixel.Color(0,0,64));
pixel.setPixelColor(4, pixel.Color(64,0,0));
pixel.setPixelColor(5, pixel.Color(0,64,0));
pixel.setPixelColor(6, pixel.Color(0,0,64));
pixel.setPixelColor(7, pixel.Color(64,0,0));
pixel.setPixelColor(8, pixel.Color(0,64,0));
pixel.setPixelColor(9, pixel.Color(0,0,64));
pixel.setPixelColor(10, pixel.Color(64,0,0));
pixel.setPixelColor(11, pixel.Color(0,64,0));
pixel.setPixelColor(12, pixel.Color(0,0,64));
pixel.setPixelColor(13, pixel.Color(64,0,0));
pixel.setPixelColor(14, pixel.Color(0,64,0));
pixel.setPixelColor(15, pixel.Color(0,0,64));
pixel.show();

  } else if (SchrittNr == 10) {
pixel.setPixelColor(0, pixel.Color(64,0,0));
pixel.setPixelColor(1, pixel.Color(0,64,0));
pixel.setPixelColor(2, pixel.Color(0,0,64));
pixel.setPixelColor(3, pixel.Color(64,0,0));
pixel.setPixelColor(4, pixel.Color(0,64,0));
pixel.setPixelColor(5, pixel.Color(0,0,64));
pixel.setPixelColor(6, pixel.Color(64,0,0));
pixel.setPixelColor(7, pixel.Color(0,64,0));
pixel.setPixelColor(8, pixel.Color(0,0,64));
pixel.setPixelColor(9, pixel.Color(64,0,0));
pixel.setPixelColor(10, pixel.Color(0,64,0));
pixel.setPixelColor(11, pixel.Color(0,0,64));
pixel.setPixelColor(12, pixel.Color(64,0,0));
pixel.setPixelColor(13, pixel.Color(0,64,0));
pixel.setPixelColor(14, pixel.Color(0,0,64));
pixel.setPixelColor(15, pixel.Color(64,0,0));

pixel.show();

  } else if (SchrittNr == 11) {
pixel.setPixelColor(0, pixel.Color(0,64,0));
pixel.setPixelColor(1, pixel.Color(0,0,64));
pixel.setPixelColor(2, pixel.Color(64,0,0));
pixel.setPixelColor(3, pixel.Color(0,64,0));
pixel.setPixelColor(4, pixel.Color(0,0,64));
pixel.setPixelColor(5, pixel.Color(64,0,0));
pixel.setPixelColor(6, pixel.Color(0,64,0));
pixel.setPixelColor(7, pixel.Color(0,0,64));
pixel.setPixelColor(8, pixel.Color(64,0,0));
pixel.setPixelColor(9, pixel.Color(0,64,0));
pixel.setPixelColor(10, pixel.Color(0,0,64));
pixel.setPixelColor(11, pixel.Color(64,0,0));
pixel.setPixelColor(12, pixel.Color(0,64,0));
pixel.setPixelColor(13, pixel.Color(0,0,64));
pixel.setPixelColor(14, pixel.Color(64,0,0));
pixel.setPixelColor(15, pixel.Color(0,64,0));
pixel.show();

  } else if (SchrittNr == 12) {
pixel.setPixelColor(0, pixel.Color(0,0,64));
pixel.setPixelColor(1, pixel.Color(64,0,0));
pixel.setPixelColor(2, pixel.Color(0,64,0));
pixel.setPixelColor(3, pixel.Color(0,0,64));
pixel.setPixelColor(4, pixel.Color(64,0,0));
pixel.setPixelColor(5, pixel.Color(0,64,0));
pixel.setPixelColor(6, pixel.Color(0,0,64));
pixel.setPixelColor(7, pixel.Color(64,0,0));
pixel.setPixelColor(8, pixel.Color(0,64,0));
pixel.setPixelColor(9, pixel.Color(0,0,64));
pixel.setPixelColor(10, pixel.Color(64,0,0));
pixel.setPixelColor(11, pixel.Color(0,64,0));
pixel.setPixelColor(12, pixel.Color(0,0,64));
pixel.setPixelColor(13, pixel.Color(64,0,0));
pixel.setPixelColor(14, pixel.Color(0,64,0));
pixel.setPixelColor(15, pixel.Color(0,0,64));
pixel.show();

  } else if (SchrittNr == 13) {
pixel.setPixelColor(0, pixel.Color(64,0,0));
pixel.setPixelColor(1, pixel.Color(0,64,0));
pixel.setPixelColor(2, pixel.Color(0,0,64));
pixel.setPixelColor(3, pixel.Color(64,0,0));
pixel.setPixelColor(4, pixel.Color(0,64,0));
pixel.setPixelColor(5, pixel.Color(0,0,64));
pixel.setPixelColor(6, pixel.Color(64,0,0));
pixel.setPixelColor(7, pixel.Color(0,64,0));
pixel.setPixelColor(8, pixel.Color(0,0,64));
pixel.setPixelColor(9, pixel.Color(64,0,0));
pixel.setPixelColor(10, pixel.Color(0,64,0));
pixel.setPixelColor(11, pixel.Color(0,0,64));
pixel.setPixelColor(12, pixel.Color(64,0,0));
pixel.setPixelColor(13, pixel.Color(0,64,0));
pixel.setPixelColor(14, pixel.Color(0,0,64));
pixel.setPixelColor(15, pixel.Color(64,0,0));

pixel.show();

  } else if (SchrittNr == 14) {
pixel.setPixelColor(0, pixel.Color(0,64,0));
pixel.setPixelColor(1, pixel.Color(0,0,64));
pixel.setPixelColor(2, pixel.Color(64,0,0));
pixel.setPixelColor(3, pixel.Color(0,64,0));
pixel.setPixelColor(4, pixel.Color(0,0,64));
pixel.setPixelColor(5, pixel.Color(64,0,0));
pixel.setPixelColor(6, pixel.Color(0,64,0));
pixel.setPixelColor(7, pixel.Color(0,0,64));
pixel.setPixelColor(8, pixel.Color(64,0,0));
pixel.setPixelColor(9, pixel.Color(0,64,0));
pixel.setPixelColor(10, pixel.Color(0,0,64));
pixel.setPixelColor(11, pixel.Color(64,0,0));
pixel.setPixelColor(12, pixel.Color(0,64,0));
pixel.setPixelColor(13, pixel.Color(0,0,64));
pixel.setPixelColor(14, pixel.Color(64,0,0));
pixel.setPixelColor(15, pixel.Color(0,64,0));
pixel.show();

  } else if (SchrittNr == 15) {
pixel.setPixelColor(0, pixel.Color(0,0,64));
pixel.setPixelColor(1, pixel.Color(64,0,0));
pixel.setPixelColor(2, pixel.Color(0,64,0));
pixel.setPixelColor(3, pixel.Color(0,0,64));
pixel.setPixelColor(4, pixel.Color(64,0,0));
pixel.setPixelColor(5, pixel.Color(0,64,0));
pixel.setPixelColor(6, pixel.Color(0,0,64));
pixel.setPixelColor(7, pixel.Color(64,0,0));
pixel.setPixelColor(8, pixel.Color(0,64,0));
pixel.setPixelColor(9, pixel.Color(0,0,64));
pixel.setPixelColor(10, pixel.Color(64,0,0));
pixel.setPixelColor(11, pixel.Color(0,64,0));
pixel.setPixelColor(12, pixel.Color(0,0,64));
pixel.setPixelColor(13, pixel.Color(64,0,0));
pixel.setPixelColor(14, pixel.Color(0,64,0));
pixel.setPixelColor(15, pixel.Color(0,0,64));
pixel.show();

  } else if (SchrittNr == 16) {
pixel.setPixelColor(0, pixel.Color(64,0,0));
pixel.setPixelColor(1, pixel.Color(0,64,0));
pixel.setPixelColor(2, pixel.Color(0,0,64));
pixel.setPixelColor(3, pixel.Color(64,0,0));
pixel.setPixelColor(4, pixel.Color(0,64,0));
pixel.setPixelColor(5, pixel.Color(0,0,64));
pixel.setPixelColor(6, pixel.Color(64,0,0));
pixel.setPixelColor(7, pixel.Color(0,64,0));
pixel.setPixelColor(8, pixel.Color(0,0,64));
pixel.setPixelColor(9, pixel.Color(64,0,0));
pixel.setPixelColor(10, pixel.Color(0,64,0));
pixel.setPixelColor(11, pixel.Color(0,0,64));
pixel.setPixelColor(12, pixel.Color(64,0,0));
pixel.setPixelColor(13, pixel.Color(0,64,0));
pixel.setPixelColor(14, pixel.Color(0,0,64));
pixel.setPixelColor(15, pixel.Color(64,0,0));

pixel.show();
  }
}

int Counter=0;
void loop() {

  while(millis()-Starttick < 20000) {
    Counter++;
    DoPixel(Counter%16 + 1);
    DoPixel2(Counter%3 + 1);
    delay(Warte);
  
    Warte=(Warte+(Warte/10));
  }
}


const int DatenPin2 = SCL;
const int AnzahlPixel2 = 7;
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(AnzahlPixel2, DatenPin2);

void setup2 () {
int Zaeler;
pixel2.begin();
pixel2.setPixelColor(Zaeler, pixel2.Color(0,0,0));
Starttick=millis();
}

void DoPixel2(int SchrittNr) {

if (SchrittNr == 1) {
pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,64,0));
pixel2.setPixelColor(2, pixel2.Color(0,0,64));
pixel2.setPixelColor(3, pixel2.Color(64,0,0));
pixel2.setPixelColor(4, pixel2.Color(0,64,0));
pixel2.setPixelColor(5, pixel2.Color(0,0,64));
pixel2.setPixelColor(6, pixel2.Color(64,0,0));
pixel2.show();

  } else if (SchrittNr == 2) {

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,0,64));
pixel2.setPixelColor(2, pixel2.Color(64,0,0));
pixel2.setPixelColor(3, pixel2.Color(0,64,0));
pixel2.setPixelColor(4, pixel2.Color(0,0,64));
pixel2.setPixelColor(5, pixel2.Color(64,0,0));
pixel2.setPixelColor(6, pixel2.Color(0,64,0));
pixel2.show();
  } else if (SchrittNr == 3) {


pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(64,0,0));
pixel2.setPixelColor(2, pixel2.Color(0,64,0));
pixel2.setPixelColor(3, pixel2.Color(0,0,64));
pixel2.setPixelColor(4, pixel2.Color(64,0,0));
pixel2.setPixelColor(5, pixel2.Color(0,64,0));
pixel2.setPixelColor(6, pixel2.Color(0,0,64));
pixel2.show();
  }
}
