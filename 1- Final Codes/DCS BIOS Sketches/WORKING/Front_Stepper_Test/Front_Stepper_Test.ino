
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>
//
const int BrakePress1 = 33;
const int BrakePress2 = 35;
const int BrakePress3 = 37;
const int BrakePress4 = 39;


//const int BrakePress1 = 41;
//const int BrakePress2 = 43;
//const int BrakePress3 = 45;
//const int BrakePress4 = 47;

//const int BrakePress1 = 32;
//const int BrakePress2 = 34;
//const int BrakePress3 = 36;
//const int BrakePress4 = 38;

const int stepsPerRevolution = 400;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, BrakePress1, BrakePress2, BrakePress3, BrakePress4);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
