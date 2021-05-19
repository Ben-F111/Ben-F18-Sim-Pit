
// F18 IFEI Arduino to Nextion Ver.05b - Ben Melrose



#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Arduino.h>
#include <Nextion.h>
SoftwareSerial nextion(14, 15); // SETS SERIAL TO PINS  14/15 RX/TX1

int potPin = A0;
int valPin = 0;
int brightness;


/*void onStbyAsiAirspeedChange(unsigned int newValue) {
///////////
}
DcsBios::IntegerBuffer stbyAsiAirspeedBuffer(0x74e0, 0xffff, 0, onStbyAsiAirspeedChange);


 */
//################## RPM LEFT ##################
//REMOVE IF - ELSE WHEN DCS-BIOS FIXED TO 3 CHAR
// RETAIN LAST SET AFTER ELSE

void onIfeiRpmLChange(char* newValue) {
  if (strcmp(newValue, "10") == 0) {
   nextion.print("t0.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("0\""); // ADD "0" to 10 to make 100
 nextion.write("\xFF\xFF\xFF");
   }
 else {
 nextion.print("t0.txt=\"");
 nextion.print(newValue);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\""); // ALL ELSE USE VALUE
 nextion.write("\xFF\xFF\xFF");
}

// NOZ Piture (IEFI)

if (strcmp(newValue, "0") == 0) {
nextion.print("p0.pic=");
nextion.print(0);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "30") == 0) {
nextion.print("p0.pic=");
nextion.print(0);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "31") == 0) {
nextion.print("p0.pic=");
nextion.print(1);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "32") == 0) {
nextion.print("p0.pic=");
nextion.print(2);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "35") == 0) {
nextion.print("p0.pic=");
nextion.print(3);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "36") == 0) {
nextion.print("p0.pic=");
nextion.print(3);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "37") == 0) {
nextion.print("p0.pic=");
nextion.print(4);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "38") == 0) {
nextion.print("p0.pic=");
nextion.print(4);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "40") == 0) {
nextion.print("p0.pic=");
nextion.print(4);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "41") == 0) {
nextion.print("p0.pic=");
nextion.print(5);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "43") == 0) {
nextion.print("p0.pic=");
nextion.print(5);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "44") == 0) {
nextion.print("p0.pic=");
nextion.print(6);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "47") == 0) {
nextion.print("p0.pic=");
nextion.print(6);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "49") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "50") == 0) {
nextion.print("p0.pic=");
nextion.print(7);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "51") == 0) {
nextion.print("p0.pic=");
nextion.print(8);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "64") == 0) {
nextion.print("p0.pic=");
nextion.print(8);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "68") == 0) {
nextion.print("p0.pic=");
nextion.print(8);  
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
else if (strcmp(newValue, "73") == 0) {
nextion.print("p0.pic=");
nextion.print(3);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "74") == 0) {
nextion.print("p0.pic=");
nextion.print(3);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "75") == 0) {
nextion.print("p0.pic=");
nextion.print(2);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "76") == 0) {
nextion.print("p0.pic=");
nextion.print(2);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "77") == 0) {
nextion.print("p0.pic=");
nextion.print(1);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "78") == 0) {
nextion.print("p0.pic=");
nextion.print(1);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "79") == 0) {
nextion.print("p0.pic=");
nextion.print(0);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "80") == 0) {
nextion.print("p0.pic=");
nextion.print(0);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "90") == 0) {
nextion.print("p0.pic=");
nextion.print(0);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "91") == 0) {
nextion.print("p0.pic=");
nextion.print(1);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "99") == 0) {
nextion.print("p0.pic=");
nextion.print(1);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "10") == 0) {
nextion.print("p0.pic=");
nextion.print(6);  
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
if (strcmp(newValue, "0") == 0) {
nextion.print("p1.pic=");
nextion.print(10);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "30") == 0) {
nextion.print("p1.pic=");
nextion.print(10);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "31") == 0) {
nextion.print("p1.pic=");
nextion.print(11);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "32") == 0) {
nextion.print("p1.pic=");
nextion.print(12);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "35") == 0) {
nextion.print("p1.pic=");
nextion.print(13);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "36") == 0) {
nextion.print("p1.pic=");
nextion.print(13);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "37") == 0) {
nextion.print("p1.pic=");
nextion.print(13);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "38") == 0) {
nextion.print("p1.pic=");
nextion.print(14);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "40") == 0) {
nextion.print("p1.pic=");
nextion.print(14);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "41") == 0) {
nextion.print("p1.pic=");
nextion.print(15);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "43") == 0) {
nextion.print("p1.pic=");
nextion.print(15);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "44") == 0) {
nextion.print("p1.pic=");
nextion.print(16);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "47") == 0) {
nextion.print("p1.pic=");
nextion.print(16);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "49") == 0) {
nextion.print("p1.pic=");
nextion.print(17);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "50") == 0) {
nextion.print("p1.pic=");
nextion.print(17);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "51") == 0) {
nextion.print("p1.pic=");
nextion.print(18);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "64") == 0) {
nextion.print("p1.pic=");
nextion.print(18);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "68") == 0) {
nextion.print("p1.pic=");
nextion.print(18);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "69") == 0) {
nextion.print("p1.pic=");
nextion.print(17);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "70") == 0) {
nextion.print("p1.pic=");
nextion.print(16);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "71") == 0) {
nextion.print("p1.pic=");
nextion.print(15);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "72") == 0) {
nextion.print("p1.pic=");
nextion.print(14);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "73") == 0) {
nextion.print("p1.pic=");
nextion.print(13);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "74") == 0) {
nextion.print("p1.pic=");
nextion.print(13);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "75") == 0) {
nextion.print("p1.pic=");
nextion.print(12);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "76") == 0) {
nextion.print("p1.pic=");
nextion.print(12);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "77") == 0) {
nextion.print("p1.pic=");
nextion.print(11);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "78") == 0) {
nextion.print("p1.pic=");
nextion.print(11);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "79") == 0) {
nextion.print("p1.pic=");
nextion.print(10);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "90") == 0) {
nextion.print("p1.pic=");
nextion.print(10);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "91") == 0) {
nextion.print("p1.pic=");
nextion.print(11);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "99") == 0) {
nextion.print("p1.pic=");
nextion.print(11);  
nextion.write("\xFF\xFF\xFF");
}
else if (strcmp(newValue, "10") == 0) {
nextion.print("p1.pic=");
nextion.print(16);  
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
  
  nextion.begin(19200);
  DcsBios::setup();
}

void loop() {
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


/*
 *  int brightness = analogRead(A0);
  int bright = map(brightness, 0, 1024, 0, 100);
  String dim = "dim=" + String(bright);
  myNextion.sendCommand(dim.c_str());
 */
}
