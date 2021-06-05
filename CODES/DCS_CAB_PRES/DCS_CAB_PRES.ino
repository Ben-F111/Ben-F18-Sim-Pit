/* #####################################################
DCS BIOS CODE for OPEN HORNET HYD BRAKE GAUGE
######################################################*/

#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)
#define  COIL1  8
#define  COIL2  9
#define  COIL3  10
#define  COIL4  11

int CAB_PRES = 0;
int val = 0;

Stepper stepper(STEPS, COIL1, COIL2, COIL3, COIL4);

void onPressureAltChange(unsigned int newValue) {
CAB_PRES = map(newValue, 0, 65000, 0, 1000);
}
DcsBios::IntegerBuffer pressureAltBuffer(0x7514, 0xffff, 0, onPressureAltChange);

void setup(){
  stepper.setSpeed(30);
 stepper.step(720);       //Reset Position(250 steps counter-clockwise (just over the Max travel). 
//stepper.step(-720);       //Reset Position(177 steps to the 0 Point clockwise). 
// stepper.step(50);       //Reset Position(177 steps to the 0 Point clockwise).
  DcsBios::setup();
}
int pos=0;                 //Position in steps(0-150)

void loop(){
  val = CAB_PRES;
  val = map(CAB_PRES,0,1000,620,0);    // map Steper Needle 0-4. 0=0 - 4=150
  if(abs(val - pos)> 2){         //if diference is greater than 2 steps.
      if((val - pos)> 0){
          stepper.step(-1);      // move one step to the left.
          pos++;
          }
      if((val - pos)< 0){
          stepper.step(1);       // move one step to the right.
          pos--;
          }
      }
  DcsBios::loop();
}
