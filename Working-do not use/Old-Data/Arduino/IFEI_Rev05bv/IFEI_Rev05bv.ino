
// F18 IFEI Arduino to Nextion Ver.04 - Ben Melrose



#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Arduino.h>
#include <Nextion.h>
SoftwareSerial nextion(14, 15); // SETS SERIAL TO PINS  14/15 RX/TX1

char RPML;
//char RPMR;
//char NOZL[37] = {8, 8, 8, 8, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6};
//char NOZR[37] = {8, 8, 8, 8, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6};
//int i;

int brightness;

//################## RPM LEFT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE

void onIfeiRpmLChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
   nextion.print("t0.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\"");
 nextion.write("\xFF\xFF\xFF");
 RPML = newValue;
  }
 else {
 nextion.print("t0.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
 nextion.write("\xFF\xFF\xFF");
}
if (strcmp(newValue, "64") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "65") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "66") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "67") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "68") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "69") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "70") == 0) {
nextion.print("p0.pic=");
nextion.print(6);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "71") == 0) {
nextion.print("p0.pic=");
nextion.print(5);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "72") == 0) {
nextion.print("p0.pic=");
nextion.print(4);  
nextion.write("\xFF\xFF\xFF");
}
}

DcsBios::StringBuffer<2> ifeiRpmLBuffer(0x7494, onIfeiRpmLChange);


//################## RPM RIGHT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE

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
}
DcsBios::StringBuffer<3> ifeiFfLBuffer(0x747c, onIfeiFfLChange);

//################## FUEL FLOW RIGHT ##################


void onIfeiFfRChange(char* newValue) {
 nextion.print("t5.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
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
  
  nextion.begin(9600);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
nextion.print("dim=25");
nextion.print("\xFF\xFF\xFF");
  delay (100);
}
