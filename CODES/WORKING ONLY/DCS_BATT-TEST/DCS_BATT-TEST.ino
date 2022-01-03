
#include <Servo.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"



DcsBios::ServoOutput voltU(0x753c,16, 544, 1800); //pin 16
DcsBios::ServoOutput voltE(0x753e,18, 1800, 544); // pin18
void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
