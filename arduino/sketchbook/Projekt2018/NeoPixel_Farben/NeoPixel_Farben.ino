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
// Konstante, Anzahl angeschlossener Pixel
const int AnzahlPixel = 16;

// Objekt zum Steuern der Pixel
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(AnzahlPixel, DatenPin);

// Diese Funktion wird bei jedem Einschalten aufgerufen, genauer: nach jedem "Reset"
void setup() {
  // Für klare Verhältnisse sorgen: Alle Pixel ausschalten
  int Zaehler;

  pixel.begin(); // Muss immer am Anfang einmal aufgerufen werden
  for(Zaehler=0;Zaehler<AnzahlPixel;Zaehler++) {
    // Setze das Pixel im "Bildschirmspeicher" aus
    pixel.setPixelColor(Zaehler, pixel.Color(0,0,0));
  }
  // Damit wird der Bindschirmspeicher zu den einzelnen Pixeln übertragen
  pixel.show();
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
  
  if (Counter >= 768) { Counter = 0; }

  for(i = 0; i < AnzahlPixel; i++) {
    SetColor(i, Counter+i*(768/AnzahlPixel));
  }
  pixel.show();
  Counter++;

  delay(10);
}
