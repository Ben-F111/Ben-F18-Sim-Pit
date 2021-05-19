#include <Arduino.h>
 #include "Nextion.h"
 #include <dht.h>
dht DHT;
#define DHT11_PIN 5

//TEXT FIELDS
NexText t8 = NexText(0, 2, "t0"); 
NexText t0 = NexText(0, 3, "t1");
// BUTTON FIELDS
NexButton myButton0 = NexButton(0,1, "b0");

 uint32_t digitalstate11;     
 const int relay1Pin = 13;
 byte pump1;
NexTouch *nex_listen_list[] = {
  &myButton0, //&myButton1, 
   NULL
 };

 void myButton0ReleaseCallback(void *ptr) {
   digitalstate11 = 1-digitalstate11;
   if(digitalstate11==1){
   pump1=HIGH;
    } else {
   pump1= LOW;  
    }
    }

char buffer[100] = {0}; 
 void setup(void) {
  Serial.begin(9600);
  
 pinMode(relay1Pin,OUTPUT);
   nexInit();
   myButton0.attachPop(myButton0ReleaseCallback, &myButton0);

 }
 
void loop(void) {
 
  uint16_t dht11val;
  memset(buffer,0,sizeof(buffer));

{
  int chk = DHT.read11(DHT11_PIN);
  switch (chk);
 
}
  if (pump1==HIGH){
  digitalWrite(relay1Pin, HIGH);
  } 
   if (pump1==LOW){
  digitalWrite(relay1Pin, LOW);
  } 
 
   dht11val = DHT.temperature;  
  itoa(dht11val,buffer,10);
  t8.setText(buffer);
    nexLoop(nex_listen_list);
 }
 

