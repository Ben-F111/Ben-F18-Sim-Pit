/* 


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

#define Ethernet_In_Use 1
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
IPAddress ip(172,16,1,100);
String strMyIP = "X.X.X.X";

// Raspberry Pi is Target
IPAddress targetIP(172,16,1,2);
String strTargetIP = "X.X.X.X"; 


// UDP to Keyboard Arduino DUE
IPAddress keyboardIP(172,16,1,110);
String strKeyboardIP = "X.X.X.X"; 


const unsigned int localport = 7788;
const unsigned int remoteport = 49000;
const unsigned int reflectorport = 27000;
const unsigned int keyboardport = 7788;

EthernetUDP udp;
char packetBuffer[1000];     //buffer to store the incoming data
char outpacketBuffer[1000];  //buffer to store the outgoing data
char keyboardBuffer[1000];   //buffer for Keyboard traffic






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


bool FCSGainFollowupTask = false;
long timeFCSGainOn = 0;
const int ToggleSwitchCoverMoveTime = 500;

bool GenTieFollowupTask = false;
long timeGenTieOn = 0;



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


void SendRawIPMessage(String ind) {

  String outString;
  outString = String(ind); 
  
  udp.beginPacket(targetIP, reflectorport);
  udp.print(outString);
  udp.endPacket();
  
  
  udp.beginPacket(keyboardIP, keyboardport);
  udp.print(outString);
  udp.endPacket();
}


void SendDCSBIOSMessage(int ind, int state) {

  
  switch (state) {

    // RELEASE
    case 0:
      switch (ind) {
        case 0:
          //sendDcsBiosMessage("LEFT_DDI_PB_05", "0");
          break;
        case 1:
          break;
        case 2:
          sendDcsBiosMessage("COM_COMM_RELAY_SW", "1"); 
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
          sendDcsBiosMessage("STROBE_SW", "1"); 
          break;
        case 10:
          sendDcsBiosMessage("INT_WNG_TANK_SW", "1"); 
          break;
        case 11:
          break;             
        case 12:
          break; 
        case 13:
          sendDcsBiosMessage("COM_COMM_RELAY_SW", "1"); 
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
          sendDcsBiosMessage("STROBE_SW", "1"); 
          break;
        case 21:
          // Gen Tie 
          sendDcsBiosMessage("GEN_TIE_SW", "0");
          sendDcsBiosMessage("GEN_TIE_COVER", "0");
          break;
        case 22:
          break;
        case 23:
          break;             
        case 24:
          sendDcsBiosMessage("COM_CRYPTO_SW", "1");       
          break; 
        case 25:
          sendDcsBiosMessage("LDG_TAXI_SW", "1");
          break;             
        case 26:
          sendDcsBiosMessage("LAUNCH_BAR_SW", "1");
          break;
        case 27:
          break;             
        case 28:
          sendDcsBiosMessage("CMSD_DISPENSE_BTN", "0");
          break;
        case 29:
          sendDcsBiosMessage("CB_FCS_CHAN2", "1");
          break;
        case 30:
          sendDcsBiosMessage("CB_LAUNCH_BAR", "1");
          break;
        case 31:
          sendDcsBiosMessage("FCS_RESET_BTN", "0");
          break;
        case 32:
          break;
        case 33:
          break; 
        case 34:
          break;             
        case 35:
          sendDcsBiosMessage("COM_CRYPTO_SW", "1");
          break; 
        case 36:
          break;             
        case 37:
          sendDcsBiosMessage("ANTI_SKID_SW", "1");
          break;             
        case 38:
          break;
        case 39:
          sendDcsBiosMessage("CB_FCS_CHAN1", "1");
          break;
        case 40:
          sendDcsBiosMessage("CB_SPD_BRK", "1");
          break;
        case 41:
          break;
        case 42:
          // Release is shifting toggle to Norm Position
          // Cover Up = 1
          // Cover Down = 0
          // 0 Gain Switch in Normal Position
          // 1 Gain in Switch Override position
          sendDcsBiosMessage("GAIN_SWITCH", "0"); 
          sendDcsBiosMessage("GAIN_SWITCH_COVER", "0");
          break;             
        case 43:
          break;
        case 44:
          break;          
        case 45:
          break; 
        case 46:
          sendDcsBiosMessage("COM_COMM_G_XMT_SW", "1"); 
          break; 
        case 47:
          break;             
        case 48:
          break;
        case 49:
          break;
        case 50:
          sendDcsBiosMessage("ENGINE_CRANK_SW", "1"); 
          break;
        case 51:
          // APU 
          sendDcsBiosMessage("APU_CONTROL_SW", "0");
          break;
        case 52:
          sendDcsBiosMessage("MC_SW", "1"); 
          break;             
        case 53:
          break;
        case 54:
          sendDcsBiosMessage("OXY_FLOW", "0");
          break;
        case 55:
          break;
        case 56:
          break;
        case 57:
          sendDcsBiosMessage("COM_COMM_G_XMT_SW", "1");
          break;             
        case 58:
          sendDcsBiosMessage("FLAP_SW", "1");
          break;
        case 59:
          break;  
        case 60:
          break;
        case 61:
          //sendDcsBiosMessage("ENGINE_CRANK_SW", "1"); 
          SendRawIPMessage("C12,3002,0");
          break;
        case 62:
          break;
        case 63:
          sendDcsBiosMessage("MC_SW", "1");  
          break;
        case 64:
          sendDcsBiosMessage("HYD_ISOLATE_OVERRIDE_SW", "0"); 
          break;
        case 65:
          sendDcsBiosMessage("OBOGS_SW", "0");
          break;
        case 66:
          break;
        case 67:
          break;
        case 68:
          sendDcsBiosMessage("COM_IFF_MASTER_SW", "1");
          break;
        case 69:
          sendDcsBiosMessage("FLAP_SW", "1");
          break;
        case 70:
          sendDcsBiosMessage("HOOK_BYPASS_SW", "1");
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
          sendDcsBiosMessage("COM_IFF_MODE4_SW", "0");
          break;
        case 80:
          break;
        case 81:
          break;
        case 82:
          sendDcsBiosMessage("FIRE_TEST_SW", "1");
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
          break;
        case 89:
          break;
        case 90:
          sendDcsBiosMessage("COM_ILS_UFC_MAN_SW", "0");
          break;
        case 91:
          break;
        case 92:
          break;
        case 93:
          sendDcsBiosMessage("FIRE_TEST_SW", "1");
          break;
        case 94:
          break;
        case 95:
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
          break;
        case 101:
          break;
        case 102:
          break;
        case 103:
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
          sendDcsBiosMessage("PROBE_SW", "1");
          break;
        case 111:
          sendDcsBiosMessage("EXT_WNG_TANK_SW", "1");
          break;
        case 112:
          sendDcsBiosMessage("EXT_CNT_TANK_SW", "1");
          break;
        case 113:
          sendDcsBiosMessage("FUEL_DUMP_SW", "0");
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
          sendDcsBiosMessage("PROBE_SW", "1");
          break;
        case 122:
          sendDcsBiosMessage("EXT_WNG_TANK_SW", "1");
          break;
        case 123:
          sendDcsBiosMessage("EXT_CNT_TANK_SW", "1");
          break;
        case 124:
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
          sendDcsBiosMessage("COMM1_ANT_SELECT_SW", "1");
          break;
        case 133:
          sendDcsBiosMessage("IFF_ANT_SELECT_SW", "1");
          break;
        case 134:
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
          sendDcsBiosMessage("COMM1_ANT_SELECT_SW", "1");
          break;
        case 144:
          sendDcsBiosMessage("IFF_ANT_SELECT_SW", "1");
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
          break;
        case 1:
          break;
        case 2:
          sendDcsBiosMessage("COM_COMM_RELAY_SW", "0"); 
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
          sendDcsBiosMessage("STROBE_SW", "2"); 
          break;
        case 10:
          sendDcsBiosMessage("INT_WNG_TANK_SW", "0"); 
          break;
        case 11:
          break;             
        case 12:
          break; 
        case 13:
          sendDcsBiosMessage("COM_COMM_RELAY_SW", "2"); 
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
          sendDcsBiosMessage("STROBE_SW", "0"); 
          break;
        case 21:
          // Gen Tie
          sendDcsBiosMessage("GEN_TIE_COVER", "1");
          GenTieFollowupTask = true;
          timeGenTieOn = millis() + ToggleSwitchCoverMoveTime;


          break;
        case 22:
          break;
        case 23:
          break;             
        case 24:
          sendDcsBiosMessage("COM_CRYPTO_SW", "0");
          break; 
        case 25:
          sendDcsBiosMessage("LDG_TAXI_SW", "0");
          break;             
        case 26:
          sendDcsBiosMessage("LAUNCH_BAR_SW", "0");
          break;
        case 27:
          break;             
        case 28:
          sendDcsBiosMessage("CMSD_DISPENSE_BTN", "1");
          break;
        case 29:
          sendDcsBiosMessage("CB_FCS_CHAN2", "0");
          break;
        case 30:
          sendDcsBiosMessage("CB_LAUNCH_BAR", "0");
          break;
        case 31:
          sendDcsBiosMessage("FCS_RESET_BTN", "1");
          break;
        case 32:
          break;
        case 33:
          break; 
        case 34:
          break;             
        case 35:
          sendDcsBiosMessage("COM_CRYPTO_SW", "2");
          break; 
        case 36:
          break;             
        case 37:
          sendDcsBiosMessage("ANTI_SKID_SW", "0");
          break;             
        case 38:
          break;
        case 39:
          sendDcsBiosMessage("CB_FCS_CHAN1", "0");
          break;
        case 40:
          sendDcsBiosMessage("CB_SPD_BRK", "0");
          break;
        case 41:
          break;
        case 42:
          // Press is shifting toggle to Override Position      
          sendDcsBiosMessage("GAIN_SWITCH_COVER", "1");
          FCSGainFollowupTask = true;
          timeFCSGainOn = millis() + ToggleSwitchCoverMoveTime;
          
          break;             
        case 43:
          break;
        case 44:
          break;          
        case 45:
          break; 
        case 46:
          sendDcsBiosMessage("COM_COMM_G_XMT_SW", "2");
          break; 
        case 47:
          break;             
        case 48:
          break;
        case 49:
          break;
        case 50:
          //sendDcsBiosMessage("ENGINE_CRANK_SW", "0"); 
          SendRawIPMessage("LALT d");
          break;
        case 51:
          //sendDcsBiosMessage("APU_CONTROL_SW", "1");
          //SendRawIPMessage("C12,3001,1"); 
          SendRawIPMessage("LALT r");
          break;
        case 52:
          sendDcsBiosMessage("MC_SW", "2"); 
          break;             
        case 53:
          break;
        case 54:        
          sendDcsBiosMessage("OXY_FLOW", "65535");
          break;
        case 55:
          break;
        case 56:
          break;
        case 57:
          sendDcsBiosMessage("COM_COMM_G_XMT_SW", "0");
          break;             
        case 58:
          sendDcsBiosMessage("FLAP_SW", "0");
          break;
        case 59:
          break;  
        case 60:
          break;
        case 61:
          //sendDcsBiosMessage("ENGINE_CRANK_SW", "2"); 
          SendRawIPMessage("C12,3003,1");
          break;
        case 62:
          break;
        case 63:
          sendDcsBiosMessage("MC_SW", "0"); 
          break;
        case 64:
         sendDcsBiosMessage("HYD_ISOLATE_OVERRIDE_SW", "1"); 
          break;
        case 65:
          sendDcsBiosMessage("OBOGS_SW", "1");
          break;
        case 66:
          break;
        case 67:
          break;
        case 68:
          sendDcsBiosMessage("COM_IFF_MASTER_SW", "0");
          break;
        case 69:
          sendDcsBiosMessage("FLAP_SW", "2");
          break;
        case 70:
          sendDcsBiosMessage("HOOK_BYPASS_SW", "0");
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
          sendDcsBiosMessage("COM_IFF_MODE4_SW", "2");
          break;
        case 80:
          break;
        case 81:
          break;
        case 82:
          sendDcsBiosMessage("FIRE_TEST_SW", "2");
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
          break;
        case 89:
          break;
        case 90:
          sendDcsBiosMessage("COM_ILS_UFC_MAN_SW", "1");
          break;
        case 91:
          break;
        case 92:
          break;
        case 93:
          sendDcsBiosMessage("FIRE_TEST_SW", "0");
          break;
        case 94:
          break;
        case 95:
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
          break;
        case 101:
          break;
        case 102:
          break;
        case 103:
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
          sendDcsBiosMessage("PROBE_SW", "0");
          break;
        case 111:
          sendDcsBiosMessage("EXT_WNG_TANK_SW", "2");
          break;
        case 112:
          sendDcsBiosMessage("EXT_CNT_TANK_SW", "2");
          break;
        case 113:
          sendDcsBiosMessage("FUEL_DUMP_SW", "1");
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
          sendDcsBiosMessage("PROBE_SW", "2");
          break;
        case 122:
          sendDcsBiosMessage("EXT_WNG_TANK_SW", "0");
          break;
        case 123:
          sendDcsBiosMessage("EXT_CNT_TANK_SW", "0");
          break;
        case 124:
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
          sendDcsBiosMessage("COMM1_ANT_SELECT_SW", "2");
          break;
        case 133:
          sendDcsBiosMessage("IFF_ANT_SELECT_SW", "2");
          break;
        case 134:
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
          sendDcsBiosMessage("COMM1_ANT_SELECT_SW", "0");
          break;
        case 144:
          sendDcsBiosMessage("IFF_ANT_SELECT_SW", "0");
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
      break; 
     }
  }  
}


// Comm Pots
DcsBios::PotentiometerEWMA<5, 128, 5> comVox("COM_VOX", 0);
DcsBios::PotentiometerEWMA<5, 128, 5> comIcs("COM_ICS", 1);
DcsBios::PotentiometerEWMA<5, 128, 5> comRwr("COM_RWR", 2);
DcsBios::PotentiometerEWMA<5, 128, 5> comMidsA("COM_MIDS_A", 3);
DcsBios::PotentiometerEWMA<5, 128, 5> comMidsB("COM_MIDS_B", 4);
DcsBios::PotentiometerEWMA<5, 128, 5> comTacan("COM_TACAN", 5);
DcsBios::PotentiometerEWMA<5, 128, 5> comAux("COM_AUX", 12);
DcsBios::PotentiometerEWMA<5, 128, 5> comWpn("COM_WPN", 13);
DcsBios::PotentiometerEWMA<5, 128, 5> positionDimmer("POSITION_DIMMER", 15);

// Ext Lights Pots
DcsBios::PotentiometerEWMA<5, 128, 5> formationDimmer("FORMATION_DIMMER", 8);


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


  // Handle Switches with Safety covers
  if (FCSGainFollowupTask == true) {
    if (millis() >= timeFCSGainOn) {
      sendDcsBiosMessage("GAIN_SWITCH", "1");
      FCSGainFollowupTask = false;   
    }
  }

  if (GenTieFollowupTask == true) {
    if (millis() >= timeGenTieOn) {
      sendDcsBiosMessage("GEN_TIE_SW", "1");
      GenTieFollowupTask = false;
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
