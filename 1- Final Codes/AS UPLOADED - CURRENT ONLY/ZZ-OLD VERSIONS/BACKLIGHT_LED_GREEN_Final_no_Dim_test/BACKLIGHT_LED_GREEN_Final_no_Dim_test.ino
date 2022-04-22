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

#define NUM_LEDS_CHA 132// Open Hornet UPPER INST PANEL = (
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
#define NUM_LEDS_CHD 102 // Generic Backlight loop - LIP (78 LEDS) Change as required
#define DATA_PIN_CHD 46 // ARDUINO PIN that LIP is connected to
// ECM JETT - LEDS 0-77 (78 LEDS)
//    12 OCLOCK = LED 74
//    03 OCLOCK = LED 75 // NOT USED
//    06 OCLOCK = LED 76
//    09 OCLOCK = LED 77 // NOT USED
// VID RECORD PANEL - LEDS 78 - 93 (16 LEDS) // NO INDIVIDUAL LEDS REQUIRED
// JETT STATION SELECT - LEDS 94 - 102 (8 LEDS)
//    SPIN (1) = LED 114 (115TH LED)
//    SPIN (2) = LED 121 (122ND LED)
#define NUM_LEDS_CHE 385 // Generic Backlight loop - LIP (400 LEDS) Change as required
#define DATA_PIN_CHE 48 // ARDUINO PIN that Right Console is connected to
//
//#define NUM_LEDS_CHF 401 // Generic Backlight loop - SPARE (400 LEDS) Change as required
//#define DATA_PIN_CHF 49 // ARDUINO PIN that Right Console is connected to

#define LED_TYPE    WS2812B

// Define the array of leds relating to SPIN RCVRY Backlight PCB Version 2.0 by L Walker
// GREEN BACKLIGHT LED NUMBERS

int uipPanelsMA[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 29};
int NumMA = 22;
int uipPanelsHD[] = {29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 75, 77, 78, 79, 80, 81, 82, 83, 84};
int NumHD = 55;
int uipPanelsSP[] = {85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 115, 116, 117, 118, 119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 134, 135};
int NumSP = 53;

// CONNECTION ORDER - "MASTER ARM" - "HUD" - "SPIN" ON THE UPPER INST PANEL


// GREEN BACKLIGHT LED NUMBERS
int ecmJettPanel[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73};
int NumECM = 74;
int VidRecPanel[ ] = {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93};
int NumVR = 16;
int JETTStSel[ ] = {94, 95, 96, 97, 98, 99, 100, 101, 102};
int NumJSS = 8;

// CONNECTION ORDER - "ECM JETT" - "VID RECORD" - "JETT STASTION SELECT" ON THE LOWER INST PANEL

int u1; // upper inst panel Set 1 uipPanelsMA - Master Arm
int u2; // upper inst panel Set 2 uipPanelsHD - HUD Panel
int u3; // upper inst panel Set 2 uipPanelsSP - Spin Panel

int l1; // lower inst panel Set 1 - ECM JETT
int l2; // lower inst panel Set 2 - VID RECORD
int l3; // lower inst panel Set 3 - JETT STATION SELECT

int i; //General int

int b; //NUM_LEDS_CHB 393 LEFT CONSOLE
int m; //NUM_LEDS_CHE 385 RIGHT CONSOLE


int instDim; // Consoles Dimmer Knob Value - Via DCS
int conDim;
int cautDim; // Caution Dimmer Knob Value - Via DCS
int spinLT; //Spin Light Dimmer Value
int spinOn; // Spin Light On or Off
int consSW; // NVG/NITE/DAY Switch
int ecmLT; // EMC JETT LIGHT (GREEN)
int ecmON; // EMC Light ON (GREEN)
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
CRGB leds_D[NUM_LEDS_CHD];
CRGB leds_E[NUM_LEDS_CHE];
//CRGB leds_F[NUM_LEDS_CHF];
/// ADD EXTRA CHANNELS AS REQUIRED
/*
void onCockkpitLightModeSwChange(unsigned int newValue) {
  consSW = newValue; // (0, 1, 2)
}
DcsBios::IntegerBuffer cockkpitLightModeSwBuffer(0x74c8, 0x0600, 9, onCockkpitLightModeSwChange);

void onWarnCautionDimmerChange(unsigned int newValue) {
  cautDim = map(newValue, 0, 65535, 20, 150);
  // MAP The Caution Lights DIM Value from DCS to 20 - 150 ## NOTE MAX Can Be 255 ##
}
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);
*/
// SPIN LIGHT

void onSpinLtChange(unsigned int newValue) {
  if (newValue == 1) {
    //spinLT = cautDim; // Set Brightness of "SPIN" Light LEDS
    spinLT = 120; // Set Brightness of "SPIN" Light LEDS
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
    //aaLT = cautDim; // Set Brightness of "AA" Light LEDS
    aaLT = 120; // Set Brightness of "AA" Light LEDS
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
    //agLT = cautDim; // Set Brightness of "AAG" Light LEDS
    agLT = 120; // Set Brightness of "AAG" Light LEDS
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
    // rdyLT = cautDim; // Set Brightness of "AAG" Light LEDS
    rdyLT = 120; // Set Brightness of "AAG" Light LEDS
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
    //disLT = cautDim; // Set Brightness of "AAG" Light LEDS
    disLT = 120; // Set Brightness of "AAG" Light LEDS
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
    //ecmLT = cautDim; // Set Brightness of "ECM" Light LEDS
    ecmLT = 120; // Set Brightness of "ECM" Light LEDS
    ecmON = 1; // SET the "ECM" Light (ON)
  }
  else {
    ecmON = 0; // Set Brightness of "ECM/JETT" Light LEDS to 0
    ecmLT = 0; // SET the "ECM/JETT" Light (OFF)
  }
}
DcsBios::IntegerBuffer cmsdJetSelLBuffer(0x74d6, 0x8000, 15, onCmsdJetSelLChange);


void setup()
{

  FastLED.addLeds<WS2812B, DATA_PIN_CHA, RGB>(leds_A, NUM_LEDS_CHA);
  FastLED.addLeds<WS2812B, DATA_PIN_CHB, RGB>(leds_B, NUM_LEDS_CHB);
  //FastLED.addLeds<WS2812B, DATA_PIN_CHC, RGB>(leds_C, NUM_LEDS_CHC);  // ADD IF REQUIRED
  FastLED.addLeds<WS2812B, DATA_PIN_CHD, RGB>(leds_D, NUM_LEDS_CHD);
  FastLED.addLeds<WS2812B, DATA_PIN_CHE, RGB>(leds_E, NUM_LEDS_CHE);
  //FastLED.addLeds<WS2812B, DATA_PIN_CHF, RGB>(leds_F, NUM_LEDS_CHF);  // ADD IF REQUIRED

  DcsBios::setup();
}

void loop() {
  noInterrupts();
  DcsBios::loop();
  interrupts();
  {

    // CONSOLES DIMMERS

    conDim = map(analogRead(A4) , 0, 1024, 10, 150);

    for (u1 = 0; u1 < NumMA; u1++) //22
      leds_A[uipPanelsMA[u1]] = CHSV( 0, 255, conDim); // SET MASTER ARM GREEN BRIGHTNESS
    for (u2 = 0; u2 < NumHD; u2++) //55
      leds_A[uipPanelsHD[u2]] = CHSV( 0, 255, conDim); // SET HUD GREEN BRIGHTNESS
    for (u3 = 0; u3 < NumSP; u3++) //53
      leds_A[uipPanelsSP[u3]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS

    for (b = 0; b < NUM_LEDS_CHB; b++)
      leds_B[b] = CHSV( 0, 255, conDim); // SET LEFT CONSOLE GREEN BRIGHTNESS (ALL)

    for (l1 = 0; l1 < NumECM; l1++) //77
      leds_D[ecmJettPanel[l1]] = CHSV( 0, 255, conDim); // SET MASTER ARM GREEN BRIGHTNESS
    for (l2 = 0; l2 < NumVR; l2++) //22
      leds_D[VidRecPanel[l2]] = CHSV( 0, 255, conDim); // SET HUD GREEN BRIGHTNESS
    for (l3 = 0; l3 < NumJSS; l3++) //8
      leds_D[JETTStSel[l3]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS//   for (j = 0; j < NUM_LEDS_CHD; j++)

    for (m = 0; m < NUM_LEDS_CHE; m++)
      leds_E[m] = CHSV( 0, 255, conDim); // SET RIGHT CONSOLE GREEN BRIGHTNESS (ALL)
    //     leds_D[ecmJettPanel[j]] = CHSV( 0, 255, conDim); // SET SPIN PANEL GREEN BRIGHTNESS

    FastLED.show();
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

  static int EcmON;
  if (ecmON != EcmON)
  {
    EcmON = ecmON;
    leds_D[74] = CHSV( 0, 255, ecmLT); // EMC LED 75 "96 = RED"
    leds_D[76] = CHSV( 0, 255, ecmLT); // EMC LED 77 "96 = RED"
    FastLED.show();
  }

}

//END
