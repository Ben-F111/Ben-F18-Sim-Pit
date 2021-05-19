
// F18 IFEI Arduino to Nextion Ver.04 - Ben Melrose



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

//################## RPM LEFT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE
// SEE VOID LOOP FOR NOZL POSITION

void onIfeiRpmLChange(char* newValue){
  if (strcmp(newValue, "10") == 0) {
   nextion.print("t0.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
 nextion.write("\xFF\xFF\xFF");
   }
 else {
 nextion.print("t0.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
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
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
 nextion.write("\xFF\xFF\xFF");
  }
 else {
 nextion.print("t1.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
 nextion.write("\xFF\xFF\xFF");
  NOZR = newValue; // SETS RPM TO NOZR - USED BELOW 
 }
}
DcsBios::StringBuffer<2> ifeiRpmRBuffer(0x7496, onIfeiRpmRChange);


//################## TEMP LEFT ##################

void onIfeiTempLChange(char* newValue) {
 nextion.print("t3.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiTempLBuffer(0x7498, onIfeiTempLChange);


//################## TEMP RIGHT ##################

void onIfeiTempRChange(char* newValue) {
 nextion.print("t2.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<3> ifeiTempRBuffer(0x749c, onIfeiTempRChange);


//################## FUEL FLOW LEFT ##################

void onIfeiFfLChange(char* newValue) {
 nextion.print("t4.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
FFXL = newValue;
}

DcsBios::StringBuffer<3> ifeiFfLBuffer(0x747c, onIfeiFfLChange);

//################## FUEL FLOW RIGHT ##################


void onIfeiFfRChange(char* newValue) {
 nextion.print("t5.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
nextion.write("\xFF\xFF\xFF");
FFXR = newValue;
}
DcsBios::StringBuffer<3> ifeiFfRBuffer(0x7480, onIfeiFfRChange);


//################## OIL LEFT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE

void onIfeiOilPressLChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
 nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "11") == 0) {
 nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "12") == 0) {
 nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "13") == 0) {
 nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "14") == 0) {
 nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "15") == 0) {
 nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
}else {
   nextion.print("t7.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
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
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "11") == 0) {
 nextion.print("t6.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "12") == 0) {
 nextion.print("t6.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "13") == 0) {
 nextion.print("t6.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "14") == 0) {
 nextion.print("t6.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
} else if (strcmp(newValue, "15") == 0) {
 nextion.print("t6.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
nextion.write("\xFF\xFF\xFF");
}else {
   nextion.print("t6.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
}
DcsBios::StringBuffer<2> ifeiOilPressRBuffer(0x7492, onIfeiOilPressRChange);

//################## FUEL LOWER ##################

void onIfeiFuelDownChange(char* newValue) {
  nextion.print("t9.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
//6
DcsBios::StringBuffer<6> ifeiFuelDownBuffer(0x7484, onIfeiFuelDownChange);

//################## FUEL UPPER ##################

void onIfeiFuelUpChange(char* newValue) {
 nextion.print("t8.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<6> ifeiFuelUpBuffer(0x748a, onIfeiFuelUpChange);
//################## BINGO ##################

void onIfeiBingoChange(char* newValue) {
 nextion.print("t10.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<5> ifeiBingoBuffer(0x7462, onIfeiBingoChange);

//################## TOP CLOCK ##################

void onIfeiClockHChange(char* newValue) {
 nextion.print("t11.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockHBuffer(0x7468, onIfeiClockHChange);

// ":"

void onIfeiDd1Change(char* newValue) {
 nextion.print("t12.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd1Buffer(0x7474, onIfeiDd1Change);

void onIfeiClockMChange(char* newValue) {
 nextion.print("t22.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockMBuffer(0x746a, onIfeiClockMChange);

// ":"

void onIfeiDd2Change(char* newValue) {
 nextion.print("t23.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd2Buffer(0x7476, onIfeiDd2Change);

void onIfeiClockSChange(char* newValue) {
 nextion.print("t24.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiClockSBuffer(0x746c, onIfeiClockSChange);


//################## TIMMER CLOCK ########################

void onIfeiTimerHChange(char* newValue) {
 nextion.print("t25.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerHBuffer(0x746e, onIfeiTimerHChange);

// ":"

void onIfeiDd3Change(char* newValue) {
 nextion.print("t26.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd3Buffer(0x7478, onIfeiDd3Change);

void onIfeiTimerMChange(char* newValue) {
 nextion.print("t27.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<2> ifeiTimerMBuffer(0x7470, onIfeiTimerMChange);

// ":"

void onIfeiDd4Change(char* newValue) {
 nextion.print("t28.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");
}
DcsBios::StringBuffer<1> ifeiDd4Buffer(0x747a, onIfeiDd4Change);

void onIfeiTimerSChange(char* newValue) {
 nextion.print("t29.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
nextion.write("\xFF\xFF\xFF");


}


DcsBios::StringBuffer<2> ifeiTimerSBuffer(0x7472, onIfeiTimerSChange);

////////######## <><> NOZ WORKING <><> ########\\\\\\\\


////////########      END       #######/////////


void setup() {

  nextion.begin(19200);
  DcsBios::setup();
}


void loop() {

{switch (atoi(NOZL)){ // NOZ LEFT POSITION IFEI

case 64 ... 67:nextion.print("p0.pic=8"); break;
case 68 ... 69:nextion.print("p0.pic=7"); break;
case 70 ... 71:nextion.print("p0.pic=5"); break;
case 72 ... 73:nextion.print("p0.pic=6"); break;
case 74 ... 75:nextion.print("p0.pic=3"); break;
case 76 ... 77:nextion.print("p0.pic=2"); break;
case 78 ... 79:nextion.print("p0.pic=1"); break;
case 80 ... 90:nextion.print("p0.pic=0"); break;
case 91 ... 99:nextion.print("p0.pic=1"); break;
}
nextion.write("\xFF\xFF\xFF");}


 
 {      
switch (atoi(FFXL)){ // NOZ LEFT POSITION IFEI
case 110 ... 119:nextion.print("p0.pic=4"); break;
case 120 ... 129:nextion.print("p0.pic=5"); break;
case 130 ... 139:nextion.print("p0.pic=5"); break;
case 140 ... 149:nextion.print("p0.pic=5"); break;
case 150 ... 159:nextion.print("p0.pic=6"); break;
case 160 ... 169:nextion.print("p0.pic=6"); break;
case 170 ... 179:nextion.print("p0.pic=6"); break;
case 180 ... 189:nextion.print("p0.pic=7"); break;
case 190 ... 199:nextion.print("p0.pic=7"); break;
case 200 ... 209:nextion.print("p0.pic=8"); break;
case 210 ... 500:nextion.print("p0.pic=9"); break;
}
    
    nextion.write("\xFF\xFF\xFF"); }
    
{

    switch (atoi(NOZR)){ // NOZ RIGHT POSITION IFEI
case 64:nextion.print("p1.pic=18"); break;
case 68:nextion.print("p1.pic=18"); break;
case 69:nextion.print("p1.pic=17"); break;
case 70:nextion.print("p1.pic=16"); break;
case 71:nextion.print("p1.pic=15"); break;
case 72:nextion.print("p1.pic=14"); break;
case 73:nextion.print("p1.pic=13"); break;
case 74:nextion.print("p1.pic=13"); break;
case 75:nextion.print("p1.pic=12"); break;
case 76:nextion.print("p1.pic=12"); break;
case 77:nextion.print("p1.pic=11"); break;
case 78:nextion.print("p1.pic=11"); break;
case 79:nextion.print("p1.pic=10"); break;
case 80:nextion.print("p1.pic=10"); break;
case 90:nextion.print("p1.pic=10"); break;
case 91:nextion.print("p1.pic=11"); break;
case 99:nextion.print("p1.pic=11"); break;
case 10:nextion.print("p1.pic=16"); break;
}
    nextion.write("\xFF\xFF\xFF");
}


  
 // brightness = analogRead(potPin);
//  valPin = map(brightness, 0, 1024, 10, 100);
 // nextion.print("dim=valPin");
//nextion.print("\xFF\xFF\xFF");
  delay (500);
  DcsBios::loop();

  int brightness = analogRead(A0);
   int bright = map(brightness, 0, 1000, 0, 100);
  String dim = "dim=" + String(bright);
 brightness = bright;

 
if (brightness <= 10){
nextion.print("dim=10");
nextion.print("\xFF\xFF\xFF");

}
else if (brightness <= 40){
  nextion.print("dim=50");
nextion.print("\xFF\xFF\xFF");
nextion.print("t0.pco=22249");
nextion.write("\xFF\xFF\xFF");
nextion.print("t1.pco=22249");
nextion.write("\xFF\xFF\xFF");
nextion.print("t13.pco=22249");
nextion.write("\xFF\xFF\xFF");
nextion.print("t17.pco=22249");
nextion.write("\xFF\xFF\xFF");

}
else if (brightness <= 60){
 nextion.print("dim=60");
 nextion.print("\xFF\xFF\xFF");
 nextion.print("t0.pco=65535");
 nextion.write("\xFF\xFF\xFF");
  nextion.print("t1.pco=65535");
 nextion.write("\xFF\xFF\xFF");
  nextion.print("t13.pco=65535");
 nextion.write("\xFF\xFF\xFF");
   nextion.print("t17.pco=65535");
 nextion.write("\xFF\xFF\xFF");
}
else if (brightness >= 61){
  nextion.print("dim=100");
nextion.print("\xFF\xFF\xFF");
}

 nextion.print("t29.txt=\"");
  nextion.print(bright);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
 
nextion.write("\xFF\xFF\xFF");

}
/*
 *  int brightness = analogRead(A0);
  int bright = map(brightness, 0, 1024, 0, 100);
  String dim = "dim=" + String(bright);
  myNextion.sendCommand(dim.c_str());
 */
