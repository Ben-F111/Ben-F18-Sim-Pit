
// F18 IFEI Arduino to Nextion Ver.12G - Ben Melrose (FP Flight Panels)
//com6

#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Arduino.h>
#include <Nextion.h>
SoftwareSerial nextion(18,19); // SETS SERIAL TO PINS  14/15 RX/TX1

int potPin = A0;
int valPin = 0;
int brightness;
//int SPD;
int RPML; // RPM LEFT
int RPMR; // RPM RIGHT
int NOZL;
int NOZR;
int FFXL;
int FFXR;
int BINGO;
int OILL;
int OILR;
int TMPL;
int TMPR;
int NOZOFF;
int FLUP;
int FLLW;
//int BINGOBIT;
int CODESBIT;
int SPBIT;
int OCOFFBIT;
int ifeiCol; //IFEI Colour (Green or White)



//################## RPM LEFT ##################Y
void onIfeiRpmLChange(char* newValue) {
    RPML = atol(newValue);
    nextion.print("t0.txt=\"");
      nextion.print(RPML);
    nextion.print("\"");
        nextion.write("\xFF\xFF\xFF");
   
}
DcsBios::StringBuffer<3> ifeiRpmLBuffer(0x749e, onIfeiRpmLChange);

//################## RPM RIGHT ##################Y
void onIfeiRpmRChange(char* newValue) {
    RPMR = atol(newValue);
    nextion.print("t1.txt=\"");
    nextion.print(RPMR);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiRpmRBuffer(0x74a2, onIfeiRpmRChange);

//################## TEMP LEFT ##################Y
void onIfeiTempLChange(char* newValue) {
  TMPL = atol(newValue);
  nextion.print("t3.txt=\"");
  nextion.print(TMPL);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiTempLBuffer(0x74a6, onIfeiTempLChange);


//################## TEMP RIGHT ##################Y
void onIfeiTempRChange(char* newValue) {
  TMPR = atol(newValue);
  nextion.print("t2.txt=\"");
  nextion.print(TMPR);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiTempRBuffer(0x74aa, onIfeiTempRChange);

//################## FUEL FLOW LEFT ##################Y
void onIfeiFfLChange(char* newValue) {
  FFXL = atol(newValue);
  nextion.print("t4.txt=\"");
  nextion.print(FFXL);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiFfLBuffer(0x7482, onIfeiFfLChange);

//################## FUEL FLOW RIGHT ##################Y
void onIfeiFfRChange(char* newValue) {
  FFXR = atol(newValue);
  nextion.print("t5.txt=\"");
  nextion.print(FFXR);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiFfRBuffer(0x7486, onIfeiFfRChange);

//################## OIL LEFT ##################Y
void onIfeiOilPressLChange(char* newValue) {
    OILL = atol(newValue);
    nextion.print("t7.txt=\"");
    nextion.print(OILL);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiOilPressLBuffer(0x7496, onIfeiOilPressLChange);

//################## OIL RIGHT ##################Y
void onIfeiOilPressRChange(char* newValue) {
     OILR = atol(newValue);
     nextion.print("t6.txt=\"");
    nextion.print(OILR);
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiOilPressRBuffer(0x749a, onIfeiOilPressRChange);

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
DcsBios::StringBuffer<6> ifeiFuelDownBuffer(0x748a, onIfeiFuelDownChange);

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
DcsBios::StringBuffer<6> ifeiFuelUpBuffer(0x7490, onIfeiFuelUpChange);

//################## FUEL LOWER (MODE Button)##################

//################## BINGO ################## Y
void onIfeiBingoChange(char* newValue) {
  BINGO = atol(newValue);
  nextion.print("t10.txt=\"");
  nextion.print(BINGO);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<5> ifeiBingoBuffer(0x7468, onIfeiBingoChange);

//################## TOP CLOCK ##################
void onIfeiClockHChange(char* newValue) {
  int TIMETOP = atol(newValue);
  nextion.print("t11.txt=\"");
  nextion.print(TIMETOP);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockHBuffer(0x746e, onIfeiClockHChange);

// ":"

void onIfeiDd1Change(char* newValue) {
  nextion.print("t12.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd1Buffer(0x747a, onIfeiDd1Change);

void onIfeiClockMChange(char* newValue) {
  nextion.print("t22.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockMBuffer(0x7470, onIfeiClockMChange);

// ":"

void onIfeiDd2Change(char* newValue) {
  nextion.print("t23.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd2Buffer(0x747c, onIfeiDd2Change);

void onIfeiClockSChange(char* newValue) {
  nextion.print("t24.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockSBuffer(0x7472, onIfeiClockSChange);

//################# TIMMER CLOCK ########################
void onIfeiTimerHChange(char* newValue) {

  nextion.print("t25.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerHBuffer(0x7474, onIfeiTimerHChange);

// ":"

void onIfeiDd3Change(char* newValue) {
  nextion.print("t26.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd3Buffer(0x747e, onIfeiDd3Change);

void onIfeiTimerMChange(char* newValue) {
  nextion.print("t27.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerMBuffer(0x7476, onIfeiTimerMChange);

// ":"

void onIfeiDd4Change(char* newValue) {
  nextion.print("t28.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd4Buffer(0x7480, onIfeiDd4Change);

void onIfeiTimerSChange(char* newValue) {
  nextion.print("t29.txt=\"");
  nextion.print(newValue);
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerSBuffer(0x7478, onIfeiTimerSChange);

////////########  IEFI LABELS    #######/////////

// ************** BINGO ***************************
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

DcsBios::StringBuffer<1> ifeiBingoTextureBuffer(0x74c6, onIfeiBingoTextureChange);

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

DcsBios::StringBuffer<3> ifeiSpBuffer(0x74b2, onIfeiSpChange);

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
DcsBios::StringBuffer<3> ifeiCodesBuffer(0x74ae, onIfeiCodesChange);

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
DcsBios::StringBuffer<1> ifeiFfTextureBuffer(0x74c0, onIfeiFfTextureChange);

////////######## <><> NOZ LEFT WORKING <><> ########\\\\\\\\

void onExtNozzlePosLChange(unsigned int newValue) {
//if (NOZOFF == HIGH){
NOZL = map(newValue, 0, 65535, 0, 100);
if (ifeiCol == 1) {
   switch (NOZL) { // NOZ LEFT POSITION IFEI
      case 0 ... 9: nextion.print("p0.pic=0"); break;
      case 10 ... 19: nextion.print("p0.pic=1"); break;
      case 20 ... 29: nextion.print("p0.pic=2"); break;
      case 30 ... 39: nextion.print("p0.pic=3"); break;
      case 40 ... 49: nextion.print("p0.pic=4"); break;
      case 50 ... 59: nextion.print("p0.pic=5"); break;
      case 60 ... 69: nextion.print("p0.pic=6"); break;
      case 70 ... 79: nextion.print("p0.pic=7"); break;
      case 80 ... 89: nextion.print("p0.pic=8"); break;
      case 90 ... 95: nextion.print("p0.pic=9"); break;
      case 96 ... 100: nextion.print("p0.pic=10"); break;
    }
    nextion.write("\xFF\xFF\xFF");
}
else if (ifeiCol == 0) {
     switch (NOZL) { // NOZ RIGHT POSITION IFEI
      case 0 ... 9: nextion.print("p0.pic=26"); break;
      case 10 ... 19: nextion.print("p0.pic=27"); break;
      case 20 ... 29: nextion.print("p0.pic=28"); break;
      case 30 ... 39: nextion.print("p0.pic=29"); break;
      case 40 ... 49: nextion.print("p0.pic=30"); break;
      case 50 ... 59: nextion.print("p0.pic=31"); break;
      case 60 ... 69: nextion.print("p0.pic=32"); break;
      case 70 ... 79: nextion.print("p0.pic=33"); break;
      case 80 ... 89: nextion.print("p0.pic=34"); break;
      case 90 ... 95: nextion.print("p0.pic=35"); break;
      case 96 ... 100: nextion.print("p0.pic=36"); break;
    }
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::IntegerBuffer extNozzlePosLBuffer(0x7568, 0xffff, 0, onExtNozzlePosLChange);

////////######## <><> NOZ RIGHT WORKING <><> ########\\\\\\\\

void onExtNozzlePosRChange(unsigned int newValue) {
NOZR = map(newValue, 0, 65535, 0, 100);
if (ifeiCol == 1) {
   switch (NOZR) { // NOZ RIGHT POSITION IFEI
      case 0 ... 9: nextion.print("p1.pic=11"); break;
      case 10 ... 19: nextion.print("p1.pic=12"); break;
      case 20 ... 29: nextion.print("p1.pic=13"); break;
      case 30 ... 39: nextion.print("p1.pic=14"); break;
      case 40 ... 49: nextion.print("p1.pic=15"); break;
      case 50 ... 59: nextion.print("p1.pic=16"); break;
      case 60 ... 69: nextion.print("p1.pic=17"); break;
      case 70 ... 79: nextion.print("p1.pic=18"); break;
      case 80 ... 89: nextion.print("p1.pic=19"); break;
      case 90 ... 95: nextion.print("p1.pic=20"); break;
      case 96 ... 100: nextion.print("p1.pic=21"); break;
    }
    nextion.write("\xFF\xFF\xFF");
}
else if (ifeiCol == 0) {
   switch (NOZR) { // NOZ RIGHT POSITION IFEI
      case 0 ... 9: nextion.print("p1.pic=37"); break;
      case 10 ... 19: nextion.print("p1.pic=38"); break;
      case 20 ... 29: nextion.print("p1.pic=39"); break;
      case 30 ... 39: nextion.print("p1.pic=40"); break;
      case 40 ... 49: nextion.print("p1.pic=41"); break;
      case 50 ... 59: nextion.print("p1.pic=42"); break;
      case 60 ... 69: nextion.print("p1.pic=43"); break;
      case 70 ... 79: nextion.print("p1.pic=44"); break;
      case 80 ... 89: nextion.print("p1.pic=45"); break;
      case 90 ... 95: nextion.print("p1.pic=46"); break;
      case 96 ... 100: nextion.print("p1.pic=47"); break;
    }
    nextion.write("\xFF\xFF\xFF");
  }
}
DcsBios::IntegerBuffer extNozzlePosRBuffer(0x7566, 0xffff, 0, onExtNozzlePosRChange);

///////////// OIL Texture ///////////////////////
void onIfeiOilTextureChange(char* newValue) {
  if (strcmp(newValue, "1") == 0) {
    nextion.print("t16.txt=\"");
    nextion.print("OIL");
    nextion.print("\"");
    nextion.write("\xFF\xFF\xFF");
    nextion.print("t21.txt=\"");
    nextion.print("NOZ"); //NOZ LABLE
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
DcsBios::StringBuffer<1> ifeiOilTextureBuffer(0x74c4, onIfeiOilTextureChange);

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
DcsBios::StringBuffer<1> ifeiRpmTextureBuffer(0x74bc, onIfeiRpmTextureChange);

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
DcsBios::StringBuffer<1> ifeiTempTextureBuffer(0x74be, onIfeiTempTextureChange);

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
DcsBios::StringBuffer<1> ifeiZTextureBuffer(0x74dc, onIfeiZTextureChange);



void onIfeiRpointerTextureChange(char* newValue) {
if (strcmp(newValue, "1") == 0){
  NOZOFF = HIGH;
  nextion.print("p0.pic=0");
  nextion.write("\xFF\xFF\xFF");
  nextion.print("p1.pic=11");
  nextion.write("\xFF\xFF\xFF");
  }
else {
  NOZOFF = LOW; 
  nextion.print("p0.pic=22");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("p1.pic=22");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t0.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t1.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t2.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t3.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t4.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t5.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t6.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF"); 

  nextion.print("t7.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t7.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t10.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t11.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t30.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");

  nextion.print("t31.txt=\"");
  nextion.print("");
  nextion.print("\"");
  nextion.write("\xFF\xFF\xFF");
  
}

}
DcsBios::StringBuffer<1> ifeiRpointerTextureBuffer(0x74da, onIfeiRpointerTextureChange);
/*
///////////// IFEI COLOUR TEXT GREN OR WHITE ///////////////////////

void onIfeiDispIntLtChange(unsigned int newValue) {
ifeiCol = newValue;
if(ifeiCol == 0) {
    nextion.print("t0.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t1.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t2.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t3.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t4.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t5.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t6.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t7.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t8.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t9.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t10.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t11.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t12.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t13.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t14.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t15.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t16.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t17.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t18.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t19.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t20.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t21.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t22.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t23.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t24.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t25.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t26.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t27.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t28.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t29.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t30.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t31.pco=2016");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t32.pco=2016");
      nextion.write("\xFF\xFF\xFF");
      }
       if(ifeiCol == 1) {
    nextion.print("t0.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t1.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t2.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t3.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t4.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t5.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t6.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t7.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t8.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t9.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t10.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t11.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t12.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t13.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t14.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t15.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t16.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t17.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t18.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t19.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t20.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t21.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t22.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t23.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t24.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t25.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t26.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t27.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t28.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t29.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t30.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t31.pco=65535");
         nextion.write("\xFF\xFF\xFF");
    nextion.print("t32.pco=65535");
      nextion.write("\xFF\xFF\xFF");}
}
DcsBios::IntegerBuffer ifeiDispIntLtBuffer(0x74d6, 0x2000, 13, onIfeiDispIntLtChange);
*/
/////////////////////XXXXXXXXXXXXXXXXXXXXXXXXXXXX END OF DCS BIOS WORKING XXXXXXXXXXXXXXXXXXXXXXXXXXXX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void setup() {
  //    Serial.begin(57600);
  //  onIfeiFuelUpChange(fuelLevel);
  // nextion.begin(19200);
  //nextion.begin(115200);
  nextion.begin(256000);

  DcsBios::setup();
}
void loop() {

  ////////########      END       #######/////////

  DcsBios::loop();
  ////////########  SCREEN DIM    #######/////////
  delay (0);
  {
    int brightness = analogRead(A4);
    int bright = map(brightness, 10, 1100, 2, 100);
    String dim = "dim=" + String(bright);
    brightness = bright;
    nextion.print(dim.c_str());
    nextion.write("\xFF\xFF\xFF");
  }
}
