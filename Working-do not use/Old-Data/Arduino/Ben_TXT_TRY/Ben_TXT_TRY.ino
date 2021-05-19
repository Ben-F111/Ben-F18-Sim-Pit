void setup() {
  // put your setup code here, to run once:
#include "Nextion.h"
}

void loop() {
  // put your main code here, to run repeatedly:

  myNextion.setComponentText("t0", "Hello");
  delay(1000);
  int value = 1234;
  myNextion.setComponentText("t0", String(value));
  delay(1000);
}


/*
 * void loop() {
  nextion.print("t0.txt=\"Hello\"");
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
  delay(1000);
  */
 */
