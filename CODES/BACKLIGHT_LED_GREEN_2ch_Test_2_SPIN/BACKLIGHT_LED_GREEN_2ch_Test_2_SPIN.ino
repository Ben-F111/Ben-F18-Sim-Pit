#include <FastLED.h>

#define NUM_LEDS_CHA 52
#define DATA_PIN_CHA 16



#define LED_TYPE    WS2812B

#define MAX_BRIGHTNESS 150
#define MIN_BRIGHTNESS 0  
const int brightnessInPin = A4;  // The Analog input pin that the brightness control potentiometer is attached to.

// Define the array of leds

int GRN; // GREEN LEDS
int RD; // RED LEDS

int spinPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52};
int flashRed[ ] = {29, 36};
int i; 
int dim;

CRGB leds_A[NUM_LEDS_CHA];

void setup() 
{ 
//  FastLED[0];
 FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
}

void loop() {
    green(); 
    spin();
}
void green() {
 int dim = map(analogRead(brightnessInPin), 0, 1023, 0, 160);
for (i = 0; i < 50; i++) 
  {
  leds_A[spinPanel[i]] = CHSV( 0, 255, dim);
}
       FastLED.show();
}
 void spin() {
static unsigned long lastTime = 0;
  const long interval = 250;
  static bool state = 0;

  unsigned long now = millis();

  if ( now - lastTime > interval && state == 0) {
    state = 1;
    lastTime = now;
  leds_A[29] = CHSV( 96, 255, 255);
    leds_A[36] = CHSV( 96, 255, 255);
   FastLED.show();
  }

  if ( now - lastTime > interval && state == 1) {
    state = 0;
    lastTime = now;
    leds_A[29] = CRGB::Black;
  leds_A[36] = CRGB::Black;
    FastLED.show();
  }
}
