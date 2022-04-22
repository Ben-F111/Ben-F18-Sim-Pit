// UPDATED TO DSCS-BIOS FP EDITION FOR OPEN HORNET


////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
//||               FUNCTION = HORNET LED OUTPUT MAX 7219              ||\\
//||              LOCATION IN THE PIT = LIP LEFTHAND SIDE             ||\\
//||            ARDUINO PROCESSOR TYPE = Arduino MEGA 256             ||\\
//||      ARDUINO CHIP SERIAL NUMBER = SN - 7533133393935151F160      ||\\
//||      ETHERNET SHEILD MAC ADDRESS = MAC - A8:61:0A:AE:6F:83       ||\\
//||                    CONNECTED COM PORT = COM 9                    ||\\
//||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\


/*
  JAN VERSION 1.0 - TESTED WORKING
  DCS BACKLIGHTING CODE BY BEN MELROSE
  TO CONTROL OPEN HORNET WS2812B LED BACK LIGHT PANELS BY L.WALKER
  CODE IS CURRENTLY WRITTEN FOR 6 CHANNELS OF BACKLIGHTING-

  CODE WILL BE UPDATED TO A FULL PIT CODE DURING TESTING
  NVD / DAY / NIGHT SWITCH ADDED

*/

#include "FastLED.h"

#define DCSBIOS_DEFAULT_SERIAL
//#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#define FASTLED_ALLOW_INTERRUPTS 0
//

// PINS 40, 42, 44 LEFT SIDE POWER BOARD
// PINS 46, 48, 49 RIGHT SIDE POWER BOARD

#define NUM_LEDS_CHA 137// Open Hornet UPPER INST PANEL Backlight PCBS Version 2.0 by L Walker
#define DATA_PIN_CHA 40// ARDUINO PIN that UPPER INST PANEL is connected to // CHANGE AS REQUIRED
// CONNECTION ORDER - -->"MASTER ARM" --> "HUD" --> "SPIN" ON THE UPPER INST PANEL
//MASTER ARM - LEDS 0-28 (29 LEDS)
//    READY = LED 0&1 (1ST 2ND LED)
//    DISCH = LED 2&3 (3RD 4TH LED)
//    AA = LEDS 25 & 26
//    AG = LEDS 27 & 28
// HUD PANEL - LEDS 29 - 84 (55 LEDS) // NO INDIVIDUAL LEDS REQUIRED
// SPIN RECRY PANEL - LEDS 85 - 138 (53 LEDS)
//    SPIN (1) = LED 114 (115TH LED)
//    SPIN (2) = LED 121 (122ND LED)
//
#define NUM_LEDS_CHB 393 // Generic Backlight loop - Left Console (~400 LEDS) Change as required
#define DATA_PIN_CHB 42// ARDUINO PIN that Left Console is connected to
//
//#define NUM_LEDS_CHC 401 // Generic Backlight loop - Right Console (400 LEDS) Change as required
//#define DATA_PIN_CHC 44 // ARDUINO PIN that Left Console is connected to
//
//#define NUM_LEDS_CHD 401 // Generic Backlight loop - UIP (78 LEDS) Change as required
//#define DATA_PIN_CHD 46 // ARDUINO PIN that LIP is connected to
//
#define NUM_LEDS_CHE 385 // Generic Backlight loop - LIP (400 LEDS) Change as required
#define DATA_PIN_CHE 48 // ARDUINO PIN that Right Console is connected to
//
//#define NUM_LEDS_CHF 401 // Generic Backlight loop - SPARE (400 LEDS) Change as required
//#define DATA_PIN_CHF 49 // ARDUINO PIN that Right Console is connected to

#define LED_TYPE    WS2812B

// Define the array of leds relating to SPIN RCVRY Backlight PCB Version 2.0 by L Walker
// GREEN BACKLIGHT LED NUMBERS
//int spinPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53};
int uipPanelsMA[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 115, 116, 117, 118, 119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 134, 135, 136};
int uipPanelsHD[] = {29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 75, 77, 78, 79, 80, 81, 82, 83, 84};
int uipPanelsSP[] = {85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 134, 135};


// CONNECTION ORDER - "MASTER ARM" - "HUD" - "SPIN" ON THE UPPER INST PANEL

// GREEN BACKLIGHT LED NUMBERS
//int emcJettPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7};//, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 75, 77};

int u1;
int u2;
int u3;


int i; //General int
int j; // INT used for JETT Button
int k;
int l;
int m;


int instDim; // Consoles Dimmer Knob Value - Via DCS
int conDim;
int cautDim; // Caution Dimmer Knob Value - Via DCS
int spinLT; //Spin Light Dimmer Value
int spinOn; // Spin Light On or Off
int consSW; // NVG/NITE/DAY Switch
int emcLT; // EMC JETT LIGHT (GREEN)
int emcON; // EMC Light ON (GREEN)
int aaLT; // AA light MASTER ARM
int aaON; // AA light MASTER ARM
int agLT; // AG light MASTER ARM
int agON; // AG light MASTER ARM
int rdyLT; // READY light MASTER ARM
int rdyON; // READY light MASTER ARM
int disLT; // DISCH light MASTER ARM
int disON; // DISCH light MASTER ARM

CRGB leds_A[NUM_LEDS_CHA];
CRGB leds_B[NUM_LEDS_CHB];
//CRGB leds_C[NUM_LEDS_CHC];
//CRGB leds_D[NUM_LEDS_CHD];
CRGB leds_E[NUM_LEDS_CHE];
//CRGB leds_F[NUM_LEDS_CHF];
/// ADD EXTRA CHANNELS AS REQUIRED

void onCockkpitLightModeSwChange(unsigned int newValue) {
  consSW = newValue; // (0, 1, 2)
}
DcsBios::IntegerBuffer cockkpitLightModeSwBuffer(0x74c8, 0x0600, 9, onCockkpitLightModeSwChange);

void onConsolesDimmerChange(unsigned int newValueI) {
  //if (consSW == 2) {
   // instDim = 1;
  //}
//  else {
    instDim = map(newValueI, 0, 65535, 0, 220);
//  }
 }
DcsBios::IntegerBuffer consolesDimmerBuffer(0x7544, 0xffff, 0, onConsolesDimmerChange);

void onInstPnlDimmerChange(unsigned int newValueC) {
 // if (consSW == 2) {
  //  conDim = 1;
 // }
//  else {
    conDim = map(newValueC, 0, 65535, 0, 220);
  //}
}
DcsBios::IntegerBuffer instPnlDimmerBuffer(0x7546, 0xffff, 0, onInstPnlDimmerChange);


void onWarnCautionDimmerChange(unsigned int newValue) {
  cautDim = map(newValue, 0, 65535, 20, 150);
  // MAP The Caution Lights DIM Value from DCS to 0 - 150 ## NOTE MAX Can Be 255 ##
}
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);

// SPIN LIGHT

void onSpinLtChange(unsigned int newValue) {
  if (newValue == 1) {
    spinLT = cautDim; // Set Brightness of "SPIN" Light LEDS
    spinOn = 1; // SET the "SPIN" Light (ON)
  }
  else {
    spinLT = 0; // Set Brightness of "SPIN" Light LEDS to 0
    spinOn = 0; // SET the "SPIN" Light (OFF)
  }
}
DcsBios::IntegerBuffer spinLtBuffer(0x742a, 0x0800, 11, onSpinLtChange);

//AA LIGHT

void onMasterModeAaLtChange(unsigned int newValue) {
  if (newValue == 1) {
    aaLT = cautDim; // Set Brightness of "AA" Light LEDS
    aaON = 1; // SET the "AA" Light (ON)
  }
  else {
    aaON = 0; // Set Brightness of "AA" Light LEDS to 0
    aaLT = 0; // SET the "AA" Light (OFF)
  }
}
DcsBios::IntegerBuffer masterModeAaLtBuffer(0x740c, 0x0200, 9, onMasterModeAaLtChange);

// AG LIGHT

void onMasterModeAgLtChange(unsigned int newValue) {
  if (newValue == 1) {
    agLT = cautDim; // Set Brightness of "AAG" Light LEDS
    agON = 1; // SET the "AG" Light (ON)
  }
  else {
    agON = 0; // Set Brightness of "AG" Light LEDS to 0
    agLT = 0; // SET the "AG" Light (OFF)
  }
}
DcsBios::IntegerBuffer masterModeAgLtBuffer(0x740c, 0x0400, 10, onMasterModeAgLtChange);

// READY LIGHT

void onMcReadyChange(unsigned int newValue) {
  if (newValue == 1) {
    rdyLT = cautDim; // Set Brightness of "AAG" Light LEDS
    rdyON = 1; // SET the "AG" Light (ON)
  }
  else {
    rdyON = 0; // Set Brightness of "AG" Light LEDS to 0
    rdyLT = 0; // SET the "AG" Light (OFF)
  }
}
DcsBios::IntegerBuffer mcReadyBuffer(0x740c, 0x8000, 15, onMcReadyChange);

// DISCH LIGHT

void onMcDischChange(unsigned int newValue) {
  if (newValue == 1) {
    disLT = cautDim; // Set Brightness of "AAG" Light LEDS
    disON = 1; // SET the "AG" Light (ON)
  }
  else {
    disON = 0; // Set Brightness of "AG" Light LEDS to 0
    disLT = 0; // SET the "AG" Light (OFF)
  }
}
DcsBios::IntegerBuffer mcDischBuffer(0x740c, 0x4000, 14, onMcDischChange);

void onCmsdJetSelLChange(unsigned int newValue) {
  if (newValue == 1) {
    emcLT = cautDim; // Set Brightness of "SPIN" Light LEDS
    emcON = 1; // SET the "SPIN" Light (ON)
  }
  else {
    emcON = 0; // Set Brightness of "SEL/JETT" Light LEDS to 0
    emcLT = 0; // SET the "SEL/JETT" Light (OFF)
  }
}
DcsBios::IntegerBuffer cmsdJetSelLBuffer(0x74d6, 0x8000, 15, onCmsdJetSelLChange);


void setup()
{

  FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);
 // FastLED.addLeds<WS2812B, DATA_PIN_CHC, RGB>(leds_C, NUM_LEDS_CHC);
 // FastLED.addLeds<WS2812B, DATA_PIN_CHD, RGB>(leds_D, NUM_LEDS_CHD);
  FastLED.addLeds<WS2812B, DATA_PIN_CHE, RGB>(leds_E, NUM_LEDS_CHE);    // ADD IF REQUIRED
//  FastLED.addLeds<WS2812B, DATA_PIN_CHF, RGB>(leds_F, NUM_LEDS_CHF);    // ADD IF REQUIRED
  FastLED.clear(); // CLEAR ALL LEDS AFTER TEST
  FastLED.setBrightness(0);
  FastLED.show();
  delay(2000);
  FastLED.clear(); // CLEAR ALL LEDS AFTER TEST
  FastLED.show();
  delay(1000);
  //TEST ALL LEDS -RED-BLUE-BLACK-GREEN
  fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##.
 // fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Red); // Set all Console LEDs to ##GREEN##.
 // fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Red); // Set all Console LEDs to ##GREEN##.
  fill_solid(leds_E, NUM_LEDS_CHE, CRGB::Red); // Set all Console LEDs to ##GREEN##.
 // fill_solid(leds_F, NUM_LEDS_CHF, CRGB::Red); // Set all Console LEDs to ##GREEN##.
  FastLED.setBrightness(150); // SET CONSOLE PANELS GREEN BRIGHTNESS = TEST ALL GREEN
  FastLED.show();
  delay(1000);
  fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Green); // Set all Console LEDs to ##RED##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Green); // Set all Console LEDs to ##RED##.
 // fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Green); // Set all Console LEDs to ##RED##.
 // fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Green); // Set all Console LEDs to ##RED##.
  fill_solid(leds_E, NUM_LEDS_CHE, CRGB::Green); // Set all Console LEDs to ##RED##.
 // fill_solid(leds_F, NUM_LEDS_CHF, CRGB::Green); // Set all Console LEDs to ##RED##.
  FastLED.setBrightness(150); // SET CONSOLE PANELS RED BRIGHTNESS - TEST ALL RED
  FastLED.show();
  delay(1000);
  fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Blue); // Set all Console LEDs to ##BLUE##.
  fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Blue); // Set all Console LEDs to ##BLUE##.
  //fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Blue); // Set all Console LEDs to ##BLUE##.
  //fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Blue); // Set all Console LEDs to ##BLUE##.
  fill_solid(leds_E, NUM_LEDS_CHE, CRGB::Blue); // Set all Console LEDs to ##BLUE##.
//  fill_solid(leds_F, NUM_LEDS_CHF, CRGB::Blue); // Set all Console LEDs to ##BLUE##.
  FastLED.setBrightness(150); // SET CONSOLE PANELS BLUE BRIGHTNESS - TEST ALL BLUE
  FastLED.show();
  delay(500);
  FastLED.clear(); // CLEAR ALL LEDS AFTER TEST
  // FastLED.setBrightness(0);
  FastLED.show();
  //  BELOW USED FOR TESTING ONLY - ALL LIGHTS STAY ON
  /*fill_solid(leds_A, NUM_LEDS_CHA, CRGB::Red); // Set all Console LEDs to ##GREEN##.
    fill_solid(leds_B, NUM_LEDS_CHB, CRGB::Red); // Set all Console LEDs to ##GREEN##.
    fill_solid(leds_C, NUM_LEDS_CHC, CRGB::Red); // Set all Console LEDs to ##GREEN##.
    fill_solid(leds_D, NUM_LEDS_CHD, CRGB::Red); // Set all Console LEDs to ##GREEN##.
    fill_solid(leds_E, NUM_LEDS_CHE, CRGB::Red); // Set all Console LEDs to ##GREEN##.
    fill_solid(leds_F, NUM_LEDS_CHF, CRGB::Red); // Set all Console LEDs to ##GREEN##.
    FastLED.setBrightness(100); // SET CONSOLE PANELS GREEN BRIGHTNESS = TEST ALL GREEN
    FastLED.show();
  */
  DcsBios::setup();
}

void loop() {
  noInterrupts();
  DcsBios::loop();
  interrupts();
  {   
      // CONSOLES DIMMERS
        static int previnstDim;
    if ((instDim != previnstDim) && (consSW != 2))
    {
      for (k = 0; k < NUM_LEDS_CHB; k++)
        leds_B[k] = CHSV( 0, 255, instDim); // SET CONSOLE GREEN BRIGHTNESS
      for (m = 0; m < NUM_LEDS_CHE; m++)
       leds_E[m] = CHSV( 0, 255, instDim); // SET CONSOLE GREEN BRIGHTNESS
    }else{
        for (k = 0; k < NUM_LEDS_CHB; k++)
        leds_B[k] = CHSV( 0, 255, 0); // SET CONSOLE GREEN BRIGHTNESS
      for (m = 0; m < NUM_LEDS_CHE; m++)
       leds_E[m] = CHSV( 0, 255, 0); // SET CONSOLE GREEN BRIGHTNESS
    
       FastLED.show();
    }
  }
  {
    static int prevConDim;
    if (conDim != prevConDim)
    {
      prevConDim = conDim;
      for (u1 = 0; u1 < 124; u1++)
        leds_A[uipPanelsMA[u1]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS
      for (u2 = 0; u2 < 55; u2++)
        leds_A[uipPanelsHD[u2]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS
      for (u3 = 0; u3 < 53; u3++)
        leds_A[uipPanelsSP[u3]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS

      //  for (j = 0; j < 78; j++)
      //  leds_D[emcJettPanel[j]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS
      FastLED.show();
    }
  }
  // SET SPIN RED LEDS to FLASH via DCS OUTPUT
  static int SpinOn;
  if (spinOn != SpinOn)
  {
    SpinOn = spinOn;
    leds_A[114] = CHSV( 96, 255, spinLT); // SPIN PANEL LED 29 "96 = RED"
    leds_A[121] = CHSV( 96, 255, spinLT); // SPIN PANEL LED 36 "96 = RED"
    FastLED.show();
  }

  // SET AA GREEN LEDS to via DCS OUTPUT// MASTER ARM PANEL
  static int AAOn;
  if (aaON != AAOn)
  {
    AAOn = aaON;
    leds_A[25] = CHSV( 0, 255, aaLT); // AA PANEL "GREEN"
    leds_A[26] = CHSV( 0, 255, aaLT); // AA PANEL "GREEN"
    FastLED.show();
  }

  // SET AG GREEN LEDS to via DCS OUTPUT// MASTER ARM PANEL
  static int AGOn;
  if (agON != AGOn)
  {
    AGOn = agON;
    leds_A[27] = CHSV( 0, 255, agLT); // AG PANEL "GREEN"
    leds_A[28] = CHSV( 0, 255, agLT); // AG PANEL "GREEN"
    FastLED.show();
  }

  // READY LIGHT// MASTER ARM PANEL
  static int RDYOn;
  if (rdyON != RDYOn)
  {
    RDYOn = rdyON;
    leds_A[0] = CHSV( 70, 255, rdyLT); // READY LIGHT
    leds_A[2] = CHSV( 70, 255, rdyLT); // READY LIGHT
    FastLED.show();
  }

  // DISCH LIGHT// MASTER ARM PANEL
  static int DISOn;
  if (disON != DISOn)
  {
    DISOn = disON;
    leds_A[1] = CHSV( 0, 255, disLT); // DISCH LIGHT "GREEN"
    leds_A[3] = CHSV( 0, 255, disLT); // DISCH LIGHT "GREEN"
    FastLED.show();
  }

/*
  static int EmcON;
  if (emcON != EmcON)
  {
    EmcON = emcON;
    leds_D[74] = CHSV( 0, 255, emcLT); // EMC LED 75 "96 = RED"
    leds_D[76] = CHSV( 0, 255, emcLT); // EMC LED 77 "96 = RED"
    FastLED.show();
  }
*/
}

//END
