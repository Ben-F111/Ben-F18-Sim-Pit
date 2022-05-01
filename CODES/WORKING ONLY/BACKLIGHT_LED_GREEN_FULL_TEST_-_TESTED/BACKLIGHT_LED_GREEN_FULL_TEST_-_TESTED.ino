// COMM 4 - Current PC Conection for Ref

/*
 * OCT 2021 VERSION 1.0 - TESTED WORKING
DCS BACKLIGHTING CODE BY BEN MELROSE
TO CONTROL OPEN HORNET WS2812B LED BACK LIGHT PANELS BY L.WALKER
CODE IS CURRENTLY WRITTEN FOR 3 CHANNELS OF BACKLIGHTING
CODE WILL BE UPDATED TO A FULL PIT CODE DURING TESTING
 */

#include "FastLED.h"
#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define NUM_LEDS_CHA 52 // Open Hornet SPIN RCVRY Backlight PCB Version 2.0 by L Walker
#define DATA_PIN_CHA 9 // ARDUINO PIN that SPIN RCVRY is connected to
#define NUM_LEDS_CHB 400 // Generic Backlight loop - Left Console (400 LEDS) Change as required
#define DATA_PIN_CHB 8 // ARDUINO PIN that Left Console is connected to
#define NUM_LEDS_CHC 400 // Generic Backlight loop - Right Console (400 LEDS) Change as required
#define DATA_PIN_CHC 10 // ARDUINO PIN that Right Console is connected to

#define LED_TYPE    WS2812B

// Define the array of leds relating to SPIN RCVRY Backlight PCB Version 2.0 by L Walker
// GREEN BACKLIGHT LED NUMBERS
int spinPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52};
// SPIN LIGHT LED NUMBERS
int flashRed[ ] = {29, 36}; // NOTE Not Used, LEDs asigned below, however left in here for REF

int i; //General int
unsigned int instDim; // Consoles Dimmer Knob Value - Via DCS
unsigned int cautDim; // Caution Dimmer Knob Value - Via DCS
int spinLT; //Spin Light Dimmer Value
int spinOn; // Spin Light On or Off

CRGB leds_A[NUM_LEDS_CHA]; // LEDS CHAN A - PIN 16
CRGB leds_B[NUM_LEDS_CHB]; // LEDS CHAN B - PIN 17
CRGB leds_C[NUM_LEDS_CHC]; // LEDS CHAN C - PIN 18
/// ADD EXTRA CHANNELS AS REQUIRED

void onConsolesDimmerChange(unsigned int newValue) {
instDim = map(newValue, 0, 65535, 0, 150);
// MAP The Concole DIM Value from DCS to 0 - 150 ## NOTE MAX Can Be 255 ##
}
DcsBios::IntegerBuffer consolesDimmerBuffer(0x7544, 0xffff, 0, onConsolesDimmerChange);

void onWarnCautionDimmerChange(unsigned int newValue) {
cautDim = map(newValue, 0, 65535, 0, 150);
// MAP The Caution Lights DIM Value from DCS to 0 - 150 ## NOTE MAX Can Be 255 ##
}
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);

void onSpinLtChange(unsigned int newValue) {
if (newValue == 1) {
  spinLT = cautDim; // Set Brightness of "SPIN" Light LEDS
  spinOn =1; // SET the "SPIN" Light (ON)
  }
  else {
    spinLT = 0; // Set Brightness of "SPIN" Light LEDS to 0
    spinOn = 0; // SET the "SPIN" Light (OFF)
        }
  }
DcsBios::IntegerBuffer spinLtBuffer(0x742a, 0x0800, 11, onSpinLtChange);

void setup() 
{ 
   DcsBios::setup();
 FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);   
 FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);   
 FastLED.addLeds<WS2812B, DATA_PIN_CHC, RGB>(leds_C, NUM_LEDS_CHC);   
}

void loop() {
     DcsBios::loop();

      static int prevInstDim;
  if (instDim != prevInstDim)
  {
    prevInstDim = instDim;
    for (i = 0; i < 50; i++) 
    // SPIN Green LEDS CHANNEL A (PIN 16)
    leds_A[spinPanel[i]] = CHSV( 0, 255, instDim); // SET SPIN PANEL GREEN BRIGHTNESS
    // generic Consoles below "GREEN" CHANNEL B AND C (PINS 17 & 18)
    fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Red); // Set all Console LEDs to ##GREEN##. 
    FastLED.setBrightness(instDim); // SET CONSOLE PANELS GREEN BRIGHTNESS
    FastLED.show();
  }
  // SET SPIN RED LEDS to FLASH via DCS OUTPUT
        static int SpinOn;
  if (spinOn != SpinOn)
  {
    SpinOn = spinOn;
  leds_A[29] = CHSV( 96, 255, spinLT); // SPIN PANEL LED 29 "96 = RED"
  leds_A[36] = CHSV( 96, 255, spinLT); // SPIN PANEL LED 36 "96 = RED"
    FastLED.show();
  }
}

//END
