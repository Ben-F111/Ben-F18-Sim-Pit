// COMM 6

#include <FastLED.h>
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define NUM_LEDS_CHA 400
#define DATA_PIN_CHA 16

#define NUM_LEDS_CHB 400
#define DATA_PIN_CHB 17

#define LED_TYPE    WS2812B

#define MAX_BRIGHTNESS 200    // Thats full on, watch the power!
#define MIN_BRIGHTNESS 0       // set to a minimum of 25%
const int brightnessInPin = A4;  // The Analog input pin that the brightness control potentiometer is attached to.
int instOff;



// Define the array of leds
CRGB leds_A[NUM_LEDS_CHA];
CRGB leds_B[NUM_LEDS_CHB];

void onConsoleIntLtChange(unsigned int newValue) {
  if (newValue == 1) {
      fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Green); // Set all to ##GREEN##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all to ##GREEN##.
    FastLED.setBrightness(0);
    FastLED.show();
  }
    if (newValue == 0) {
 fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all to ##GREEN##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Green); // Set all to ##GREEN##.
    FastLED.setBrightness(200);    
    FastLED.show();
  }
}
DcsBios::IntegerBuffer consoleIntLtBuffer(0x74d4, 0x2000, 13, onConsoleIntLtChange);

void setup() 
{

  }

void loop() {
   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all to ##GREEN##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Green); // Set all to ##GREEN##.
    FastLED.setBrightness(200);    
    FastLED.show();
}
