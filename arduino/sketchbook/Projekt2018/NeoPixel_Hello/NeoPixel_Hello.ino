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
const int AnzahlPixel = 20;
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

// Diese Funktion wird immer wieder aufgerufen wenn sie beendet wird
void loop() {
  // Setze das 1. Pixel (Index 0) auf Rot, alle anderen aus
  pixel.setPixelColor(0, pixel.Color(255,0,0));
  pixel.setPixelColor(1, pixel.Color(0,0,0));
  pixel.setPixelColor(2, pixel.Color(0,0,0));
  pixel.show();

  //Warte 1 Sekunde (1000 Millisekunden)
  delay(1000);

  // Setze das 2. Pixel (Index 1) auf Grün, alle anderen aus
  pixel.setPixelColor(0, pixel.Color(0,0,0));
  pixel.setPixelColor(1, pixel.Color(0,255,0));
  pixel.setPixelColor(2, pixel.Color(0,0,0));
  pixel.show();

  //Warte 1 Sekunde (1000 Millisekunden)
  delay(1000);

  // Setze das 3. Pixel (Index 2) auf Blau, alle anderen aus
  pixel.setPixelColor(0, pixel.Color(0,0,0));
  pixel.setPixelColor(1, pixel.Color(0,0,0));
  pixel.setPixelColor(2, pixel.Color(0,0,255));
  pixel.show();

  //Warte 1 Sekunde (1000 Millisekunden)
  delay(1000);
}
