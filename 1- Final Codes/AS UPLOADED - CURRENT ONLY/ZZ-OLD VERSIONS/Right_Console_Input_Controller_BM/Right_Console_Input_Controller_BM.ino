  ////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
  //||               FUNCTION = HORNET LED OUTPUT MAX 7219              ||\\
  //||              LOCATION IN THE PIT = LIP RIGHTHAND SIDE            ||\\
  //||            ARDUINO PROCESSOR TYPE = Arduino Mega 2560            ||\\
  //||      ARDUINO CHIP SERIAL NUMBER = SN - 759323138383511130C1      ||\\
  //||      ETHERNET SHEILD MAC ADDRESS = MAC - A8:61:0A:AE:83:08       ||\\
  //||                    CONNECTED COM PORT = COM 4                    ||\\
  //||               ****ADD ASSIGNED COM PORT NUMBER****               ||\\
  //||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
  ////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
 
  /* 
   *  *//* 

This Superceedes udp_input_controller
Split from udp_input_controller_2 20200802

Heavily based on 
https://github.com/calltherain/ArduinoUSBJoystick

Interface for DCS BIOS

Mega2560 R3, 
digital Pin 22~37 used as rows. 0-15, 16 Rows
digital pin 38~48 used as columns. 0-10, 11 Columns

it's a 16 * 11  matrix, due to the loss of pins/Columns used by the Ethernet and SD Card Shield, Digital I/O 50 through 53 are not available.
Pin 49 is available but isn't used. This gives a total number of usable Inputs as 176 (remember numbering starts at 0 - so 0-175)

The code pulls down a row and reads values from the Columns.
Row 0 - Col 0 = Digit 0
Row 0 - Col 10 = Digit 10
Row 15 - Col 0 = Digit 165
Row 15 = Col 10 = Digit 175

So - Digit = Row * 11 + Col

*/



#define Ethernet_In_Use 0
#define DCSBIOS_In_Use 1

#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"


// Ethernet Related
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// These local Mac and IP Address will be reassigned early in startup based on 
// the device ID as set by address pins
byte mac[] = {0x00,0xDD,0x3E,0xCA,0x36,0x99};
IPAddress ip(172,16,1,103);
String strMyIP = "X.X.X.X";

// Raspberry Pi is Target
IPAddress targetIP(172,16,1,2);
String strTargetIP = "X.X.X.X"; 

const unsigned int localport = 7788;
const unsigned int remoteport = 26027;
const unsigned int reflectorport = 27000;

EthernetUDP udp;
char packetBuffer[1000];     //buffer to store the incoming data
char outpacketBuffer[1000];  //buffer to store the outgoing data







#define NUM_BUTTONS 256
#define BUTTONS_USED_ON_PCB 176
#define NUM_AXES  8        // 8 axes, X, Y, Z, etc
#define STATUS_LED_PORT 7
#define FLASH_TIME 200

// 
struct joyReport_t {
   int button[NUM_BUTTONS]; // 1 Button per byte - was originally one bit per byte - but we have plenty of storage space
};



// Go through the man loop a number of times before sending data to the Sim
// This allows analog averages to be established and the DigitalButton array to be populated
// This has to more than simply the number of elements in the array as the array values are initialised
// to 0, so it actually takes more than 30 interations before the average it fully established
int LoopsBeforeSendingAllowed = 40; 
bool SendingAllowed = false;


// Debounce delay was 20mS - but encountered longer bounces with Circuit Breakers, increased to 60mS 20210329
const int ScanDelay = 80;       // This is in microseconds
const int DebounceDelay =60;    // In milliseconds

joyReport_t joyReport;
joyReport_t prevjoyReport;


unsigned long joyEndDebounce[NUM_BUTTONS];  // Holds the time we'll look at any more changes in a given input

long prevLEDTransition = millis();
int cButtonID[16];
bool bFirstTime = false;


unsigned long currentMillis = 0;
unsigned long previousMillis = 0;




// Note Pin 4 and Pin 10 cannot be used for other purposes.
//Arduino communicates with both the W5500 and SD card using the SPI bus (through the ICSP header). 
//This is on digital pins 10, 11, 12, and 13 on the Uno and pins 50, 51, and 52 on the Mega. 
//On both boards, pin 10 is used to select the W5500 and pin 4 for the SD card. These pins cannot be used for general I/O. 
//On the Mega, the hardware SS pin, 53, is not used to select either the W5500 or the SD card, 
//but it must be kept as an output or the SPI interface won't work.


char stringind[5];
String outString;


bool RadarFollowupTask = false;
long TimeRadarOn = 0;
const int RadarMoveTime = 300;

bool RadarPushFollowupTask = false;
long TimeRadarOff = 0;



void setup() {



  // Set the output ports to output
  for( int portId = 22; portId < 38; portId ++ )
  {
    pinMode( portId, OUTPUT );
  }


  // Set the input ports to input - port 50-53 used by Ethernet Shield
  for( int portId = 38; portId < 50; portId ++ )
  {
    // Even though we've already got 10K external pullups
    pinMode( portId, INPUT_PULLUP);
  }



  // Initialise all arrays
  for (int ind=0; ind < NUM_BUTTONS ; ind++) {

    // Clear current and last values to 0 for button inputs
    joyReport.button[ind] = 0;
    prevjoyReport.button[ind] = 0;

    // Set the end 
    joyEndDebounce[ind] = 0;
  }

  
  if (DCSBIOS_In_Use == 1) DcsBios::setup();

  if (Ethernet_In_Use == 1) {
    Ethernet.begin( mac, ip);
    
    udp.begin( localport );
    udp.beginPacket(targetIP, reflectorport);
    udp.println("Init UDP - " + strMyIP + " " + String(millis()) + "mS since reset.");
  }
}


void FindInputChanges()
{

  for (int ind=0; ind < NUM_BUTTONS; ind++)
    if (bFirstTime) {

      bFirstTime = false;
      // Just Copy Array and perform no actions - this may change in the future
      prevjoyReport.button[ind] = joyReport.button[ind];    
    }
    else {
      // Not the first time - see if there is a difference from last time
      // If there is perform action and update prev array BUT only if we past the end of the debounce period
      if ( prevjoyReport.button[ind] != joyReport.button[ind] && millis() > joyEndDebounce[ind] ){

        // First things first - set a new debounce period
        joyEndDebounce[ind] = millis() + DebounceDelay;        

        sprintf(stringind, "%03d", ind);
 
        
        if (prevjoyReport.button[ind] == 0) {
          outString = outString +  "1"; 
          if (DCSBIOS_In_Use == 1) SendDCSBIOSMessage(ind, 1);
          if (Ethernet_In_Use == 1) SendIPMessage(ind, 1);
        }  
        else {   
          outString = outString + "0"; 
          if (DCSBIOS_In_Use == 1) SendDCSBIOSMessage(ind, 0);
          if (Ethernet_In_Use == 1) SendIPMessage(ind, 0);
        }


        prevjoyReport.button[ind] = joyReport.button[ind]; 


      }
      
    }

}

void SendIPMessage(int ind, int state) {

  String outString;
  outString = String(ind) + ":" + String(state); 
  
  udp.beginPacket(targetIP, reflectorport);
  udp.print(outString);
  udp.endPacket();
  
  
  udp.beginPacket(targetIP, remoteport);
  udp.print(outString);
  udp.endPacket();
}

void SendDCSBIOSMessage(int ind, int state) {

  
  switch (state) {

    // RELEASE
    case 0:
      switch (ind) {

        case 0:
          sendDcsBiosMessage("FLIR_SW","1");
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          break; 
        case 4:
          break;
        case 5:
          break;
        case 6:
          break;
        case 7:
          break;
        case 8:
          break;
        case 9:
          sendDcsBiosMessage("HOOK_LEVER","1");
          break;
        case 10:
          sendDcsBiosMessage("WING_FOLD_ROTATE","1");
          break;
        case 11:
          sendDcsBiosMessage("FLIR_SW","1");
          break; 
        case 12:
          break; 
        case 13:
          break;
        case 14:
          break; 
        case 15:
          break;
        case 16:
          break;
        case 17:
          break;
        case 18:
          break;
        case 19:
          break;
        case 20:
          sendDcsBiosMessage("WING_FOLD_ROTATE","1");
          break;
        case 21:
          sendDcsBiosMessage("AV_COOL_SW","1");
          break;
        case 22:
          sendDcsBiosMessage("LTD_R_SW","1");
          break;
        case 23:
          break; 
        case 24:
          break; 
        case 25:
          break; 
        case 26:
          break;
        case 27:
          break;             
        case 28:
          break;
        case 29:
          break;
        case 30:
          break;
        case 31:
          sendDcsBiosMessage("WING_FOLD_PULL","0");
          break;
        case 32:
          break;
        case 33:
          sendDcsBiosMessage("LST_NFLR_SW","0");
          break; 
        case 34:
          sendDcsBiosMessage("RADAR_SW_PULL","1");
          TimeRadarOn = millis() + RadarMoveTime;
          RadarPushFollowupTask = true;
          break;
        case 35:
          break; 
        case 36:
          break;             
        case 37:
          break;
        case 38:
          break;
        case 39:
          break;
        case 40:
          break;
        case 41:
          break;
        case 42:
          sendDcsBiosMessage("BATTERY_SW","1");
          break;
        case 43:
          sendDcsBiosMessage("L_GEN_SW","0"); //1
          break;
        case 44:
          break;
        case 45:
          break; 
        case 46:
          break; 
        case 47:
          break;
        case 48:
          break;
        case 49:
          break;
        case 50:
          break;
        case 51:
          break;
        case 52:
          break;
        case 53:
          sendDcsBiosMessage("BATTERY_SW","1");
          break;
        case 54:
          sendDcsBiosMessage("R_GEN_SW","0");
          break;
        case 55:
          break;
        case 56:
          break;
        case 57:
          break;
        case 58:
          break;
        case 59:
          break;
        case 60:
          break;
        case 61:
          break;
        case 62:
          break;
        case 63:
          break;
        case 64:
          break;
        case 65:
          break;
        case 66:
          break;
        case 67:
          break;
        case 68:
          break;
        case 69:
          break;
        case 70:
          break;
        case 71:
          break;
        case 72:
          break;
        case 73:
          break;
        case 74:
          break;
        case 75:
          break;
        case 76:
          break;
        case 77:
          break;
        case 78:
          break;
        case 79:
          break;
        case 80:
          break;
        case 81:
          break;
        case 82:
          break;
        case 83:
          break;
        case 84:
          break;
        case 85:
          break;
        case 86:
          break;
        case 87:
          break;
        case 88:
          sendDcsBiosMessage("LIGHTS_TEST_SW","0");
          break;
        case 89:
          sendDcsBiosMessage("COCKKPIT_LIGHT_MODE_SW","1");
          break;
        case 90:
          sendDcsBiosMessage("WSHIELD_ANTI_ICE_SW","1");
          break;
        case 91:
          sendDcsBiosMessage("ECS_MODE_SW","1");
          break;
        case 92:
          sendDcsBiosMessage("CABIN_PRESS_SW","1");
          break;
        case 93:
          sendDcsBiosMessage("PITOT_HEAT_SW","0");
          break;
        case 94:
          break;
        case 95:
        sendDcsBiosMessage("RADALT_TEST_SW","0");
          break;
        case 96:
          break;
        case 97:
          break;
        case 98:
          break;
        case 99:
          break;
        case 100:
          sendDcsBiosMessage("COCKKPIT_LIGHT_MODE_SW","1");
          break;
        case 101:
          sendDcsBiosMessage("WSHIELD_ANTI_ICE_SW","1");
          break;
        case 102:
          sendDcsBiosMessage("ECS_MODE_SW","1");
          break;
        case 103:
          sendDcsBiosMessage("CABIN_PRESS_SW","1");
          break;
        case 104:
          break;
        case 105:
          break;
        case 106:
          break;
        case 107:
          break;
        case 108:
          break;
        case 109:
          break;
        case 110:
          sendDcsBiosMessage("CANOPY_SW","1");
          break;
        case 111:
          sendDcsBiosMessage("CB_FCS_CHAN3","1");
          break;
        case 112:
          break;
        case 113:
          sendDcsBiosMessage("ENG_ANTIICE_SW","1");
          break;
        case 114:
          break;
        case 115:
          break;
        case 116:
          break;
        case 117:
          break;
        case 118:
          break;
        case 119:
          break;
        case 120:
          break;
        case 121:
          sendDcsBiosMessage("CANOPY_SW","1");
          break;
        case 122:
          sendDcsBiosMessage("CB_FCS_CHAN4","1");
          break;
        case 123:
          break;
        case 124:
          sendDcsBiosMessage("ENG_ANTIICE_SW","1");
          break;
        case 125:
          break;
        case 126:
          break;
        case 127:
          break;
        case 128:
          break;
        case 129:
          break;
        case 130:
          break;
        case 131:
          break;
        case 132:
          sendDcsBiosMessage("FCS_BIT_SW","1");
          break;
        case 133: 
          sendDcsBiosMessage("CB_LG","1");
          break;
        case 134:
          sendDcsBiosMessage("CB_HOOOK","1");
          break;
        case 135:
          break;
        case 136:
          break;
        case 137:
          break;
        case 138:
          break;
        case 139:
          break;
        case 140:
          break;
        case 141:
          break;
        case 142:
          break;
        case 143:
          break;
        case 144:
          break;
        case 145:
          break;
        case 146:
          break;
        case 147:
          break;
        case 148:
          break;
        case 149:
          break;
        case 150:
          break;
        case 151:
          break;
        case 152:
          break;
        case 153:
          break;
        case 154:
          break;
        case 155:
          break;
        case 156:
          break;
        case 157:
          break;
        case 158:
          break;
        case 159:
          break;
        case 160:
          break;
        case 161:
          break;
        case 162:
          break;
        case 163:
          break;
        case 164:
          break;
        case 165:
          break;
        case 166:
          break;
        case 167:
          break;
        case 168:
          break;
        case 169:
          break;
        case 170:
          break;
        case 171:
          break;
        case 172:
          break;
        case 173:
          break;
        case 174:
          break;
        case 175:
          break;
        case 176:
          break;
        case 177:
          break;
        case 178:
          break;
        case 179:
          break;
        default:
          break; 
      } 
      break;


      
    case 1:

      // PRESS - CLOSE
      switch (ind) {
         case 0:
          sendDcsBiosMessage("FLIR_SW","2");
          break;
        case 1:
          sendDcsBiosMessage("RADAR_SW","0");
          break;
        case 2:
          sendDcsBiosMessage("INS_SW","0"); //OFF
          break;
        case 3:
          break; 
        case 4:
          break;
        case 5:
          break;
        case 6:
          break;
        case 7:
          sendDcsBiosMessage("KY58_MODE_SELECT","0");
          break;
        case 8:
          sendDcsBiosMessage("KY58_FILL_SELECT","0");
          break;
        case 9:
          sendDcsBiosMessage("HOOK_LEVER","0");
          break;
        case 10:
          sendDcsBiosMessage("WING_FOLD_ROTATE","2");
          break;
        // PRESS - CLOSE
        case 11:
          sendDcsBiosMessage("FLIR_SW","0");
          break; 
        case 12:
          sendDcsBiosMessage("RADAR_SW","1");
          break; 
        case 13:
          sendDcsBiosMessage("INS_SW","1"); //CV
          break;
        case 14:
          break; 
        case 15:
          break;
        case 16:
          break;
        case 17:
          break;
        case 18:
          sendDcsBiosMessage("KY58_MODE_SELECT","1");
          break;
        case 19:
          sendDcsBiosMessage("KY58_FILL_SELECT","1");
          break;
        case 20:
          sendDcsBiosMessage("WING_FOLD_ROTATE","0");
          break;
        // PRESS - CLOSE
        case 21:
          sendDcsBiosMessage("AV_COOL_SW","0");
          break;
        case 22:
          sendDcsBiosMessage("LTD_R_SW","2");
          break;
        case 23:
          sendDcsBiosMessage("RADAR_SW","2");
          break; 
        case 24:
          sendDcsBiosMessage("INS_SW","2"); //GND
          break; 
        case 25:
          break; 
        case 26:
          break;
        case 27:
          break;             
        case 28:
          break;
        case 29:
          sendDcsBiosMessage("KY58_MODE_SELECT","2");
          break;
        case 30:
          sendDcsBiosMessage("KY58_FILL_SELECT","2");
          break;
        // PRESS - CLOSE
        case 31:
          sendDcsBiosMessage("WING_FOLD_PULL","1");
          break;
        case 32:
          break;
        case 33:
          sendDcsBiosMessage("LST_NFLR_SW","1");
          break; 
        case 34:
          sendDcsBiosMessage("RADAR_SW_PULL","1");
          RadarFollowupTask = true;
          TimeRadarOn = millis() + RadarMoveTime;         
          break;
        case 35:
          sendDcsBiosMessage("INS_SW","3"); //NAV
          break; 
        case 36:
          break;             
        case 37:
          break;
        case 38:
          break;
        case 39:
          break;
        case 40:
          sendDcsBiosMessage("KY58_MODE_SELECT","3");
          break;
        // PRESS - CLOSE
        case 41:
          sendDcsBiosMessage("KY58_FILL_SELECT","3");
          break;
        case 42:
          sendDcsBiosMessage("BATTERY_SW","2");
          break;
        case 43:
          sendDcsBiosMessage("L_GEN_SW","1"); //0
          break;
        case 44:
          break;
        case 45:
          break; 
        case 46:
          sendDcsBiosMessage("INS_SW","4"); //IFA
          break; 
        case 47:
          break;
        case 48:
          break;
        case 49:
          break;
        case 50:
          break;
        // PRESS - CLOSE
        case 51:
          sendDcsBiosMessage("KY58_POWER_SELECT","0");
          break;
        case 52:
          sendDcsBiosMessage("KY58_FILL_SELECT","4");
          break;
        case 53:
          sendDcsBiosMessage("BATTERY_SW","0");
          break;
        case 54:
          sendDcsBiosMessage("R_GEN_SW","1");
          break;
        case 55:
          break;
        case 56:
          break;
        case 57:
          sendDcsBiosMessage("INS_SW","5"); //GYRO
          break;
        case 58:
          break;
        case 59:
          break;
        case 60:
          break;
        // PRESS - CLOSE
        case 61:
          break;
        case 62:
          sendDcsBiosMessage("KY58_POWER_SELECT","1");
          break;
        case 63:
          sendDcsBiosMessage("KY58_FILL_SELECT","5");
          break;
        case 64:
          break;
        case 65:
          break;
        case 66:
          break;
        case 67:
          break;
        case 68:
          sendDcsBiosMessage("INS_SW","6"); //GB
          break;
        case 69:
          break;
        case 70:
          break;
        // PRESS - CLOSE
        case 71:
          break;
        case 72:
          break;
        case 73:
          sendDcsBiosMessage("KY58_POWER_SELECT","2");
          break;
        case 74:
          sendDcsBiosMessage("KY58_FILL_SELECT","6");
          break;
        case 75:
          break;
        case 76:
          break;
        case 77:
          break;
        case 78:
          break;
        case 79:
          sendDcsBiosMessage("INS_SW","7"); //TEST
          break;
        case 80:
          break;
        // PRESS - CLOSE
        case 81:
          break;
        case 82:
          break;
        case 83:
          break;
        case 84:
          break;
        case 85:
          sendDcsBiosMessage("KY58_FILL_SELECT","7");
          break;
        case 86:
          break;
        case 87:
          break;
        case 88:
          sendDcsBiosMessage("LIGHTS_TEST_SW","1");
          break;
        case 89:
          sendDcsBiosMessage("COCKKPIT_LIGHT_MODE_SW","2");
          break;
        case 90:
          sendDcsBiosMessage("WSHIELD_ANTI_ICE_SW","0");
          break;
        // PRESS - CLOSE
        case 91:
          sendDcsBiosMessage("ECS_MODE_SW","0");
          break;
        case 92:
          sendDcsBiosMessage("CABIN_PRESS_SW","2");
          break;
        case 93:
          sendDcsBiosMessage("PITOT_HEAT_SW","1");
          break;
        case 94:
          sendDcsBiosMessage("BLEED_AIR_KNOB","0");
          break;
        case 95:     
        sendDcsBiosMessage("RADALT_TEST_SW","1");
          break;
        case 96:
          break;
        case 97:
          break;
        case 98:
          break;
        case 99:
          break;
        case 100:
          sendDcsBiosMessage("COCKKPIT_LIGHT_MODE_SW","0");
          break;
        // PRESS - CLOSE
        case 101:
          sendDcsBiosMessage("WSHIELD_ANTI_ICE_SW","2");
          break;
        case 102:
          sendDcsBiosMessage("ECS_MODE_SW","2");
          break;
        case 103:
          sendDcsBiosMessage("CABIN_PRESS_SW","0");
          break;
        case 104:
          break;
        case 105:
          sendDcsBiosMessage("BLEED_AIR_KNOB","1");
          break;
        case 106:
          break;
        case 107:
          break;
        case 108:
          break;
        case 109:
          break;
        case 110:
          sendDcsBiosMessage("CANOPY_SW","2");
          break;
        // PRESS - CLOSE
        case 111:
          sendDcsBiosMessage("CB_FCS_CHAN3","0");
          break;
        case 112:
          break;
        case 113:
          sendDcsBiosMessage("ENG_ANTIICE_SW","2");
          break;
        case 114:
          break;
        case 115:
          break;
        case 116:
          sendDcsBiosMessage("BLEED_AIR_KNOB","2");
          break;
        case 117:
          break;
        case 118:
          break;
        case 119:
          break;
        case 120:
      // PRESS - CLOSE
          break;
        case 121:
          sendDcsBiosMessage("CANOPY_SW","0");
          break;
        case 122:
          sendDcsBiosMessage("CB_FCS_CHAN4","0");
          break;
        case 123:
          break;
        case 124:
          sendDcsBiosMessage("ENG_ANTIICE_SW","0");
          break;
        case 125:
          break;
        case 126:
          break;
        case 127:
          sendDcsBiosMessage("BLEED_AIR_KNOB","3");
          break;
        case 128:
          break;
        case 129:
          break;
        case 130:
          break;
        // PRESS - CLOSE
        case 131:
          break;
        case 132:
          sendDcsBiosMessage("FCS_BIT_SW","0");
          break;
        case 133:
          sendDcsBiosMessage("CB_LG","0");
          break;
        case 134:
          sendDcsBiosMessage("CB_HOOOK","0");
          break;
        case 135:
          break;
        case 136:
          break;
        case 137:
          break;
        case 138:
          break;
        case 139:
          break;
        case 140:
          break;
        // PRESS - CLOSE
        case 141:
          break;
        case 142:
          break;
        case 143:
          break;
        case 144:
          break;
        case 145:
          break;
        case 146:
          break;
        case 147:
          break;
        case 148:
          break;
        case 149:
          break;
        case 150:
          break;
        // PRESS - CLOSE
        case 151:
          break;
        case 152:
          break;
        case 153:
          break;
        case 154:
          break;
        case 155:
          break;
        case 156:
          break;
        case 157:
          break;
        case 158:
          break;
        case 159:
          break;
        case 160:
          break;
        // PRESS - CLOSE
        case 161:
          break;
        case 162:
          break;
        case 163:
          break;
        case 164:
          break;
        case 165:
          break;
        case 166:
          break;
        case 167:
          break;
        case 168:
          break;
        case 169:
          break;
        case 170:
          break;
        // PRESS - CLOSE
        case 171:
          break;
        case 172:
          break;
        case 173:
          break;
        case 174:
          break;
        case 175:
          break;
        case 176:
          break;
        case 177:
          break;
        case 178:
          break;
        case 179:
          break;       
 
        default:
// PRESS - CLOSE
          break; 
      break; 
     }
  }  
}

//ECS PANEL
DcsBios::PotentiometerEWMA<5, 128, 5> cabinTemp("CABIN_TEMP", 9);  //"YYY" = DCS_BIOS INPUT NAME and X = PIN
DcsBios::PotentiometerEWMA<5, 128, 5> suitTemp("SUIT_TEMP", 8); //"YYY" = DCS_BIOS INPUT NAME and X = PIN

//INTR LTS PANEL
DcsBios::PotentiometerEWMA<5, 128, 5> chartDimmer("CHART_DIMMER", 3); //set//"YYY" = DCS_BIOS INPUT NAME and X = PIN
DcsBios::PotentiometerEWMA<5, 128, 5> consolesDimmer("CONSOLES_DIMMER", 0); //set //"YYY" = DCS_BIOS INPUT NAME and X = PIN
DcsBios::PotentiometerEWMA<5, 128, 5> floodDimmer("FLOOD_DIMMER", 2); //"YYY" = DCS_BIOS INPUT NAME and X = PIN
DcsBios::PotentiometerEWMA<5, 128, 5> instPnlDimmer("INST_PNL_DIMMER", 1); //"YYY" = DCS_BIOS INPUT NAME and X = PIN
DcsBios::PotentiometerEWMA<5, 128, 5> warnCautionDimmer("WARN_CAUTION_DIMMER", 4); //"YYY" = DCS_BIOS INPUT NAME and X = PIN

//DE-FOG PANEL (INTR LTS)

DcsBios::PotentiometerEWMA<5, 128, 5> defogHandle("DEFOG_HANDLE", 5); //set//"YYY" = DCS_BIOS INPUT NAME and X = PIN

//KY58 PANEL
DcsBios::PotentiometerEWMA<5, 128, 5> ky58Volume("KY58_VOLUME", 10); //"YYY" = DCS_BIOS INPUT NAME and X = PIN

// controlPosition: 0 to 65,535 value representing the analog, real world control value
// dcsPosition: 0 to 65,535 value reported from DCS for the provided address
// return: −32,768 to 32,767 signed integer to be sent to the DCS rotary control.  0 will not be sent.
int HornetRadaltMapper(unsigned int controlPosition, unsigned int dcsPosition)
{
  unsigned int a = map(controlPosition,0,65530,161000,1800);  // Silly right now, but to reduce the range if your analog pot doesn't reach max deflection, reduce the first 65535 number
  unsigned int b = map(dcsPosition,0,64355,0,65535);  // Observationally, in DCS the max value for RADALT_HEIGHT is 64355

  // Careful here since we are on 16 bit microcontrollers and doing some signed v unsigned maths.  Probably a better way to do this, but this works.
  unsigned int delta = (a >= b)?a - b:b-a;

  const unsigned int MAX_ROTATION = 20000;  // Always keep less than 32767
  if( delta > MAX_ROTATION )
    delta = MAX_ROTATION;

  if( a >= b )
    return (int)delta;
  else
    return -1*(int)delta;
}

DcsBios::RotarySyncingPotentiometer radaltHeight("RADALT_HEIGHT", 11, 0x7518, 0xffff, 0, HornetRadaltMapper);






void loop() {

  if (DCSBIOS_In_Use == 1) DcsBios::loop();

  //turn off all rows first
  for ( int rowid = 0; rowid < 16; rowid ++ )
  {
    //turn on the current row
    // why differentiate? rows


    if (rowid == 0) 
      PORTC =  0xFF;
    if (rowid == 8)
      PORTA = 0xFF;
      
    if (rowid < 8)
    {
      // Shift 1 right  - this is actually pulling port down
      PORTA = ~(0x1 << rowid);
    }
    else
    {
      PORTC = ~(0x1 << (15 - rowid) );
    }


    
    //we must have such a delay so the digital pin output can go LOW steadily, 
    //without this delay, the row PIN will not 100% at LOW during yet,
    //so check the first column pin's value will return incorrect result.
    delayMicroseconds(ScanDelay);

    int colResult[16];
    // Reading upper pins
    //pin 38, PD7
    colResult[0] = (PIND & B10000000)== 0 ? 0 : 1;
    //pin 39, PG2
    colResult[1] = (PING & B00000100)== 0 ? 0 : 1;
    //pin 40, PG1
    colResult[2] = (PING & B00000010)== 0 ? 0 : 1;
    //pin 41, PG0
    colResult[3] = (PING & B00000001)== 0 ? 0 : 1;

    //pin 42, PL7
    colResult[4] = (PINL & B10000000)== 0 ? 0 : 1;
    //pin 43, PL6
    colResult[5] = (PINL & B01000000)== 0 ? 0 : 1;
    //pin 44, PL5
    colResult[6] = (PINL & B00100000)== 0 ? 0 : 1;
    //pin 45, PL4
    colResult[7] = (PINL & B00010000)== 0 ? 0 : 1;

    //pin 46, PL3
    colResult[8] = (PINL & B00001000)== 0 ? 0 : 1;
    //pin 47, PL2
    colResult[9] = (PINL & B00000100)== 0 ? 0 : 1;
    //pin 48, PL1
    colResult[10] =(PINL & B00000010) == 0 ? 0 : 1;
    //pin 49, PL0
    //pin 49 is not used on the PCB design - more a mistake than anything else as it is available for us
    //colResult[11] =(PINL & B00000001) == 0 ? 0 : 1;
    colResult[11] = 1;

    // Unable to use pins 50-53 per the following
    //This is on digital pins 10, 11, 12, and 13 on the Uno and pins 50, 51, and 52 on the Mega. 
    //On both boards, pin 10 is used to select the W5500 and pin 4 for the SD card. These pins cannot be used for general I/O. 
    //On the Mega, the hardware SS pin, 53, is not used to select either the W5500 or the SD card, 
    //pin 50, PB3
    //colResult[12] =(PINB & B00001000) == 0 ? 0 : 1;
    colResult[12] = 1;
    //pin 51, PB2
    //colResult[13] =(PINB & B00000100) == 0 ? 0 : 0;
    colResult[13] = 1;
    //pin 52, PB1
    //colResult[14] =(PINB & B00000010) == 0 ? 0 : 0;
    colResult[14] = 1;
    //pin 53, PB0
    //colResult[15] =(PINB & B00000001) == 0 ? 0 : 1;
    colResult[15] = 1;    

    
    // There are 11 Columns per row - gives a total of 176 possible inputs
    // Have left the arrays dimensioned as per original code - if CPU or Memory becomes scarce reduce array
    for ( int colid = 0; colid < 16; colid ++ )
    {
      if ( colResult[ colid ] == 0 )
      {
        joyReport.button[ (rowid * 11) + colid ] =  1;
      }
      else
      {
        joyReport.button[ (rowid * 11) + colid ] =  0;
      }
    }
  }


  FindInputChanges();


  // Radar Emergency required a pull up and then activate
  if (RadarFollowupTask == true) {
    if (millis() >= TimeRadarOn) {
      sendDcsBiosMessage("RADAR_SW","3");
      RadarFollowupTask = false;   
    }
  }

  if (RadarPushFollowupTask == true) {
    if (millis() >= TimeRadarOn) {
      sendDcsBiosMessage("RADAR_SW","2");
      RadarPushFollowupTask = false;  
    }   
  }
            

         

  currentMillis = millis();


}

void CaseTemplate() {


//        case 0:
//          break;
//        case 1:
//          break;
//        case 2:
//          break;
//        case 3:
//          break; 
//        case 4:
//          break;
//        case 5:
//          break;
//        case 6:
//          break;
//        case 7:
//          break;
//        case 8:
//          break;
//        case 9:
//          break;
//        case 10:
//          break;
//        case 11:
//          break; 
//        case 12:
//          break; 
//        case 13:
//          break;
//        case 14:
//          break; 
//        case 15:
//          break;
//        case 16:
//          break;
//        case 17:
//          break;
//        case 18:
//          break;
//        case 19:
//          break;
//        case 20:
//          break;
//        case 21:
//          break;
//        case 22:
//          break;
//        case 23:
//          break; 
//        case 24:
//          break; 
//        case 25:
//          break; 
//        case 26:
//          break;
//        case 27:
//          break;             
//        case 28:
//          break;
//        case 29:
//          break;
//        case 30:
//          break;
//        case 31:
//          break;
//        case 32:
//          break;
//        case 33:
//          break; 
//        case 34:
//          break;
//        case 35:
//          break; 
//        case 36:
//          break;             
//        case 37:
//          break;
//        case 38:
//          break;
//        case 39:
//          break;
//        case 40:
//          break;
//        case 41:
//          break;
//        case 42:
//          break;
//        case 43:
//          break;
//        case 44:
//          break;
//        case 45:
//          break; 
//        case 46:
//          break; 
//        case 47:
//          break;
//        case 48:
//          break;
//        case 49:
//          break;
//        case 50:
//          break;
//        case 51:
//          break;
//        case 52:
//          break;
//        case 53:
//          break;
//        case 54:
//          break;
//        case 55:
//          break;
//        case 56:
//          break;
//        case 57:
//          break;
//        case 58:
//          break;
//        case 59:
//          break;
//        case 60:
//          break;
//        case 61:
//          break;
//        case 62:
//          break;
//        case 63:
//          break;
//        case 64:
//          break;
//        case 65:
//          break;
//        case 66:
//          break;
//        case 67:
//          break;
//        case 68:
//          break;
//        case 69:
//          break;
//        case 70:
//          break;
//        case 71:
//          break;
//        case 72:
//          break;
//        case 73:
//          break;
//        case 74:
//          break;
//        case 75:
//          break;
//        case 76:
//          break;
//        case 77:
//          break;
//        case 78:
//          break;
//        case 79:
//          break;
//        case 80:
//          break;
//        case 81:
//          break;
//        case 82:
//          break;
//        case 83:
//          break;
//        case 84:
//          break;
//        case 85:
//          break;
//        case 86:
//          break;
//        case 87:
//          break;
//        case 88:
//          break;
//        case 89:
//          break;
//        case 90:
//          break;
//        case 91:
//          break;
//        case 92:
//          break;
//        case 93:
//          break;
//        case 94:
//          break;
//        case 95:
//          break;
//        case 96:
//          break;
//        case 97:
//          break;
//        case 98:
//          break;
//        case 99:
//          break;
//        case 100:
//          break;
//        case 101:
//          break;
//        case 102:
//          break;
//        case 103:
//          break;
//        case 104:
//          break;
//        case 105:
//          break;
//        case 106:
//          break;
//        case 107:
//          break;
//        case 108:
//          break;
//        case 109:
//          break;
//        case 110:
//          break;
//        case 111:
//          break;
//        case 112:
//          break;
//        case 113:
//          break;
//        case 114:
//          break;
//        case 115:
//          break;
//        case 116:
//          break;
//        case 117:
//          break;
//        case 118:
//          break;
//        case 119:
//          break;
//        case 120:
//          break;
//        case 121:
//          break;
//        case 122:
//          break;
//        case 123:
//          break;
//        case 124:
//          break;
//        case 125:
//          break;
//        case 126:
//          break;
//        case 127:
//          break;
//        case 128:
//          break;
//        case 129:
//          break;
//        case 130:
//          break;
//        case 131:
//          break;
//        case 132:
//          break;
//        case 133:
//          break;
//        case 134:
//          break;
//        case 135:
//          break;
//        case 136:
//          break;
//        case 137:
//          break;
//        case 138:
//          break;
//        case 139:
//          break;
//        case 140:
//          break;
//        case 141:
//          break;
//        case 142:
//          break;
//        case 143:
//          break;
//        case 144:
//          break;
//        case 145:
//          break;
//        case 146:
//          break;
//        case 147:
//          break;
//        case 148:
//          break;
//        case 149:
//          break;
//        case 150:
//          break;
//        case 151:
//          break;
//        case 152:
//          break;
//        case 153:
//          break;
//        case 154:
//          break;
//        case 155:
//          break;
//        case 156:
//          break;
//        case 157:
//          break;
//        case 158:
//          break;
//        case 159:
//          break;
//        case 160:
//          break;
//        case 161:
//          break;
//        case 162:
//          break;
//        case 163:
//          break;
//        case 164:
//          break;
//        case 165:
//          break;
//        case 166:
//          break;
//        case 167:
//          break;
//        case 168:
//          break;
//        case 169:
//          break;
//        case 170:
//          break;
//        case 171:
//          break;
//        case 172:
//          break;
//        case 173:
//          break;
//        case 174:
//          break;
//        case 175:
//          break;
//        case 176:
//          break;
//        case 177:
//          break;
//        case 178:
//          break;
//        case 179:
//          break;

}
