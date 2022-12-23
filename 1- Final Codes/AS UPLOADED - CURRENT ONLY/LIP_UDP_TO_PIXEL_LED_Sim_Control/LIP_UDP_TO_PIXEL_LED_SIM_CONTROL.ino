////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
//||               FUNCTION = HORNET LED OUTPUT MAX 7219              ||\\
//||              LOCATION IN THE PIT = LIP LEFTHAND SIDE             ||\\
//||            ARDUINO PROCESSOR TYPE = Arduino MEGA 256             ||\\
//||      ARDUINO CHIP SERIAL NUMBER = SN - 7533133393935151F160      ||\\
//||      ETHERNET SHEILD MAC ADDRESS = MAC - A8:61:0A:AE:6F:83       ||\\
//||                    CONNECTED COM PORT = COM 9                    ||\\
//||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\

/*
  Drives Pixel LEDs - this was needed as Interrupts from DCS BIOS using serial disrupted updates. These values are delimited by
  by '='



*/
#define Ethernet_In_Use 1
const int Serial_In_Use = 1;
#define Reflector_In_Use 0



// ###################################### Begin Pixel Led Related #############################

// PixelLighting
#include <FastLED.h>
String COLOUR   =  "GREEN";         // The color name that you want to show, e.g. Green, Red, Blue, White
int startUpBrightness =   250;       // LED Brightness 0 = Off, 255 = 100%.
#define MAX_BRIGHTNESS 255          // This is relative to master used with CHSV
#define MAX_MASTER_BRIGHTNESS 255   // Overrides all brightness - used with setbrightness method
#define MAX_WARNING_BRIGHTNESS 255

// Set your power supplies 5V current limit.

#define CURRENT_LIMIT   20000   // Current in mA (1000mA = 1 Amp). Most ATX PSUs provide 20A maximum.

// Defining how many pixels each backlighting connector has connected, if a connector is not used set it to zero.
// Led Counts for LIP and UIP Panels
#define ECM_JETT_LED_COUNT      78
#define VIDEO_RECORD_LED_COUNT  16
#define STANDBY_LED_COUNT       6
#define PLACARD_LED_COUNT       8
#define MASTER_ARM_LED_COUNT    29
#define HUD_CONTROL_LED_COUNT   56
#define SPIN_RECOVERY_LED_COUNT 53

#define LEFT_CONSOLE_LED_COUNT 500
#define RIGHT_CONSOLE_LED_COUNT_WO_SC 326
#define SIM_CONTROL_LED_COUNT 20

const int  LIP_CONSOLE_LED_COUNT = ECM_JETT_LED_COUNT + STANDBY_LED_COUNT + VIDEO_RECORD_LED_COUNT + PLACARD_LED_COUNT;
const int  UIP_CONSOLE_LED_COUNT = MASTER_ARM_LED_COUNT + HUD_CONTROL_LED_COUNT + SPIN_RECOVERY_LED_COUNT;

const int  RIGHT_CONSOLE_LED_COUNT = RIGHT_CONSOLE_LED_COUNT_WO_SC + SIM_CONTROL_LED_COUNT;

// Defining what data pin each backlighting connector is connected to.

// If using small test rig
// The Max7219 connector uses Pin 14,15,16
// Order on connector is 5V GND 16 15 14 Last pin is not connected
//


// Connections using Lukes Power Distribution
#define UIP_PIN                 40
#define LIP_PIN                 46
// Not used as locking collides 44
// Not used as locking collides 46
#define LEFT_CONSOLE_PIN        42
#define RIGHT_CONSOLE_PIN       48

// Some other setup information. Don't change these unless you have a reason to.

#define LED_TYPE     WS2812B  // OPENHORNET backlighting LEDs are WS2812B
#define COLOUR_ORDER GRB      // OPENHORNET backlighting LEDs are GRB (green, red, blue)
#define SOLID_SPEED  100     // The refresh rate delay in ms. Leave this at around 1000 (1 second)

// Setting up the blocks of memory that will be used for storing and manipulating the led data;

CRGB LEFT_CONSOLE_LED[LEFT_CONSOLE_LED_COUNT];
CRGB RIGHT_CONSOLE_LED[RIGHT_CONSOLE_LED_COUNT];
CRGB LIP_CONSOLE_LED[LIP_CONSOLE_LED_COUNT];
CRGB UIP_CONSOLE_LED[UIP_CONSOLE_LED_COUNT];

#define CHSVRed   0
#define CHSVGreen 96
#define CHSVYellow 45

unsigned long NEXT_LED_UPDATE = 0;

// Indicator Variables
bool ECM_JET = false;
bool MASTER_ARM_DISCH_READY = false;
bool MASTER_ARM_DISCH = false;
bool MASTER_ARM_AA = false;
bool MASTER_ARM_AG = false;
bool SPIN = false;


int ledptr = 0;
int consoleBrightness = 50;                     // Global Value for Console Brightness
int SimControlBrightness = 50;
int warningBrightness = 50;                   // Global value for Indicator Brightness
unsigned long timeBeforeNextLedUpdate = 0;
unsigned long minTimeBetweenLedUpdates = 40;    // Provides time foir several updates to be put together before throwing to the led strings
bool LedUpdateNeeded = false;                   // Flags if we have something to update


// The Panels are chained so calculate starting position
// LIP Panel wiring ECM -> Video Record/IFEI -> Placard
// UIP Panel wiring Master Arm -> Hud Control -> Spin Recovery
// Two chains are required for UIP/LIP as the Jet Station Select Placard and
//    (as of the version 1) the Spin Recovery do not pass the data signal through

const int ECM_JET_START_POS       = 0;
const int STANDBY_START_POS       = ECM_JETT_LED_COUNT;
const int VID_RECORD_START_POS    = ECM_JETT_LED_COUNT + STANDBY_LED_COUNT;
const int PLACARD_LED_START_POS   = VID_RECORD_START_POS + VIDEO_RECORD_LED_COUNT;

const int MASTER_ARM_START_POS    = 0;
const int HUD_CONTROL_START_POS   = MASTER_ARM_LED_COUNT;
const int SPIN_RECOVERY_START_POS = HUD_CONTROL_START_POS + HUD_CONTROL_LED_COUNT;

const int SIM_CONTROL_START_POS = RIGHT_CONSOLE_LED_COUNT_WO_SC;


// Special Led Positions on LIP and UIP Panels
#define MASTER_ARM_READY_1 0
#define MASTER_ARM_READY_2 2
#define MASTER_ARM_DISCH_1 1
#define MASTER_ARM_DISCH_2 3
#define MASTER_ARM_AA_1 25
#define MASTER_ARM_AA_2 26
#define MASTER_ARM_AG_1 27
#define MASTER_ARM_AG_2 28


// ECM Testing
#define ECM_JETT_1 74
#define ECM_JETT_2 75
#define ECM_JETT_3 76
#define ECM_JETT_4 77


#define SPIN_1 29
#define SPIN_2 36


// ###################################### End Pixel Led Related #############################




// ###################################### Begin Ethernet Related #############################
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// These local Mac and IP Address will be reassigned early in startup based on
// the device ID as set by address pins
#define EthernetStartupDelay 500
byte mac[] = {0xA8, 0x61, 0x0A, 0x9E, 0x83, 0x05};
IPAddress ip(172, 16, 1, 105);
String strMyIP = "172.16.1.105";

// Reflector
IPAddress reflectorIP(172, 16, 1, 10);
String strReflectorIP = "172.16.1.10";

const unsigned int ledport = 7789;
const unsigned int remoteport = 49000;
const unsigned int reflectorport = 27000;


// Packet Length
int keyboardpacketSize;
int keyboardLen;
int ledPacketSize;
int ledLen;


EthernetUDP ledudp;                   //Left and Right Consoles


char ledpacketBuffer[1000];           //buffer to store led data
char outpacketBuffer[1000];           //buffer to store the outgoing data


// ###################################### End Ethernet Related #############################




// Variables picked up from 737 Code
bool Debug_Display = true;
char *ParameterNamePtr;
char *ParameterValuePtr;

void setup() {

  if (Serial_In_Use) {
    Serial.begin(250000);
    Serial.println("UDP to Pixel Startup");
  }


  if (Ethernet_In_Use == 1) {
    delay(EthernetStartupDelay);
    Ethernet.begin( mac, ip);

    ledudp.begin(ledport);


    if (Reflector_In_Use == 1) {
      ledudp.beginPacket(reflectorIP, reflectorport);
      ledudp.println("Init UDP Pixel Led - " + strMyIP + " " + String(millis()) + "mS since reset.");
      ledudp.endPacket();
    }



  }

  // Activate Backlights
  // Telling the system the type, their data pin, what color order they are and how many there are;
  FastLED.addLeds<LED_TYPE, LEFT_CONSOLE_PIN, COLOUR_ORDER>(LEFT_CONSOLE_LED, LEFT_CONSOLE_LED_COUNT);
  FastLED.addLeds<LED_TYPE, RIGHT_CONSOLE_PIN, COLOUR_ORDER>(RIGHT_CONSOLE_LED, RIGHT_CONSOLE_LED_COUNT);
  FastLED.addLeds<LED_TYPE, LIP_PIN, COLOUR_ORDER>(LIP_CONSOLE_LED, LIP_CONSOLE_LED_COUNT);
  FastLED.addLeds<LED_TYPE, UIP_PIN, COLOUR_ORDER>(UIP_CONSOLE_LED, UIP_CONSOLE_LED_COUNT);




  // The dimming method used. We have a large number of pixels so dithering is not ideal.
  FastLED.setDither(false);

  // Set the LEDs to their defined brightness.
  FastLED.setBrightness(startUpBrightness);

  // FastLED Power management set at 5V, and the defined current limit.
  FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);


  // Now apply everything we just told it about the setup.
  fill_solid( LEFT_CONSOLE_LED, LEFT_CONSOLE_LED_COUNT, CRGB::Green);
  fill_solid( RIGHT_CONSOLE_LED, RIGHT_CONSOLE_LED_COUNT_WO_SC, CRGB::Green);
  fill_solid( LIP_CONSOLE_LED, LIP_CONSOLE_LED_COUNT, CRGB::Green);
  fill_solid( UIP_CONSOLE_LED, UIP_CONSOLE_LED_COUNT, CRGB::Green);

  for (ledptr = SIM_CONTROL_START_POS;
       ledptr <= (SIM_CONTROL_START_POS + SIM_CONTROL_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    RIGHT_CONSOLE_LED[ledptr] = CHSV( CHSVRed, 255, 100);// GREEN
  }

    for (ledptr = STANDBY_START_POS;
       ledptr <= (STANDBY_START_POS + STANDBY_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    LIP_CONSOLE_LED[ledptr] = CHSV( CHSVRed, 255, 200);// GREEN
  }

  FastLED.show();
  delay(2000);
  fill_solid( LEFT_CONSOLE_LED, LEFT_CONSOLE_LED_COUNT, CRGB::Red);
  fill_solid( RIGHT_CONSOLE_LED, RIGHT_CONSOLE_LED_COUNT_WO_SC, CRGB::Red);
  fill_solid( LIP_CONSOLE_LED, LIP_CONSOLE_LED_COUNT, CRGB::Red);
  fill_solid( UIP_CONSOLE_LED, UIP_CONSOLE_LED_COUNT, CRGB::Red);

  for (ledptr = SIM_CONTROL_START_POS;
       ledptr <= (SIM_CONTROL_START_POS + SIM_CONTROL_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    RIGHT_CONSOLE_LED[ledptr] = CHSV(CHSVGreen, 225, 100);//RED
  }

     for (ledptr = STANDBY_START_POS;
       ledptr <= (STANDBY_START_POS + STANDBY_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    LIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, 200);// RED
  }

  FastLED.show();
  delay(2000);


  // Now apply everything we just told it about the setup.
  fill_solid( LEFT_CONSOLE_LED, LEFT_CONSOLE_LED_COUNT, CRGB::Yellow);
  fill_solid( RIGHT_CONSOLE_LED, RIGHT_CONSOLE_LED_COUNT, CRGB::Yellow);
  fill_solid( LIP_CONSOLE_LED, LIP_CONSOLE_LED_COUNT, CRGB::Yellow);
  fill_solid( UIP_CONSOLE_LED, UIP_CONSOLE_LED_COUNT, CRGB::Yellow);

  FastLED.show();
  delay(2000);

  fill_solid( LEFT_CONSOLE_LED, LEFT_CONSOLE_LED_COUNT, CRGB::Black);
  fill_solid( RIGHT_CONSOLE_LED, RIGHT_CONSOLE_LED_COUNT, CRGB::Black);
  fill_solid( LIP_CONSOLE_LED, LIP_CONSOLE_LED_COUNT, CRGB::Black);
  fill_solid( UIP_CONSOLE_LED, UIP_CONSOLE_LED_COUNT, CRGB::Black);

  FastLED.show();
  delay(1000);


  // Now apply everything we just told it about the setup.
  fill_solid( LEFT_CONSOLE_LED, LEFT_CONSOLE_LED_COUNT, CRGB::Green);
  fill_solid( RIGHT_CONSOLE_LED, RIGHT_CONSOLE_LED_COUNT, CRGB::Green);
  fill_solid( LIP_CONSOLE_LED, LIP_CONSOLE_LED_COUNT, CRGB::Green);
  fill_solid( UIP_CONSOLE_LED, UIP_CONSOLE_LED_COUNT, CRGB::Green);
  for (ledptr = SIM_CONTROL_START_POS;
       ledptr <= (SIM_CONTROL_START_POS + SIM_CONTROL_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    RIGHT_CONSOLE_LED[ledptr] = CHSV( CHSVRed, 255, 100);//GREEN
   
  }

     for (ledptr = STANDBY_START_POS;
       ledptr <= (STANDBY_START_POS + STANDBY_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    LIP_CONSOLE_LED[ledptr] = CHSV( CHSVRed, 255, 200);// GREEN
  }
  FastLED.show();
  delay(1000);

  NEXT_LED_UPDATE = millis() + 1000;
}

void SetBacklighting()
{
  bool bLocalDebug = false;
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Entering SetBacklighting");
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Console Brightness: " + String(consoleBrightness));
  // Left and Right Consoles are entirely flood so nothing special needed there
  // Forward console has exceptions
  for (ledptr = 0; ledptr <= (LEFT_CONSOLE_LED_COUNT - 1); ledptr++) {
    LEFT_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }

  for (ledptr = 0; ledptr <= (RIGHT_CONSOLE_LED_COUNT - 1); ledptr++) {
    RIGHT_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }

  // SIM CONTROL
  for (ledptr = SIM_CONTROL_START_POS;
       ledptr <= (SIM_CONTROL_START_POS + SIM_CONTROL_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    RIGHT_CONSOLE_LED[ledptr] = CHSV( CHSVRed, 255, consoleBrightness); //consoleBrightness
  }

    // Standby
   for (ledptr = STANDBY_START_POS;
       ledptr <= (STANDBY_START_POS + STANDBY_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    LIP_CONSOLE_LED[ledptr]= CHSV( CHSVRed, 255, consoleBrightness + (50)); //consoleBrightness
  }

  // LIP and UIP have exceptions - so walk through panel by panel
  // LIP Panel wiring ECM -> Video Record/IFEI -> Placard
  // UIP Panel wiring Master Arm -> Hud Control -> Spin Recovery
  // Panel Led Positions are defined above

  // ******************************************************************************
  // LIP
  // ECM

  for (ledptr = ECM_JET_START_POS;
       ledptr <= (ECM_JET_START_POS + ECM_JETT_LED_COUNT - 1); ledptr++) {
    if (ledptr !=  ECM_JETT_2 && ledptr != ECM_JETT_3 && ledptr !=  ECM_JETT_4 )
       
      LIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }
  // Video Record
  for (ledptr = VID_RECORD_START_POS;
       ledptr <= (VID_RECORD_START_POS + VIDEO_RECORD_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    LIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }
  // Placard
  for (ledptr = PLACARD_LED_START_POS;
       ledptr <= (PLACARD_LED_START_POS + PLACARD_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    LIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }
  // ******************************************************************************

  // UIP
  // MASTER ARM
  for (ledptr = MASTER_ARM_START_POS;
       ledptr <= (MASTER_ARM_START_POS + MASTER_ARM_LED_COUNT - 1); ledptr++) {
    if (ledptr != MASTER_ARM_READY_1 && ledptr != MASTER_ARM_READY_2 &&
        ledptr != MASTER_ARM_DISCH_1 && ledptr != MASTER_ARM_DISCH_2 &&
        ledptr != MASTER_ARM_AA_1 && ledptr != MASTER_ARM_AA_2  &&
        ledptr != MASTER_ARM_AG_1 && ledptr != MASTER_ARM_AG_2)
      UIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }
  // HUD CONTROL
  for (ledptr = HUD_CONTROL_START_POS;
       ledptr <= (HUD_CONTROL_START_POS + HUD_CONTROL_LED_COUNT  - 1); ledptr++) {
    // There are no special function leds - so no check needed
    UIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }
  // SPIN
  for (ledptr = SPIN_RECOVERY_START_POS;
       ledptr <= (SPIN_RECOVERY_START_POS + SPIN_RECOVERY_LED_COUNT  - 1); ledptr++) {
    // Check to see if it is a special led - if it isn't adjust brightness
    if (ledptr != SPIN_1 && ledptr != SPIN_2)
      UIP_CONSOLE_LED[ledptr] = CHSV( CHSVGreen, 255, consoleBrightness);
  }

  if (Debug_Display || bLocalDebug ) Serial.println("Exiting SetBacklighting");
}
void Update_ECMJet() {

  if (ECM_JET == true) {
   // LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_1 ] = CHSV( CHSVGreen, 255, consoleBrightness);
LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_2 ] = CHSV( CHSVGreen, 255, 0);
    LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_3 ] = CHSV( CHSVGreen, 255, warningBrightness);
  LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_4 ] = CHSV( CHSVGreen, 255, 0);
  } else {
  //  LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_1 ] = CHSV( CHSVGreen, 255, consoleBrightness);
    LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_2 ] = CHSV( CHSVGreen, 255, 0);
    LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_3 ] = CHSV( CHSVGreen, 255, 0);
    LIP_CONSOLE_LED[ECM_JET_START_POS + ECM_JETT_4 ] = CHSV( CHSVGreen, 255, 0);
  }
  LedUpdateNeeded = true;
}
void Update_MASTER_ARM_DISCH_READY() {

  if (MASTER_ARM_DISCH_READY == true) {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_READY_1 ] = CHSV( CHSVYellow, 255, warningBrightness);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_READY_2 ] = CHSV( CHSVYellow, 255, warningBrightness);
  } else {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_READY_1 ] = CHSV( CHSVYellow, 255, 0);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_READY_2 ] = CHSV( CHSVYellow, 255, 0);
  }
  LedUpdateNeeded = true;
}
void Update_MASTER_ARM_DISCH() {

  if (MASTER_ARM_DISCH == true) {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_DISCH_1 ] = CHSV( CHSVGreen, 255, warningBrightness);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_DISCH_2 ] = CHSV( CHSVGreen, 255, warningBrightness);
  } else {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_DISCH_1 ] = CHSV( CHSVGreen, 255, 0);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_DISCH_2 ] = CHSV( CHSVGreen, 255, 0);
  }
  LedUpdateNeeded = true;
}

void Update_MASTER_ARM_AA() {

  if (MASTER_ARM_AA == true) {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AA_1 ] = CHSV( CHSVGreen, 255, warningBrightness);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AA_2 ] = CHSV( CHSVGreen, 255, warningBrightness);
  } else {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AA_1 ] = CHSV( CHSVGreen, 255, 0);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AA_2 ] = CHSV( CHSVGreen, 255, 0);
  }
  LedUpdateNeeded = true;
}

void Update_MASTER_ARM_AG() {

  if (MASTER_ARM_AG == true) {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AG_1 ] = CHSV( CHSVGreen, 255, warningBrightness);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AG_2 ] = CHSV( CHSVGreen, 255, warningBrightness);
  } else {
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AG_1 ] = CHSV( CHSVGreen, 255, 0);
    UIP_CONSOLE_LED[MASTER_ARM_START_POS + MASTER_ARM_AG_2 ] = CHSV( CHSVGreen, 255, 0);
  }
  LedUpdateNeeded = true;
}


void Update_SPIN() {

  if (SPIN == true) {
    UIP_CONSOLE_LED[SPIN_RECOVERY_START_POS + SPIN_1 ] = CHSV( CHSVYellow, 255, warningBrightness);
    UIP_CONSOLE_LED[SPIN_RECOVERY_START_POS + SPIN_2 ] = CHSV( CHSVYellow, 255, warningBrightness);
  } else {
    UIP_CONSOLE_LED[SPIN_RECOVERY_START_POS + SPIN_1 ] = CHSV( CHSVYellow, 255, 0);
    UIP_CONSOLE_LED[SPIN_RECOVERY_START_POS + SPIN_2 ] = CHSV( CHSVYellow, 255, 0);
  }
  LedUpdateNeeded = true;
}

void SetWarningLighting()
{

  bool bLocalDebug = false;

  if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Entering SetWarningLighting");
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("warning Brightness: " + String(warningBrightness));

  Update_ECMJet();
  Update_MASTER_ARM_DISCH_READY();
  Update_MASTER_ARM_DISCH();
  Update_MASTER_ARM_AA();
  Update_MASTER_ARM_AG();
  Update_SPIN();

  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Exiting SetWarninglighting");
}




void ProcessReceivedString()
{

  // Reading values from led packetBuffer which is global
  // All received values are strings for readability
  // From 737 Project

  bool bLocalDebug = true;
  int tempVar = 0;

  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Processing Led Packet");

  if (Reflector_In_Use == 1) {
    ledudp.beginPacket(reflectorIP, reflectorport);
    ledudp.println("Pixel Led Packet received");
    ledudp.endPacket();
  }

  String sWrkStr = "";
  const char *delim  = "=";



  ParameterNamePtr = strtok(ledpacketBuffer, delim);
  String ParameterNameString(ParameterNamePtr);
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Parameter Name " + ParameterNameString);

  ParameterValuePtr   = strtok(NULL, delim);
  String ParameterValue(ParameterValuePtr);
  if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Parameter Value " + ParameterValue);



  if (ParameterNameString.equalsIgnoreCase("ConsoleBrightness")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Found Console Brightness");
    consoleBrightness = ParameterValue.toInt(); //ParameterValue.toInt()

    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Console Brightness: " + String(consoleBrightness));
    if (consoleBrightness >= MAX_BRIGHTNESS) consoleBrightness = MAX_BRIGHTNESS;

    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Console Brightness: " + String(consoleBrightness));
    if (SimControlBrightness >= MAX_BRIGHTNESS) SimControlBrightness = MAX_BRIGHTNESS;

    SetBacklighting();
    LedUpdateNeeded = true;
  }

  if (ParameterNameString.equalsIgnoreCase("WarningBrightness")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use) Serial.println("Found Warning Brightness");
    warningBrightness = ParameterValue.toInt();

    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Warning Brightness: " + String(warningBrightness));
    if (warningBrightness >= MAX_WARNING_BRIGHTNESS) warningBrightness = MAX_WARNING_BRIGHTNESS;

    SetWarningLighting();
    LedUpdateNeeded = true;;
  }




  if (ParameterNameString.equalsIgnoreCase("ECM_JET")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Found ECM Jet");
    tempVar = ParameterValue.toInt();
    if (tempVar == 1)
      ECM_JET = true;
    else
      ECM_JET = false;
    Update_ECMJet();
  }



  if (ParameterNameString.equalsIgnoreCase("MASTER_ARM_DISCH_READY")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Found MASTER_ARM_DISCH_READY");
    tempVar = ParameterValue.toInt();
    if (tempVar == 1)
      MASTER_ARM_DISCH_READY = true;
    else
      MASTER_ARM_DISCH_READY = false;
    Update_MASTER_ARM_DISCH_READY();
  }




  if (ParameterNameString.equalsIgnoreCase("MASTER_ARM_DISCH")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Found MASTER_ARM_DISCH");
    tempVar = ParameterValue.toInt();
    if (tempVar == 1)
      MASTER_ARM_DISCH = true;
    else
      MASTER_ARM_DISCH = false;
    Update_MASTER_ARM_DISCH();
  }



  if (ParameterNameString.equalsIgnoreCase("MASTER_ARM_AA")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Found MASTER_ARM_AA");
    tempVar = ParameterValue.toInt();
    if (tempVar == 1)
      MASTER_ARM_AA = true;
    else
      MASTER_ARM_AA = false;
    Update_MASTER_ARM_AA();
  }



  if (ParameterNameString.equalsIgnoreCase("MASTER_ARM_AG")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Found MASTER_ARM_AG");
    tempVar = ParameterValue.toInt();
    if (tempVar == 1)
      MASTER_ARM_AG = true;
    else
      MASTER_ARM_AG = false;
    Update_MASTER_ARM_AG();
  }



  if (ParameterNameString.equalsIgnoreCase("SPIN")) {
    if ((Debug_Display || bLocalDebug ) && Serial_In_Use)  Serial.println("Found SPIN");
    tempVar = ParameterValue.toInt();
    if (tempVar == 1)
      SPIN = true;
    else
      SPIN = false;
    Update_SPIN();
  }
}




void loop() {


  // slowly Dim Leds off after initial start up in setup
  if ((millis() >= NEXT_LED_UPDATE) && (startUpBrightness != 0)) {
    NEXT_LED_UPDATE = millis() + 10;

    startUpBrightness--;
    FastLED.setBrightness(startUpBrightness);
    FastLED.show();

    // If we've completed the startup dimming - set master level
    // And then set console leds to 0;
    if (startUpBrightness == 0) {
      // SetBacklightingColour();
      FastLED.setBrightness(MAX_MASTER_BRIGHTNESS);
      consoleBrightness = 0; //0
      warningBrightness = 100;
      SetBacklighting();
      SetWarningLighting();
      FastLED.show();
    }

  }



  ledPacketSize = ledudp.parsePacket();
  ledLen = ledudp.read(ledpacketBuffer, 999);

  if (ledLen > 0) {
    ledpacketBuffer[ledLen] = 0;
  }
  if (ledPacketSize) {
    ProcessReceivedString();
  }




  if ((LedUpdateNeeded == true) && (millis() >= timeBeforeNextLedUpdate)) {
    FastLED.show();
    LedUpdateNeeded = false;
    timeBeforeNextLedUpdate = millis() + minTimeBetweenLedUpdates;
  }

}
