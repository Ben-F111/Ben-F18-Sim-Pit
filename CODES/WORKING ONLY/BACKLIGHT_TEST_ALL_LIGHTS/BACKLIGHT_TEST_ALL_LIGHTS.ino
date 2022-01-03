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

   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
  
    FastLED.setBrightness(225); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
    delay(3000);
  FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);  

   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Black); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Black); // Set all Console LEDs to ##GREEN##. 
  
    FastLED.setBrightness(225); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
delay(2000);
  DcsBios::setup();

  }

void loop() {
   DcsBios::loop();

    FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);  

   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
  
    FastLED.setBrightness(225); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
    delay(500);

        FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);  

   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Green); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Green); // Set all Console LEDs to ##GREEN##. 
  
    FastLED.setBrightness(225); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
    delay(500);
            FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);  

   fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Blue); // Set all Console LEDs to ##GREEN##. 
   fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Blue); // Set all Console LEDs to ##GREEN##. 
  
    FastLED.setBrightness(225); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
    delay(500);
}

 /*
void setColorHSV(byte h, byte s, byte v) {
  // create a new HSV color
  CHSV color = CHSV(h, s, v);

  // use FastLED to set the color of all LEDs in the strip to the same color
  fill_solid(leds_A, NUM_LEDS_CHA, color);
    fill_solid(leds_B, NUM_LEDS_CHB, color);
}*/
