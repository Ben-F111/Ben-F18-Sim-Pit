
#include <Servo.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
Servo  trimservo;
bool  TrimServoFollowupTask = false;
int TrimServoMoveTime = 1000;
int timeTrimServoOff = 0;

// not used
//DcsBios::ServoOutput rudTrim(0x7528, 18, 544, 2400);



  void onToTrimBtnChange(unsigned int newValue) {

if (newValue == 1){
  digitalWrite(LED_BUILTIN, HIGH);
  trimservo.attach(18);
  delay (10);
  trimservo.write(89);
   delay (50);
   trimservo.write(90);
   TrimServoFollowupTask = true;
   timeTrimServoOff = millis() + TrimServoMoveTime;
     
}
else { digitalWrite(LED_BUILTIN, LOW);
   trimservo.detach();
}
}
DcsBios::IntegerBuffer toTrimBtnBuffer(0x74b4, 0x2000, 13, onToTrimBtnChange);
 
  
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    trimservo.attach(18);
    delay (10);
    trimservo.detach();
    
  DcsBios::setup();
}

void loop() {
/* if ((TrimServoFollowupTask == true) && (millis() >= timeTrimServoOff)){
    TrimServoFollowupTask = false;
    trimservo.detach();
    digitalWrite(LED_BUILTIN, LOW);
  }*/
  DcsBios::loop();
}
