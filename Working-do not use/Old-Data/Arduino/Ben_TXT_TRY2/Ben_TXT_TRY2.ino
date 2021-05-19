#include <SoftwareSerial.h>
#include "Nextion.h"

SoftwareSerial nextion(14, 15);

void setup() {
  nextion.begin(115200);
    nextion.print("t0.txt=\"Hello\"");
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
}

void loop() {
  nextion.print("t0.txt=\"Hello\"");
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
  delay(1000);
    nextion.print("t0.txt=\"World\"");
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
  delay(1000);
}
