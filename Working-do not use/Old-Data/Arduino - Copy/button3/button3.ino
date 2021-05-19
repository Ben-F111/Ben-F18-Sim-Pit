#include <Arduino.h>
#include "Nextion.h"
#include "NexTouch.h"
// parameters  page #, component .id, component .objname

//NexText bt0 = NexText(0, 1, "bt0"); 
NexDSButton myButton0 = NexDSButton(0,1,"bt0");
NexButton myButton1 = NexButton(0,2,"b0");
NexButton b2 = NexButton(0,4,"b0");
 uint32_t digitalstate0, digitalstate1, digitalstate2  = 0;
 int relay1Pin = 13;
 int relay2Pin = 12;

byte pump1;
byte pump1txt;
byte pump2;
byte pump2txt;

NexTouch *nex_listen_list[] = {
  &myButton1, &myButton0, &b2,
  NULL
};

 void myButton1ReleaseCallback(void *ptr) {
   digitalstate0 = 1-digitalstate0;
   if(digitalstate0==1) {
     pump1=HIGH;
   } else {
     pump1=LOW;
   }
 }
  void b2ReleaseCallback(void *ptr) {
   digitalstate2 = 1-digitalstate2;
   if(digitalstate2==1) {
     pump2=HIGH;
   } else {
     pump2=LOW;
   }
 }


/* void myButton0ReleaseCallback(void *ptr) {
   digitalstate0 = 1-digitalstate0;
   if(digitalstate0==1) {
     pump2=HIGH;
   } else {
     pump2=LOW;
   }
 }*/
char buffer[100] = {0}; 
void setup(void) {
   pinMode(relay1Pin,OUTPUT);
   pinMode(relay2Pin,OUTPUT);
  nexInit();
// myButton0.attachPop(myButton0ReleaseCallback, &myButton0); 
  myButton1.attachPop(myButton1ReleaseCallback, &myButton1); 
  b2.attachPop(b2ReleaseCallback, &b2);
}
 void loop() {

//PUMP1 OUTPUT ********************************************************
  if (pump1==HIGH && pump2==HIGH) {
   pump1txt=HIGH;
sendCommand("bt0.pic0=4");
//sendCommand("ref bt0");
  digitalWrite(relay1Pin, HIGH);
    } else{
pump1txt=LOW;
sendCommand("bt0.pic0=5");
sendCommand("ref bt0");
digitalWrite(relay1Pin, LOW);
  }
  if (pump2==HIGH){// && pump2==HIGH) {
  
//sendCommand("bt0.pic0=4");

  digitalWrite(relay2Pin, HIGH);
    } else{
//sendCommand("bt0.pic0=5");
//sendCommand("ref bt0");
digitalWrite(relay2Pin, LOW);
  }
   nexLoop(nex_listen_list);
 }


