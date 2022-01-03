/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */
#define DCSBIOS_IRQ_SERIAL
#include "LedControl.h"
#include "DcsBios.h"

// Master
//#define LEFT_EWI 1
//#define UFC_PANEL 1       // Currently should equal LEFT_EWI
//#define RIGHT_EWI 2
//#define CAUTION_PANEL 3
//#define SELECT_JET_PANEL 0


// Play area
#define LEFT_EWI 0
#define UFC_PANEL 0       // Currently should equal LEFT_EWI
#define RIGHT_EWI 2
#define CAUTION_PANEL 3
#define SELECT_JET_PANEL 1

// NO GO  - LEFT EWI - ORANGE
#define NO_GO_A_ROW 0
#define NO_GO_A_COL 0
#define NO_GO_B_ROW 1
#define NO_GO_B_COL 0

// GO  - LEFT EWI - GREEN
#define GO_A_ROW 2
#define GO_A_COL 0
#define GO_B_ROW 3
#define GO_B_COL 0

// RIGHT BLEED - LEFT EWI - RED
#define R_BLEED_A_ROW 0
#define R_BLEED_A_COL 1
#define R_BLEED_B_ROW 1
#define R_BLEED_B_COL 1


// LEFT BLEED - LEFT EWI - RED
#define L_BLEED_A_ROW 2
#define L_BLEED_A_COL 1
#define L_BLEED_B_ROW 3
#define L_BLEED_B_COL 1

// STBY - LEFT EWI - ORANGE
#define STBY_A_ROW 0
#define STBY_A_COL 2
#define STBY_B_ROW 1
#define STBY_B_COL 2

// SPD BRK- LEFT EWI - GREEN
#define SPD_BRK_A_ROW 2
#define SPD_BRK_A_COL 2
#define SPD_BRK_B_ROW 3
#define SPD_BRK_B_COL 2

// REC - LEFT EWI - ORANGE
#define REC_A_ROW 0
#define REC_A_COL 3
#define REC_B_ROW 1
#define REC_B_COL 3

// LAUNCH BAR RED - LEFT EWI - RED
#define L_BAR_RED_A_ROW 2
#define L_BAR_RED_A_COL 3
#define L_BAR_RED_B_ROW 3
#define L_BAR_RED_B_COL 3

// XMIT - LEFT EWI - GREEN
#define XMIT_A_ROW 0
#define XMIT_A_COL 4
#define XMIT_B_ROW 1
#define XMIT_B_COL 4

// LAUNCH BAR GREEN - LEFT EWI - GREEN
#define L_BAR_GREEN_A_ROW 2
#define L_BAR_GREEN_A_COL 4
#define L_BAR_GREEN_B_ROW 3
#define L_BAR_GREEN_B_COL 4

// ASPJ OH - LEFT EWI - ORANGE
#define ASPJ_OH_A_ROW 0
#define ASPJ_OH_A_COL 5
#define ASPJ_OH_B_ROW 1
#define ASPJ_OH_B_COL 5

// MASTER CAUTION - LEFT EWI - ORANGE
#define MSTR_CAUT_A_ROW 4
#define MSTR_CAUT_A_COL 1
#define MSTR_CAUT_B_ROW 4
#define MSTR_CAUT_B_COL 2
#define MSTR_CAUT_C_ROW 5
#define MSTR_CAUT_C_COL 1
#define MSTR_CAUT_D_ROW 5
#define MSTR_CAUT_D_COL 2

// LEFT FIRE - LEFT EWI - RED
#define LEFT_FIRE_A_ROW 6
#define LEFT_FIRE_A_COL 1
#define LEFT_FIRE_B_ROW 6
#define LEFT_FIRE_B_COL 2
#define LEFT_FIRE_C_ROW 7
#define LEFT_FIRE_C_COL 1
#define LEFT_FIRE_D_ROW 7
#define LEFT_FIRE_D_COL 2

// RCDR ON  - RIGHT EWI - GREEN
#define RCDR_ON_A_ROW 0
#define RCDR_ON_A_COL 0
#define RCDR_ON_B_ROW 1
#define RCDR_ON_B_COL 0

// DISP  - RIGHT EWI - GREEN
#define DISP_A_ROW 2
#define DISP_A_COL 0
#define DISP_B_ROW 3
#define DISP_B_COL 0

// SPARE 1 - RIGHT EWI - GREEN
#define SPARE_1_A_ROW 0
#define SPARE_1_A_COL 1
#define SPARE_1_B_ROW 1
#define SPARE_1_B_COL 1

// SPARE 2 - RIGHT EWI - GREEN
#define SPARE_2_A_ROW 2
#define SPARE_2_A_COL 1
#define SPARE_2_B_ROW 3
#define SPARE_2_B_COL 1

// SPARE 3 - RIGHT EWI - GREEN
#define SPARE_3_A_ROW 0
#define SPARE_3_A_COL 2
#define SPARE_3_B_ROW 1
#define SPARE_3_B_COL 2

// SPARE 4 - RIGHT EWI - GREEN
#define SPARE_4_A_ROW 2
#define SPARE_4_A_COL 2
#define SPARE_4_B_ROW 3
#define SPARE_4_B_COL 2

// SPARE 5 - RIGHT EWI - GREEN
#define SPARE_5_A_ROW 0
#define SPARE_5_A_COL 3
#define SPARE_5_B_ROW 1
#define SPARE_5_B_COL 3

// SAM - RIGHT EWI - RED
#define SAM_A_ROW 2
#define SAM_A_COL 3
#define SAM_B_ROW 3
#define SAM_B_COL 3

// AI - RIGHT EWI - RED
#define AI_A_ROW 0
#define AI_A_COL 4
#define AI_B_ROW 1
#define AI_B_COL 4

// AAA - RIGHT EWI - RED
#define AAA_A_ROW 2
#define AAA_A_COL 4
#define AAA_B_ROW 3
#define AAA_B_COL 4

// CW - RIGHT EWI - RED
#define CW_A_ROW 0
#define CW_A_COL 5
#define CW_B_ROW 1
#define CW_B_COL 5

// APU FIRE - RIGHT EWI - RED
#define APU_FIRE_A_ROW 4
#define APU_FIRE_A_COL 1
#define APU_FIRE_B_ROW 4
#define APU_FIRE_B_COL 2
#define APU_FIRE_C_ROW 5
#define APU_FIRE_C_COL 1
#define APU_FIRE_D_ROW 5
#define APU_FIRE_D_COL 2

// RIGHT FIRE - RIGHT EWI - RED
#define RIGHT_FIRE_A_ROW 6
#define RIGHT_FIRE_A_COL 1
#define RIGHT_FIRE_B_ROW 6
#define RIGHT_FIRE_B_COL 2
#define RIGHT_FIRE_C_ROW 7
#define RIGHT_FIRE_C_COL 1
#define RIGHT_FIRE_D_ROW 7
#define RIGHT_FIRE_D_COL 2

#define CK_SEAT_COL_A 0
#define CK_SEAT_ROW_A 0
#define CK_SEAT_COL_B 0
#define CK_SEAT_ROW_B 1

#define FCS_HOT_COL_A 1
#define FCS_HOT_ROW_A 0
#define FCS_HOT_COL_B 1
#define FCS_HOT_ROW_B 1

#define FUEL_LO_COL_A 2
#define FUEL_LO_ROW_A 0
#define FUEL_LO_COL_B 2
#define FUEL_LO_ROW_B 1

#define L_GEN_COL_A 3
#define L_GEN_ROW_A 0
#define L_GEN_COL_B 3
#define L_GEN_ROW_B 1

#define APU_ACC_COL_A 0
#define APU_ACC_ROW_A 2
#define APU_ACC_COL_B 0
#define APU_ACC_ROW_B 3

#define GEN_TIE_COL_A 1
#define GEN_TIE_ROW_A 2
#define GEN_TIE_COL_B 1
#define GEN_TIE_ROW_B 3

#define FCES_COL_A 2
#define FCES_ROW_A 2
#define FCES_COL_B 2
#define FCES_ROW_B 3

#define R_GEN_COL_A 3
#define R_GEN_ROW_A 2
#define R_GEN_COL_B 3
#define R_GEN_ROW_B 3

#define BATT_SW_COL_A 0
#define BATT_SW_ROW_A 4
#define BATT_SW_COL_B 0
#define BATT_SW_ROW_B 5

#define C_SPARE_1_COL_A 1
#define C_SPARE_1_ROW_A 4
#define C_SPARE_1_COL_B 1
#define C_SPARE_1_ROW_B 5

#define C_SPARE_2_COL_A 2
#define C_SPARE_2_ROW_A 4
#define C_SPARE_2_COL_B 2
#define C_SPARE_2_ROW_B 5

#define C_SPARE_3_COL_A 3
#define C_SPARE_3_ROW_A 4
#define C_SPARE_3_COL_B 3
#define C_SPARE_3_ROW_B 5

#define UFC_OPT1_COL_A 4
#define UFC_OPT1_ROW_A 4
#define UFC_OPT1_COL_B 3
#define UFC_OPT1_ROW_B 4

#define UFC_OPT2_COL_A 4
#define UFC_OPT2_ROW_A 5
#define UFC_OPT2_COL_B 3
#define UFC_OPT2_ROW_B 5

#define UFC_OPT3_COL_A 4
#define UFC_OPT3_ROW_A 6
#define UFC_OPT3_COL_B 3
#define UFC_OPT3_ROW_B 6

#define UFC_OPT4_COL_A 3
#define UFC_OPT4_ROW_A 7
#define UFC_OPT4_COL_B 4
#define UFC_OPT4_ROW_B 7

#define UFC_OPT5_COL_A 5
#define UFC_OPT5_ROW_A 4


#define SEL_CENTER_COL_A 0
#define SEL_CENTER_ROW_A 0

#define SEL_LEFT_INNER_COL_A 1
#define SEL_LEFT_INNER_ROW_A 0

#define SEL_LEFT_OUTER_COL_A 2
#define SEL_LEFT_OUTER_ROW_A 0

#define SEL_RIGHT_INNER_COL_A 0
#define SEL_RIGHT_INNER_ROW_A 1

#define SEL_RIGHT_OUTER_COL_A 1
#define SEL_RIGHT_OUTER_ROW_A 1

#define NOSE_GEAR_COL_A 3
#define NOSE_GEAR_ROW_A 0
#define NOSE_GEAR_COL_B 3
#define NOSE_GEAR_ROW_B 1

#define LEFT_GEAR_COL_A 4
#define LEFT_GEAR_ROW_A 0
#define LEFT_GEAR_COL_B 4
#define LEFT_GEAR_ROW_B 1

#define RIGHT_GEAR_COL_A 2
#define RIGHT_GEAR_ROW_A 1
#define RIGHT_GEAR_COL_B 1
#define RIGHT_GEAR_ROW_B 2

#define HALF_FLAPS_COL_A 5
#define HALF_FLAPS_ROW_A 0
#define HALF_FLAPS_COL_B 5
#define HALF_FLAPS_ROW_B 1

#define FULL_FLAPS_COL_A 0
#define FULL_FLAPS_ROW_A 2
#define FULL_FLAPS_COL_B 2
#define FULL_FLAPS_ROW_B 2

#define AMBER_FLAPS_COL_A 3
#define AMBER_FLAPS_ROW_A 2
#define AMBER_FLAPS_COL_B 4
#define AMBER_FLAPS_ROW_B 2


#define STATUS_LED_PORT 6
int devices = 2;

LedControl lc=LedControl(9,8,7,devices); 


/* paste code snippets from the reference documentation here */
DcsBios::Switch2Pos lightsTestSw("LIGHTS_TEST_SW", 22);
DcsBios::LED sjCtrLt(0x742e, 0x4000, 13);


void onFlpLgRightGearLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,RIGHT_GEAR_COL_A,RIGHT_GEAR_ROW_A,newValue);
  lc.setLed(SELECT_JET_PANEL,RIGHT_GEAR_COL_B,RIGHT_GEAR_ROW_B,newValue);  
}
DcsBios::IntegerBuffer flpLgRightGearLtBuffer(0x7430, 0x2000, 13, onFlpLgRightGearLtChange);

void onFlpLgNoseGearLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,NOSE_GEAR_COL_A,NOSE_GEAR_ROW_A,newValue);
  lc.setLed(SELECT_JET_PANEL,NOSE_GEAR_COL_B,NOSE_GEAR_ROW_B,newValue);
}
DcsBios::IntegerBuffer flpLgNoseGearLtBuffer(0x7430, 0x0800, 11, onFlpLgNoseGearLtChange);

void onFlpLgLeftGearLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,LEFT_GEAR_COL_A,LEFT_GEAR_ROW_A,newValue);
  lc.setLed(SELECT_JET_PANEL,LEFT_GEAR_COL_B,LEFT_GEAR_ROW_B,newValue); 
}
DcsBios::IntegerBuffer flpLgLeftGearLtBuffer(0x7430, 0x1000, 12, onFlpLgLeftGearLtChange);

void onFlpLgHalfFlapsLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,HALF_FLAPS_COL_A,HALF_FLAPS_ROW_A,newValue);
  lc.setLed(SELECT_JET_PANEL,HALF_FLAPS_COL_B,HALF_FLAPS_ROW_B,newValue); 
}
DcsBios::IntegerBuffer flpLgHalfFlapsLtBuffer(0x7430, 0x4000, 14, onFlpLgHalfFlapsLtChange);


void onFlpLgFullFlapsLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,FULL_FLAPS_COL_A,FULL_FLAPS_ROW_A,newValue);
  lc.setLed(SELECT_JET_PANEL,FULL_FLAPS_COL_B,FULL_FLAPS_ROW_B,newValue);  
}
DcsBios::IntegerBuffer flpLgFullFlapsLtBuffer(0x7430, 0x8000, 15, onFlpLgFullFlapsLtChange);

void onFlpLgFlapsLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,AMBER_FLAPS_COL_A,AMBER_FLAPS_ROW_A,newValue);
  lc.setLed(SELECT_JET_PANEL,AMBER_FLAPS_COL_B,AMBER_FLAPS_ROW_B,newValue); 
}
DcsBios::IntegerBuffer flpLgFlapsLtBuffer(0x7460, 0x0001, 0, onFlpLgFlapsLtChange);

void onSjRoLtChange(unsigned int newValue) {
lc.setLed(SELECT_JET_PANEL,SEL_RIGHT_OUTER_COL_A,SEL_RIGHT_OUTER_ROW_A,newValue);
}
DcsBios::IntegerBuffer sjRoLtBuffer(0x7430, 0x0400, 10, onSjRoLtChange);

void onSjRiLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,SEL_RIGHT_INNER_COL_A,SEL_RIGHT_INNER_ROW_A,newValue); 
}
DcsBios::IntegerBuffer sjRiLtBuffer(0x7430, 0x0200, 9, onSjRiLtChange);

void onSjLoLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,SEL_LEFT_OUTER_COL_A,SEL_LEFT_OUTER_ROW_A,newValue);
}
DcsBios::IntegerBuffer sjLoLtBuffer(0x7430, 0x0100, 8, onSjLoLtChange);


void onSjLiLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,SEL_LEFT_INNER_COL_A,SEL_LEFT_INNER_ROW_A,newValue);
}
DcsBios::IntegerBuffer sjLiLtBuffer(0x742e, 0x8000, 15, onSjLiLtChange);

void onSjCtrLtChange(unsigned int newValue) {
  lc.setLed(SELECT_JET_PANEL,SEL_CENTER_COL_A,SEL_CENTER_ROW_A,newValue);
}
DcsBios::IntegerBuffer sjCtrLtBuffer(0x742e, 0x4000, 14, onSjCtrLtChange);


void onUfcOptionCueing1Change(char* newValue) {
  if (newValue[0] == ':') {
    lc.setLed(UFC_PANEL,UFC_OPT1_COL_A,UFC_OPT1_ROW_A,1);
    lc.setLed(UFC_PANEL,UFC_OPT1_COL_B,UFC_OPT1_ROW_B,1); }
  else {
    lc.setLed(UFC_PANEL,UFC_OPT1_COL_A,UFC_OPT1_ROW_A,0);
    lc.setLed(UFC_PANEL,UFC_OPT1_COL_B,UFC_OPT1_ROW_B,0); }   
}
DcsBios::StringBuffer<1> ufcOptionCueing1Buffer(0x7428, onUfcOptionCueing1Change);

void onUfcOptionCueing2Change(char* newValue) {
  if (newValue[0] == ':') {
    lc.setLed(UFC_PANEL,UFC_OPT2_COL_A,UFC_OPT2_ROW_A,1);
    lc.setLed(UFC_PANEL,UFC_OPT2_COL_B,UFC_OPT2_ROW_B,1); }
  else {
    lc.setLed(UFC_PANEL,UFC_OPT2_COL_A,UFC_OPT2_ROW_A,0);
    lc.setLed(UFC_PANEL,UFC_OPT2_COL_B,UFC_OPT2_ROW_B,0); }   
}
DcsBios::StringBuffer<1> ufcOptionCueing2Buffer(0x742a, onUfcOptionCueing2Change);

void onUfcOptionCueing3Change(char* newValue) {
  if (newValue[0] == ':') {
    lc.setLed(UFC_PANEL,UFC_OPT3_COL_A,UFC_OPT3_ROW_A,1);
    lc.setLed(UFC_PANEL,UFC_OPT3_COL_B,UFC_OPT3_ROW_B,1); }
  else {
    lc.setLed(UFC_PANEL,UFC_OPT3_COL_A,UFC_OPT3_ROW_A,0);
    lc.setLed(UFC_PANEL,UFC_OPT3_COL_B,UFC_OPT3_ROW_B,0); }     
}
DcsBios::StringBuffer<1> ufcOptionCueing3Buffer(0x742c, onUfcOptionCueing3Change);

void onUfcOptionCueing4Change(char* newValue) {
  if (newValue[0] == ':') {
    lc.setLed(UFC_PANEL,UFC_OPT4_COL_A,UFC_OPT4_ROW_A,1);
    lc.setLed(UFC_PANEL,UFC_OPT4_COL_B,UFC_OPT4_ROW_B,1); }
  else {
    lc.setLed(UFC_PANEL,UFC_OPT4_COL_A,UFC_OPT4_ROW_A,0);
    lc.setLed(UFC_PANEL,UFC_OPT4_COL_B,UFC_OPT4_ROW_B,0); } 
}
DcsBios::StringBuffer<1> ufcOptionCueing4Buffer(0x742e, onUfcOptionCueing4Change);


void onUfcOptionCueing5Change(char* newValue) {
  if (newValue[0] == ':') {
    lc.setLed(UFC_PANEL,UFC_OPT5_COL_A,UFC_OPT5_ROW_A,1); }
  else {
    lc.setLed(UFC_PANEL,UFC_OPT5_COL_A,UFC_OPT5_ROW_A,0); } 
}
DcsBios::StringBuffer<1> ufcOptionCueing5Buffer(0x7430, onUfcOptionCueing5Change);

void onClipApuAccLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,APU_ACC_COL_A,APU_ACC_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,APU_ACC_COL_B,APU_ACC_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipApuAccLtBuffer(0x74a2, 0x8000, 15, onClipApuAccLtChange);




void onClipBattSwLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,BATT_SW_COL_A,BATT_SW_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,BATT_SW_COL_B,BATT_SW_ROW_B,newValue);  
}
DcsBios::IntegerBuffer clipBattSwLtBuffer(0x74a6, 0x0100, 8, onClipBattSwLtChange);

void onClipCkSeatLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,CK_SEAT_COL_A,CK_SEAT_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,CK_SEAT_COL_B,CK_SEAT_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipCkSeatLtBuffer(0x74a2, 0x4000, 14, onClipCkSeatLtChange);


void onClipFcesLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,FCES_COL_A,FCES_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,FCES_COL_B,FCES_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipFcesLtBuffer(0x74a6, 0x2000, 13, onClipFcesLtChange);

void onClipFcsHotLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,FCS_HOT_COL_A,FCS_HOT_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,FCS_HOT_COL_B,FCS_HOT_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipFcsHotLtBuffer(0x74a6, 0x0200, 9, onClipFcsHotLtChange);

void onClipFuelLoLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,FUEL_LO_COL_A,FUEL_LO_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,FUEL_LO_COL_B,FUEL_LO_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipFuelLoLtBuffer(0x74a6, 0x1000, 12, onClipFuelLoLtChange);

void onClipGenTieLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,GEN_TIE_COL_A,GEN_TIE_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,GEN_TIE_COL_B,GEN_TIE_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipGenTieLtBuffer(0x74a6, 0x0400, 10, onClipGenTieLtChange);

void onClipLGenLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,L_GEN_COL_A,L_GEN_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,L_GEN_COL_B,L_GEN_ROW_B,newValue);   
}
DcsBios::IntegerBuffer clipLGenLtBuffer(0x74a6, 0x8000, 15, onClipLGenLtChange);

void onClipRGenLtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,R_GEN_COL_A,R_GEN_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,R_GEN_COL_B,R_GEN_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipRGenLtBuffer(0x74ae, 0x0100, 8, onClipRGenLtChange);


void onClipSpareCtn1LtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,C_SPARE_1_COL_A,C_SPARE_1_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,C_SPARE_1_COL_B,C_SPARE_1_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipSpareCtn1LtBuffer(0x74a6, 0x0800, 11, onClipSpareCtn1LtChange);

void onClipSpareCtn2LtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,C_SPARE_2_COL_A,C_SPARE_2_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,C_SPARE_2_COL_B,C_SPARE_2_ROW_B,newValue);
}
DcsBios::IntegerBuffer clipSpareCtn2LtBuffer(0x74a6, 0x4000, 14, onClipSpareCtn2LtChange);


void onClipSpareCtn3LtChange(unsigned int newValue) {
  lc.setLed(CAUTION_PANEL,C_SPARE_3_COL_A,C_SPARE_3_ROW_A,newValue);
  lc.setLed(CAUTION_PANEL,C_SPARE_3_COL_B,C_SPARE_3_ROW_B,newValue);  
}
DcsBios::IntegerBuffer clipSpareCtn3LtBuffer(0x74ae, 0x0200, 9, onClipSpareCtn3LtChange);


void onFireLeftLtChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,LEFT_FIRE_A_COL,LEFT_FIRE_A_ROW,newValue);
  lc.setLed(LEFT_EWI,LEFT_FIRE_B_COL,LEFT_FIRE_B_ROW,newValue);
  lc.setLed(LEFT_EWI,LEFT_FIRE_C_COL,LEFT_FIRE_C_ROW,newValue);
  lc.setLed(LEFT_EWI,LEFT_FIRE_D_COL,LEFT_FIRE_D_ROW,newValue);
}
DcsBios::IntegerBuffer fireLeftLtBuffer(0x7408, 0x0040, 6, onFireLeftLtChange);

void onMasterCautionLtChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,MSTR_CAUT_A_COL,MSTR_CAUT_A_ROW,newValue);
  lc.setLed(LEFT_EWI,MSTR_CAUT_B_COL,MSTR_CAUT_B_ROW,newValue);
  lc.setLed(LEFT_EWI,MSTR_CAUT_C_COL,MSTR_CAUT_C_ROW,newValue);
  lc.setLed(LEFT_EWI,MSTR_CAUT_D_COL,MSTR_CAUT_D_ROW,newValue);
}
DcsBios::IntegerBuffer masterCautionLtBuffer(0x7408, 0x0200, 9, onMasterCautionLtChange);

void onLhAdvGoChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,GO_A_COL,GO_A_ROW,newValue);
  lc.setLed(LEFT_EWI,GO_B_COL,GO_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvGoBuffer(0x740a, 0x0010, 4, onLhAdvGoChange);


void onLhAdvNoGoChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,NO_GO_A_COL,NO_GO_A_ROW,newValue);
  lc.setLed(LEFT_EWI,NO_GO_B_COL,NO_GO_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvNoGoBuffer(0x740a, 0x0020, 5, onLhAdvNoGoChange);


void onLhAdvLBleedChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,L_BLEED_A_COL,L_BLEED_A_ROW,newValue);
  lc.setLed(LEFT_EWI,L_BLEED_B_COL,L_BLEED_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvLBleedBuffer(0x7408, 0x0800, 11, onLhAdvLBleedChange);

void onLhAdvRBleedChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,R_BLEED_A_COL,R_BLEED_A_ROW,newValue);
  lc.setLed(LEFT_EWI,R_BLEED_B_COL,R_BLEED_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvRBleedBuffer(0x7408, 0x1000, 12, onLhAdvRBleedChange);

void onLhAdvSpdBrkChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,SPD_BRK_A_COL,SPD_BRK_A_ROW,newValue);
  lc.setLed(LEFT_EWI,SPD_BRK_B_COL,SPD_BRK_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvSpdBrkBuffer(0x7408, 0x2000, 13, onLhAdvSpdBrkChange);

void onLhAdvStbyChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,STBY_A_COL,STBY_A_ROW,newValue);
  lc.setLed(LEFT_EWI,STBY_B_COL,STBY_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvStbyBuffer(0x7408, 0x4000, 14, onLhAdvStbyChange);


void onLhAdvLBarRedChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,L_BAR_RED_A_COL,L_BAR_RED_A_ROW,newValue);
  lc.setLed(LEFT_EWI,L_BAR_RED_B_COL,L_BAR_RED_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvLBarRedBuffer(0x7408, 0x8000, 15, onLhAdvLBarRedChange);


void onLhAdvRecChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,REC_A_COL,REC_A_ROW,newValue);
  lc.setLed(LEFT_EWI,REC_B_COL,REC_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvRecBuffer(0x740a, 0x0001, 0, onLhAdvRecChange);

void onLhAdvLBarGreenChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,L_BAR_GREEN_A_COL,L_BAR_GREEN_A_ROW,newValue);
  lc.setLed(LEFT_EWI,L_BAR_GREEN_B_COL,L_BAR_GREEN_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvLBarGreenBuffer(0x740a, 0x0002, 1, onLhAdvLBarGreenChange);

void onLhAdvXmitChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,XMIT_A_COL,XMIT_A_ROW,newValue);
  lc.setLed(LEFT_EWI,XMIT_B_COL,XMIT_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvXmitBuffer(0x740a, 0x0004, 2, onLhAdvXmitChange);

void onLhAdvAspjOhChange(unsigned int newValue) {
  lc.setLed(LEFT_EWI,ASPJ_OH_A_COL,ASPJ_OH_A_ROW,newValue);
  lc.setLed(LEFT_EWI,ASPJ_OH_B_COL,ASPJ_OH_B_ROW,newValue);
}
DcsBios::IntegerBuffer lhAdvAspjOhBuffer(0x740a, 0x0008, 3, onLhAdvAspjOhChange);


void onRhAdvDispChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,DISP_A_COL,DISP_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,DISP_B_COL,DISP_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvDispBuffer(0x740a, 0x0100, 8, onRhAdvDispChange);

void onRhAdvRcdrOnChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,RCDR_ON_A_COL,RCDR_ON_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,RCDR_ON_B_COL,RCDR_ON_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvRcdrOnBuffer(0x740a, 0x0080, 7, onRhAdvRcdrOnChange);


void onRhAdvSpareRh1Change(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,SPARE_1_A_COL,SPARE_1_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,SPARE_1_B_COL,SPARE_1_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvSpareRh1Buffer(0x740a, 0x2000, 13, onRhAdvSpareRh1Change);

void onRhAdvSpareRh2Change(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,SPARE_2_A_COL,SPARE_2_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,SPARE_2_B_COL,SPARE_2_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvSpareRh2Buffer(0x740a, 0x4000, 14, onRhAdvSpareRh2Change);

void onRhAdvSpareRh3Change(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,SPARE_3_A_COL,SPARE_3_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,SPARE_3_B_COL,SPARE_3_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvSpareRh3Buffer(0x740a, 0x8000, 15, onRhAdvSpareRh3Change);

void onRhAdvSpareRh4Change(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,SPARE_4_A_COL,SPARE_4_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,SPARE_4_B_COL,SPARE_4_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvSpareRh4Buffer(0x740c, 0x0001, 0, onRhAdvSpareRh4Change);


void onRhAdvSamChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,SAM_A_COL,SAM_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,SAM_B_COL,SAM_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvSamBuffer(0x740a, 0x0200, 9, onRhAdvSamChange);

void onRhAdvSpareRh5Change(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,SPARE_5_A_COL,SPARE_5_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,SPARE_5_B_COL,SPARE_5_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvSpareRh5Buffer(0x740c, 0x0002, 1, onRhAdvSpareRh5Change);

void onRhAdvAiChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,AI_A_COL,AI_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,AI_B_COL,AI_B_ROW,newValue); 
}
DcsBios::IntegerBuffer rhAdvAiBuffer(0x740a, 0x0400, 10, onRhAdvAiChange);


void onRhAdvAaaChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,AAA_A_COL,AAA_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,AAA_B_COL,AAA_B_ROW,newValue);   
}
DcsBios::IntegerBuffer rhAdvAaaBuffer(0x740a, 0x0800, 11, onRhAdvAaaChange);


void onRhAdvCwChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,CW_A_COL,CW_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,CW_B_COL,CW_B_ROW,newValue);
}
DcsBios::IntegerBuffer rhAdvCwBuffer(0x740a, 0x1000, 12, onRhAdvCwChange);


void onFireApuLtChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,APU_FIRE_A_COL,APU_FIRE_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,APU_FIRE_B_COL,APU_FIRE_B_ROW,newValue);
  lc.setLed(RIGHT_EWI,APU_FIRE_C_COL,APU_FIRE_C_ROW,newValue);
  lc.setLed(RIGHT_EWI,APU_FIRE_D_COL,APU_FIRE_D_ROW,newValue);
}
DcsBios::IntegerBuffer fireApuLtBuffer(0x740c, 0x0004, 2, onFireApuLtChange);



void onFireRightLtChange(unsigned int newValue) {
  lc.setLed(RIGHT_EWI,RIGHT_FIRE_A_COL,RIGHT_FIRE_A_ROW,newValue);
  lc.setLed(RIGHT_EWI,RIGHT_FIRE_B_COL,RIGHT_FIRE_B_ROW,newValue);
  lc.setLed(RIGHT_EWI,RIGHT_FIRE_C_COL,RIGHT_FIRE_C_ROW,newValue);
  lc.setLed(RIGHT_EWI,RIGHT_FIRE_D_COL,RIGHT_FIRE_D_ROW,newValue);
}
DcsBios::IntegerBuffer fireRightLtBuffer(0x740c, 0x0010, 4, onFireRightLtChange);


void setup() {

//  pinMode(STATUS_LED_PORT, OUTPUT);
//  digitalWrite(STATUS_LED_PORT, 0);
//  delay(300);
//  digitalWrite(STATUS_LED_PORT, 1);
//  delay(300);
//  digitalWrite(STATUS_LED_PORT, 0);
//  delay(300); 
//  digitalWrite(STATUS_LED_PORT, 1);
//  delay(300); 
//  digitalWrite(STATUS_LED_PORT, 0);
//  delay(300);
//  digitalWrite(STATUS_LED_PORT, 1);
//  delay(300);
//  digitalWrite(STATUS_LED_PORT, 0);
//  delay(300);  

  
  devices=lc.getDeviceCount();
  
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,8);
    /* and clear the display */
    lc.clearDisplay(address);
  }
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
