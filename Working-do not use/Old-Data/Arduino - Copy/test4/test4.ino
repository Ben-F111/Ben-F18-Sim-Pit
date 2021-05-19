#include <SoftwareSerial.h>
#include <Nextion.h>

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

int led = 13;
void setup() {
  Serial.begin(9600);
  myNextion.init(); // send the initialization commands for Page 0
pinMode(led, OUTPUT);
}

void loop() {
  String message = myNextion.listen(); //check for message
  if (message == "65 0 1 1 ffff ffff ffff") {
    digitalWrite(led, HIGH);
  }
  if (message == "65 0 2 1 ffff ffff ffff") {
    digitalWrite(led, LOW); 
  }

}
