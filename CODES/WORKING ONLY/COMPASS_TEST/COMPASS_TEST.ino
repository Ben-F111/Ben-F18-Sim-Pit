/* #####################################################
  DCS BIOS CODE for OPEN HORNET HYD BRAKE GAUGE
  ######################################################*/


#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)
#define  COIL1  8
#define  COIL2 10
#define  COIL3  9
#define  COIL4  11
int IRSensor = 0;

int HYD_BRK = 0;
int val = 0;

Stepper stepper(STEPS, COIL1, COIL2, COIL3, COIL4);


void setup() {
 Serial.begin(9600);
 Serial.print("ONLINE");
  pinMode (IRSensor, INPUT);
  stepper.setSpeed(10);
  stepper.step(820);       //Reset Position(250 steps counter-clockwise (just over the Max travel).
  stepper.step(-820);

}

void loop() {
    stepper.step(1000);

  int statusSensor = digitalRead (IRSensor);
  if (statusSensor == 1){
    Serial.print("ON");
} 
else 
{
  Serial.print("OFF");
}
}
