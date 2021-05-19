#include <Arduino.h>
#include "Nextion.h"
#include "NexTouch.h"

#include <dht.h>

dht DHT;
//***** Temp Sen Pin *****
#define DHT11_PIN 5
// parameters  page #, component .id, component .objname

//***** Control Buttons *****
NexDSButton pump1btn = NexDSButton(0,2,"bt0");
NexDSButton pump2btn = NexDSButton(0,3,"bt1");
NexDSButton uvbtn = NexDSButton(0,4,"bt2");
NexDSButton chillbtn = NexDSButton(0,5,"bt3");
NexDSButton skimbtn = NexDSButton(0,12,"bt4");
NexDSButton drainbtn = NexDSButton(0,6,"bt5");
NexDSButton fillatobtn = NexDSButton(0,7,"bt6");
NexDSButton fillsaltbtn = NexDSButton(0,8,"bt7");
NexDSButton fullautobtn = NexDSButton(0,9,"bt8");
NexDSButton feedbtn = NexDSButton(0,10,"bt9");
NexDSButton waterchgbtn = NexDSButton(0,11,"bt10");

 uint32_t digitalstate0, digitalstate1, digitalstate2, digitalstate3, digitalstate4, 
 digitalstate5, digitalstate6, digitalstate7, digitalstate8, digitalstate9, digitalstate10  = 0;

 
 //***** Relay Outputs 13-6 - 8 total *****
 int pump1Pin = 13;
 int pump2Pin = 12;
 int uvPin = 11;
 int chillPin = 10;
 int skimPin = 9;
 int drainPin = 8;
 int atoPin = 7;
 int saltPin = 6;

//***** tank inputs *****

int salthigh = 22;
int saltlow = 23;
int atohigh = 24;
int atolow = 25;
int topoff = 26;
int sumpchg = 27;
int sumphigh = 28;
int sumplow = 29;
int sumplvl = 30;
int socks = 31;

//***** Internal code bits *****
byte alarm;
byte pump1;
byte pump2;
byte uv;
byte chill;
byte skim;
byte drain;
byte ato;
byte salt;
byte fullauto;
byte waterchange;
byte feedtank;
byte sumpfull;
byte sumpempty;
byte sump;
byte temphigh;
byte templow;
byte saltfull;
byte saltempty;
byte atofull;
byte atoempty;
byte topfull;
byte topempty;



NexTouch *nex_listen_list[] = {
  &pump1btn, &pump2btn, &uvbtn, &chillbtn, &skimbtn, &drainbtn, 
  &fillatobtn, &fillsaltbtn, &fullautobtn, &feedbtn, &waterchgbtn,
  NULL
};
//*****converts button to bits *****

 void pump1btnReleaseCallback(void *ptr) {
   digitalstate0 = 1-digitalstate0;
   if(digitalstate0==1) {
     pump1=HIGH;
   } else {
     pump1=LOW;
   }
 }
  void pump2btnReleaseCallback(void *ptr) {
   digitalstate1 = 1-digitalstate1;
   if(digitalstate1==1) {
     pump2=HIGH;
   } else {
     pump2=LOW;
   }
 }
   void uvbtnReleaseCallback(void *ptr) {
   digitalstate2 = 1-digitalstate2;
   if(digitalstate2==1) {
     uv=HIGH;
   } else {
     uv=LOW;
   }
 }  void chillbtnReleaseCallback(void *ptr) {
   digitalstate3 = 1-digitalstate3;
   if(digitalstate3==1) {
     chill=HIGH;
   } else {
     chill=LOW;
   }
 }  void skimbtnReleaseCallback(void *ptr) {
   digitalstate4 = 1-digitalstate4;
   if(digitalstate4==1) {
     skim=HIGH;
   } else {
     skim=LOW;
   }
 }  void drainbtnReleaseCallback(void *ptr) {
   digitalstate5 = 1-digitalstate5;
   if(digitalstate5==1) {
     drain=HIGH;
   } else {
     drain=LOW;
   }
 }  void fillatobtnReleaseCallback(void *ptr) {
   digitalstate6 = 1-digitalstate6;
   if(digitalstate6==1) {
     ato=HIGH;
   } else {
     ato=LOW;
   }
 }  void fillsaltbtnReleaseCallback(void *ptr) {
   digitalstate7 = 1-digitalstate7;
   if(digitalstate7==1) {
     salt=HIGH;
   } else {
     salt=LOW;
   }
 }  void fullautobtnReleaseCallback(void *ptr) {
   digitalstate8 = 1-digitalstate8;
   if(digitalstate8==1) {
     fullauto=HIGH;
   } else {
     fullauto=LOW;
   }
 }  void feedbtnReleaseCallback(void *ptr) {
   digitalstate9 = 1-digitalstate9;
   if(digitalstate9==1) {
     feedtank=HIGH;
   } else {
     feedtank=LOW;
   }
 }  void waterchgbtnReleaseCallback(void *ptr) {
   digitalstate10 = 1-digitalstate10;
   if(digitalstate10==1) {
     waterchange=HIGH;
   } else {
     waterchange=LOW;
   }
 }

char buffer[100] = {0}; 
void setup(void) {

//***** Output bits *****

 /*int pump1Pin = 13;
 int pump2Pin = 12;
 int uvPin = 11
 int chillPin = 10;
 int skimPin = 9;
 int drainPin = 8;
 int atoPin = 7;
 int saltPin = 6;
 */
  
   pinMode(pump1Pin,OUTPUT);
   pinMode(pump2Pin,OUTPUT);
   pinMode(uvPin,OUTPUT);
   pinMode(chillPin,OUTPUT);
   pinMode(skimPin,OUTPUT);
   pinMode(drainPin,OUTPUT);
   pinMode(atoPin,OUTPUT);
   pinMode(saltPin,OUTPUT);

   //***** input bits*****

   /*
int salthigh = 22;
int saltlow = 23;
int atohigh = 24;
int atolow = 25;
int topoff = 26;
int sumpchg = 27;
int sumphigh = 28;
int sumplow = 29;
int sumplvl = 30;
int socks = 31;
    */
    
    pinMode(salthigh,INPUT);
    pinMode(saltlow,INPUT);
    pinMode(atohigh,INPUT);
    pinMode(atolow,INPUT);
    pinMode(topoff,INPUT);
    pinMode(sumpchg,INPUT);
    pinMode(sumphigh,INPUT);
    pinMode(sumplow,INPUT);
    pinMode(sumplvl,INPUT);
    pinMode(socks,INPUT);

digitalWrite(salthigh, HIGH);
digitalWrite(saltlow, HIGH);       
digitalWrite(atohigh, HIGH);       
digitalWrite(atolow, HIGH);       
digitalWrite(topoff, HIGH);       
digitalWrite(sumpchg, HIGH);       
digitalWrite(sumphigh, HIGH);       
digitalWrite(sumplow, HIGH);       
digitalWrite(sumplvl, HIGH);       
digitalWrite(socks, HIGH);              

  nexInit();

  /*
   *   &pump1btn, &pump2btn, &uvbtn, &chillbtn, &skimbtn, &drainbtn, 
  &fillatobtn, &fillsaltbtn, &fullautobtn, &feedbtn, &waterchgbtn,
   */
pump1btn.attachPop(pump1btnReleaseCallback, &pump1btn); 
pump2btn.attachPop(pump2btnReleaseCallback, &pump2); 
uvbtn.attachPop(uvbtnReleaseCallback, &uvbtn); 
chillbtn.attachPop(chillbtnReleaseCallback, &chillbtn); 
drainbtn.attachPop(drainbtnReleaseCallback, &drainbtn); 
fillatobtn.attachPop(fillatobtnReleaseCallback, &fillatobtn); 
fillsaltbtn.attachPop(fillsaltbtnReleaseCallback, &fillsaltbtn); 
fullautobtn.attachPop(fullautobtnReleaseCallback, &fullautobtn); 
feedbtn.attachPop(feedbtnReleaseCallback, &feedbtn); 
waterchgbtn.attachPop(waterchgbtnReleaseCallback, &waterchgbtn); 

    dbSerialPrintln("setup done"); 
      Serial.begin(9600);
} 
void serialFlush(){
  while(Serial.available() > 0) {
  }
}
 void loop() {

//PUMP1 OUTPUT ********************************************************
/*
  if (pump1==HIGH && pump2==HIGH) {
  
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
   nexLoop(nex_listen_list);*/
 }


