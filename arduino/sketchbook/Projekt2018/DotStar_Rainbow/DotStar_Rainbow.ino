// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 64 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    5
#define CLOCKPIN   4
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void SetColor(int row, int col, int ctr) {
  const int dimmer = 1;
  
  ctr = ctr % 768;
  if (ctr < 256) {
    strip.setPixelColor(row*8+7-col, strip.Color((255-ctr)/dimmer,ctr/dimmer,0));
  } else if (ctr < 512) {
    strip.setPixelColor(row*8+7-col, strip.Color(0,(511-ctr)/dimmer,(ctr-256)/dimmer));
  } else if (ctr < 768) {
    strip.setPixelColor(row*8+7-col, strip.Color((ctr-512)/dimmer,0,(767-ctr)/dimmer));
  }
}


void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  strip.setBrightness(16);
}

int Counter = 0;
void loop() {
  int i;
  int Anz;

  Anz = 28;
  if (Counter >= 768) { Counter = 0; }  
  for(i = 0; i < Anz; i++) {
    if (i < 8) 
      SetColor(i, 0, Counter+i*(768/Anz));
    else if (i < 15)
      SetColor(7, i-7, Counter+i*(768/Anz));
    else if (i < 22)
      SetColor(21-i, 7, Counter+i*(768/Anz));
    else
      SetColor(0, 28-i, Counter+i*(768/Anz));
  }

  Anz = 12;
  if (Counter >= 768) { Counter = 0; }  
  for(i = 0; i < Anz; i++) {
    if (i < 4) 
      SetColor(5-i, 2, Counter+i*(768/Anz));
    else if (i < 7)
      SetColor(2, i-1, Counter+i*(768/Anz));
    else if (i < 10)
      SetColor(i-4, 5, Counter+i*(768/Anz));
    else
      SetColor(5, 14-i, Counter+i*(768/Anz));
  }

  strip.show();                     // Refresh strip
  Counter++;
//  delay(1);                        // Pause milliseconds
}

