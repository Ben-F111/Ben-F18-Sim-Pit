#include <Arduino.h>
 #include "Nextion.h"
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
NexText t0 = NexText(0, 1, "t0"); 
NexText t1 = NexText(0, 2, "t1");
int DistanceCm;

NexTouch *nex_listen_list[] = {

   NULL

 };
   char buffer[100] = {0}; 
void setup()   /****** SETUP: RUNS ONCE ******/
{
Serial.begin(9600);

 }//--(end setup )---


void loop()   
{
  delay(1000);
  DistanceCm = sonar.ping_cm();
 Serial.print(DistanceCm); 
  Serial.println("  ");  

/*
if ( DistanceCm <= 7){
      t0.setText("LOW");
}

if ( DistanceCm >8 && DistanceCm < 14){
      t0.setText("MED");
} */
if (DistanceCm ==14){
      t0.setText("HIGH");  
}


if ( DistanceCm == 7){
 Serial.println("LOW");  
 
}
/*
if ( DistanceCm >=8 && DistanceCm <= 14){
 Serial.println("MED"); 
  
} 
if ( DistanceCm >14){
  Serial.println("HIGH");
    
}*/
 nexLoop(nex_listen_list);
  t0.setText(buffer);
}


//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

// None
//*********( THE END )***********


