#include <Nextion.h>          // Nextion Library
//#include <SoftwareSerial3.h>   // Library for Software Serial3 Port
//#include <Keyboard.h>         // Keyboard Emulation
#include "NexTouch.h"
//SoftwareSerial3 HMISerial3(14,15);  // Nextion TX to pin 10 and RX to pin 11 of Arduino

NexButton myButton0 = NexButton(0,1,"b0");
NexButton myButton1 = NexButton(0,2,"b1");
NexButton myButton2 = NexButton(0,3,"b2");
NexButton myButton3 = NexButton(0,4,"b3");
NexButton myButton4 = NexButton(0,5,"b4");
NexButton myButton5 = NexButton(0,6,"b5");
NexButton myButton6 = NexButton(0,7,"b6");
NexButton myButton7 = NexButton(0,8,"b7");

// This part is used to list the possible touchscreen events in an Array
 NexTouch *nex_listen_list[] = {
   &myButton0, &myButton1, &myButton2, &myButton3,
   &myButton4, &myButton5, &myButton6, &myButton7,
   NULL
 };

void myButton0ReleaseCallback(void *ptr) {
    Serial3.print("dim=0");
  Serial3.write(0xff);
  Serial3.write(0xff);
 Serial3.write(0xff);

 }

 void myButton1ReleaseCallback(void *ptr) {
    Serial3.print("dim=50");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

 }

 void myButton2ReleaseCallback(void *ptr) {
    Serial3.print("dim=100");
 Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

 }
 void myButton3ReleaseCallback(void *ptr) {

 }
 void myButton4ReleaseCallback(void *ptr) {

 }
 void myButton5ReleaseCallback(void *ptr) {

 }
 void myButton6ReleaseCallback(void *ptr) {
  
 }
 void myButton7ReleaseCallback(void *ptr) {
 
 }

// Main Setup 
void setup() {
  
  Serial3.begin(9600);  // Start Software Serial3 at 9600bauds
    
  nexInit();  //  Nextion Display initalize
   myButton0.attachPop(myButton0ReleaseCallback, &myButton0);
   myButton1.attachPop(myButton1ReleaseCallback, &myButton1);
   myButton2.attachPop(myButton2ReleaseCallback, &myButton2);
   myButton3.attachPop(myButton3ReleaseCallback, &myButton3);
   myButton4.attachPop(myButton4ReleaseCallback, &myButton4);
   myButton5.attachPop(myButton5ReleaseCallback, &myButton5);
   myButton6.attachPop(myButton6ReleaseCallback, &myButton6);
   myButton7.attachPop(myButton7ReleaseCallback, &myButton7);

 // Keyboard.begin();  // Start Keyboard emulation
{
 Serial3.print("b0.pic=1");
   Serial3.write("\xFF\xFF\xFF");
  }
  delay(1000);
{
 Serial3.print("b0.pic=0");
   Serial3.write("\xFF\xFF\xFF");
  }

       //set brirghtness
  Serial3.print("dim=0");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
  delay(500);
   Serial3.print("dim=50");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
  delay(500);
   Serial3.print("dim=100");
  Serial3.write(0xff);
  Serial3.write(0xff);
 Serial3.write(0xff);
  delay(500);

}
// Main Loop
void loop() {

   nexLoop(nex_listen_list);  // Check for any touch event and run the associated function
 
}
