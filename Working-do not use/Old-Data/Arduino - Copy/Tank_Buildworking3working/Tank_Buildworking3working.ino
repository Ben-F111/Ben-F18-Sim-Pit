#include <Arduino.h>
 #include "Nextion.h"
 #include <dht.h>
dht DHT;
#define DHT11_PIN 5
NexText t0 = NexText(0, 2, "t0"); 
NexText t1 = NexText(0, 3, "t1");

NexButton myButton0 = NexButton(0, 1, "b0");

 uint32_t digitalstate11 = 0;     
 const int relay1Pin = 13;
 byte pump1;
 byte pump1txt;
NexTouch *nex_listen_list[] = {
  &myButton0, //&myButton1, 
   NULL
 };

 void myButton0ReleaseCallback(void *ptr) {
   digitalstate11 = 1-digitalstate11;
   if(digitalstate11==1){
   pump1=HIGH;
   //   t1.setText("ON"); //this works but in wrong spot
 
    } else {
   pump1=LOW;  
  //     t1.setText("OFF"); //this works but in wrong spot

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
  t1.setText("ON");
  }
if (pump1txt == LOW){
    t1.setText("OFF");
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

