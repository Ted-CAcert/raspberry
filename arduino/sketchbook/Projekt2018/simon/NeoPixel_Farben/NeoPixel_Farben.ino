/* NeoPixel_Hello
 *  
 * Einfaches Programm für die NeoPixel von Adafruit.com
 * https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels
 */

/*
 * "Einbinden" der benätigten Bibliotheken
 */
 // Grundlagen für alle Arduino-Funktionen
#include <Arduino.h>
// Bibliothek zum Steuern der NeoPixels
#include <Adafruit_NeoPixel.h>

// Konstante, über welches "Beinchen" werden die Daten geschickt
const int DatenPin = 5;
const int DatenPin2 = SCL;
// Konstante, Anzahl angeschlossener Pixel
const int AnzahlPixel = 20;

// Objekt zum Steuern der Pixel
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(AnzahlPixel, DatenPin);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(AnzahlPixel, DatenPin2);

// Diese Funktion wird bei jedem Einschalten aufgerufen, genauer: nach jedem "Reset"
void setup() {
  // Für klare Verhältnisse sorgen: Alle Pixel ausschalten
  int Zaehler;

  pixel.begin(); // Muss immer am Anfang einmal aufgerufen werden
  pixel2.begin(); // Muss immer am Anfang einmal aufgerufen werden
  for(Zaehler=0;Zaehler<AnzahlPixel;Zaehler++) {
    // Setze das Pixel im "Bildschirmspeicher" aus
    pixel.setPixelColor(Zaehler, pixel.Color(0,0,0));
    pixel2.setPixelColor(Zaehler, pixel.Color(0,0,0));
  }
  // Damit wird der Bindschirmspeicher zu den einzelnen Pixeln übertragen
  pixel.show();
  pixel2.show();
}

void SetColor(int PixIndex, int ctr) {
  const int dimmer = 1;
  
  ctr = ctr % 768;
  if (ctr < 256) {
    pixel.setPixelColor(PixIndex, pixel.Color((255-ctr)/dimmer,ctr/dimmer,0));
  } else if (ctr < 512) {
    pixel.setPixelColor(PixIndex, pixel.Color(0,(511-ctr)/dimmer,(ctr-256)/dimmer));
  } else if (ctr < 768) {
    pixel.setPixelColor(PixIndex, pixel.Color((ctr-512)/dimmer,0,(767-ctr)/dimmer));
  }
}

int Counter = 0;
// Diese Funktion wird immer wieder aufgerufen wenn sie beendet wird
void loop() {
  int i;
  int Helligkeit;
/*  
  for(Counter=0;Counter<768;Counter++) {
  
    for(i = 0; i < AnzahlPixel; i++) {
      SetColor(i, Counter+(i*(768/AnzahlPixel)));
    }
    pixel.show();
  
    delay(2.5);
  }
*/
  for(Counter=0;Counter<20;Counter++) {
    for (i = 0; i < AnzahlPixel; i++) {
      if (i == Counter) {
        Helligkeit=255;
      } else if (abs(Counter-i) == 1) {
        Helligkeit=64;
      } else if (abs(Counter-i) == 2) {
        Helligkeit=16;        
      } else {
        Helligkeit = 0;
      }
     switch(i%3) {
      case 0:
        pixel.setPixelColor(i, pixel.Color(Helligkeit,0,0));
        pixel2.setPixelColor(i, pixel.Color(0,0,Helligkeit));
        break;
        
      case 1:
        pixel.setPixelColor(i, pixel.Color(0,Helligkeit,0));
        pixel2.setPixelColor(i, pixel.Color(Helligkeit,0,0));
        break;
        
      case 2:
        pixel.setPixelColor(i, pixel.Color(0,0,Helligkeit));
        pixel2.setPixelColor(i, pixel.Color(0,Helligkeit,0));
        break;
        
      }
    }
    pixel.show();
    pixel2.show();
    delay(100);
  }
}
