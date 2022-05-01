// COMM 6

#include "FastLED.h"
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define NUM_LEDS_CHA 52
#define DATA_PIN_CHA 16



#define LED_TYPE    WS2812B


// Define the array of leds

int GRN; // GREEN LEDS
int RD; // RED LEDS

int spinPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52};
int flashRed[ ] = {29, 36}; // Not Used, LEDs asigned below
int i; 
int dim;
unsigned int instDim;
unsigned int cautDim;
int spinLT;
int spinOn;

CRGB leds_A[NUM_LEDS_CHA];

void onConsolesDimmerChange(unsigned int newValue) {
instDim = map(newValue, 0, 65535, 0, 150);
}
DcsBios::IntegerBuffer consolesDimmerBuffer(0x7544, 0xffff, 0, onConsolesDimmerChange);

void onWarnCautionDimmerChange(unsigned int newValue) {
cautDim = map(newValue, 0, 65535, 0, 150);
}
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);

void onSpinLtChange(unsigned int newValue) {
if (newValue == 1) {
  spinLT = cautDim;
  spinOn =1;
  }
  else {
    spinLT = 0;
    spinOn = 0;
}
}
DcsBios::IntegerBuffer spinLtBuffer(0x742a, 0x0800, 11, onSpinLtChange);


void setup() 
{ 
   DcsBios::setup();
 FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
}

void loop() {
     DcsBios::loop();
  //  green(); 
   // spin();
      static int prevInstDim;
  if (instDim != prevInstDim)
  {
    prevInstDim = instDim;
    for (i = 0; i < 50; i++) 
  leds_A[spinPanel[i]] = CHSV( 0, 255, instDim);
    FastLED.show();
  }
        static int SpinOn;
  if (spinOn != SpinOn)
  {
    SpinOn = spinOn;
  leds_A[29] = CHSV( 96, 255, spinLT);
  leds_A[36] = CHSV( 96, 255, spinLT);
    FastLED.show();
  }
}
/*void green() {
 int dim = map(analogRead(brightnessInPin), 0, 1023, 0, 160);
for (i = 0; i < 50; i++) 
  {
  leds_A[spinPanel[i]] = CHSV( 0, 255, dim);
}
       FastLED.show();
}
*/
/*
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
*/
void setColorHSV(byte h, byte s, byte v) {
  // create a new HSV color
  CHSV color = CHSV(h, s, v);

  // use FastLED to set the color of all LEDs in the strip to the same color
  fill_solid(leds_A, NUM_LEDS_CHA, color);
   
}
//END
