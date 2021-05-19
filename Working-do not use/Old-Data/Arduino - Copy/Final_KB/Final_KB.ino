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

int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


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
//int buttonState =0;

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
byte socksblk;
byte drainsump;
byte sumpdraintog;
byte sumpfill;
byte sumpfilltog;



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
boolean running = false;

//********************************************* TIMERS ************************************************************

unsigned long prevTempMillis = 0;          // change names
unsigned long refreshTempInterval = 5000;  // change names
unsigned long prevLEDResetMillis = 0;      // change names
unsigned long ledResetInterval = 60000;    // change names

char buffer[10] = {0}; 

//*****************************************************************************************************************
//********************************************* FUNCTION 1 ********************************************************
void doFunction1()
//********************************************* FUNCTION 1 ********************************************************
//*****************************************************************************************************************

//********************************************* ALL WATER LEVELS **************************************************
{
if (sump==LOW){
sendCommand("bt68.pic0=22");} else{
sendCommand("bt68.pic0=21");}
//********************************************** SALT TANK LEVEL **************************************************
saltfull = digitalRead(salthigh);
saltempty = digitalRead(saltlow);

if (saltfull==LOW){
sendCommand("bt22.pic0=44");
 } else{
sendCommand("bt22.pic0=47"); }  
if (saltempty==LOW){
sendCommand("bt21.pic0=43");
 } else{
sendCommand("bt21.pic0=47");} 
sendCommand("bt20.pic0=45"); // Fixed Red on
//********************************************** SUMP MAIN LEVEL **************************************************
sumpwchg = digitalRead(sumpchg);
sumpfull = digitalRead(sumphigh);
sumpempty = digitalRead(sumplow);

if (sumpwchg==LOW){
sendCommand("bt103.pic0=46");
 } else{
sendCommand("bt103.pic0=47");}  
 if (sumpfull==LOW){
sendCommand("bt102.pic0=44");
 } else{
sendCommand("bt102.pic0=47");}  
if (sumpempty==LOW){
sendCommand("bt101.pic0=43");
} else{
sendCommand("bt101.pic0=47");} 

sendCommand("bt100.pic0=45");// Fixed Red on

//********************************************** ATO TANK LEVEL ***************************************************
atofull = digitalRead(atohigh);
atoempty = digitalRead(atolow);

if (atofull==LOW){
sendCommand("bt32.pic0=44");
 } else{
sendCommand("bt32.pic0=47");}  
if (atoempty==LOW){
sendCommand("bt31.pic0=43");
} else{
sendCommand("bt31.pic0=47");} 

sendCommand("bt30.pic0=45");  // Fixed Red on

//********************************************** TOP OFF LEVEL ****************************************************
topfull = digitalRead(topoff);

if (topfull==LOW){
sendCommand("bt41.pic0=44");
 } else{
sendCommand("bt41.pic0=47"); }  

sendCommand("bt40.pic0=45"); // Fixed Red on

//********************************************** FILTER SOCKS *****************************************************
socksblk = digitalRead(socks);

if (socksblk==LOW){
sendCommand("bt50.pic0=36");
 } else{
sendCommand("bt50.pic0=35");
}
//********************************************** FINAL SUMP LEVEL *************************************************
sump = digitalRead(sumplvl);

if (sump==LOW){
sendCommand("bt68.pic0=22");} else{
sendCommand("bt68.pic0=21");
}



} // END FUNCTION1
//********************************************* END FUNCTION 1 ****************************************************
//*****************************************************************************************************************

//*****************************************************************************************************************
//********************************************* FUNCTION 2 ********************************************************
void doFunction2()
{
  //PUT TEMP AND OTHER SENSORS HERE LATER 
}
//********************************************* END FUNCTION 2 ****************************************************
//*****************************************************************************************************************

//*****************************************************************************************************************
//********************************************* VOID SETUP ********************************************************
void setup(void) {
//********************************************* VOID SETUP ********************************************************
//*****************************************************************************************************************


//********************************************* OUTPUT BITS *******************************************************
   pinMode(pump1Pin,OUTPUT);
   pinMode(pump2Pin,OUTPUT);
   pinMode(uvPin,OUTPUT);
   pinMode(chillPin,OUTPUT);
   pinMode(skimPin,OUTPUT);
   pinMode(drainPin,OUTPUT);
   pinMode(atoPin,OUTPUT);
   pinMode(saltPin,OUTPUT);

//********************************************* INPUT BITS *******************************************************
   
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

//********************************************* SET INPUT BITS ***************************************************
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
//digitalWrite(sumpfilltog, LOW);         //******* CHECK ON SIM *******************

digitalWrite(pump1Pin, HIGH);
digitalWrite(pump2Pin, HIGH);       
digitalWrite(uvPin, HIGH);       
digitalWrite(chillPin, HIGH);       
digitalWrite(skimPin, LOW);       
digitalWrite(drainPin, LOW);       
digitalWrite(atoPin, LOW);       
digitalWrite(saltPin, LOW);  

// **** Sets bits back to normal arfter a restart****  

  nexInit();
  /*
   *   &pump1btn, &pump2btn, &uvbtn, &chillbtn, &skimbtn, &drainbtn, 
  &fillatobtn, &fillsaltbtn, &fullautobtn, &feedbtn, &waterchgbtn,
   */
 
pump1btn.attachPop(pump1btnReleaseCallback, &pump1btn); 
pump2btn.attachPop(pump2btnReleaseCallback, &pump2); 
uvbtn.attachPop(uvbtnReleaseCallback, &uvbtn); 
chillbtn.attachPop(chillbtnReleaseCallback, &chillbtn); 
skimbtn.attachPop(skimbtnReleaseCallback, &skimbtn); 
drainbtn.attachPop(drainbtnReleaseCallback, &drainbtn); 
fillatobtn.attachPop(fillatobtnReleaseCallback, &fillatobtn); 
fillsaltbtn.attachPop(fillsaltbtnReleaseCallback, &fillsaltbtn); 
fullautobtn.attachPop(fullautobtnReleaseCallback, &fullautobtn); 
feedbtn.attachPop(feedbtnReleaseCallback, &feedbtn); 
waterchgbtn.attachPop(waterchgbtnReleaseCallback, &waterchgbtn); 

//********************************************* SERIAL 115200 ****************************************************
      Serial.begin(115200);
          dbSerialPrintln("setup done"); 
} 
void serialFlush(){
  while(Serial.available() > 0) {
  }
}

//*****************************************************************************************************************
//********************************************* VOID LOOP ********************************************************
 void loop() {
//********************************************* VOID LOOP ********************************************************
//*****************************************************************************************************************


//********************************************* LOOP 1 TIMER *****************************************************

    unsigned long currentMillis = millis();
   if (currentMillis - prevTempMillis >= refreshTempInterval) 
  {
    prevTempMillis = currentMillis; // save the last time we checked
    yield();
doFunction1();
  }
//********************************************* LOOP 2 TIMER *****************************************************
  if (currentMillis - prevLEDResetMillis >= ledResetInterval)
  {
    prevLEDResetMillis = currentMillis; // save the last time we checked
    yield();
doFunction2();
  }
//********************************************* WATER CHANGE BUTTON **********************************************
  if (waterchangesw==HIGH) {
sendCommand("bt10.pic0=41");
} else{
sendCommand("bt10.pic0=40"); } 

//********************************************* TANK ALARM *******************************************************

// Tank OK

if (sump == LOW || socksblk == LOW || topfull == HIGH || pump1sw == HIGH || pump2sw == HIGH ){
  sendCommand("bt111.pic0=1");
 // sendCommand("bt111.pic1=1");
}
else { sendCommand("bt111.pic0=26"); }

//********************************************* PUMP 1 OUTPUT ****************************************************

  if (pump1sw==LOW && waterchangesw==LOW) {  
sendCommand("bt60.pic0=27"); // Pump 1
//sendCommand("bt62.pic0=27"); // Chiller
  digitalWrite(pump1Pin, HIGH);
 // digitalWrite(chillPin, HIGH);
    } else{
sendCommand("bt60.pic0=24"); // Pump 1
//sendCommand("bt62.pic0=24"); // Chiller
digitalWrite(pump1Pin, LOW);
//digitalWrite(chillPin, LOW);
  }
//********************************************* PUMP 2 OUTPUT ****************************************************
  if (pump2sw==LOW && waterchangesw==LOW) {
sendCommand("bt61.pic0=27");// Pump 2
//sendCommand("bt63.pic0=27"); //UV
  digitalWrite(pump2Pin, HIGH);
//  digitalWrite(uvPin, HIGH);
    } else{
sendCommand("bt61.pic0=24"); // Pump 2
//sendCommand("bt63.pic0=24"); // UV
digitalWrite(pump2Pin, LOW);
//  digitalWrite(uvPin, LOW);}
    }
//********************************************* UV OUTPUT ********************************************************
   
  if (uvsw==LOW && waterchangesw==LOW && pump2sw == LOW) {
sendCommand("bt63.pic0=27"); //UV
  digitalWrite(uvPin, HIGH);
    } else{
sendCommand("bt63.pic0=24"); // UV
  digitalWrite(uvPin, LOW);}

//********************************************* CHILLER OUTPUT ****************************************************

  if (chillsw==LOW && waterchangesw==LOW && pump1sw == LOW) {  
sendCommand("bt62.pic0=27"); // Chiller
digitalWrite(chillPin, HIGH);
    } else{
sendCommand("bt62.pic0=24"); // Chiller
digitalWrite(chillPin, LOW);}

//********************************************* SKIMMER OUTPUT ****************************************************

 if (skimsw==LOW && waterchangesw==LOW){
  sendCommand("bt64.pic0=27"); // Skimmer
 digitalWrite(skimPin, HIGH);
  } else{
    sendCommand("bt64.pic0=24"); // Skimmer
digitalWrite(skimPin, LOW);}

//********************************************* WATER CHANGE ****************************************************
  
//byte drainsump;
//byte sumpdraintog;
//byte sumpfill;
//byte sumpfilltog;

//********************************************* DRAIN SUMP AUTO *************************************************

if (waterchangesw == HIGH && sumpfill == LOW){
  drainsump = HIGH;
 }
if (drainsump == HIGH && sumpwchg == LOW) {
  sumpdraintog = HIGH;
}
if (sumpdraintog == HIGH && sumpempty == LOW) {
  digitalWrite(drainPin, HIGH);
   sendCommand("bt65.pic0=27"); // Sumpdrain
}
//********************************************* FILL DRAIN CUTOFF ***********************************************
if (sumpdraintog == HIGH && sumpempty == HIGH) {
  digitalWrite(drainPin, LOW);
  sendCommand("bt65.pic0=25"); // sumpdrain
  sumpdraintog = LOW;
   drainsump = LOW;
   sumpfill = HIGH;
  }else{   
}
if (sumpfill == HIGH && sumpwchg == HIGH) {
  sumpfilltog = HIGH;
}
//********************************************* FILL SUMP AUTO **************************************************
if (sumpfilltog == HIGH) {
  digitalWrite(saltPin, HIGH);
  sendCommand("bt66.pic0=27"); // Salt
}
//********************************************* FILL SUMP CUTOFF ************************************************
if (sumpfilltog == HIGH && sumpwchg == LOW) {
  digitalWrite(saltPin, LOW);
    sendCommand("bt66.pic0=25"); // Salt
waterchangesw = LOW;
sumpfilltog = LOW;  
sumpfill = LOW;
  }

//********************************************* END WATER CHANGE *************************************************

//********************************************* DRAIN OUTPUT MANUAL **********************************************

 if (drainsw == HIGH && waterchangesw == LOW && sumpempty == LOW){
 sendCommand("bt65.pic0=27"); // Drain
digitalWrite(drainPin, HIGH);}
 if (drainsw == LOW  && waterchangesw == LOW || sumpempty == HIGH){
 sendCommand("bt65.pic0=25"); // Drain

 digitalWrite(drainPin, LOW);
 drainsw = LOW;}

//********************************************* SUMP FILL OUTPUT MANUAL ******************************************

 if (saltsw == HIGH && waterchangesw == LOW && saltempty == LOW && sumpwchg == HIGH)  {
 sendCommand("bt66.pic0=27"); // Saltwater
 digitalWrite(saltPin, HIGH);}
 
if (saltsw == LOW && waterchangesw == LOW || sumpwchg == LOW || saltempty == HIGH) {
 sendCommand("bt66.pic0=25"); // saltwater
digitalWrite(saltPin, LOW);
saltsw = LOW; }

//********************************************* ATO FILL OUTPUT MANUAL *******************************************

 if (atosw == HIGH && atoempty == LOW){
 sendCommand("bt67.pic0=27"); // Saltwater
 digitalWrite(atoPin, HIGH);}
 
if (atosw == LOW || atoempty == HIGH) {
 sendCommand("bt67.pic0=25"); // saltwater
digitalWrite(atoPin, LOW);
atosw = LOW;}

nexLoop(nex_listen_list);}

//********************************************* END END END END ***************************************************

