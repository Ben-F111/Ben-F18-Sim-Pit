// COMM 6

#include "FastLED.h"
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

/* Declare a Master Caution Reset button on pin 10 */
DcsBios::Switch2Pos ufcMasterCaution("UFC_MASTER_CAUTION", 10);
/* Make the LED connected to pin 13 into a Master Caution Light */
DcsBios::LED masterCautionLt(0x7408, 0x0200, 13);

void setup() 
{
    DcsBios::setup();
//    pinMode(LED_BUILTIN, OUTPUT);
 // FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
 // FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);   

// can set "instOFF" here in set up to any value (0-65000) and the below maps fine
// change "instOff" to grab the value from DCS-BIOS, No Joy
  }

void loop() {
 
   byte byte0 = (255); 
  byte byte1 = (225);
  byte byte2 = (200); // brigtness value
// byte byte2 = (60);
    setColorHSV(byte0, byte1, byte2);
  FastLED.show();
  }
 
void setColorHSV(byte h, byte s, byte v) {
  // create a new HSV color
  CHSV color = CHSV(h, s, v);

  // use FastLED to set the color of all LEDs in the strip to the same color
  FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);
}
