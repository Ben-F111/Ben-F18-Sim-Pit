#include <FastLED.h>
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define NUM_LEDS 400
#define DATA_PIN 16

#define LED_TYPE    WS2812B
int navMode;

#define MAX_BRIGHTNESS 200    // Thats full on, watch the power!
#define MIN_BRIGHTNESS 0       // set to a minimum of 25%
const int brightnessInPin = A4;  // The Analog input pin that the brightness control potentiometer is attached to.

// Define the array of leds
CRGB leds[NUM_LEDS];

void onCockkpitLightModeSwChange(unsigned int newValue) {
navMode = newValue;
}
DcsBios::IntegerBuffer cockkpitLightModeSwBuffer(0x74c8, 0x0600, 9, onCockkpitLightModeSwChange);

void setup() 
{ 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);   
   fill_solid(leds, NUM_LEDS, CRGB::Red); // Set all to ##GREEN##.
}

void loop() 
{ 

 if (navMode = 2) { 
    
  FastLED.setBrightness(0);
 
   FastLED.show();
 } else;
 { 
   int mappedValue = map(analogRead(brightnessInPin), 30, 1023, 0, 255);
  FastLED.setBrightness(constrain(mappedValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
        FastLED.show();
 }

  


}
