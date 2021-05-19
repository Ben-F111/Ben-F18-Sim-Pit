
// F18 IFEI Arduino to Nextion Ver.05b - Ben Melrose



#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Arduino.h>
#include <Nextion.h>
SoftwareSerial nextion(14, 15); // SETS SERIAL TO PINS  14/15 RX/TX1

int potPin = A0;
int valPin = 0;
int brightness;
int ALT;


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
void onPressureAltChange(unsigned int newValue) {
    /* your code here */
}
DcsBios::IntegerBuffer pressureAltBuffer(0x7504, 0xffff, 0, onPressureAltChange);

// NOZ Piture (IEFI)
    switch (atoi(newValue))
    {
  case 0:
 nextion.print("p0.pic=0");
 break;
  case 70:
       nextion.print("p0.pic=7");
 break;
 case 90:
       nextion.print("p0.pic=9");
            break;
    }

    nextion.write("\xFF\xFF\xFF");
}

DcsBios::StringBuffer<2> ifeiRpmLBuffer(0x7494, onIfeiRpmLChange);


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
// This needs to got to t29 (Total 30 lines)
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
}  // This needs to got to t29 (Total 30 lines)
else if (brightness >= 61){
  nextion.print("dim=100");
nextion.print("\xFF\xFF\xFF");
}
 nextion.print("t29.txt=\"");
  nextion.print(bright);  // This is the value you want to send to that object and atribute mention before.
 nextion.print("\"");
 // This needs to got to t29 (Total 30 lines)
 
nextion.write("\xFF\xFF\xFF");

}
