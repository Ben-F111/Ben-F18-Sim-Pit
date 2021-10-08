
#include <Servo.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
Servo  trimservo;
//bool  TrimServoFollowupTask = false;
//int TrimServoMoveTime = 1000;
//int timeTrimServoOff = 0;

// not used
//DcsBios::ServoOutput rudTrim(0x7528, 18, 544, 2400);



  void onToTrimBtnChange(unsigned int newValue) {

if (newValue == 1){
  digitalWrite(LED_BUILTIN, HIGH);
  trimservo.attach(18);
   trimservo.write(90);
  delay(0);

///   TrimServoFollowupTask = true;
 //  timeTrimServoOff = millis() + TrimServoMoveTime;
     
}
else    trimservo.detach();

}
DcsBios::IntegerBuffer toTrimBtnBuffer(0x74b4, 0x2000, 13, onToTrimBtnChange);
 
  
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
  trimservo.attach(18);
     trimservo.write(90); 
  trimservo.detach();
 
  DcsBios::setup();
}

void loop() {
//  if ((TrimServoFollowupTask == true) && (millis() >= timeTrimServoOff)){
//     TrimServoFollowupTask = false;
//    trimservo.detach();
//    digitalWrite(LED_BUILTIN, LOW);
//  }
  DcsBios::loop();
}
