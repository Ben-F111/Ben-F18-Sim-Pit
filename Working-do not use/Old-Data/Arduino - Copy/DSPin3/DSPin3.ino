#include <Arduino.h>
 #include "Nextion.h"

 // parameters  page #, component .id, component .objname

NexButton myButton0 = NexButton(0,17, "b0");
NexButton myButton1 = NexButton(0,18, "b1");
 uint32_t digitalstate11, digitalstate12 = 0;     

 const int relay1Pin = 13;
 byte pump1;
 byte pump2;

NexTouch *nex_listen_list[] = {
  &myButton0, &myButton1, 
   NULL
 };

 void myButton0ReleaseCallback(void *ptr) {
   digitalstate11 = 1-digitalstate11;
   if(digitalstate11==1){
   digitalWrite(relay1Pin, HIGH);
    }
 }
  void myButton1ReleaseCallback(void *ptr) {
  digitalstate12 = 1-digitalstate12;
 if(digitalstate12==1){
 digitalWrite(relay1Pin, LOW);

 }
  }
 void setup(void) {
 pinMode(relay1Pin,OUTPUT);
   nexInit();
   myButton0.attachPop(myButton0ReleaseCallback, &myButton0);
   myButton1.attachPop(myButton1ReleaseCallback, &myButton1);
 }
 
 void loop(void) {
   nexLoop(nex_listen_list);
 
 }


