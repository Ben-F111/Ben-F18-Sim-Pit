
/* #####################################################
DCS BIOS CODE for OPEN HORNET DES_DES_CAB_ALT GAUGE
######################################################*/
String readString;
#include <Servo.h>
//#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_DEFAULT_SERIAL
#include "LedControl.h"
#include "DcsBios.h"

#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)
#define  COIL1  36
#define  COIL2  34
#define  COIL3  32
#define  COIL4  38

//Servo myservo;  // create servo object to control a servo
int DES_CAB_ALT = 0;
int val = 0;
int pos = 0;     
int tempv = 0;
Stepper stepper(STEPS, COIL1, COIL2, COIL3, COIL4);


//###########################################################################################
// TEST USING ROTARY ENC //OUT WHATS NOT USED - WORK BUT NOT ENUGH "STEPS" AND NEEDS TO BE RESET ECH TIME
//DcsBios::RotaryEncoder CABaltHeight("CABALT_HEIGHT", "-3200", "+3200", 16, 17);
//###########################################################################################


//###########################################################################################
//TEST USING POT //OUT WHATS NOT USED
//DcsBios::RotarySyncingPotentiometer altSetPressureBuffer("CABALT_HEIGHT", A4, 0x7518, 0xffff, 0);//changed 0x7518 from 0x7516
//DcsBios::RotarySyncingPotentiometer CABaltHeight("CABALT_HEIGHT", A4, 0x7518, 0xffff, 0);
//###########################################################################################
//DcsBios::LED CABaltGreenLamp(0x74a0, 0x0100, 19);
//DcsBios::LED lowAltWarnLt(0x749c, 0x8000, 18);
//DcsBios::Switch2Pos CABaltTestSw("CABALT_TEST_SW", 0);

//###########################################################################################
void onPressureAltChange(unsigned int newValue) {
DES_CAB_ALT = map(newValue, 0, 65000, 40, 720);

/// CABAR ALT LOOP WORKING ======<
//###########################################################################################
          
    
}
DcsBios::IntegerBuffer pressureAltBuffer(0x7514, 0xffff, 0, onPressureAltChange);



//###########################################################################################


//DcsBios::ServoOutput CABaltOffFlag(0x751c,2, 1000, 1420); //happy

void setup(){
// myservo.attach(45);
//myservo.writeMicroseconds(1420);  // set servo to "Off Point"
//delay(300);
//myservo.detach();
//###########################################################################################
/// CABAR ALT WORKING ======> SET CABAR ALT STEPPER TO 0 FEET
stepper.setSpeed(20);
stepper.step(720);       //Reset FULL ON Position 
stepper.step(-720);       //Reset FULL OFF Position
//stepper.step(0);       //Reset 0 Position
pos = 0;
DES_CAB_ALT = map(0, 0, 65000, 40, 720);
/// CABAR ALT WORKING ======< SET CABAR ALT STEPPER TO 0 FEET

//###########################################################################################

  DcsBios::setup();
}

//####################################################### ####################################
/// CABAR ALT LOOP WORKING ======>
         
void loop(){

  val = DES_CAB_ALT;
  if(abs(val - pos)> 2){         //if diference is greater than 2 steps.
      if((val - pos)> 0){
          stepper.step(1);      // move one step to the left.
          pos++;
          }
      if((val - pos)< 0){
          stepper.step(-1);       // move one step to the right.
          pos--;
          }
  }
  DcsBios::loop();
}
//###########################################################################################
