// UPDATED TO DSCS-BIOS FP EDITION FOR OPEN HORNET


////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
//||               FUNCTION = KEY STROKE - NO EXTERNAL CONNECTION     ||\\
//||              LOCATION IN THE PIT = UIP RIGHT SIDE                ||\\
//||            ARDUINO PROCESSOR TYPE = Arduino Due                  ||\\
//||      ARDUINO CHIP SERIAL NUMBER = SN - 859373138373516121E2      ||\\
//||            PROGRAM PORT CONNECTED COM PORT = COM 11 PROGRAM      ||\\
//||            PROGRAM PORT CONNECTED COM PORT = COM 14 NATIVE       ||\\
//||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\

// Tell DCS-BIOS to use a serial connection and use interrupt-driven
// communication. The main program will be interrupted to prioritize
// processing incoming data.
//
// This should work on any Arduino that has an ATMega328 controller
// (Uno, Pro Mini, many others).


/*
    IF PORT WON'T PROGRAM JUST TRY OTHER PORT - NATIVE PORT SEEMS TO BE MORE RELIABLE
    ARE SEEING ISSUE WITH THIS UNIT NOT RELIABLY BRINGING UP ETHERNET

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


#define ProgramVersion 11

String readString;
#include <Servo.h>


#include "LedControl.h"



int Ethernet_In_Use = 1;            // Check to see if jumper is present - if it is disable Ethernet calls. Used for Testing
#define Reflector_In_Use 1
#define Serial_In_Use 0

// Ethernet Related
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte myMac[] = {0xA8, 0x61, 0x0A, 0x9E, 0x83, 0x6E};
IPAddress myIP(172, 16, 1, 110);
String strMyIP = "172.16.1.110  ";

// Reflector
IPAddress reflectorIP(172, 16, 1, 10);
String strReflectorIP = "172.16.1.10";


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

char trimpacketBuffer[1000];                //buffer to store trim data
char keyboardpacketBuffer[1000];            //buffer to store keyboard data






#define RED_STATUS_LED_PORT 5                 // RED LED is used for monitoring ethernet
#define GREEN_STATUS_LED_PORT 13              // RED LED is used for monitoring ethernet
#define FLASH_TIME 1000
unsigned long NEXT_STATUS_TOGGLE_TIMER = 0;
bool GREEN_LED_STATE = false;
bool RED_LED_STATE = false;
unsigned long timeSinceRedLedChanged = 0;


bool Debug_Display = false;
bool KEYBOARD_INITIALISED = false;
#define DELAY_BEFORE_INITALISING_KEYBOARD 180000   // Number of milliseconds before attempting to initalise keyboard - need PC booted 


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





// **************** Begin Keyboard area *************************


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

  if (Debug_Display || bLocalDebug ) SendIPDebug("Packet Received");
  if (Debug_Display || bLocalDebug ) SendIPDebug("Len is ");
  if (Debug_Display || bLocalDebug ) SendIPDebug(String((packetLength)));

  if (Reflector_In_Use == 1) {
    keyboardudp.beginPacket(reflectorIP, reflectorport);
    keyboardudp.println("Keyboard Packet Received");
    keyboardudp.endPacket();
  }

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
        if (Debug_Display || bLocalDebug ) SendIPDebug("Left Alt in Use");
      }
      else if (thisElement == "RALT") {
        rightAltInUse = true;
        if (Debug_Display || bLocalDebug ) SendIPDebug("Right Alt in Use");
      }
      else if (thisElement == "ALT") {
        altInUse = true;
        if (Debug_Display || bLocalDebug )  SendIPDebug("Alt in Use");
      }
      else if (thisElement == "CTRL") {
        ctrlInUse = true;
        if (Debug_Display || bLocalDebug ) SendIPDebug("Right Alt in Use");
      }
      else if (thisElement == "SHIFT") {
        shiftInUse = true;
        if (Debug_Display || bLocalDebug )  SendIPDebug("Shift in Use");
      }
      else if (thisElement == "LSHIFT") {
        lShiftInUse = true;
        if (Debug_Display || bLocalDebug )  SendIPDebug("Left Shift in Use");
      }
      else if (thisElement == "RSHIFT") {
        rShiftInUse = true;
        if (Debug_Display || bLocalDebug ) SendIPDebug("Right Shift in Use");
      }
      else if (thisElement == "LCTRL") {
        lCtrlInUse = true;
        if (Debug_Display || bLocalDebug )  SendIPDebug("Left Controlin Use");
      }
      else if (thisElement == "RCTRL") {
        rCtrlInUse = true;
        if (Debug_Display || bLocalDebug )  SendIPDebug("Right Control in Use");
      }
      else if (thisElement == "LWIN") {
        lWinInUse = true;
        if (Debug_Display || bLocalDebug ) SendIPDebug("Left Windows in Use");
      }
      else if (thisElement == "RWIN") {
        rWinInUse = true;
        if (Debug_Display || bLocalDebug )  SendIPDebug("Right Windows in Use");
      }
      thisElement = "";
    }

    else {
      thisElement = thisElement + String(keyboardpacketBuffer[characterPtr]);
    }

  }
  if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);




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
    if (Debug_Display || bLocalDebug ) SendIPDebug("Found trailing CR - removing it");
    thisElement.remove(thisElement.length() - 1);
  }




  if (thisElement.length() == 1) {
    // We are hitting a single character to send
    if (Debug_Display || bLocalDebug ) SendIPDebug("Correct length of Element - Sending");
    if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
    thisElement.toCharArray(keyToPress, 2);

    Keyboard.press(keyToPress[0]);

  } else
  {
    // Function Keys
    if (thisElement == "F1") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F1);
    }
    else if (thisElement == "F2") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(KEY_F2);
    }
    else if (thisElement == "F3") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F3);
    }
    else if (thisElement == "F4") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(KEY_F4);
    }
    else if (thisElement == "F5") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F5);
    }
    else if (thisElement == "F6") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F6);
    }
    else if (thisElement == "F7") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F7);
    }
    else if (thisElement == "F8") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F8);
    }
    else if (thisElement == "F9") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(KEY_F9);
    }
    else if (thisElement == "F10") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(KEY_F10);
    }
    else if (thisElement == "F11") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(KEY_F11);
    }
    else if (thisElement == "F12") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
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
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(220);
    }
    else if (thisElement == "KEYPAD*") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "KEYPAD-") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "KEYPAD+") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "KEYPADENTER") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(233);
    }
    else if (thisElement == "ESC") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
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
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(234);
    }
    else if (thisElement == "NUM1") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(225);
    }
    else if (thisElement == "NUM2") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(226);
    }
    else if (thisElement == "NUM3") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(227);
    }
    else if (thisElement == "NUM4") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(228);
    }
    else if (thisElement == "NUM5") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(229);
    }
    else if (thisElement == "NUM6") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(230);
    }
    else if (thisElement == "NUM7") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(231);
    }
    else if (thisElement == "NUM8") {
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
      Keyboard.press(232);
    }
    else if (thisElement == "NUM9") {
      if (Debug_Display || bLocalDebug )  SendIPDebug(thisElement);
      Keyboard.press(233);
    }


    else {
      if (Debug_Display || bLocalDebug ) SendIPDebug("Incorrect length of Element");
      if (Debug_Display || bLocalDebug ) SendIPDebug(thisElement);
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


// ************* End Keyboard Area *****************************


void SendIPDebug( String DebugToSend) {
  if (Reflector_In_Use == 1) {
    senderudp.beginPacket(reflectorIP, reflectorport);
    senderudp.print(DebugToSend);
    senderudp.endPacket();
  }
}


void UpdateRedStatusLed() {
  if ((RED_LED_STATE == false) && (millis() >= (timeSinceRedLedChanged + FLASH_TIME ) )) {
    digitalWrite( RED_STATUS_LED_PORT, true);
    RED_LED_STATE = true;
    timeSinceRedLedChanged = millis();

  }
}

void CHECK_KEYBOARD_INITIALISED() {

  if (KEYBOARD_INITIALISED != true) {

    if (Reflector_In_Use == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("Init Keyboard Start" + String(millis()) + "mS since reset.");
      keyboardudp.endPacket();
    }

    Keyboard.begin();

    if (Reflector_In_Use == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("Init Keyboard Complete" + String(millis()) + "mS since reset.");
      keyboardudp.endPacket();
    }

    KEYBOARD_INITIALISED = true;

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



  Ethernet.begin( myMac, myIP);
  keyboardudp.begin(keyboardport);
  senderudp.begin(ledport);

  // Let Ethernet Settle
  delay(EthernetStartupDelay);

  if (Reflector_In_Use == 1) {
    keyboardudp.beginPacket(reflectorIP, reflectorport);
    keyboardudp.println("Init UDP to Keyboard - version:" + String(ProgramVersion) + " " + strMyIP + " " + String(millis()) + "mS since reset.");
    keyboardudp.endPacket();
  }



  NEXT_STATUS_TOGGLE_TIMER = millis() + 1000;


  if (Reflector_In_Use == 1) {
    keyboardudp.beginPacket(reflectorIP, reflectorport);
    keyboardudp.println("Exiting Setup");
    keyboardudp.endPacket();
  }

}


void loop() {

  if ((millis() >= DELAY_BEFORE_INITALISING_KEYBOARD) && (KEYBOARD_INITIALISED != true)) {

    if (Reflector_In_Use == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("Init Keyboard Start" + String(millis()) + "mS since reset.");
      keyboardudp.endPacket();
    }

    Keyboard.begin();

    if (Reflector_In_Use == 1) {
      keyboardudp.beginPacket(reflectorIP, reflectorport);
      keyboardudp.println("Init Keyboard Complete" + String(millis()) + "mS since reset.");
      keyboardudp.endPacket();
    }

    KEYBOARD_INITIALISED = true;

  }

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



  // ****************** Begin Keyboard Loop Area *******************
  if ((releaseKeysNeeded == true)  && (millis() >= timeBeforeReleaseAllKeys)) {

    Keyboard.releaseAll();

    if (Serial_In_Use == 1)  {
      SendIPDebug("Keys Released");
    }
    releaseKeysNeeded = false;
  }




  // Handle Keyboard updates
  keyboardpacketSize = keyboardudp.parsePacket();
  keyboardLen = keyboardudp.read(keyboardpacketBuffer, 999);

  if (keyboardLen > 0) {
    keyboardpacketBuffer[keyboardLen] = 0;
  }
  if (keyboardpacketSize) {
    CHECK_KEYBOARD_INITIALISED();
    SendCharactersToKeyboard(keyboardLen);
  }

  // ****************** End Keyboard Loop Area *******************


}
