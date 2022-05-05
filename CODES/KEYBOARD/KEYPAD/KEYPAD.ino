#include <Nextion.h>          // Nextion Library
#include <SoftwareSerial.h>   // Library for Software Serial Port
//#include <Keyboard.h>         // Keyboard Emulation
SoftwareSerial HMISerial(14, 15);  // Nextion TX to pin 10 and RX to pin 11 of Arduino

// This part is used to declare all the objects we can receive from the Nextion Display
// (page, ID, Object name)

NexHotspot F1 = NexHotspot(0, 1, "b0");

NexTouch *nextion_touch_events[] =
{
  &F1,  

  NULL  // End string
};

//  This part is for the different functions for Main Screen Touch events (page zero)
//void F1PushCallback(void *ptr)

void F1PushCallback()
{
 // Keyboard.press('p');
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);
  //Keyboard.release('p');
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}
// Main Setup
void setup() {
  delay(1000);
  HMISerial.begin(9600);  // Start Software Serial at 9600bauds
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  nexInit();  //  Nextion Display initalize

  //Keyboard.begin();  // Start Keyboard emulation
  {
    HMISerial.print("b0.pic=1");
    HMISerial.write("\xFF\xFF\xFF");
  }
  delay(1000);
  {
    HMISerial.print("b0.pic=0");
    HMISerial.write("\xFF\xFF\xFF");
  }

  F1.attachPush(F1PushCallback);
}

// Main Loop
void loop() {

  //delay(10);
  nexLoop(nextion_touch_events); 
}
