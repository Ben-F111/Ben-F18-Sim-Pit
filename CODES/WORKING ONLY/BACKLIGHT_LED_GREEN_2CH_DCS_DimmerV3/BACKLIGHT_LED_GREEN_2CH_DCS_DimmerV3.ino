// COMM 6

#include "FastLED.h"
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define NUM_LEDS_CHA 400
#define DATA_PIN_CHA 8

#define NUM_LEDS_CHB 400
#define DATA_PIN_CHB 9
#define LED_TYPE    WS2812B


unsigned int instOff;
int dimLVL;


// Define the array of leds
CRGB leds_A[NUM_LEDS_CHA];
CRGB leds_B[NUM_LEDS_CHB];

void onConsolesDimmerChange(unsigned int newValue) {
instOff = map(newValue, 0, 65535, 0, 150);
}
DcsBios::IntegerBuffer consolesDimmerBuffer(0x7544, 0xffff, 0, onConsolesDimmerChange);


void setup() 
{

     FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);  
 
       fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    FastLED.setBrightness(225); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();

   DcsBios::setup();
// can set "instOFF" here in set up to any value (0-65000) and the below maps fine
// change "instOff" to grab the value from DCS-BIOS, No Joy
  }

void loop() {
  static int prevInstOff;
  if (instOff != prevInstOff)
  {
    fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    FastLED.setBrightness(instOff); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
  }
   DcsBios::loop();
}
 /*
void setColorHSV(byte h, byte s, byte v) {
  // create a new HSV color
  CHSV color = CHSV(h, s, v);

  // use FastLED to set the color of all LEDs in the strip to the same color
  fill_solid(leds_A, NUM_LEDS_CHA, color);
    fill_solid(leds_B, NUM_LEDS_CHB, color);
}*/
