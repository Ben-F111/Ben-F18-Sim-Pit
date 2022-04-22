// UPDATED TO DSCS-BIOS FP EDITION FOR OPEN HORNET


////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
//||               FUNCTION = RIGHT HORNET OUTPUT                     ||\\
//||              LOCATION IN THE PIT = LIP RIGHTHAND SIDE             ||\\
//||            ARDUINO PROCESSOR TYPE = Arduino DUE                  ||\\
//||      ARDUINO CHIP SERIAL NUMBER = SN - 859373138373516121E2      ||\\
//||      ETHERNET SHEILD MAC ADDRESS = MAC -                         ||\\
//||            PROGRAM PORT CONNECTED COM PORT = COM 11              ||\\
//||            NATIVE PORT CONNECTED COM PORT = COM 12              ||\\
//||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\

// Tell DCS-BIOS to use a serial connection and use interrupt-driven
// communication. The main program will be interrupted to prioritize
// processing incoming data.
//
// This should work on any Arduino that has an ATMega328 controller
// (Uno, Pro Mini, many others).
String readString;
#include <Servo.h>
//#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_DEFAULT_SERIAL
#include "LedControl.h"
#include "DcsBios.h"

#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)

#define  COILRA1  45 // RA = RAD ALT
#define  COILRA2  47
#define  COILRA3  41
#define  COILRA4  43

#define  COILCA1  36 // CA = CAB ALT
#define  COILCA2  34
#define  COILCA3  32
#define  COILCA4  38
Servo RAD_ALT_servo; 
Servo HYD_LFT_servo; 
Servo HYD_RHT_servo; 
Servo BATT_U_servo; 
Servo BATT_E_servo; 
int RAD_ALT = 0;
int valRA = 0;
int CAB_ALT;
int valCA = 0;
int CurCABALT = 0;
int posCA = 0;
int posRA = 0;
Stepper stepperRA(STEPS, COILRA1, COILRA2, COILRA3, COILRA4); // RAD ALT
Stepper stepperCA(STEPS, COILCA1, COILCA2, COILCA3, COILCA4); // CAB ALT

//###########################################################################################
void onRadaltAltPtrChange(unsigned int newValueRA) {
  RAD_ALT = map(newValueRA, 0, 65000, 720, 10);
}
DcsBios::IntegerBuffer radaltAltPtrBuffer(0x751a, 0xffff, 0, onRadaltAltPtrChange);
//###########################################################################################

void onPressureAltChange(unsigned int newValueCA) {
  CAB_ALT = map(newValueCA, 0, 65000, 45, 720);
}
DcsBios::IntegerBuffer pressureAltBuffer(0x7514, 0xffff, 0, onPressureAltChange);



DcsBios::ServoOutput radaltOffFlag(0x751c, 11, 1000, 1420);
DcsBios::ServoOutput hydIndLeft(0x751e, 7, 560, 2300);
DcsBios::ServoOutput hydIndRight(0x7520, 6, 560, 2300);
DcsBios::ServoOutput voltE(0x753e, 9, 1800, 550);
DcsBios::ServoOutput voltU(0x753c, 8, 550, 1800);

void setup() {

  RAD_ALT_servo.attach(11);
  RAD_ALT_servo.writeMicroseconds(1420);  // set servo to "Off Point"
 delay(300);
 RAD_ALT_servo.detach();

 HYD_LFT_servo.attach(7);
   HYD_LFT_servo.writeMicroseconds(1100);  // set servo to "Mid Point"
 delay(300);
  HYD_LFT_servo.detach();

 HYD_RHT_servo.attach(6);
   HYD_RHT_servo.writeMicroseconds(1100);  // set servo to "Mid Point"
 delay(300);
  HYD_RHT_servo.detach();

   BATT_U_servo.attach(8);
   BATT_U_servo.writeMicroseconds(1100);  // set servo to "Mid Point"
 delay(300);
  BATT_U_servo.detach();

   BATT_E_servo.attach(9);
   BATT_E_servo.writeMicroseconds(1100);  // set servo to "Mid Point"
 delay(300);
  BATT_E_servo.detach();


 
  //###########################################################################################
  /// RADAR ALT WORKING ======> SET RADAR ALT STEPPER TO 0 FEET
  stepperRA.setSpeed(50);
  stepperRA.step(720);       //Reset FULL ON Position
  stepperRA.step(-720);       //Reset FULL OFF Position
posRA = 0;
RAD_ALT = map(0, 0, 65000, 720, 10);
  /// RADAR ALT WORKING ======< SET RADAR ALT STEPPER TO 0 FEET

  /// CABIN ALT WORKING ======> SET CABIN ALT STEPPER TO 0 FEET
  stepperCA.setSpeed(40);
  stepperCA.step(720);       //Reset FULL ON Position
  stepperCA.step(-720);       //Reset FULL OFF Position
posCA = 0;
CAB_ALT = map(0, 0, 65000, 40, 720);
  /// CABIN ALT WORKING ======< SET CABIN ALT STEPPER TO 0 FEET


  //###########################################################################################

  DcsBios::setup();
}


//####################################################### ####################################
/// RADAR ALT LOOP WORKING ======>


void loop() {
{
  valRA = RAD_ALT;
  if (abs(valRA - posRA) > 2) {      //if diference is greater than 2 steps.
    if ((valRA - posRA) > 0) {
      stepperRA.step(-1);      // move one step to the left.
      posRA++;
    }
    if ((valRA - posRA) < 0) {
      stepperRA.step(1);       // move one step to the right.
      posRA--;
    }
  }
    /// RADAR ALT LOOP WORKING ======<
    //###########################################################################################
{
    valCA = CAB_ALT;
    // 
    if (abs(valCA - posCA) > 2) {      //if diference is greater than 2 steps.
      if ((valCA - posCA) > 0) {
        stepperCA.step(1);      // move one step to the left.
        posCA++;
      }
      if ((valCA - posCA) < 0) {
        stepperCA.step(-1);       // move one step to the right.
        posCA--;
      }
      /// CABIN ALT LOOP WORKING ======<
      //###########################################################################################
    }

    }
  }
  DcsBios::loop();
}
