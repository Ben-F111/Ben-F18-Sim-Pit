#include <Nextion.h>          // Nextion Library
//#include <SoftwareSerial1.h>   // Library for Software Serial Port
#include <Keyboard.h>         // Keyboard Emulation
#include "NexTouch.h"
//SoftwareSerial HMISerial(0,11);
#define KEY_PAUSE (76+136)

NexButton myButton0 = NexButton(0, 1, "b0");
NexButton myButton1 = NexButton(0, 2, "b1");
NexButton myButton2 = NexButton(0, 3, "b2");
NexButton myButton3 = NexButton(0, 4, "b3");
NexButton myButton4 = NexButton(0, 5, "b4");
NexButton myButton5 = NexButton(0, 6, "b5");
NexButton myButton6 = NexButton(0, 7, "b6");
NexButton myButton7 = NexButton(0, 8, "b7");
NexButton myButton8 = NexButton(0, 9, "b8");
NexButton myButton9 = NexButton(0, 10, "b9");
NexButton myButton10 = NexButton(0, 11, "b10");
NexButton myButton11 = NexButton(0, 12, "b11");
NexButton myButton12 = NexButton(0, 13, "b12");
NexButton myButton13 = NexButton(0, 14, "b13");
NexButton myButton14 = NexButton(0, 15, "b14");
NexButton myButton15 = NexButton(0, 16, "b15");
NexButton myButton20 = NexButton(0, 17, "b20");
NexButton myButton21 = NexButton(0, 18, "b20");
// This part is used to list the possible touchscreen events in an Array
NexTouch *nex_listen_list[] = {
  &myButton0, &myButton1, &myButton2, &myButton3,
  &myButton4, &myButton5, &myButton6, &myButton7,
  &myButton8, &myButton9, &myButton10, &myButton11,
  &myButton12, &myButton13, &myButton14, &myButton15,
  &myButton20, &myButton21,
  NULL
};

void myButton0ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F1);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F1);
}

void myButton1ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F2);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F2);
}

void myButton2ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F3);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F3);
}
void myButton3ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F4);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F4);
}
void myButton4ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F5);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F5);
}
void myButton5ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F6);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F6);
}
void myButton6ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F7);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F7);
}
void myButton7ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F8);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F8);
}
void myButton8ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F9);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F9);
}
void myButton9ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F10);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F10);
}
void myButton10ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F11);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F11);
}
void myButton11ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F12);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F12);
}
void myButton12ReleaseCallback(void *ptr) {
  Keyboard.press('\\'); //atc comms
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release('\\');
}
void myButton13ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_LEFT_CTRL); //SALUTE
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('S');
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton14ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_F2);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
void myButton15ReleaseCallback(void *ptr) {
Keyboard.press(KEY_PAUSE);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_PAUSE);
}
void myButton20ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_F5);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_F5);
}
void myButton21ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.release(KEY_F1);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.releaseAll();
}
/*
  void myButton17ReleaseCallback(void *ptr) {
  Keyboard.press(KEY_XX);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_XX);
  }
  /*
   void myButtonXReleaseCallback(void *ptr) {
  Keyboard.press(KEY_XX);
  delay(250);  //  Delay of 250ms is needed for Mame to register the key press
  Keyboard.release(KEY_XX);
  }/*
*/

// Main Setup
void setup() {

  Serial1.begin(9600);  // Start Software Serial at 9600bauds

  nexInit();  //  Nextion Display initalize
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
  myButton13.attachPop(myButton13ReleaseCallback, &myButton14);
  myButton13.attachPop(myButton13ReleaseCallback, &myButton15);
  myButton13.attachPop(myButton13ReleaseCallback, &myButton20);
  myButton13.attachPop(myButton13ReleaseCallback, &myButton21);
  Keyboard.begin();  // Start Keyboard emulation
  {
    Serial1.print("b0.pic=1");
    Serial1.write("\xFF\xFF\xFF");
  }
  delay(1000);
  {
    Serial1.print("b0.pic=0");
    Serial1.write("\xFF\xFF\xFF");
  }

  //set brirghtness
  Serial1.print("dim=0");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(500);
  Serial1.print("dim=50");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(500);
  Serial1.print("dim=100");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(500);

}
// Main Loop
void loop() {

  nexLoop(nex_listen_list);  // Check for any touch event and run the associated function

}
