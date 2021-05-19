#include <Arduino.h>
 #include "Nextion.h"
 #include "NexTouch.h"
 #include <dht.h>
dht DHT;
#include <NewPing.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define  TRIGGER_PIN  3
#define  ECHO_PIN     2
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters).
                         //Maximum sensor distance is rated at 400-500cm.
/*-----( Declare objects )-----*/
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
/*-----( Declare Variables )-----*/

NexDSButton b1 = NexDSButton(0,21,"bt19");
NexDSButton b2 = NexDSButton(0,2,"bt0");
uint32_t digitalstate21 = 0;
int DistanceCm;
NexTouch *nex_listen_list[] = {
&b1, &b2,
   NULL

 };
   char buffer[100] = {0}; 
void setup()   /****** SETUP: RUNS ONCE ******/
{
Serial.begin(9600);

 }//--(end setup )---


void loop()   
{
  delay(500);
  DistanceCm = sonar.ping_cm();
 Serial.print(DistanceCm); 
  Serial.println("  ");  

  

if (DistanceCm >=14){
sendCommand("bt19.pic0=21");
sendCommand("ref bt19");
Serial.print("high"); 
}
else{

//if (DistanceCm <= 7){
sendCommand("bt19.pic1=22");
sendCommand("ref bt19"); 
Serial.print("Low");  
 
}


/*
if ( DistanceCm >=8 && DistanceCm <= 14){
 Serial.println("MED"); 
  
} 
if ( DistanceCm >14){
  Serial.println("HIGH");
    
}*/
 nexLoop(nex_listen_list);

}


//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

// None
//*********( THE END )***********


