
#include <Nextion.h>


#include <Keyboard.h>
#include <KeyboardLayout.h>


// F18 IFEI Arduino to Nextion Ver.12G - Ben Melrose (FP Flight Panels)


#define DCSBIOS_IRQ_SERIAL
#include <Arduino.h>
//#include <Nextion.h>


//SoftwareSerial nextion(18, 19); // SETS SERIAL TO PINS  14/15 RX/TX1

void setup() {
;
 nextion.begin(9600);

}
void loop() {

}
