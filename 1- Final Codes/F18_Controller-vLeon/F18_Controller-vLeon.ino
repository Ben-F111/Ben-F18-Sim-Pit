


#include <Nextion.h>          // Nextion Library
//#include <SoftwareSerial1.h>   // Library for Software Serial Port
#include <Keyboard.h>         // Keyboard Emulation
#include "NexTouch.h"
#include <Mouse.h>
//SoftwareSerial HMISerial(0,11);


int horzPin = A0;  // Analog output of horizontal joystick pin
int vertPin = A1;  // Analog output of vertical joystick pin
int selPin = 7;  // select button pin of joystick

int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
const int sensitivity = 500;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouseClickFlag = 0;

//int invertMouse = 1;        //Invert joystick based on orientation
int invertMouse = -1;         //Noninverted joystick based on orientation
#define KEY_PAUSE (76+136)

// - MATCHES NEXTION VERSION "SIM_CONTROL_V2.0.HMI"


//PAGE - MENU
NexButton myButtonM0 = NexButton(1, 1, "b0"); //START/FLY

//PAGE - KEYPAD MAP
// B0 - NOT USED INTERNAL "MAIN MENU"
NexButton myButton0 = NexButton(2, 2, "b1"); //F1
NexButton myButton1 = NexButton(2, 3, "b2"); //F2
NexButton myButton2 = NexButton(2, 4, "b3"); //F3
NexButton myButton3 = NexButton(2, 5, "b4"); //F4
NexButton myButton4 = NexButton(2, 6, "b5"); //F5
NexButton myButton5 = NexButton(2, 7, "b6"); //F6
NexButton myButton6 = NexButton(2, 8, "b7"); //F7
NexButton myButton7 = NexButton(2, 9, "b8"); //F8
NexButton myButton8 = NexButton(2, 10, "b9"); //F9
NexButton myButton9 = NexButton(2, 11, "b10"); //F10
NexButton myButton10 = NexButton(2, 121, "b11"); //F11
NexButton myButton11 = NexButton(2, 13, "b12"); //F12
NexButton myButton12 = NexButton(2, 14, "b13"); //LABELS TOGGLE
NexButton myButton13 = NexButton(2, 15, "b14"); //ATC "\"
NexButton myButton14 = NexButton(2, 16, "b15"); // SALUTE
NexButton myButton15 = NexButton(2, 17, "b16"); //COCKPIT "LEFT ALT F1"
NexButton myButton16 = NexButton(2, 18, "b17"); //SNAP VIEW
NexButton myButton17 = NexButton(2, 19, "b18"); //GROUND CREW
NexButton myButton18 = NexButton(2, 20, "b19"); // PAUSE KEY
NexButton myButton19 = NexButton(2, 21, "b20"); // EXIT 'ECS KEY'
NexButton myButton20 = NexButton(2, 22, "b21"); // AUTO START (BM TO ASIGN)

// COMPUTER CONTROL
//NexButton myButtonC0 = NexButton(4, 1, "b0"); //VOLUME UP  // VOL not working yet
//NexButton myButtonC1 = NexButton(4, 5, "b4"); //VOLUME MUTE  // VOL not working yet
//NexButton myButtonC2 = NexButton(4, 3, "b2"); //VOLUME DOWN  // VOL not working yet
//NexButton myButtonTAB = NexButton(4, 7, "b7");
//NexButton myButtonENT = NexButton(4, 9, "b5");
//NexButton myButtonAE = NexButton(4, 10, "b6");
//NexButton myButtonWIN = NexButton(4, 11, "b8");

// VIEWS
NexButton myButtonIN = NexButton(5, 9, "b0"); //IN
NexButton myButtonOUT = NexButton(5, 11, "b4"); //OUT
NexButton myButtonUP = NexButton(5, 4, "b9"); //UP
NexButton myButtonLF = NexButton(5, 8, "b13"); //LEFT
NexButton myButtonCN = NexButton(5, 5, "b10"); //CENTRE
NexButton myButtonDN = NexButton(5, 6, "b11"); //DOWN
NexButton myButtonRS = NexButton(5, 10, "b2"); //RESET

NexButton myButtonTL = NexButton(5, 12, "b5"); //TOP LEFT "7"
NexButton myButtonTR = NexButton(5, 13, "b6"); //TOP RIGHT "9"
NexButton myButtonBL = NexButton(5, 14, "b7"); //BOT LEFT "1"
NexButton myButtonBR = NexButton(5, 15, "b8"); //BOT RIGHT "3"


// This part is used to list the possible touchscreen events in an Array
NexTouch *nex_listen_list[] = {

  //MENU
  &myButtonM0,
  //KEYPAD
  &myButton0, &myButton1, &myButton2, &myButton3,
  &myButton4, &myButton5, &myButton6, &myButton7,
  &myButton8, &myButton9, &myButton10, &myButton11,
  &myButton12, &myButton13, &myButton14, &myButton15,
  &myButton16, &myButton17, &myButton18, &myButton19,
  &myButton20,

  // COMP CONTROL
  // &myButtonC0,  &myButtonC1,  &myButtonC2, // VOL not working yet
  //  &myButtonTAB, &myButtonENT, &myButtonAE, &myButtonWIN,

  // VIEWS
  &myButtonIN, &myButtonOUT, &myButtonUP, &myButtonLF,
  &myButtonCN, &myButtonDN, &myButtonRS,

   &myButtonTL, &myButtonTR, &myButtonBL, &myButtonBR,



  NULL
};

//MENU
void myButtonM0ReleaseCallback(void *ptr) {
  Keyboard.press(208);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}

//KEYPAD
void myButton0ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F1);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton1ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F2);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton2ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F3);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton3ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F4);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton4ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F5);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton5ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F6);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton6ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F7);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton7ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F8);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton8ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F9);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton9ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F10);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton10ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F11);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton11ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F12);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton12ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_LEFT_SHIFT); // toggle lables
  delay(10);
  Keyboard.press(KEY_F2);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton13ReleaseCallback(void *ptr) {
  Keyboard.press('\\'); //atc comms
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll(); \
}
void myButton14ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_LEFT_CTRL); //SALUTE
  delay(10);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('S');
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton15ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_LEFT_ALT);
  delay(10);
  Keyboard.press(KEY_F1);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F1);
  Keyboard.release(KEY_LEFT_ALT);

}
void myButton16ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F5);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton17ReleaseCallback(void *ptr) {
  Keyboard.press('\\');
  delay(10);
  Keyboard.press(KEY_F8);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton18ReleaseCallback(void *ptr) {
  Keyboard.press(208);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton19ReleaseCallback(void *ptr) {
  Keyboard.press(177);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(177);
}
void myButton20ReleaseCallback(void *ptr) {
  Keyboard.press(131);
  delay(10);
  Keyboard.press(210);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}

// COMPUTER CONTROL // currently not in use
/*
  void myButtonC0ReleaseCallback(void *ptr) {
  //Keyboard.press(243);
  Keyboard.press(0x80);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
  void myButtonC1ReleaseCallback(void *ptr) {
  Keyboard.press(129 + 0x81);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
  void myButtonC2ReleaseCallback(void *ptr) {
  Keyboard.press(179);
  //  Keyboard.press(245);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }

  void myButtonTABReleaseCallback(void *ptr) {
  Keyboard.press(179);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
  void myButtonENTReleaseCallback(void *ptr) {
  Keyboard.press(176);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
  void myButtonAEReleaseCallback(void *ptr) {
   Keyboard.press(130);

  delay(250);  //  Delay of 250ms is needed for Mame to register the key press

      Keyboard.releaseAll();
  }
  void myButtonWINReleaseCallback(void *ptr) {
    Keyboard.press(131);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
*/

// VIEWS
  void myButtonINReleaseCallback(void *ptr) {
  Keyboard.press(221);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
    void myButtonOUTReleaseCallback(void *ptr) {
  Keyboard.press(220);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
    void myButtonUPReleaseCallback(void *ptr) {
  Keyboard.press(232);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
    void myButtonLFReleaseCallback(void *ptr) {
  Keyboard.press(228);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
    void myButtonCNReleaseCallback(void *ptr) {
  Keyboard.press(229);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
    void myButtonDNReleaseCallback(void *ptr) {
  Keyboard.press(226);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
      void myButtonRSReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F1);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
        void myButtonTLReleaseCallback(void *ptr) {
  Keyboard.press(231);
 // delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  }
      void myButtonTRReleaseCallback(void *ptr) {
  Keyboard.press(233);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
      void myButtonBLReleaseCallback(void *ptr) {
  Keyboard.press(225);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }
      void myButtonBRReleaseCallback(void *ptr) {
  Keyboard.press(227);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
  }



// Main Setup
void setup() {

  Serial1.begin(9600);  // Start Software Serial at 9600bauds
  Keyboard.releaseAll();
  nexInit();  //  Nextion Display initalize

  myButtonM0.attachPop(myButtonM0ReleaseCallback, &myButtonM0);

  myButton0.attachPop(myButton0ReleaseCallback, &myButton0);
  myButton1.attachPop(myButton1ReleaseCallback, &myButton1);
  myButton2.attachPop(myButton2ReleaseCallback, &myButton2);
  myButton3.attachPop(myButton3ReleaseCallback, &myButton3);
  myButton4.attachPop(myButton4ReleaseCallback, &myButton4);
  myButton5.attachPop(myButton5ReleaseCallback, &myButton5);
  myButton6.attachPop(myButton6ReleaseCallback, &myButton6);
  myButton7.attachPop(myButton7ReleaseCallback, &myButton7);
  myButton8.attachPop(myButton8ReleaseCallback, &myButton8);
  myButton9.attachPop(myButton9ReleaseCallback, &myButton9);
  myButton10.attachPop(myButton10ReleaseCallback, &myButton10);
  myButton11.attachPop(myButton11ReleaseCallback, &myButton11);
  myButton12.attachPop(myButton12ReleaseCallback, &myButton12);
  myButton13.attachPop(myButton13ReleaseCallback, &myButton13);
  myButton14.attachPop(myButton14ReleaseCallback, &myButton14);
  myButton15.attachPop(myButton15ReleaseCallback, &myButton15);
  myButton16.attachPop(myButton16ReleaseCallback, &myButton16);
  myButton17.attachPop(myButton17ReleaseCallback, &myButton17);
  myButton18.attachPop(myButton18ReleaseCallback, &myButton18);
  myButton19.attachPop(myButton19ReleaseCallback, &myButton19);
  myButton20.attachPop(myButton20ReleaseCallback, &myButton20);

  //myButtonC0.attachPop(myButtonC0ReleaseCallback, &myButtonC0); // VOL not working yet
  //myButtonC1.attachPop(myButtonC1ReleaseCallback, &myButtonC1); // VOL not working yet
  //myButtonC2.attachPop(myButtonC2ReleaseCallback, &myButtonC2); // VOL not working yet

  // myButtonTAB.attachPop(myButtonTABReleaseCallback, &myButtonTAB);
  // myButtonENT.attachPop(myButtonENTReleaseCallback, &myButtonENT);
  // myButtonAE.attachPop(myButtonAEReleaseCallback, &myButtonAE);
  // myButtonWIN.attachPop(myButtonWINReleaseCallback, &myButtonWIN);

myButtonIN.attachPop(myButtonINReleaseCallback, &myButtonIN);
myButtonOUT.attachPop(myButtonOUTReleaseCallback, &myButtonOUT);
myButtonUP.attachPop(myButtonUPReleaseCallback, &myButtonUP);
myButtonLF.attachPop(myButtonLFReleaseCallback, &myButtonLF);
myButtonCN.attachPop(myButtonCNReleaseCallback, &myButtonCN);
myButtonDN.attachPop(myButtonDNReleaseCallback, &myButtonDN);
myButtonRS.attachPop(myButtonRSReleaseCallback, &myButtonRS);
myButtonTR.attachPop(myButtonTRReleaseCallback, &myButtonTR);
myButtonTL.attachPop(myButtonTLReleaseCallback, &myButtonTL);
myButtonBL.attachPop(myButtonBLReleaseCallback, &myButtonBL);
myButtonBR.attachPop(myButtonBRReleaseCallback, &myButtonBR);

 

  Keyboard.begin();  // Start Keyboard emulation

  /*
    pinMode(horzPin, INPUT);  // Set both analog pins as inputs
    pinMode(vertPin, INPUT);
    pinMode(selPin, INPUT);  // set button select pin as input
    digitalWrite(selPin, HIGH);  // Pull button select pin high
    delay(1000);  // short delay to let outputs settle
    vertZero = analogRead(vertPin);  // get the initial values
    horzZero = analogRead(horzPin);  // Joystick should be in neutral position when reading these
    //  Mouse.begin(); //Init mouse emulation
  */



  Serial1.print("dim=100");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(500);

}
// Main Loop
void loop() {

  nexLoop(nex_listen_list);  // Check for any touch event and run the associated function




  /*vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
    horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

    if (vertValue != 0)
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
    if (horzValue != 0)
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis

    if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
    {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT);  // click the left button down
    }
    else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
    {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_LEFT);  // release the left button
    }*/
}


/*

  Key Hexadecimal value Decimal value
  KEY_LEFT_CTRL 0x80  128
  KEY_LEFT_SHIFT  0x81  129
  KEY_LEFT_ALT  0x82  130
  KEY_LEFT_GUI  0x83  131
  KEY_RIGHT_CTRL  0x84  132
  KEY_RIGHT_SHIFT 0x85  133
  KEY_RIGHT_ALT 0x86  134
  KEY_RIGHT_GUI 0x87  135
  KEY_UP_ARROW  0xDA  218
  KEY_DOWN_ARROW  0xD9  217
  KEY_LEFT_ARROW  0xD8  216
  KEY_RIGHT_ARROW 0xD7  215
  KEY_BACKSPACE 0xB2  178
  KEY_TAB 0xB3  179
  KEY_RETURN  0xB0  176
  KEY_ESC 0xB1  177
  KEY_INSERT  0xD1  209
  KEY_DELETE  0xD4  212
  KEY_PAGE_UP 0xD3  211
  KEY_PAGE_DOWN 0xD6  214
  KEY_HOME  0xD2  210
  KEY_END 0xD5  213
  KEY_CAPS_LOCK 0xC1  193
  KEY_F1  0xC2  194
  KEY_F2  0xC3  195
  KEY_F3  0xC4  196
  KEY_F4  0xC5  197
  KEY_F5  0xC6  198
  KEY_F6  0xC7  199
  KEY_F7  0xC8  200
  KEY_F8  0xC9  201
  KEY_F9  0xCA  202
  KEY_F10 0xCB  203
  KEY_F11 0xCC  204
  KEY_F12 0xCD  205
*/


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
