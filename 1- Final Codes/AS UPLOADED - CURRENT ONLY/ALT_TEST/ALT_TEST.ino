// UPDATED TO DCS-BIOS FP EDITION FOR OPEN HORNET


////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
//||               FUNCTION = ALL RIGHT OUTPUT                        ||\\
//||              LOCATION IN THE PIT = LIP RIGHT SIDE                ||\\
//||            ARDUINO PROCESSOR TYPE = Arduino Mega                 ||\\
//||      ARDUINO CHIP SERIAL NUMBER = SN - 95035313935351604011      ||\\
//||            PROGRAM PORT CONNECTED COM PORT = COM 15              ||\\
//||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\

// BM NOTES, TAKE OUT LEFT HAND SIDE (TRIM) (BRK PRES)

// Tell DCS-BIOS to use a serial connection and use interrupt-driven
// communication. The main program will be interrupted to prioritize
// processing incoming data.
//
// This should work on any Arduino that has an ATMega328 controller
// (Uno, Pro Mini, many others).

// Last Checked 20220403


#define ProgramVersion 10

#define DCSBIOS_IRQ_SERIAL


#include "LedControl.h"
#include "DcsBios.h"



// Ethernet Related
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


//const unsigned int trimport = 7791;           // Listening for trigger to centre trim servo


#define DCSBIOS_DEFAULT_SERIAL
#include "LedControl.h"
#include "DcsBios.h"

#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)


#define  COILCA1  41 // CA = CAB ALT
#define  COILCA2  43
#define  COILCA3  45
#define  COILCA4  47

int CAB_ALT;
int CurCABALT = 0;
int posCA = 0;
int valCA = 0;

Stepper stepperCA(STEPS, COILCA1, COILCA2, COILCA3, COILCA4); // CAB ALT


void onStbyAlt100FtPtrChange(unsigned int newValueCA) {
  CAB_ALT = map(newValueCA, 0, 65000, 0, 720);
}
DcsBios::IntegerBuffer stbyAlt100FtPtrBuffer(0x74f4, 0xffff, 0, onStbyAlt100FtPtrChange);






void setup() {


  /// CABIN ALT WORKING ======> SET CABIN ALT STEPPER TO 0 FEET
  stepperCA.setSpeed(60);
  stepperCA.step(720);       //Reset FULL ON Position
  stepperCA.step(-720);       //Reset FULL OFF Position
  //stepperCA.step(30);       //Reset FULL OFF Position
  
  //CAB_ALT = map(0, 0, 65000, 0, 720);
  /// CABIN ALT WORKING ======< SET CABIN ALT STEPPER TO 0 FEET



 



  DcsBios::setup();
}

void loop() {


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





  DcsBios::loop();
}
