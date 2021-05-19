#include <Arduino.h>
 #include "Nextion.h"

 // parameters  page #, component .id, component .objname

NexButton myButton0 = NexButton(0,17, "b0");

 uint32_t digitalstate11 = 0; 
         
 const int relay1Pin = 13;
 
NexTouch *nex_listen_list[] = {
  &myButton0, 
   NULL
 };
 void myButton0ReleaseCallback(void *ptr) {
   digitalstate11 = 1-digitalstate11;
   if(digitalstate11==1){
   digitalWrite(relay1Pin, HIGH);
   }else{
      digitalWrite(relay1Pin, LOW);
   }  
 }

 void setup(void) {
 pinMode(relay1Pin,OUTPUT);
   nexInit();
   myButton0.attachPop(myButton0ReleaseCallback, &myButton0);
 }

 void loop(void) {

   nexLoop(nex_listen_list);
 }


