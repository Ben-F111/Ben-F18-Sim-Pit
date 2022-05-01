/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */

#include "Keyboard.h" //keyboard library
#include <Keyboard.h>;

//char ctrlKey = KEY_LEFT_CTRL; //just in case

int PressCrank = 0; //count if crank has been pushed
int PressApu = 0; //count if APU has been pushed
int StateCrank = 1; //Crank switch position
int StateApu = 0; //APU switch position
byte SimApu = 0; // APU sim switch position (may not be in this position at start)

#define DCSBIOS_DEFAULT_SERIAL

#include "DcsBios.h"

/* paste code snippets from the reference documentation here */

//Crank switch position output
void onEngineCrankSwChange(unsigned int newValue) {
    pinMode (10, OUTPUT);
    if (newValue == 1){ //if crank switch in middle position
      digitalWrite(10, LOW); 
    }
    else{//if crank switch in left/right
      digitalWrite(10, HIGH); //5v to activate MOSFET and feed 12v to crank switch solenoid
    }
}
DcsBios::IntegerBuffer engineCrankSwBuffer(0x74c2, 0x0600, 9, onEngineCrankSwChange);

//---APU switch position outputjj
DcsBios::LED apuControlSw(0x74c2, 0x0100, 13); //added underscore to name. APU switch in pos 1, send 5v to MOSFET and 12v to APU solenoid. 

void onApuControlSwChange(unsigned int newValue) {
    SimApu = newValue;
}
DcsBios::IntegerBuffer apuControlSwBuffer(0x74c2, 0x0100, 8, onApuControlSwChange);

DcsBios::LED apuReadyLt(0x74c2, 0x0800, 12);



void setup() {
  pinMode(2, INPUT_PULLUP); //Connected to APU switch on
  pinMode(7, INPUT_PULLUP); //Connected to Crank switch left on
  pinMode(8, INPUT_PULLUP); //Connected to Crank switch right on
  Keyboard.begin(); //Emulate keyboard
  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();

/*if (digitalRead(7) == LOW){ //read switch state without debounce
  StateCrank = 0; //Crank switch is in left position
}

if (digitalRead(8) == LOW){//Crank switch is in right position
  StateCrank = 2;
}

if (digitalRead(7) == HIGH && digitalRead(8) == HIGH){//Crank switch is in center
  StateCrank = 1;
}
*/
if (digitalRead(2) == LOW){//APU switch is in on position
  StateApu = 1;
}

if (digitalRead(2) == HIGH) {//APU switch is in off position
  StateApu = 0;
}

/*if (StateCrank == 0 && PressCrank == 0){//Crank switch is moved to left on
  Keyboard.press('q'); //Crank LEFT bound to 'q' in sim
  delay(100);
  Keyboard.releaseAll(); //release keys
  PressCrank = 1; //Switch has been manually moved left/right
}

if (StateCrank == 2 && PressCrank == 0){//Crank switch is moved to right on
  Keyboard.press('p'); //Crank RIGHT bound to 'p' in sim
  delay(100);
  Keyboard.releaseAll(); //release keys
  PressCrank = 1; //Switch has been manually moved left/right
}

//test if we can detect switch being moved to off
if (StateCrank == 1 && PressCrank == 1){//Crank switch manually moved to center
  Keyboard.press('4'); //Crank OFF bound to '4' in sim
  delay(100);
  Keyboard.releaseAll(); //release keys
  PressCrank = 0; //Crank switch is no longer left/right
}

if (StateCrank == 1){//Crank switch released to center in DCS
  Keyboard.releaseAll(); //release keys
  PressCrank = 0; //Crank switch is no longer left/right
}
*/
if (StateApu == 1 && PressApu == 0){//APU switch moved to on
  if (SimApu == 0) {
    Keyboard.press('j'); //APU CYCLE ON/OFF bound to 'j' in sim
    delay(100);
    Keyboard.releaseAll(); //release keys
  }
  PressApu = 1; //APU switch has been manually moved to on
}

//test if we can detect switch being moved to off
if (StateApu == 0 && PressApu == 1){//APU switch manually moved to off
  if (SimApu == 1) {
    Keyboard.press('j');
    delay(100);
    Keyboard.releaseAll(); //release keys
  }
  PressApu = 0; //APU switch is no longer in on
}

if (StateApu == 0){//APU switch released to off in DCS
  Keyboard.releaseAll(); //release keys
  PressApu = 0;
}

}
