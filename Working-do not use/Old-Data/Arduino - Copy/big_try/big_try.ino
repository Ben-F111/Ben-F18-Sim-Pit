#include <Arduino.h>
 #include "Nextion.h"
 // parameters  page #, component .id, component .objname
 NexButton myButton0 = NexButton(0,1,"b0");
 NexButton myButton1 = NexButton(0,2,"b1");
NexButton myButton2 = NexButton(0,3,"b2");
NexButton myButton3 = NexButton(0,4,"b3");
 NexButton myButton4 = NexButton(0,5,"b4");
 NexButton myButton5 = NexButton(0,6,"b5");
NexButton myButton6 = NexButton(0,7,"b6");
NexButton myButton7 = NexButton(0,8,"b7");

 uint32_t digitalstate0, digitalstate1, digitalstate2, digitalstate3,
          digitalstate4, digitalstate5, digitalstate6, digitalstate7 = 0;
 const int Pin1 = 13;
 const int Pin2 = 12;
 const int Pin3 = 11;
 const int Pin4 = 10;
 const int Pin5 = 9;
 const int Pin6 = 8;
 const int Pin7 = 7;
 const int Pin8 = 6;

 
 NexTouch *nex_listen_list[] = {
   &myButton0, &myButton1, &myButton2, &myButton3,
   &myButton4, &myButton5, &myButton6, &myButton7,
   NULL
 };

 void myButton0ReleaseCallback(void *ptr) {
   digitalstate0 = 1-digitalstate0;
   if(digitalstate0==1) {
     digitalWrite(Pin1, HIGH);
   } else {
     digitalWrite(Pin1, LOW);
   }
 }

 void myButton1ReleaseCallback(void *ptr) {
   digitalstate1 = 1-digitalstate1;
   if(digitalstate1==1) {
     digitalWrite(Pin2, HIGH);
   } else {
     digitalWrite(Pin2, LOW);
   }
 }

 void myButton2ReleaseCallback(void *ptr) {
   digitalstate2 = 1-digitalstate2;
   if(digitalstate2==1) {
     digitalWrite(Pin3, HIGH);
   } else {
     digitalWrite(Pin3, LOW);
   }
 }
 void myButton3ReleaseCallback(void *ptr) {
   digitalstate3 = 1-digitalstate3;
   if(digitalstate3==1) {
     digitalWrite(Pin4, HIGH);
   } else {
     digitalWrite(Pin4, LOW);
   }
 }
 void myButton4ReleaseCallback(void *ptr) {
   digitalstate4 = 1-digitalstate4;
   if(digitalstate4==1) {
     digitalWrite(Pin5, HIGH);
   } else {
     digitalWrite(Pin5, LOW);
   }
 }
 void myButton5ReleaseCallback(void *ptr) {
   digitalstate5 = 1-digitalstate5;
   if(digitalstate5==1) {
     digitalWrite(Pin6, HIGH);
   } else {
     digitalWrite(Pin6, LOW);
   }
 }
 void myButton6ReleaseCallback(void *ptr) {
   digitalstate6 = 1-digitalstate6;
   if(digitalstate6==1) {
     digitalWrite(Pin7, HIGH);
   } else {
     digitalWrite(Pin7, LOW);
   }
 }
 void myButton7ReleaseCallback(void *ptr) {
   digitalstate7 = 1-digitalstate7;
   if(digitalstate7==1) {
     digitalWrite(Pin8, HIGH);
   } else {
     digitalWrite(Pin8, LOW);
   }
 }

 void setup(void) {
   pinMode(Pin1,OUTPUT);
   pinMode(Pin2,OUTPUT);
   pinMode(Pin3,OUTPUT);
   pinMode(Pin4,OUTPUT);
   pinMode(Pin5,OUTPUT);
   pinMode(Pin6,OUTPUT);
   pinMode(Pin7,OUTPUT);
   pinMode(Pin8,OUTPUT);

   nexInit();
   myButton0.attachPop(myButton0ReleaseCallback, &myButton0);
   myButton1.attachPop(myButton1ReleaseCallback, &myButton1);
   myButton2.attachPop(myButton2ReleaseCallback, &myButton2);
   myButton3.attachPop(myButton3ReleaseCallback, &myButton3);
   myButton4.attachPop(myButton4ReleaseCallback, &myButton4);
   myButton5.attachPop(myButton5ReleaseCallback, &myButton5);
   myButton6.attachPop(myButton6ReleaseCallback, &myButton6);
   myButton7.attachPop(myButton7ReleaseCallback, &myButton7);
 }

 void loop(void) {
   nexLoop(nex_listen_list);
 }


