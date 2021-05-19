#include <Arduino.h>
#include "Nextion.h"
// parameters  page #, component .id, component .objname
NexDSButton myButton0 = NexDSButton(0,1,"bt0");
NexDSButton myButton1 = NexDSButton(0,2,"bt1");
NexDSButton myButton2 = NexDSButton(0,3,"bt2");
NexDSButton myButton3 = NexDSButton(0,4,"bt3"); 
NexDSButton myButton4 = NexDSButton(0,5,"bt4");
NexDSButton myButton5 = NexDSButton(0,6,"bt5"); 
NexDSButton myButton6 = NexDSButton(0,7,"bt6");
NexDSButton myButton7 = NexDSButton(0,8,"bt7"); 

uint32_t digitalstate0, digitalstate1, digitalstate2, digitalstate3,
         digitalstate4, digitalstate5, digitalstate6, digitalstate7 = 0;
int ledPin = 13;

NexTouch *nex_listen_list[] = {
  &myButton0, &myButton1, &myButton2, &myButton3,
  &myButton4, &myButton5, &myButton6, &myButton7,
  NULL
};

void myButton0ReleaseCallback(void *ptr) {
  digitalstate0 = 1-digitalstate0;
  if(digitalstate0==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}

void myButton1ReleaseCallback(void *ptr) {
  digitalstate1 = 1-digitalstate1;
  if(digitalstate1==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}

void myButton2ReleaseCallback(void *ptr) {
  digitalstate2 = 1-digitalstate2;
  if(digitalstate2==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}
void myButton3ReleaseCallback(void *ptr) {
  digitalstate3 = 1-digitalstate3;
  if(digitalstate3==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}
void myButton4ReleaseCallback(void *ptr) {
  digitalstate4 = 1-digitalstate4;
  if(digitalstate4==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}
void myButton5ReleaseCallback(void *ptr) {
  digitalstate5 = 1-digitalstate5;
  if(digitalstate5==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}
void myButton6ReleaseCallback(void *ptr) {
  digitalstate6 = 1-digitalstate6;
  if(digitalstate6==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}
void myButton7ReleaseCallback(void *ptr) {
  digitalstate7 = 1-digitalstate7;
  if(digitalstate7==1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
}

void setup(void) {
  pinMode(ledPin,OUTPUT);
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

