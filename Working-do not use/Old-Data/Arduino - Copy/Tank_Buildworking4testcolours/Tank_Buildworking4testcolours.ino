#include <Arduino.h>
 #include "Nextion.h"
 #include <dht.h>
dht DHT;
#define DHT11_PIN 5
NexText t0 = NexText(0, 1, "t0");
NexButton myButton0 = NexButton(0, 5, "b0");
NexDSButton myButton1 = NexDSButton(0,6,"bt0");  

 uint32_t digitalstate11 = 0;     
 const int relay1Pin = 13;
 byte pump1;
 byte pump1txt;
NexTouch *nex_listen_list[] = {
  &myButton0, &myButton1, 
   NULL
 };

 void myButton0ReleaseCallback(void *ptr) {
   digitalstate11 = 1-digitalstate11;
   if(digitalstate11==1){
   pump1=HIGH;
    } else {
   pump1=LOW;  
 }
    }
char buffer[100] = {0}; 

//***********************************************************************
 void setup(void) {

 pinMode(relay1Pin,OUTPUT);
   nexInit();
   myButton0.attachPop(myButton0ReleaseCallback, &myButton0);
 

 }
//***********************************************************************
 
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
    bt0.setDSButton(1);
  }
if (pump1txt == LOW){
    bt0.setDSButton(0);
  }

//TEMP SENSOR *********************************************************

  uint16_t dht11val;
  memset(buffer,0,sizeof(buffer));
{
  int chk = DHT.read11(DHT11_PIN);
  switch (chk);
}
//*********************************************************************
   nexLoop(nex_listen_list);
     dht11val = DHT.temperature;  
  itoa(dht11val,buffer,10);
  t0.setText(buffer);

 }

