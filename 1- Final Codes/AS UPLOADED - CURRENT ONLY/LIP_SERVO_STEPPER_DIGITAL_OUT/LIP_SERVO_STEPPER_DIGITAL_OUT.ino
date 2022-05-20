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

String readString;
#include <Servo.h>
#define DCSBIOS_IRQ_SERIAL
//#define DCSBIOS_DEFAULT_SERIAL

#include "LedControl.h"
#include "DcsBios.h"


int Ethernet_In_Use = 1;            // Check to see if jumper is present - if it is disable Ethernet calls. Used for Testing
#define Reflector_In_Use 1
#define DCSBIOS_In_Use 1
#define Serial_In_Use 0

// Ethernet Related
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte myMac[] = {0xA8, 0x61, 0x0A, 0x9E, 0x83, 0x6F};
IPAddress myIP(172, 16, 1, 111);
String strMyIP = "172.16.1.111  ";

// Reflector
IPAddress reflectorIP(172, 16, 1, 10);
String strReflectorIP = "172.16.1.10";

//const unsigned int trimport = 7791;           // Listening for trigger to centre trim servo



// Arduino Due for Keystroke translation and Pixel Led driving
IPAddress ledTargetIP(172, 16, 1, 105);
String strLedTargetIP = "172.16.1.105";

const unsigned int localport = 7788;
const unsigned int keyboardport = 7788;
const unsigned int ledport = 7789;
const unsigned int remoteport = 7790;
const unsigned int reflectorport = 27000;

#define EthernetStartupDelay 1000

// Packet Length
int trimPacketSize;
int trimLen;
int keyboardpacketSize;
int keyboardLen;


EthernetUDP senderudp;                   //Left and Right Consoles
EthernetUDP keyboardudp;              // Keyboard

char trimpacketBuffer[1000];           //buffer to store trim data
char keyboardpacketBuffer[1000];      //buffer to store keyboard data




#define RED_STATUS_LED_PORT 5               // RED LED is used for monitoring ethernet
#define GREEN_STATUS_LED_PORT 13               // RED LED is used for monitoring ethernet
#define FLASH_TIME 1000
unsigned long NEXT_STATUS_TOGGLE_TIMER = 0;
bool GREEN_LED_STATE = false;
bool RED_LED_STATE = false;
unsigned long timeSinceRedLedChanged = 0;


bool Debug_Display = false;

#define LAUNCH_BAR_PORT 27
#define HOOK_BYPASS_PORT 28
#define FUEL_DUMP_PORT 29
#define APU_PORT 30
#define ENGINE_CRANK_PORT 31


#define BLEED_AIR_SOL_PORT 22
#define PITOT_HEAT_PORT 23
#define LASER_ARM_PORT 24
#define CANOPY_MAG_PORT 25

unsigned long NEXT_PORT_TOGGLE_TIMER = 0;
bool PORT_OUTPUT_STATE = false;


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

#define  COILBP1  39 // BP = BRAKE PRESSURE
#define  COILBP2  37
#define  COILBP3  35
#define  COILBP4  33

int RAD_ALT = 0;
int valRA = 0;
int CAB_ALT;
int valCA = 0;
int CurCABALT = 0;
int BRAKE_PRESSURE = 0;
int valBP = 0;
int posCA = 0;
int posRA = 0;
int posBP = 0;

Stepper stepperRA(STEPS, COILRA1, COILRA2, COILRA3, COILRA4); // RAD ALT
Stepper stepperCA(STEPS, COILCA1, COILCA2, COILCA3, COILCA4); // CAB ALT
Stepper stepperBP(STEPS, COILBP1, COILBP2, COILBP3, COILBP4); // BRAKE PRESSURE
#define BrakePressureZeroPoint 0
#define BrakePressureMaxPoint 150






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


void onHydIndBrakeChange(unsigned int newValueBP) {
  BRAKE_PRESSURE = map(newValueBP, 0, 65000, BrakePressureZeroPoint, BrakePressureMaxPoint);
}
DcsBios::IntegerBuffer hydIndBrakeBuffer(0x7506, 0xffff, 0, onHydIndBrakeChange);


Servo RAD_ALT_servo;
Servo HYD_LFT_servo;
Servo HYD_RHT_servo;
Servo BATT_U_servo;
Servo BATT_E_servo;
Servo TRIM_servo;

#define RadarAltServoPin 11
#define HydLeftServoPin 7
#define HydRightServoPin 6
#define VoltEServoPin 9
#define VoltUServoPin 8
#define TrimServoPin 12

#define RAD_ALT_servo_Off_Pos     1420
#define HYD_LFT_servo_Max_Pos     2340
#define HYD_RHT_servo_Max_pos     2340
#define BATT_U_servo_Max_Pos      1900
#define BATT_E_servo_Max_Pos      180
#define TRIM_servo_Off_Center_Pos 800

#define RAD_ALT_servo_Hidden_Pos  1050
#define HYD_LFT_servo_Min_Pos     600
#define HYD_RHT_servo_Min_pos     600
#define BATT_U_servo_Min_Pos      400
#define BATT_E_servo_Min_Pos      1800
#define TRIM_servo_Center_Pos     1450


DcsBios::ServoOutput radaltOffFlag(0x751c, RadarAltServoPin, RAD_ALT_servo_Hidden_Pos, RAD_ALT_servo_Off_Pos);
DcsBios::ServoOutput hydIndLeft(0x751e, HydLeftServoPin, HYD_LFT_servo_Min_Pos, HYD_LFT_servo_Max_Pos);
DcsBios::ServoOutput hydIndRight(0x7520, HydRightServoPin, HYD_RHT_servo_Min_pos, HYD_RHT_servo_Max_pos);
DcsBios::ServoOutput voltE(0x753e, VoltEServoPin, BATT_E_servo_Min_Pos, BATT_E_servo_Max_Pos);
DcsBios::ServoOutput voltU(0x753c, VoltUServoPin, BATT_U_servo_Min_Pos, BATT_U_servo_Max_Pos);
// DcsBios::ServoOutput rudTrim(0x7528, TrimServoPin, 544, 2400);


void onLaunchBarSwChange(unsigned int newValue) {
  digitalWrite(LAUNCH_BAR_PORT, newValue);
}
DcsBios::IntegerBuffer launchBarSwBuffer(0x7480, 0x2000, 13, onLaunchBarSwChange);


void onHookBypassSwChange(unsigned int newValue) {
  digitalWrite(HOOK_BYPASS_PORT, newValue);
}
DcsBios::IntegerBuffer hookBypassSwBuffer(0x7480, 0x4000, 14, onHookBypassSwChange);


void onApuControlSwChange(unsigned int newValue) {
  digitalWrite(APU_PORT, newValue);
}
DcsBios::IntegerBuffer apuControlSwBuffer(0x74c2, 0x0100, 8, onApuControlSwChange);


void onEngineCrankSwChange(unsigned int newValue) {
  bool CrankSwitchState = false;
  if (newValue != 1) {
    CrankSwitchState = true;
  }
  else
  {
    CrankSwitchState = false;
  }

  digitalWrite(ENGINE_CRANK_PORT, CrankSwitchState);
}
DcsBios::IntegerBuffer engineCrankSwBuffer(0x74c2, 0x0600, 9, onEngineCrankSwChange);



void onFuelDumpSwChange(unsigned int newValue) {
  digitalWrite(FUEL_DUMP_PORT, newValue);
}
DcsBios::IntegerBuffer fuelDumpSwBuffer(0x74b4, 0x0100, 8, onFuelDumpSwChange);


void onPitotHeatSwChange(unsigned int newValue) {
  digitalWrite(PITOT_HEAT_PORT, newValue);
}
DcsBios::IntegerBuffer pitotHeatSwBuffer(0x74c8, 0x0100, 8, onPitotHeatSwChange);


void onLtdRSwChange(unsigned int newValue) {
  digitalWrite(LASER_ARM_PORT, newValue);
}
DcsBios::IntegerBuffer ltdRSwBuffer(0x74c8, 0x4000, 14, onLtdRSwChange);


void onCanopySwChange(unsigned int newValue) {
  bool CanopySwitchState = false;
  if (newValue == 2) {
    CanopySwitchState = true;
  }
  else
  {
    CanopySwitchState = false;
  }
  digitalWrite(CANOPY_MAG_PORT, CanopySwitchState);
  digitalWrite( RED_STATUS_LED_PORT, newValue);
}
DcsBios::IntegerBuffer canopySwBuffer(0x74ce, 0x0300, 8, onCanopySwChange);




void CenterTrimServo() {
  TRIM_servo.attach(TrimServoPin);
  TRIM_servo.writeMicroseconds(1100);  // set servo to "Mid Point"
  delay(10);
  TRIM_servo.writeMicroseconds(800);  // set servo to "Mid Point"
  delay(300);
  TRIM_servo.detach();
}



void onToTrimBtnChange(unsigned int newValue) {
  if (newValue == 1) {
    CenterTrimServo();
  }
}
DcsBios::IntegerBuffer toTrimBtnBuffer(0x74b4, 0x2000, 13, onToTrimBtnChange);

void onMcReadyChange(unsigned int newValue) {
  if (newValue == 1) {
    SendIPString("MASTER_ARM_DISCH_READY=1");
  } else {
    SendIPString("MASTER_ARM_DISCH_READY=0");
  }
}
DcsBios::IntegerBuffer mcReadyBuffer(0x740c, 0x8000, 15, onMcReadyChange);

void onMcDischChange(unsigned int newValue) {
  if (newValue == 1) {
    SendIPString("MASTER_ARM_DISCH=1");
  } else {
    SendIPString("MASTER_ARM_DISCH=0");
  }
}
DcsBios::IntegerBuffer mcDischBuffer(0x740c, 0x4000, 14, onMcDischChange);

void onMasterModeAaLtChange(unsigned int newValue) {
  if (newValue == 1) {
    SendIPString("MASTER_ARM_AA=1");
  } else {
    SendIPString("MASTER_ARM_AA=0");
  }
}
DcsBios::IntegerBuffer masterModeAaLtBuffer(0x740c, 0x0200, 9, onMasterModeAaLtChange);


void onMasterModeAgLtChange(unsigned int newValue) {
  if (newValue == 1) {
    SendIPString("MASTER_ARM_AG=1");
  } else {
    SendIPString("MASTER_ARM_AG=0");
  }
}
DcsBios::IntegerBuffer masterModeAgLtBuffer(0x740c, 0x0400, 10, onMasterModeAgLtChange);


void onSpinLtChange(unsigned int newValue) {
  if (newValue == 1) {
    SendIPString("SPIN=1");
  } else {
    SendIPString("SPIN=0");
  }
}
DcsBios::IntegerBuffer spinLtBuffer(0x742a, 0x0800, 11, onSpinLtChange);


void onCmsdJetSelLChange(unsigned int newValue) {
  if (newValue == 1) {
    SendIPString("ECM_JET=1");
  } else {
    SendIPString("ECM_JET=0");
  }
}
DcsBios::IntegerBuffer cmsdJetSelLBuffer(0x74d4, 0x8000, 15, onCmsdJetSelLChange);



void onConsoleIntLtChange(unsigned int newValue) {
  int ConsolesDimmerValue = 0;

  ConsolesDimmerValue = map(newValue, 0, 65000, 0, 100);
  SendIPString("ConsoleBrightness=" + String(ConsolesDimmerValue));
}

DcsBios::IntegerBuffer consoleIntLtBuffer(0x7558, 0xffff, 0, onConsoleIntLtChange);



void onWarnCautionDimmerChange(unsigned int newValue) {
  int WarnCautionDimmerValue = 0;

  WarnCautionDimmerValue = map(newValue, 0, 65000, 0, 255);
  SendIPString("WarningBrightness=" + String(WarnCautionDimmerValue));/* your code here */
}
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);






void SendIPString(String LedToSend) {
  // Used to Send Desired Keystrokes to Due acting as Keyboard
  if (Ethernet_In_Use == 1) {

    if (Reflector_In_Use == 1) {
      senderudp.beginPacket(reflectorIP, reflectorport);
      senderudp.print("LED instructions " + LedToSend);
      senderudp.endPacket();
    }
    senderudp.beginPacket(ledTargetIP, ledport);
    senderudp.print(LedToSend);
    senderudp.endPacket();
    UpdateRedStatusLed();
  }
}



void UpdateRedStatusLed() {
  if ((RED_LED_STATE == false) && (millis() >= (timeSinceRedLedChanged + FLASH_TIME ) )) {
    digitalWrite( RED_STATUS_LED_PORT, true);
    RED_LED_STATE = true;
    timeSinceRedLedChanged = millis();

  }
}






void setup() {


  pinMode( RED_STATUS_LED_PORT,  OUTPUT);
  pinMode( GREEN_STATUS_LED_PORT,  OUTPUT);

  digitalWrite( RED_STATUS_LED_PORT, true);
  digitalWrite( GREEN_STATUS_LED_PORT, true);
  delay(FLASH_TIME);
  digitalWrite( RED_STATUS_LED_PORT, false);
  digitalWrite( GREEN_STATUS_LED_PORT, false);

  delay(FLASH_TIME);
  pinMode( APU_PORT,  OUTPUT);
  pinMode( ENGINE_CRANK_PORT,  OUTPUT);
  pinMode( FUEL_DUMP_PORT,  OUTPUT);
  pinMode( HOOK_BYPASS_PORT,  OUTPUT);
  pinMode( LAUNCH_BAR_PORT,  OUTPUT);
  pinMode( BLEED_AIR_SOL_PORT,  OUTPUT);
  pinMode( PITOT_HEAT_PORT,  OUTPUT);
  pinMode( LASER_ARM_PORT,  OUTPUT);
  pinMode( CANOPY_MAG_PORT,  OUTPUT);


  // Turn everything off
  digitalWrite(APU_PORT, false);
  digitalWrite(ENGINE_CRANK_PORT, false);
  digitalWrite(FUEL_DUMP_PORT, false);
  digitalWrite(HOOK_BYPASS_PORT, false);
  digitalWrite(LAUNCH_BAR_PORT, false);
  digitalWrite(BLEED_AIR_SOL_PORT, false);
  digitalWrite(PITOT_HEAT_PORT, false);
  digitalWrite(LASER_ARM_PORT, false);
  digitalWrite(CANOPY_MAG_PORT, false);


  // Turn everything off
  digitalWrite(APU_PORT, false);
  digitalWrite(ENGINE_CRANK_PORT, false);
  digitalWrite(FUEL_DUMP_PORT, false);
  digitalWrite(HOOK_BYPASS_PORT, false);
  digitalWrite(LAUNCH_BAR_PORT, false);




  RAD_ALT_servo.attach(RadarAltServoPin);
  HYD_LFT_servo.attach(HydLeftServoPin);
  HYD_RHT_servo.attach(HydRightServoPin);
  BATT_U_servo.attach(VoltUServoPin);
  BATT_E_servo.attach(VoltEServoPin);
  TRIM_servo.attach(TrimServoPin);

  RAD_ALT_servo.writeMicroseconds(RAD_ALT_servo_Off_Pos);  // set servo to "Off Point"
  HYD_LFT_servo.writeMicroseconds(HYD_LFT_servo_Max_Pos);  // set servo to Max
  HYD_RHT_servo.writeMicroseconds(HYD_RHT_servo_Max_pos);  // set servo to Max
  BATT_U_servo.writeMicroseconds(BATT_U_servo_Max_Pos);  // set servo to "Mid Point"
  BATT_E_servo.writeMicroseconds(BATT_E_servo_Max_Pos);  // set servo to "Mid Point"
  TRIM_servo.writeMicroseconds(TRIM_servo_Off_Center_Pos);  // set servo to just off Centre
  delay(1000);

  RAD_ALT_servo.writeMicroseconds(RAD_ALT_servo_Hidden_Pos);  // set servo to min
  HYD_LFT_servo.writeMicroseconds(HYD_LFT_servo_Min_Pos);  // set servo to min
  HYD_RHT_servo.writeMicroseconds(HYD_RHT_servo_Min_pos);  // set servo to min
  BATT_U_servo.writeMicroseconds(BATT_U_servo_Min_Pos);  // set servo to min
  BATT_E_servo.writeMicroseconds(BATT_E_servo_Min_Pos);  // set servo to min
  TRIM_servo.writeMicroseconds(TRIM_servo_Center_Pos);  // set servo to centre
  delay(500);


  RAD_ALT_servo.detach();
  HYD_RHT_servo.detach();
  HYD_LFT_servo.detach();
  BATT_U_servo.detach();
  BATT_E_servo.detach();
  TRIM_servo.detach();


  //###########################################################################################
  /// RADAR ALT WORKING ======> SET RADAR ALT STEPPER TO 0 FEET
  stepperRA.setSpeed(50);
  stepperRA.step(720);       //Reset FULL ON Position
  stepperRA.step(-720);       //Reset FULL OFF Position
  posRA = 0;
  RAD_ALT = map(0, 0, 65000, 720, 10);
  /// RADAR ALT WORKING ======< SET RADAR ALT STEPPER TO 0 FEET

  /// CABIN ALT WORKING ======> SET CABIN ALT STEPPER TO 0 FEET
  stepperCA.setSpeed(60);
  stepperCA.step(700);       //Reset FULL ON Position
  stepperCA.step(-720);       //Reset FULL OFF Position
  stepperCA.step(30);       //Reset FULL OFF Position
  posCA = 0;
  CAB_ALT = map(0, 0, 65000, 40, 720);
  /// CABIN ALT WORKING ======< SET CABIN ALT STEPPER TO 0 FEET



  // BRAKE PRESSURE

  stepperBP.setSpeed(60);
  stepperBP.step(BrakePressureMaxPoint);       //Reset FULL ON Position
  delay(1000);
  stepperBP.step(-(BrakePressureMaxPoint + 100) );       //Reset FULL OFF Position
  posBP = 0;
  BRAKE_PRESSURE = map(0, 0, 65000, BrakePressureZeroPoint, BrakePressureMaxPoint);
  /// BRAKE PRESSURE




  if (Ethernet_In_Use == 1) {

    Ethernet.begin( myMac, myIP);
    keyboardudp.begin(keyboardport);
    senderudp.begin(ledport);

    if (Reflector_In_Use == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("Init Digital Output - version:" + String(ProgramVersion) + " " + strMyIP + " " + String(millis()) + "mS since reset.");
      keyboardudp.endPacket();
    }
  }


  NEXT_PORT_TOGGLE_TIMER = millis() + 1000;
  NEXT_STATUS_TOGGLE_TIMER = millis() + 1000;



  DcsBios::setup();

  if (Reflector_In_Use == 1) {
    keyboardudp.beginPacket(reflectorIP, reflectorport);
    keyboardudp.println("Exiting Setup");
    keyboardudp.endPacket();
  }

}


void loop() {

  if (millis() >= NEXT_STATUS_TOGGLE_TIMER) {
    GREEN_LED_STATE = !GREEN_LED_STATE;
    digitalWrite( GREEN_STATUS_LED_PORT, GREEN_LED_STATE);
    NEXT_STATUS_TOGGLE_TIMER = millis() + FLASH_TIME;
  }

  // Turn off Red status led after flashtime
  if ((RED_LED_STATE == true) && (millis() >= (timeSinceRedLedChanged + FLASH_TIME ) )) {
    digitalWrite( RED_STATUS_LED_PORT, false);
    RED_LED_STATE = false;
    timeSinceRedLedChanged = millis();

  }

  // **************************** Handle Steppers

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


  /// BRAKE PRESSURE
  //###########################################################################################
  {
    valBP = BRAKE_PRESSURE;
    //
    if (abs(valBP - posBP) > 2) {      //if diference is greater than 2 steps.
      if ((valBP - posBP) > 0) {
        stepperBP.step(1);      // move one step to the left.
        posBP++;
      }
      if ((valBP - posBP) < 0) {
        stepperBP.step(-1);       // move one step to the right.
        posBP--;
      }
    }

  }



  DcsBios::loop();
}
