/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

const int hydPress2Pin = 6;
const int hydPress1Pin = 7;
const int battUPin = 8;
const int battEPin = 9;
const int altFlagPin = 11;
const int trimPin = 12;

Servo hydPress2Servo;
Servo hydPress1Servo;
Servo battEServo;
Servo battUServo;
Servo altFlagServo;  
Servo trimServo;


int pos = 0;   

void setup() {
  hydPress2Servo.attach(hydPress2Pin);
  hydPress1Servo.attach(hydPress1Pin);  
  battUServo.attach(battUPin);
  battEServo.attach(battEPin);
  altFlagServo.attach(altFlagPin); 
  trimServo.attach(trimPin);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    hydPress2Servo.write(pos);
    hydPress1Servo.write(pos);
    battUServo.write(pos);
    battEServo.write(pos);
    altFlagServo.write(pos);              
    trimServo.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    hydPress2Servo.write(pos);
    hydPress1Servo.write(pos);
    battUServo.write(pos);
    battEServo.write(pos);
    altFlagServo.write(pos);             
    trimServo.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
