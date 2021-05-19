#include <Arduino.h>
 #include "Nextion.h"
 
#include <dht.h>
 // parameters  page #, component .id, component .objname


NexText t0 = NexText(0, 9, "t0"); 
NexText t1 = NexText(0, 10, "t1"); 
NexText t2 = NexText(0, 11, "t2"); 
NexText t3 = NexText(0, 12, "t3"); 
NexText t4 = NexText(0, 13, "t4"); 
NexText t5 = NexText(0, 14, "t5"); 
NexText t6 = NexText(0, 15, "t6"); 
NexText t7 = NexText(0, 16, "t7"); 

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
 const int relay1Pin = 13;
 const int relay2Pin = 12;
 const int relay3Pin = 11;
 const int relay4Pin = 10;
 const int relay5Pin = 9;
 const int relay6Pin = 8;
 const int relay7Pin = 7;
 const int relay8Pin = 6;
byte pump1;
byte pump1txt;
byte pump2;
byte pump2txt;
byte pump3;
byte pump3txt;
byte pump4;
byte pump4txt;
byte pump5;
byte pump5txt;
byte pump6;
byte pump6txt;
byte pump7;
byte pump7txt;
byte pump8;
byte pump8txt;
 
 NexTouch *nex_listen_list[] = {
   &myButton0, &myButton1, &myButton2, &myButton3,
   &myButton4, &myButton5, &myButton6, &myButton7,
   NULL
 };

 void myButton0ReleaseCallback(void *ptr) {
   digitalstate0 = 1-digitalstate0;
   if(digitalstate0==1) {
     pump1=HIGH;
   } else {
     pump1=LOW;
   t2.setText("ON"); }
 }

 void myButton1ReleaseCallback(void *ptr) {
   digitalstate1 = 1-digitalstate1;
   if(digitalstate1==1) {
      pump2=HIGH;
   } else {
     pump2=LOW;
   }
 }

 void myButton2ReleaseCallback(void *ptr) {
   digitalstate2 = 1-digitalstate2;
   if(digitalstate2==1) {
    pump3=HIGH;
   } else {
     pump3=LOW;
   }
 }
 void myButton3ReleaseCallback(void *ptr) {
   digitalstate3 = 1-digitalstate3;
   if(digitalstate3==1) {
      pump4=HIGH;
   } else {
     pump4=LOW;
   }
 }
 void myButton4ReleaseCallback(void *ptr) {
   digitalstate4 = 1-digitalstate4;
   if(digitalstate4==1) {
     pump5=HIGH;
   } else {
     pump5=LOW;
   }
 }
 void myButton5ReleaseCallback(void *ptr) {
   digitalstate5 = 1-digitalstate5;
   if(digitalstate5==1) {
     pump6=HIGH;
   } else {
     pump6=LOW;
   }
 }
 void myButton6ReleaseCallback(void *ptr) {
   digitalstate6 = 1-digitalstate6;
   if(digitalstate6==1) {
     pump7=HIGH;
   } else {
     pump7=LOW;
   }
 }
 void myButton7ReleaseCallback(void *ptr) {
   digitalstate7 = 1-digitalstate7;
   if(digitalstate7==1) {
      pump8=HIGH;
   } else {
     pump8=LOW;
   }
 }
char buffer[100] = {0}; 
 void setup(void) {
   pinMode(relay1Pin,OUTPUT);
   pinMode(relay2Pin,OUTPUT);
   pinMode(relay3Pin,OUTPUT);
   pinMode(relay4Pin,OUTPUT);
   pinMode(relay5Pin,OUTPUT);
   pinMode(relay6Pin,OUTPUT);
   pinMode(relay7Pin,OUTPUT);
   pinMode(relay8Pin,OUTPUT);

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

 void loop() {

//PUMP1 OUTPUT ********************************************************
  if (pump1==HIGH) {
    pump1txt=HIGH;
  digitalWrite(relay1Pin, HIGH);
    } else{
   pump1txt=LOW;
  digitalWrite(relay1Pin, LOW);
  }

//PUMP 1 TEXT***********************************************************
 
   if (pump1txt == HIGH){  
  t0.setText("ON");
  }
if (pump1txt == LOW){
    t0.setText("OFF");
  }
//PUMP2 OUTPUT ********************************************************
  if (pump2==HIGH) {
    pump2txt=HIGH;
  digitalWrite(relay2Pin, HIGH);
    } else{
   pump2txt=LOW;
  digitalWrite(relay2Pin, LOW);
  }

//PUMP 2 TEXT***********************************************************
 
 //  if (pump2txt == HIGH){  
  t1.setText("ON");
//  }
//if (pump2txt == LOW){
  //  t1.setText("OFF");
 // }
//PUMP3 OUTPUT ********************************************************
  if (pump3==HIGH) {
    pump3txt=HIGH;
  digitalWrite(relay3Pin, HIGH);
    } else{
   pump3txt=LOW;
  digitalWrite(relay3Pin, LOW);
  }

//PUMP 3 TEXT***********************************************************
 
   if (pump3txt == HIGH){  
  t2.setText("ON");
  }
if (pump3txt == LOW){
    t2.setText("OFF");
  }
  //PUMP4 OUTPUT ********************************************************
  if (pump4==HIGH) {
    pump4txt=HIGH;
  digitalWrite(relay4Pin, HIGH);
    } else{
   pump4txt=LOW;
  digitalWrite(relay4Pin, LOW);
  }

//PUMP 4 TEXT***********************************************************
 
   if (pump4txt == HIGH){  
  t3.setText("ON");
  }
if (pump4txt == LOW){
    t3.setText("OFF");
  }
  //PUMP5 OUTPUT ********************************************************
  if (pump5==HIGH) {
    pump5txt=HIGH;
  digitalWrite(relay5Pin, HIGH);
    } else{
   pump5txt=LOW;
  digitalWrite(relay5Pin, LOW);
  }

//PUMP 5 TEXT***********************************************************
 
   if (pump5txt == HIGH){  
  t4.setText("ON");
  }
if (pump5txt == LOW){
    t4.setText("OFF");
  }
  //PUMP6 OUTPUT ********************************************************
  if (pump6==HIGH) {
    pump6txt=HIGH;
  digitalWrite(relay6Pin, HIGH);
    } else{
   pump6txt=LOW;
  digitalWrite(relay6Pin, LOW);
  }

//PUMP 6 TEXT***********************************************************
 
   if (pump6txt == HIGH){  
  t5.setText("ON");
  }
if (pump6txt == LOW){
    t5.setText("OFF");
  }
  //PUMP7 OUTPUT ********************************************************
  if (pump7==HIGH) {
    pump7txt=HIGH;
  digitalWrite(relay7Pin, HIGH);
    } else{
   pump7txt=LOW;
  digitalWrite(relay7Pin, LOW);
  }

//PUMP 7 TEXT***********************************************************
 
   if (pump7txt == HIGH){  
  t6.setText("ON");
  }
if (pump7txt == LOW){
    t6.setText("OFF");
  }
 // PUMP8 OUTPUT ********************************************************
  if (pump8==HIGH) {
    pump8txt=HIGH;
  digitalWrite(relay8Pin, HIGH);
    } else{
   pump8txt=LOW;
  digitalWrite(relay8Pin, LOW);
  }

//PUMP 8 TEXT***********************************************************
 
   if (pump8txt == HIGH){  
  t7.setText("ON");
  }
if (pump8txt == LOW){
    t7.setText("OFF");
  }
   nexLoop(nex_listen_list);
 }


