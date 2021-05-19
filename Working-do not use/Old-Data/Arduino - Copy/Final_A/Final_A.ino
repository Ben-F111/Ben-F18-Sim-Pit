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
int saltlow = 24;
int atohigh = 26;
int atolow = 28;
int topoff = 30;
int sumpchg = 32;
int sumphigh = 34;
int sumplow = 36;
int sumplvl = 38;
int socks = 40;

//***** Internal code bits *****
byte alarm;
byte pump1;
byte pump1sw;
byte pump2;
byte pump2sw;
byte uv;
byte uvsw;
byte chill;
byte chillsw;
byte skim;
byte skimsw;
byte drain;
byte drainsw;
byte ato;
byte atosw;
byte salt;
byte saltsw;
byte fullauto;
byte fullautosw;
byte waterchange;
byte waterchangesw;
byte feedtank;
byte feedtanksw;
byte sumpwchg;
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
     pump1sw=LOW;
   } else {
     pump1sw=HIGH;
   }
 }
  void pump2btnReleaseCallback(void *ptr) {
   digitalstate1 = 1-digitalstate1;
   if(digitalstate1==1) {
     pump2sw=HIGH;
   } else {
     pump2sw=LOW;
   }
 }
   void uvbtnReleaseCallback(void *ptr) {
   digitalstate2 = 1-digitalstate2;
   if(digitalstate2==1) {
     uvsw=HIGH;
   } else {
     uvsw=LOW;
   }
 }  void chillbtnReleaseCallback(void *ptr) {
   digitalstate3 = 1-digitalstate3;
   if(digitalstate3==1) {
     chillsw=HIGH;
   } else {
     chillsw=LOW;
   }
 }  void skimbtnReleaseCallback(void *ptr) {
   digitalstate4 = 1-digitalstate4;
   if(digitalstate4==1) {
     skimsw=HIGH;
   } else {
     skimsw=LOW;
   }
 }  void drainbtnReleaseCallback(void *ptr) {
   digitalstate5 = 1-digitalstate5;
   if(digitalstate5==1) {
     drainsw=HIGH;
   } else {
     drainsw=LOW;
   }
 }  void fillatobtnReleaseCallback(void *ptr) {
   digitalstate6 = 1-digitalstate6;
   if(digitalstate6==1) {
     atosw=HIGH;
   } else {
     atosw=LOW;
   }
 }  void fillsaltbtnReleaseCallback(void *ptr) {
   digitalstate7 = 1-digitalstate7;
   if(digitalstate7==1) {
     saltsw=HIGH;
   } else {
     saltsw=LOW;
   }
      //***** NON critcal bits are set HIGH when not used, fail safe *****
 }  void fullautobtnReleaseCallback(void *ptr) {
   digitalstate8 = 1-digitalstate8;
   if(digitalstate8==1) {
     fullautosw=LOW;
   } else {
     fullautosw=HIGH;
   }
      //***** NON critcal bits are set HIGH when not used, fail safe *****
 }  void feedbtnReleaseCallback(void *ptr) {
   digitalstate9 = 1-digitalstate9;
   if(digitalstate9==1) {
     feedtanksw=LOW;
   } else {
     feedtanksw=HIGH;
   }
   //***** NON critcal bits are set HIGH when not used, fail safe *****
 }  void waterchgbtnReleaseCallback(void *ptr) {
   digitalstate10 = 1-digitalstate10;
   if(digitalstate10==1) {
     waterchangesw=LOW;
   } else {
     waterchangesw=HIGH;
   }
 }

char buffer[100] = {0}; 
void setup(void) {

//***** Output bits *****


  
   pinMode(pump1Pin,OUTPUT);
   pinMode(pump2Pin,OUTPUT);
   pinMode(uvPin,OUTPUT);
   pinMode(chillPin,OUTPUT);
   pinMode(skimPin,OUTPUT);
   pinMode(drainPin,OUTPUT);
   pinMode(atoPin,OUTPUT);
   pinMode(saltPin,OUTPUT);

   //***** input bits*****

    
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

digitalWrite(pump1Pin, HIGH);
digitalWrite(pump2Pin, HIGH);       
digitalWrite(uvPin, HIGH);       
digitalWrite(chillPin, HIGH);       
digitalWrite(skimPin, HIGH);       
digitalWrite(drainPin, LOW);       
digitalWrite(atoPin, LOW);       
digitalWrite(saltPin, LOW);  
// **** Sets bits back to normal arfter a restart****  
//waterchange=HIGH;



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


      Serial.begin(9600);
          dbSerialPrintln("setup done"); 
} 
void serialFlush(){
  while(Serial.available() > 0) {
  }
}
 void loop() {

//Get Pins data

//*****salt tank *****
saltfull = digitalRead(salthigh);
saltempty = digitalRead(saltlow);
//*****sump main*****
sump = digitalRead(sumplvl);
sumpwchg = digitalRead(sumpchg);
sumpfull = digitalRead(sumphigh);
sumpempty = digitalRead(sumplow);
//*****ato tank*****
atofull = digitalRead(atohigh);
atoempty = digitalRead(atolow);
//*****Top off tank*****
topfull = digitalRead(topoff);
//topempty = digitalRead(salthigh);


//PUMP1 OUTPUT ********************************************************

//*****SUMP MAIN LEVEL indicators*****

if (sump==LOW){
sendCommand("bt68.pic0=22");
sendCommand("bt68.pic1=22");
 } else{
sendCommand("bt68.pic0=21");
sendCommand("bt68.pic1=21");
 }

if (sumpwchg==LOW){
sendCommand("bt103.pic0=46");
sendCommand("bt103.pic1=46");
 } else{
sendCommand("bt103.pic0=47");
sendCommand("bt103.pic1=47");
 }  
 
if (sumpfull==LOW){
sendCommand("bt102.pic0=44");
sendCommand("bt102.pic1=44");
 } else{
sendCommand("bt102.pic0=47");
sendCommand("bt102.pic1=47");
 }  

if (sumpempty==LOW){
sendCommand("bt101.pic0=43");
sendCommand("bt101.pic1=43");
 } else{
sendCommand("bt101.pic0=47");
sendCommand("bt101.pic1=47");
 } 
sendCommand("bt100.pic0=45");
sendCommand("bt100.pic1=45");



//*****ATO TOP OFF TANK LEVEL indicators*****
if (topfull==LOW){

sendCommand("bt41.pic0=44");
sendCommand("bt41.pic1=44");
 } else{
sendCommand("bt41.pic0=47");
sendCommand("bt41.pic1=47");
 }  

sendCommand("bt40.pic0=45");
sendCommand("bt40.pic1=45");

//************************************

//*****ATO TANK LEVEL indicators*****
if (atofull==LOW){

sendCommand("bt32.pic0=44");
sendCommand("bt32.pic1=44");
 } else{
sendCommand("bt32.pic0=47");
sendCommand("bt32.pic1=47");
 }  

if (atoempty==LOW){

sendCommand("bt31.pic0=43");
sendCommand("bt31.pic1=43");

 } else{
sendCommand("bt31.pic0=47");
sendCommand("bt31.pic1=47");
 } 
sendCommand("bt30.pic0=45");
sendCommand("bt30.pic1=45");

//************************************

//*****SALT TANK LEVEL indicators*****
if (saltfull==LOW){

sendCommand("bt22.pic0=44");
sendCommand("bt22.pic1=44");
 } else{
sendCommand("bt22.pic0=47");
sendCommand("bt22.pic1=47");
 }  

if (saltempty==LOW){

sendCommand("bt21.pic0=43");
sendCommand("bt21.pic1=43");

 } else{
sendCommand("bt21.pic0=47");
sendCommand("bt21.pic1=47");
 } 
sendCommand("bt20.pic0=45");
sendCommand("bt20.pic1=45");

//************************************
/*saltempty = digitalRead(saltlow);
sump = digitalRead(sumplvl);
sumpchg = digitalRead(sumpwchg);
sumpfull = digitalRead(sumphigh);
sumpempty = digitalRead(sumplow);
atofull = digitalRead(atohigh);
atoempty = digitalRead(topoff);
topfull = digitalRead(salthigh);
topempty = digitalRead(salthigh);*/


//Waterchange button only

  if (waterchange==HIGH) {
  
sendCommand("bt10.pic0=41");
sendCommand("bt10.pic1=41");
 } else{
sendCommand("bt10.pic0=40");
sendCommand("bt10.pic1=40");
 }  

  if (pump1sw==LOW) {//  if (pump1sw==LOW && waterchange==LOW) {
  
sendCommand("bt60.pic0=27");
sendCommand("bt60.pic1=27");
sendCommand("bt0.pic0=28");
sendCommand("bt0.pic1=28");

  digitalWrite(pump1Pin, HIGH);
    } else{
sendCommand("bt60.pic0=24");
sendCommand("bt60.pic1=24");
sendCommand("bt0.pic0=29");
sendCommand("bt0.pic1=29");
digitalWrite(pump1Pin, LOW);
  }

   nexLoop(nex_listen_list);
 }


