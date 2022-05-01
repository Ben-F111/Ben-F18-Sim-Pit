#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 300

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3


// Define the array of leds
CRGB leds[NUM_LEDS];


const int MasterArmFirstLed = 24;

const int Ext_Led_1_Offset  = 0;
const int Ext_Led_2_Offset  = 2;

const int Discharge_Led_1_Offset  = 1;
const int Discharge_Led_2_Offset  = 3;

const int AA_Led_1_Offset  = 25;
const int AA_Led_2_Offset  = 26;

const int AG_Led_1_Offset  = 27;
const int AG_Led_2_Offset  = 28;


int Master_Backlight_Brightness = 80;
int Master_Indicator_Brightness = 255;

  
void setup() { 

  Serial.begin(250000);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed


  // Now turn the LED off, then pause
  for (int i=0; i <= 99; i++) {
    leds[i] = CRGB::Black;
  }

  // Turn the LED on, then pause
  for (int i=0; i <= 99; i++) {
    leds[i] = CRGB::Green ;
  }

  FastLED.show();

  delay(1000);

    // Turn the LED off 
  for (int i=0; i <= 99; i++) {
    leds[i] = CRGB::Black ;
  }

  FastLED.show();
  
}


void SET_AA_LED( int LedStatus) {
 
  if (LedStatus == 0) {
    leds[MasterArmFirstLed + AA_Led_1_Offset] = CRGB::Black ;
    leds[MasterArmFirstLed + AA_Led_2_Offset] = CRGB::Black ;      
  } 
  else {
    leds[MasterArmFirstLed + AA_Led_1_Offset] = CHSV(HUE_GREEN, 255, Master_Indicator_Brightness);
    leds[MasterArmFirstLed + AA_Led_2_Offset] = CHSV(HUE_GREEN, 255, Master_Indicator_Brightness);    
  }

  FastLED.show();
  
}


void SET_AG_LED( int LedStatus) {
 
  if (LedStatus == 0) {
    leds[MasterArmFirstLed + AG_Led_1_Offset] = CRGB::Black ;
    leds[MasterArmFirstLed + AG_Led_2_Offset] = CRGB::Black ;      
  } 
  else {
    leds[MasterArmFirstLed + AG_Led_1_Offset] = CHSV(HUE_GREEN, 255, Master_Indicator_Brightness);
    leds[MasterArmFirstLed + AG_Led_2_Offset] = CHSV(HUE_GREEN, 255, Master_Indicator_Brightness);    
  }

  FastLED.show();
  
}



void SET_EXT_READY_LED( int LedStatus) {
 


  if (LedStatus == 0) {
    leds[MasterArmFirstLed + Ext_Led_1_Offset] = CRGB::Black ;
    leds[MasterArmFirstLed + Ext_Led_2_Offset] = CRGB::Black ;      
  } 
  else {
    leds[MasterArmFirstLed + Ext_Led_1_Offset] = CHSV(HUE_GREEN, 255, Master_Indicator_Brightness - 60);
    leds[MasterArmFirstLed + Ext_Led_2_Offset] = CHSV(HUE_GREEN, 255, Master_Indicator_Brightness - 60) ;    
  }

  FastLED.show();
  
}


void SET_EXT_DISCHARGE_LED( int LedStatus) {
 

  if (LedStatus == 0) {
    leds[MasterArmFirstLed + Discharge_Led_1_Offset] = CRGB::Black ;
    leds[MasterArmFirstLed + Discharge_Led_2_Offset] = CRGB::Black ;      
  } 
  else {
    leds[MasterArmFirstLed + Discharge_Led_1_Offset] = CHSV(HUE_RED, 255, Master_Indicator_Brightness); 
    leds[MasterArmFirstLed + Discharge_Led_2_Offset] = CHSV(HUE_RED, 255, Master_Indicator_Brightness);    
  }

  FastLED.show();
  
}




void SET_MASTER_ARM_BACKLIGHT( int LedStatus) {

  for ( int i = 0; i <= 32; i++) {

    if (i != Ext_Led_1_Offset && i != Ext_Led_2_Offset && i != AG_Led_1_Offset && i != AG_Led_2_Offset 
       && i != AA_Led_1_Offset && i != AA_Led_2_Offset && i != Discharge_Led_1_Offset && i != Discharge_Led_2_Offset) {
    
        if (LedStatus != 0) 
          //leds[MasterArmFirstLed + i]  = CRGB::Green; 
          leds[MasterArmFirstLed + i]  = CHSV(HUE_GREEN, 255, Master_Backlight_Brightness); 
        else
          leds[MasterArmFirstLed + i]  = CRGB::Black;
        
    }
    
  }


  FastLED.show();
  
}

void loop() { 

//  for (int i = MasterArmFirstLed; i <= MasterArmFirstLed + 32; i++) {
//
//    Serial.println(i- MasterArmFirstLed + 1);
//    
//    leds[i] = CRGB::Green ;
// 
//    if (i >= 1 ) {
//      leds[i-1] = CRGB::Black ;  
//    } else {
//      
//    }
//    
//    FastLED.show(); 
//    delay(1000);
//  }

//SET_AA_LED(1);
//delay(1000);
//SET_AA_LED(0);
//delay(1000);
//
//SET_AG_LED(1);
//delay(1000);
//SET_AG_LED(0);
//delay(1000);

//SET_EXT_READY_LED(1);
//delay(1000);
//SET_EXT_READY_LED(0);
//delay(1000);
//SET_EXT_DISCHARGE_LED(1);
//delay(1000);
//SET_EXT_DISCHARGE_LED(0);
//delay(1000);
//SET_MASTER_ARM_BACKLIGHT(1);
//delay(1000);
//SET_MASTER_ARM_BACKLIGHT(0);
//delay(1000);

SET_EXT_READY_LED(1);
SET_EXT_DISCHARGE_LED(1);
SET_AA_LED(1);
SET_MASTER_ARM_BACKLIGHT(1);
delay(4000);

SET_EXT_READY_LED(0);
SET_EXT_DISCHARGE_LED(0);
SET_AA_LED(0);
SET_MASTER_ARM_BACKLIGHT(0);
delay(4000);



}
