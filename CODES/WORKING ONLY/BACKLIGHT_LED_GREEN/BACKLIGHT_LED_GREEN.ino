#include <FastLED.h>


#define NUM_LEDS_CHA 40
#define DATA_PIN_CHA 14

#define LED_TYPE    WS2812B

#define MAX_BRIGHTNESS 100    // Thats full on, watch the power!
#define MIN_BRIGHTNESS 100       // set to a minimum of 25%
const int brightnessInPin = A4;  // The Analog input pin that the brightness control potentiometer is attached to.

// Define the array of leds
CRGB leds_A[NUM_LEDS_CHA];




void setup() 
{ 
  FastLED.clear(); // CLEAR ALL LEDS AFTER TEST
     FastLED.show();
FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##. 

}

void loop() 
{ 
 int mappedValue = map(analogRead(brightnessInPin), 0, 1023, 225, 255);
 FastLED.setBrightness(constrain(mappedValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all to ##GREEN##


      FastLED.show();

}
