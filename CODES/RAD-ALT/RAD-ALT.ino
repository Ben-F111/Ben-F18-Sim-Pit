
/* #####################################################
DCS BIOS CODE for OPEN HORNET RAD_ALT GAUGE
######################################################*/
String readString;
#include <Servo.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)
#define  COIL1  52
#define  COIL2  51
#define  COIL3  50
#define  COIL4  53
Servo myservo;  // create servo object to control a servo
int RAD_ALT = 0;
int val = 0;

Stepper stepper(STEPS, COIL1, COIL2, COIL3, COIL4);


//###########################################################################################
// TEST USING ROTARY ENC //OUT WHATS NOT USED - WORK BUT NOT ENUGH "STEPS" AND NEEDS TO BE RESET ECH TIME
//DcsBios::RotaryEncoder radaltHeight("RADALT_HEIGHT", "-3200", "+3200", 16, 17);
//###########################################################################################


//###########################################################################################
//TEST USING POT //OUT WHATS NOT USED
//DcsBios::RotarySyncingPotentiometer altSetPressureBuffer("RADALT_HEIGHT", A4, 0x7516, 0xffff, 0);
//DcsBios::RotarySyncingPotentiometer radaltHeight("RADALT_HEIGHT", A4, 0x7516, 0xffff, 0);
//###########################################################################################
//DcsBios::LED radaltGreenLamp(0x74a0, 0x0100, 19);
//DcsBios::LED lowAltWarnLt(0x749c, 0x8000, 18);
//DcsBios::Switch2Pos radaltTestSw("RADALT_TEST_SW", 0);

//###########################################################################################
void onRadaltAltPtrChange(unsigned int newValue) {
RAD_ALT = map(newValue, 0, 65000, 720, 0);
}
DcsBios::IntegerBuffer radaltAltPtrBuffer(0x751a, 0xffff, 0, onRadaltAltPtrChange);
//###########################################################################################


DcsBios::ServoOutput radaltOffFlag(0x751c,2, 1000, 1420); //happy

void setup(){
 myservo.attach(45);
myservo.writeMicroseconds(1420);  // set servo to "Off Point"
delay(300);
myservo.detach();
//###########################################################################################
/// RADAR ALT WORKING ======> SET RADAR ALT STEPPER TO 0 FEET
stepper.setSpeed(20);
stepper.step(720);       //Reset FULL ON Position 
stepper.step(-720);       //Reset FULL OFF Position
stepper.step(10);       //Reset 0 Position
/// RADAR ALT WORKING ======< SET RADAR ALT STEPPER TO 0 FEET

//###########################################################################################

  DcsBios::setup();
}

//####################################################### ####################################
/// RADAR ALT LOOP WORKING ======>
int pos=0;              
void loop(){
  val = RAD_ALT;
  if(abs(val - pos)> 2){         //if diference is greater than 2 steps.
      if((val - pos)> 0){
          stepper.step(-1);      // move one step to the left.
          pos++;
          }
      if((val - pos)< 0){
          stepper.step(1);       // move one step to the right.
          pos--;
          }
/// RADAR ALT LOOP WORKING ======<
//###########################################################################################
          
      }

  DcsBios::loop();
}
//###########################################################################################
