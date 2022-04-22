////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
//||               FUNCTION = HORNET UDP to Keyboard and Pixel LED     ||\\
//||              LOCATION IN THE PIT = LIP LEFT HAND SIDE             ||\\
//||            ARDUINO PROCESSOR TYPE = Arduino Mega 2560            ||\\
//||      ARDUINO CHIP SERIAL NUMBER = SN: 95433343733351201290       ||\\
//||      ETHERNET SHEILD MAC ADDRESS = MAC                           ||\\
//||                    CONNECTED COM PORT = COM 6                    ||\\
//||               ****ADD ASSIGNED COM PORT NUMBER****               ||\\
//||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\

/*
    KNOWN ISSUE - NEED TO UNPLUG NATIVE USB PORT WHEN PROGRAMMING
*/

/*

  Receives a space delimited set of characters and sends them to the keyboard

  Also drives Pixel LEDs - this was needed as Interrupts from DCS BIOS using serial disrupted updates. These values are delimited by
  by '='

  Place Modifiers in the first part of the string being sent.

  Unable to find a way using the normal keyboard library ot send pause - so will need to remap to some other key

  Works on an Arduino DUE.

  Will use the logic from pyHWWLink_Keystroke_Sender

  Initially just sending very simple single characters

  ModifiersOfInterest = ['ALT', 'CTRL', 'SHIFT', 'LSHIFT', 'RSHIFT', 'LCTRL', 'RCTRL', 'LALT', 'RALT', 'LWIN', 'RWIN' ]

  USING UDP_TEST_SENDER.PY for testing

*/


/*
   Sim Control Panel Commands
    Head Tracking             Default Key Bindings
    -------------             --------------------
      Freeze
      Normal                  LWin + F1               F1 Head shift movement on/off
      Centre                  Num 5                   View Centre

    Time
    ----
      Fast                   LCtrl + Z                Time Accelerate
      Real                   LShift + Z               Time Normal

    Toggle
    ------
      Night Vision Glasses  RShift + H               Toggle Goggles
      Labels                LShift + F10             All Labels

    Game
    ----
      Pause                 Pause                    Pause
      Normal                Pause                    Pause
      Freeze

    View
    ----
      Cockpit               F1
      Chase                 LCtrl + F4              F4 Chase view
      External              F2                      F2 Aircraft View
      Fly by                F3                      F3 Fly-By View
      Weapon                F6                      F6 Released Weapon View
      Enemy                 LCtrl + F5              F5 Ground Hostile View
      Hud                   LAlt + F1               F1 Hud only view switch
      Map                   F10                     F10 Theater Map view




*/
#define Ethernet_In_Use 1
const int Serial_In_Use = 1;
#define Reflector_In_Use 1
#define DCSBIOS_In_Use 1


// ###################################### Begin Pixel Led Related #############################

// PixelLighting

//#define DCSBIOS_IRQ_SERIAL
#define DCSBIOS_DEFAULT_SERIAL

#include "DcsBios.h"

//#define RED_STATUS_LED_PORT 5               // RED LED is used for monitoring ethernet
//#define GREEN_STATUS_LED_PORT 13               // RED LED is used for monitoring ethernet
#define FLASH_TIME 1000
unsigned long NEXT_STATUS_TOGGLE_TIMER = 0;
bool GREEN_LED_STATE = false;
bool RED_LED_STATE = false;
unsigned long timeSinceRedLedChanged = 0;

#define LAUNCH_BAR_PORT 27
#define HOOK_BYPASS_PORT 28
#define FUEL_DUMP_PORT 29
#define APU_PORT 30

#define ENGINE_CRANK_PORT 31
unsigned long NEXT_PORT_TOGGLE_TIMER = 0;
bool PORT_OUTPUT_STATE = false;



// ###################################### Begin Keyboard Related #############################

#include <Keyboard.h>

bool leftAltInUse = false;
bool rightAltInUse = false;
bool altInUse = false;
bool ctrlInUse = false;
bool shiftInUse = false;
bool lShiftInUse = false;
bool rShiftInUse = false;
bool lCtrlInUse = false;
bool rCtrlInUse = false;
bool lWinInUse = false;
bool rWinInUse = false;

const int delayBetweenRelease = 200;          // How long is a key held down for - WARNING THIS CURRENTLY BLOCKS THE RUNNING OF OTHER CODE
// This could be optimised to set a flag and clear during the loop
unsigned long timeBeforeReleaseAllKeys = 0;
bool  releaseKeysNeeded = false;

// ###################################### End Keyboard Related #############################




// ###################################### Begin Ethernet Related #############################
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// These local Mac and IP Address will be reassigned early in startup based on
// the device ID as set by address pins
#define EthernetStartupDelay 500
byte mac[] = {0x00, 0xDD, 0x3E, 0xCA, 0x37, 0x99};
IPAddress ip(172, 16, 1, 110);
String strMyIP = "X.X.X.X";

// Reflector
IPAddress reflectorIP(172, 16, 1, 10);
String strReflectorIP = "X.X.X.X";

const unsigned int keyboardport = 7788;
const unsigned int ledport = 7789;
const unsigned int remoteport = 49000;
const unsigned int reflectorport = 27000;


// Packet Length
int keyboardpacketSize;
int keyboardLen;
int ledPacketSize;
int ledLen;
int Test_MSG;

EthernetUDP keyboardudp;              // Keyboard
EthernetUDP ledudp;                   //Left and Right Consoles


char keyboardpacketBuffer[1000];      //buffer to store keyboard data
char ledpacketBuffer[1000];           //buffer to store led data
char outpacketBuffer[1000];           //buffer to store the outgoing data


// ###################################### End Ethernet Related #############################



void onLaunchBarSwChange(unsigned int newValue) {
  digitalWrite(LAUNCH_BAR_PORT, newValue);
}
DcsBios::IntegerBuffer launchBarSwBuffer(0x7480, 0x2000, 13, onLaunchBarSwChange);


void onHookBypassSwChange(unsigned int newValue) {
  digitalWrite(HOOK_BYPASS_PORT, newValue);
}
DcsBios::IntegerBuffer hookBypassSwBuffer(0x7480, 0x4000, 14, onHookBypassSwChange);


void onApuControlSwChange(unsigned int newValue) {
 // digitalWrite(APU_PORT, newValue);
  Test_MSG = newValue;
 
  
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








// Variables picked up from 737 Code
bool Debug_Display = true;
char *ParameterNamePtr;
char *ParameterValuePtr;

void setup() {

  if (Serial_In_Use) {
    Serial.begin(250000);
    Serial.println("UDP to Keyboard Startup");
  }


  if (Ethernet_In_Use == 1) {
    delay(EthernetStartupDelay);
    Ethernet.begin( mac, ip);

    keyboardudp.begin( keyboardport );


    if (Reflector_In_Use == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("Init UDP Keyboard and Led - " + strMyIP + " " + String(millis()) + "mS since reset.");
      keyboardudp.endPacket();
    }

    ledudp.begin(ledport);
 DcsBios::setup();

  }




  Keyboard.begin();
}



void SendCharactersToKeyboard(int packetLength) {

  // Now need to walk through received string - Modifers must be sent first, so build up
  // A list of them using a space as a delimiter


  // Move these declaration to global when complete - still need to get flags on entry
  // change qualifiers for serial in use when complete - currently debuggin at all times

  altInUse = false;
  ctrlInUse = false;
  shiftInUse = false;
  leftAltInUse = false;
  rightAltInUse = false;
  lShiftInUse = false;
  rShiftInUse = false;
  lCtrlInUse = false;
  rCtrlInUse = false;
  lWinInUse = false;
  rWinInUse = false;

  String thisElement = "";
  char keyToPress[50];

  bool bLocalDebug = false;

  if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Packet Received");
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.print("Len is ");
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(packetLength);

  String thisSet = "";
  for (int characterPtr = 0; characterPtr < packetLength ; characterPtr++ ) {
    //Serial.print(packetBuffer[characterPtr]);

    // Build Out Modifier list
    // ModifiersOfInterest = ['ALT', 'CTRL', 'SHIFT', 'LSHIFT', 'RSHIFT', 'LCTRL', 'RCTRL', 'LALT', 'RALT', 'LWIN', 'RWIN' ]
    // Modifiers to do  [   , 'RWIN' ]
    // Also need to deal with Function Key or pretty much any key that is not a single character


    // We are delimiting by spaces
    if (String(keyboardpacketBuffer[characterPtr]) == " ") {
      if (thisElement == "LALT") {
        leftAltInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Left Alt in Use");
      }
      else if (thisElement == "RALT") {
        rightAltInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Right Alt in Use");
      }
      else if (thisElement == "ALT") {
        altInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Alt in Use");
      }
      else if (thisElement == "CTRL") {
        ctrlInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Right Alt in Use");
      }
      else if (thisElement == "SHIFT") {
        shiftInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Shift in Use");
      }
      else if (thisElement == "LSHIFT") {
        lShiftInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Left Shift in Use");
      }
      else if (thisElement == "RSHIFT") {
        rShiftInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Right Shift in Use");
      }
      else if (thisElement == "LCTRL") {
        lCtrlInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Left Controlin Use");
      }
      else if (thisElement == "RCTRL") {
        rCtrlInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Right Control in Use");
      }
      else if (thisElement == "LWIN") {
        lWinInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Left Windows in Use");
      }
      else if (thisElement == "RWIN") {
        rWinInUse = true;
        if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Right Windows in Use");
      }
      thisElement = "";
    }

    else {
      thisElement = thisElement + String(keyboardpacketBuffer[characterPtr]);
    }

  }
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);




  // Hold  down the special/modifier keys
  if (altInUse)
    Keyboard.press(KEY_LEFT_ALT);
  if (ctrlInUse)
    Keyboard.press(KEY_LEFT_CTRL);
  if (shiftInUse)
    Keyboard.press(KEY_LEFT_SHIFT);


  if (leftAltInUse)
    Keyboard.press(KEY_LEFT_ALT);
  if (rightAltInUse)
    Keyboard.press(KEY_RIGHT_ALT);
  if (lShiftInUse)
    Keyboard.press(KEY_LEFT_SHIFT);
  if (rShiftInUse)
    Keyboard.press(KEY_RIGHT_SHIFT);
  if (lCtrlInUse)
    Keyboard.press(KEY_LEFT_CTRL);
  if (rCtrlInUse)
    Keyboard.press(KEY_RIGHT_CTRL);
  if (lWinInUse)
    Keyboard.press(KEY_LEFT_GUI);
  if (rWinInUse)
    Keyboard.press(KEY_RIGHT_GUI);




  // If the String includes a Carriage return at the end remove it
  // This can occur while sending test strings

  if (thisElement[thisElement.length() - 1] == 0x0A) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Found trailing CR - removing it");
    thisElement.remove(thisElement.length() - 1);
  }




  if (thisElement.length() == 1) {
    // We are hitting a single character to send
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Correct length of Element - Sending");
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
    thisElement.toCharArray(keyToPress, 2);

    Keyboard.press(keyToPress[0]);

  } else
  {
    // Function Keys
    if (thisElement == "F1") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F1);
    }
    else if (thisElement == "F2") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F2);
    }
    else if (thisElement == "F3") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F3);
    }
    else if (thisElement == "F4") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F4);
    }
    else if (thisElement == "F5") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F5);
    }
    else if (thisElement == "F6") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F6);
    }
    else if (thisElement == "F7") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F7);
    }
    else if (thisElement == "F8") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F8);
    }
    else if (thisElement == "F9") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F9);
    }
    else if (thisElement == "F10") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F10);
    }
    else if (thisElement == "F11") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F11);
    }
    else if (thisElement == "F12") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_F12);
    }




    // NUMBER PAD KEYS
    // Need to add 136 to the value otherwise keyboard.press will try ASCII look up
    // Reference https://forum.arduino.cc/t/keyboard-write-with-number-pad-keys-from-leonardo/175304
    //The keypad keys are 84 through 99 (0x54 through 0x63) but the keyboard.press() function will treat values below 128 (0x7F) as "printable" so it will look them up in a table of ascii keycodes. To get past that you have to add 136 to the keycode. Try these:

    // 220 '\334' Keypad /
    // 221 '\335' Keypad *
    // 222 '\336' Keypad -
    // 223 '\337' Keypad +
    // 224 '\340' Keypad ENTER

    else if (thisElement == "KEYPAD/") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(220);
    }
    else if (thisElement == "KEYPAD*") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "KEYPAD-") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "KEYPAD+") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "KEYPADENTER") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "ESC") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(KEY_ESC);
    }


    // 225 '\341' Keypad 1 and End
    // 226 '\342' Keypad 2 and Down Arrow
    // 227 '\343' Keypad 3 and PageDn
    // 228 '\344' Keypad 4 and Left Arrow
    // 229 '\345' Keypad 5
    // 230 '\346' Keypad 6 and Right Arrow
    // 231 '\347' Keypad 7 and Home
    // 232 '\350' Keypad 8 and Up Arrow
    // 233 '\351' Keypad 9 and PageUp
    // 234 '\352' Keypad 0 and Insert
    // 235 '\353' Keypad . and Delete

    else if (thisElement == "NUM0") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(234);
    }
    else if (thisElement == "NUM1") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(225);
    }
    else if (thisElement == "NUM2") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(226);
    }
    else if (thisElement == "NUM3") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(227);
    }
    else if (thisElement == "NUM4") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(228);
    }
    else if (thisElement == "NUM5") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(229);
    }
    else if (thisElement == "NUM6") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(230);
    }
    else if (thisElement == "NUM7") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(231);
    }
    else if (thisElement == "NUM8") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(232);
    }
    else if (thisElement == "NUM9") {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
      Keyboard.press(233);
    }


    else {
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Incorrect length of Element");
      if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println(thisElement);
    }
  }




  //Key    Hexadecimal value   Decimal value
  //KEY_UP_ARROW  0xDA  218
  //KEY_DOWN_ARROW  0xD9  217
  //KEY_LEFT_ARROW  0xD8  216
  //KEY_RIGHT_ARROW   0xD7  215
  //KEY_BACKSPACE   0xB2  178
  //KEY_TAB   0xB3  179
  //KEY_RETURN  0xB0  176
  //KEY_ESC   0xB1  177
  //KEY_INSERT  0xD1  209
  //KEY_DELETE  0xD4  212
  //KEY_PAGE_UP   0xD3  211
  //KEY_PAGE_DOWN   0xD6  214
  //KEY_HOME  0xD2  210
  //KEY_END   0xD5  213
  //KEY_CAPS_LOCK   0xC1  193


  releaseKeysNeeded = true;
  timeBeforeReleaseAllKeys = millis() + delayBetweenRelease;


}
void loop() {

  if ((releaseKeysNeeded == true)  && (millis() >= timeBeforeReleaseAllKeys)) {

    Keyboard.releaseAll();

    if (Serial_In_Use == 1)  {
      Serial.println("Keys Released");
    }
    releaseKeysNeeded = false;
  }


 if (Test_MSG == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("APU TEST - 1");
      keyboardudp.endPacket();
           digitalWrite(APU_PORT, HIGH);
 }
       if (Test_MSG == 0) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("APU TEST - 0");
      keyboardudp.endPacket();
      digitalWrite(APU_PORT, HIGH);
    }
  // Handle Keyboard updates
  keyboardpacketSize = keyboardudp.parsePacket();
  keyboardLen = keyboardudp.read(keyboardpacketBuffer, 999);

  if (keyboardLen > 0) {
    keyboardpacketBuffer[keyboardLen] = 0;
  }
  if (keyboardpacketSize) {
    SendCharactersToKeyboard(keyboardLen);
  }

DcsBios::loop();

}
