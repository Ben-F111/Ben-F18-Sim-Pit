
// F18 IFEI Arduino to Nextion Ver.12F - Ben Melrose

/* NOTE – ##IMPORTANT ##
  AS OF AUG 202O THERE ARE MISSING OUTPUTS FROM DCS REV 10 BIOS
  THESE HAVE BEEN REPLICATED WITH IF AND IF ELSE STATEMENTS BELOW
  THESE DO NOT 100% REPLICATE THE IN SCREEN IFEI
  ONCE (IF) THESE ARE UPDATED A NEW CODE WILL BE RELEASED
  CODE IS A WORK IN PROGRES
  
  BEN
*/

#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Arduino.h>
#include <Nextion.h>
SoftwareSerial nextion(14, 15); // SETS SERIAL TO PINS  14/15 RX/TX1

int potPin = A0;
int valPin = 0;
int brightness;
int SPD;
int NOZL;
int NOZR;
int FFXL;
int FFXR;
int BINGO;
int FUELT;
int FUELB;
int BINGOBIT;
int CODESBIT;
int SPBIT;
int OCOFFBIT;

//################## RPM LEFT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE
// SEE VOID LOOP FOR NOZL POSITION

void onIfeiRpmLChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
    nextion.print("t0.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else {
    nextion.print("t0.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  NOZL = newValue; // SETS RPM TO NOZL - USED BELOW
}
DcsBios::StringBuffer<2> ifeiRpmLBuffer(0x7494, onIfeiRpmLChange);

//################## RPM RIGHT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE
// SEE VOID LOOP FOR NOZR POSITION

void onIfeiRpmRChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
    nextion.print("t1.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else {
    nextion.print("t1.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    NOZR = newValue; // SETS RPM TO NOZR - USED BELOW
  }
}
DcsBios::StringBuffer<2> ifeiRpmRBuffer(0x7496, onIfeiRpmRChange);

//################## TEMP LEFT ##################
void onIfeiTempLChange(char* newValue) {
  nextion.print("t3.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiTempLBuffer(0x7498, onIfeiTempLChange);

//################## TEMP RIGHT ##################
void onIfeiTempRChange(char* newValue) {
  nextion.print("t2.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiTempRBuffer(0x749c, onIfeiTempRChange);

//################## FUEL FLOW LEFT ##################
void onIfeiFfLChange(char* newValue) {
  FFXL = atol(newValue);
  if (strcmp(newValue, "0") == 0) {
  }
  else  nextion.print("t4.txt=\"");
  nextion.print(FFXL);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiFfLBuffer(0x747c, onIfeiFfLChange);

//################## FUEL FLOW RIGHT ##################
void onIfeiFfRChange(char* newValue) {
  FFXR = atol(newValue);
  nextion.print("t5.txt=\"");
  nextion.print(FFXR);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiFfRBuffer(0x7480, onIfeiFfRChange);

//################## OIL LEFT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE
void onIfeiOilPressLChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "11") == 0) {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "12") == 0) {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "13") == 0) {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "14") == 0) {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "15") == 0) {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else {
    nextion.print("t7.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<2> ifeiOilPressLBuffer(0x7490, onIfeiOilPressLChange);

//################## OIL RIGHT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE
void onIfeiOilPressRChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "11") == 0) {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "12") == 0) {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "13") == 0) {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "14") == 0) {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else if (strcmp(newValue, "15") == 0) {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("0\"");
    nextion.write("\xFF\xFF\xFF");
  } else {
    nextion.print("t6.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<2> ifeiOilPressRBuffer(0x7492, onIfeiOilPressRChange);

//################## FUEL LOWER ##################
void onIfeiFuelDownChange(char* newValue) {
  if (newValue[2] == 32) {
    nextion.print("t9.txt=\"    ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[3] == 32) {
    nextion.print("t9.txt=\"   ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[4] == 32) {
    nextion.print("t9.txt=\"  ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[5] == 32) {
    nextion.print("t9.txt=\" ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else {
    nextion.print("t9.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<6> ifeiFuelDownBuffer(0x7484, onIfeiFuelDownChange);

//################# FUEL UPPER ##################
void onIfeiFuelUpChange(char* newValue) {
  if (newValue[2] == 32) {
    nextion.print("t8.txt=\"    ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[3] == 32) {
    nextion.print("t8.txt=\"   ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[4] == 32) {
    nextion.print("t8.txt=\"  ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[5] == 32) {
    nextion.print("t8.txt=\" ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else {
    nextion.print("t8.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}

DcsBios::StringBuffer<6> ifeiFuelUpBuffer(0x748a, onIfeiFuelUpChange);

//################## FUEL LOWER (MODE Button)##################
void onIfeiTimeSetModeChange(char* newValue) {
  if (newValue[2] == 32) {
    nextion.print("t9.txt=\"    ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[4] == 32) {
    nextion.print("t9.txt=\"  ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (newValue[5] == 32) {
    nextion.print("t9.txt=\" ");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else {
    nextion.print("t9.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<6> ifeiTimeSetModeBuffer(0x74a8, onIfeiTimeSetModeChange);

//################## BINGO ##################
void onIfeiBingoChange(char* newValue) {
  BINGO = atol(newValue);
  nextion.print("t10.txt=\"");
  nextion.print(BINGO);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<6> ifeiBingoBuffer(0x7462, onIfeiBingoChange);

//################## TOP CLOCK ##################
void onIfeiClockHChange(char* newValue) {
  int TIMETOP = atol(newValue);
  nextion.print("t11.txt=\"");
  nextion.print(TIMETOP);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockHBuffer(0x7468, onIfeiClockHChange);

// ":"

void onIfeiDd1Change(char* newValue) {
  nextion.print("t12.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd1Buffer(0x7474, onIfeiDd1Change);

void onIfeiClockMChange(char* newValue) {
  nextion.print("t22.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockMBuffer(0x746a, onIfeiClockMChange);

// ":"

void onIfeiDd2Change(char* newValue) {
  nextion.print("t23.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd2Buffer(0x7476, onIfeiDd2Change);

void onIfeiClockSChange(char* newValue) {
  nextion.print("t24.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockSBuffer(0x746c, onIfeiClockSChange);

//################# TIMMER CLOCK ########################
void onIfeiTimerHChange(char* newValue) {

  nextion.print("t25.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerHBuffer(0x746e, onIfeiTimerHChange);

// ":"

void onIfeiDd3Change(char* newValue) {
  nextion.print("t26.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd3Buffer(0x7478, onIfeiDd3Change);

void onIfeiTimerMChange(char* newValue) {
  nextion.print("t27.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerMBuffer(0x7470, onIfeiTimerMChange);

// ":"

void onIfeiDd4Change(char* newValue) {
  nextion.print("t28.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd4Buffer(0x747a, onIfeiDd4Change);

void onIfeiTimerSChange(char* newValue) {
  nextion.print("t29.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}

DcsBios::StringBuffer<2> ifeiTimerSBuffer(0x7472, onIfeiTimerSChange);

////////########  IEFI LABELS    #######/////////

// ************** BING0 ***************************
void onIfeiBingoTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t19.txt=\"");
    nextion.print("BINGO");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t30.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t31.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if ((strcmp(newValue, "0") == 0) && (SPBIT == HIGH) && (OCOFFBIT == HIGH)) {
    nextion.print("t19.txt=\"");
    nextion.print("      ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t30.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t31.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if ((strcmp(newValue, "0") == 0) && (SPBIT == HIGH) && (OCOFFBIT == LOW)) {
    nextion.print("t19.txt=\"");
    nextion.print("      ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t30.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t31.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if ((strcmp(newValue, "0") == 0) && (SPBIT == LOW)) {
    nextion.print("t19.txt=\"");
    nextion.print("      ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t30.txt=\"");
    nextion.print("L");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t31.txt=\"");
    nextion.print("R");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}

DcsBios::StringBuffer<1> ifeiBingoTextureBuffer(0x74b6, onIfeiBingoTextureChange);

// ************** SP (CODES) ***************************
void onIfeiSpChange(char* newValue) {

  if (newValue[0] == 83) { // Letter = "S"
    nextion.print("t3.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t4.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t5.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t10.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    SPBIT = HIGH;
  }
  else  if (newValue[0] != 83) {
    nextion.print("t3.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    SPBIT = LOW;
  }
}

DcsBios::StringBuffer<3> ifeiSpBuffer(0x74a4, onIfeiSpChange);

void onIfeiCodesChange(char* newValue) {
  if (newValue[0] != 32) {
    nextion.print("t2.txt=\"");
    nextion.print(newValue);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    CODESBIT = HIGH;
  }
  else
    nextion.print("t2.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
  CODESBIT = LOW;
}
DcsBios::StringBuffer<3> ifeiCodesBuffer(0x74a0, onIfeiCodesChange);

///////////// FF Texture ///////////////////////
void onIfeiFfTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t15.txt=\"");
    nextion.println("  FF");
    nextion.println("x100");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (strcmp(newValue, "0") == 0) {
    nextion.print("t15.txt=\"");
    nextion.println(" ");
    nextion.println(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");

  }
}
DcsBios::StringBuffer<1> ifeiFfTextureBuffer(0x74b2, onIfeiFfTextureChange);

///////////// OIL Texture ///////////////////////
void onIfeiOilTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t16.txt=\"");
    nextion.print("OIL");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
            nextion.print("t21.txt=\"");
    nextion.print("NOZ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (strcmp(newValue, "0") == 0) {
    nextion.print("t16.txt=\"");
    nextion.print("    ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
        nextion.print("t21.txt=\"");
    nextion.print("    ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<1> ifeiOilTextureBuffer(0x74b4, onIfeiOilTextureChange);

///////////// RPM Texture ///////////////////////
void onIfeiRpmTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t13.txt=\"");
    nextion.print("RPM");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (strcmp(newValue, "0") == 0) {
    nextion.print("t13.txt=\"");
    nextion.print("      ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<1> ifeiRpmTextureBuffer(0x74ae, onIfeiRpmTextureChange);


///////////// TEMP Texture ///////////////////////
void onIfeiTempTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t14.txt=\"");
    nextion.print("TEMP");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (strcmp(newValue, "0") == 0) {
    nextion.print("t14.txt=\"");
    nextion.print("      ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<1> ifeiTempTextureBuffer(0x74b0, onIfeiTempTextureChange);

///////////// ZULU Texture ///////////////////////
void onIfeiZTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t32.txt=\"");
    nextion.print("Z");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
  else if (strcmp(newValue, "0") == 0) {
    nextion.print("t32.txt=\"");
    nextion.print(" ");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::StringBuffer<1> ifeiZTextureBuffer(0x74cc, onIfeiZTextureChange);

/////////////////////XXXXXXXXXXXXXXXXXXXXXXXXXXXX END OF DCS BIOS WORKING XXXXXXXXXXXXXXXXXXXXXXXXXXXX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

////////######## <><> NOZ WORKING <><> ########\\\\\\\\

void setup() {
  //    Serial.begin(57600);
  //  onIfeiFuelUpChange(fuelLevel);
  // nextion.begin(19200);
  //nextion.begin(115200);
  nextion.begin(256000);

  DcsBios::setup();
}
void loop() {
{ switch (atoi(NOZL)) { // NOZ LEFT POSITION IFEI

      case 64 ... 67: nextion.print("p0.pic=8"); break;
      case 68 ... 69: nextion.print("p0.pic=7"); break;
      case 70 ... 71: nextion.print("p0.pic=5"); break;
      case 72 ... 73: nextion.print("p0.pic=6"); break;
      case 74 ... 75: nextion.print("p0.pic=3"); break;
      case 76 ... 77: nextion.print("p0.pic=2"); break;
      case 78 ... 79: nextion.print("p0.pic=1"); break;
      case 80 ... 90: nextion.print("p0.pic=0"); break;
      case 91 ... 99: nextion.print("p0.pic=1"); break;
    }
    nextion.write("\xFF\xFF\xFF");
  }
  {
    switch (atoi(FFXL)) { // NOZ LEFT POSITION IFEI
      case 110 ... 119: nextion.print("p0.pic=4"); break;
      case 120 ... 129: nextion.print("p0.pic=5"); break;
      case 130 ... 139: nextion.print("p0.pic=5"); break;
      case 140 ... 149: nextion.print("p0.pic=5"); break;
      case 150 ... 159: nextion.print("p0.pic=6"); break;
      case 160 ... 169: nextion.print("p0.pic=6"); break;
      case 170 ... 179: nextion.print("p0.pic=7"); break;
      case 180 ... 189: nextion.print("p0.pic=7"); break;
      case 190 ... 199: nextion.print("p0.pic=8"); break;
      case 200 ... 209: nextion.print("p0.pic=9"); break;
      case 210 ... 500: nextion.print("p0.pic=10"); break;
    }
    nextion.write("\xFF\xFF\xFF");
  }
{ switch (atoi(NOZR)) { // NOZ RIGHT POSITION IFEI
      case 64 ... 67: nextion.print("p1.pic=19"); break;
      case 68 ... 69: nextion.print("p1.pic=18"); break;
      case 70 ... 71: nextion.print("p1.pic=16"); break;
      case 72 ... 73: nextion.print("p1.pic=15"); break;
      case 74 ... 75: nextion.print("p1.pic=14"); break;
      case 76 ... 77: nextion.print("p1.pic=13"); break;
      case 78 ... 79: nextion.print("p1.pic=12"); break;
      case 80 ... 90: nextion.print("p1.pic=11"); break;
      case 91 ... 99: nextion.print("p1.pic=12"); break;
    }
    nextion.write("\xFF\xFF\xFF");
  }
  {
    switch (atoi(FFXR)) { // NOZ RIGHT POSITION IFEI
      case 110 ... 119: nextion.print("p1.pic=15"); break;
      case 120 ... 129: nextion.print("p1.pic=16"); break;
      case 130 ... 139: nextion.print("p1.pic=16"); break;
      case 140 ... 149: nextion.print("p1.pic=16"); break;
      case 150 ... 159: nextion.print("p1.pic=17"); break;
      case 160 ... 169: nextion.print("p1.pic=17"); break;
      case 170 ... 179: nextion.print("p1.pic=18"); break;
      case 180 ... 189: nextion.print("p1.pic=18"); break;
      case 190 ... 199: nextion.print("p1.pic=19"); break;
      case 200 ... 209: nextion.print("p1.pic=20"); break;
      case 210 ... 500: nextion.print("p1.pic=21"); break;
    }
    nextion.write("\xFF\xFF\xFF");
  }

  ////////########      END       #######/////////

  DcsBios::loop();
  ////////########  SCREEN DIM    #######/////////
  delay (0);
  {
    int brightness = analogRead(A0);
    int bright = map(brightness, 10, 1100, 2, 100);
    String dim = "dim=" + String(bright);
    brightness = bright;
    nextion.print(dim.c_str());
    nextion.write("\xFF\xFF\xFF");
  }
}
