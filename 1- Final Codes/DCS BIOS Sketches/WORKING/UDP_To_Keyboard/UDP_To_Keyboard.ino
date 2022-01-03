/* 

Receives a space limited set of characters and sends them to the keyboard

Works on an Arduino DUE.

Will use the logic from pyHWWLink_Keystroke_Sender

Initially just sending very simple single characters

ModifiersOfInterest = ['ALT', 'CTRL', 'SHIFT', 'LSHIFT', 'RSHIFT', 'LCTRL', 'RCTRL', 'LALT', 'RALT', 'LWIN', 'RWIN' ]

*/

#define Ethernet_In_Use 1
const int Serial_In_Use= 1;

// Ethernet Related
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "Keyboard.h"


// These local Mac and IP Address will be reassigned early in startup based on 
// the device ID as set by address pins
byte mac[] = {0x00,0xDD,0x3E,0xCA,0x37,0x99};
IPAddress ip(172,16,1,110);
String strMyIP = "X.X.X.X";

// Raspberry Pi is Target
IPAddress targetIP(172,16,1,2);
String strTargetIP = "X.X.X.X"; 

const unsigned int localport = 7788;
const unsigned int remoteport = 49000;
const unsigned int reflectorport = 27000;

const int delayBetweenRelease = 100;

EthernetUDP udp;
char packetBuffer[1000];     //buffer to store the incoming data
char outpacketBuffer[1000];  //buffer to store the outgoing data

char leftShiftKey = KEY_LEFT_SHIFT;
char leftALTKey = KEY_LEFT_ALT;
void setup() {

  if (Serial_In_Use) {
    Serial.begin(250000);
  }


  if (Ethernet_In_Use == 1) {
    Ethernet.begin( mac, ip);
    
    udp.begin( localport );
    udp.beginPacket(targetIP, reflectorport);
    udp.println("Init UDP - " + strMyIP + " " + String(millis()) + "mS since reset.");
  }

  Keyboard.begin();
}






void Typeout(int packetLength){

  Keyboard.print("Packet received - Length ");
  Keyboard.println(String(packetLength));

  String thisSet = "";
  for (int characterPtr = 0; characterPtr < packetLength ; characterPtr++ ) {
    Keyboard.print(packetBuffer[characterPtr]);
    if (String(packetBuffer[characterPtr]) == " ") {
      Keyboard.println("");
      Keyboard.press(leftShiftKey);
      delay(delayBetweenRelease);
      Keyboard.println(thisSet);
      Keyboard.releaseAll();
      delay(delayBetweenRelease);

      thisSet = "";      
    }
    else {
      thisSet = thisSet + String(packetBuffer[characterPtr]);
    }
  }
  if (thisSet != "") {
    Keyboard.println("");
    delay(delayBetweenRelease);
    Keyboard.press(leftShiftKey);
    Keyboard.println(thisSet);
    Keyboard.releaseAll();    
    delay(delayBetweenRelease);
  }
  
}

void TurnOnAPU(int packetLength) {

// Now need to walk through received string - Modifers must be sent first, so build up
// A list of them using a space as a delimiter

bool leftAltInUse = false;
String thisElement = "";
char keyToPress[50];
  if (Serial_In_Use == 1)  {
    Serial.println("Packet Received");

    String thisSet = "";
    for (int characterPtr = 0; characterPtr < packetLength ; characterPtr++ ) {
      //Serial.print(packetBuffer[characterPtr]);
      if (String(packetBuffer[characterPtr]) == " ") {
        if (thisElement == "LALT"){
          leftAltInUse = true;    
          Serial.println("Left Alt in Use");
        }
        thisElement = "";      
      }
      else {
        thisElement = thisElement + String(packetBuffer[characterPtr]);
      }      
      
    } 
    Serial.println(thisElement);
  }

  
  if (leftAltInUse)
    Keyboard.press(leftALTKey);

  if (thisElement.length() == 1){
    Serial.println("Correct length of Element");  
    thisElement.toCharArray(keyToPress,2);
    Serial.println(String(keyToPress[0]));
    Keyboard.press(keyToPress[0]);
  } else
    Keyboard.press('r');
  delay(delayBetweenRelease);
  Keyboard.releaseAll();    

  if (Serial_In_Use == 1)  {
    Serial.println("Keys Released");
  }

}


void loop() {


  int packetSize = udp.parsePacket();

  int len = udp.read(packetBuffer, 999);

  if (len > 0) {

    packetBuffer[len] = 0;

  }

  if (packetSize) { 
    // Typeout(len);
    TurnOnAPU(len);
  }

}
