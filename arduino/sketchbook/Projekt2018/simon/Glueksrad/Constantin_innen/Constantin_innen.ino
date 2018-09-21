#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
const int DatenPin2 = SCL;
const int AnzahlPixel2 = 7;
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(AnzahlPixel2, DatenPin2);
unsigned long Starttick;
int Laufzeit;

void setup () {
int Zaeler;
pixel2.begin();
pixel2.setPixelColor(Zaeler, pixel2.Color(0,0,0));
Starttick=millis();
}


int Warte=20;
void loop() {
  
while(millis()-Starttick < 20000)

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,64,0));
pixel2.setPixelColor(2, pixel2.Color(0,0,64));
pixel2.setPixelColor(3, pixel2.Color(64,0,0));
pixel2.setPixelColor(4, pixel2.Color(0,64,0));
pixel2.setPixelColor(5, pixel2.Color(0,0,64));
pixel2.setPixelColor(6, pixel2.Color(64,0,0));
pixel2.show();

delay(Warte);

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,64,0));
pixel2.setPixelColor(2, pixel2.Color(0,0,46));
pixel2.setPixelColor(3, pixel2.Color(64,0,0));
pixel2.setPixelColor(4, pixel2.Color(0,64,0));
pixel2.setPixelColor(5, pixel2.Color(0,0,64));
pixel2.setPixelColor(6, pixel2.Color(64,0,0));
pixel2.show();

delay(Warte);

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,0,64));
pixel2.setPixelColor(2, pixel2.Color(64,0,0));
pixel2.setPixelColor(3, pixel2.Color(0,64,0));
pixel2.setPixelColor(4, pixel2.Color(0,0,64));
pixel2.setPixelColor(5, pixel2.Color(64,0,0));
pixel2.setPixelColor(6, pixel2.Color(0,64,0));
pixel2.show();

delay(Warte);

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,0,64));
pixel2.setPixelColor(2, pixel2.Color(64,0,0));
pixel2.setPixelColor(3, pixel2.Color(0,64,0));
pixel2.setPixelColor(4, pixel2.Color(0,0,64));
pixel2.setPixelColor(5, pixel2.Color(64,0,0));
pixel2.setPixelColor(6, pixel2.Color(0,64,0));
pixel2.show();

delay(Warte);

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,0,64));
pixel2.setPixelColor(2, pixel2.Color(64,0,0));
pixel2.setPixelColor(3, pixel2.Color(0,64,0));
pixel2.setPixelColor(4, pixel2.Color(0,0,64));
pixel2.setPixelColor(5, pixel2.Color(64,0,0));
pixel2.setPixelColor(6, pixel2.Color(0,64,0));
pixel2.show();

delay(Warte);

pixel2.setPixelColor(0, pixel2.Color(0,0,0));
pixel2.setPixelColor(1, pixel2.Color(0,0,64));
pixel2.setPixelColor(2, pixel2.Color(64,0,0));
pixel2.setPixelColor(3, pixel2.Color(0,64,0));
pixel2.setPixelColor(4, pixel2.Color(0,0,64));
pixel2.setPixelColor(5, pixel2.Color(64,0,0));
pixel2.setPixelColor(6, pixel2.Color(0,64,0));
pixel2.show();

Warte=(Warte+(Warte/10));
}
