/*

  ////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\
  //||                  FUNCTION = HORNET FORWARD INPUT                 ||\\
  //||              LOCATION IN THE PIT = LIP LEFTHAND SIDE             ||\\
  //||            ARDUINO PROCESSOR TYPE = Arduino Mega 2560            ||\\
  //||      ARDUINO CHIP SERIAL NUMBER = SN - 85937313337351719052      ||\\
  //||      ETHERNET SHEILD MAC ADDRESS = MAC - A8:61:0A:AE:83:18       ||\\
  //||                    CONNECTED COM PORT = COM 6                    ||\\
  //||            ****DO CHECK S/N BEFORE UPLOAD NEW DATA****           ||\\
  ////////////////////---||||||||||********||||||||||---\\\\\\\\\\\\\\\\\\\\

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
byte mac[] = {0x00, 0xDD, 0x3E, 0xCA, 0x36, 0x99};
IPAddress ip(172, 16, 1, 100);
String strMyIP = "X.X.X.X";

// Raspberry Pi is Target
IPAddress targetIP(172, 16, 1, 2);
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

const int ScanDelay = 80;
const int DebounceDelay = 20;

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

bool SpinFollowupTask = false; // Spin Button Cover
bool LFBCFollowupTask = false; // Left Fire Button Cover
bool RFBCFollowupTask = false; // Right Fire Button Cover
long timeSpinOn = 0; // Spin Button Cover
long timeLFBCOn = 0; // Left Fire Button Cover
long timeRFBCOn = 0;// Right Fire Button Cover
const int ToggleSwitchCoverMoveTime = 500;

void setup() {

  // Set the output ports to output
  for ( int portId = 22; portId < 38; portId ++ )
  {
    pinMode( portId, OUTPUT );
  }

  // Set the input ports to input - port 50-53 used by Ethernet Shield
  for ( int portId = 38; portId < 50; portId ++ )
  {
    // Even though we've already got 10K external pullups
    pinMode( portId, INPUT_PULLUP);
  }

  // Initialise all arrays
  for (int ind = 0; ind < NUM_BUTTONS ; ind++) {

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

  for (int ind = 0; ind < NUM_BUTTONS; ind++)
    if (bFirstTime) {

      bFirstTime = false;
      // Just Copy Array and perform no actions - this may change in the future
      prevjoyReport.button[ind] = joyReport.button[ind];
    }
    else {
      // Not the first time - see if there is a difference from last time
      // If there is perform action and update prev array BUT only if we past the end of the debounce period
      if ( prevjoyReport.button[ind] != joyReport.button[ind] && millis() > joyEndDebounce[ind] ) {

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
    case 0:
      switch (ind) {
        case 0:
          sendDcsBiosMessage("LEFT_DDI_PB_05", "0");
          break;
        case 1:
          sendDcsBiosMessage("LEFT_DDI_PB_20", "0");
          break;
        case 2:
          sendDcsBiosMessage("LEFT_DDI_PB_15", "0");
          break;
        case 3:
          sendDcsBiosMessage("LEFT_DDI_PB_10", "0");
          break;
        case 4: // USED BELOW
          break;
        case 5:
          sendDcsBiosMessage("RIGHT_DDI_PB_05", "0");
          break;
        case 6:
          sendDcsBiosMessage("RIGHT_DDI_PB_20", "0");
          break;
        case 7:
          sendDcsBiosMessage("RIGHT_DDI_PB_15", "0");
          break;
        case 8:
          sendDcsBiosMessage("RIGHT_DDI_PB_10", "0");
          break;
        case 9: // USED BELOW
          break;
        case 10:
          sendDcsBiosMessage("LEFT_DDI_HDG_SW", "1");
          break;
        case 11:
          sendDcsBiosMessage("LEFT_DDI_PB_04", "0");
          break;
        case 12:
          sendDcsBiosMessage("LEFT_DDI_PB_19", "0");
          break;
        case 13:
          sendDcsBiosMessage("LEFT_DDI_PB_14", "0");
          break;
        case 14:
          sendDcsBiosMessage("LEFT_DDI_PB_09", "0");
          break;
        case 15: // USED BELOW
          break;
        case 16:
          sendDcsBiosMessage("RIGHT_DDI_PB_04", "0");
          break;
        case 17:
          sendDcsBiosMessage("RIGHT_DDI_PB_19", "0");
          break;
        case 18:
          sendDcsBiosMessage("RIGHT_DDI_PB_14", "0");
          break;
        case 19:
          sendDcsBiosMessage("RIGHT_DDI_PB_09", "0");
          break;
        case 20: // USED BELOW
          break;
        case 21:
          sendDcsBiosMessage("LEFT_DDI_HDG_SW", "1");
          break;
        case 22:
          sendDcsBiosMessage("LEFT_DDI_PB_03", "0");
          break;
        case 23:
          sendDcsBiosMessage("LEFT_DDI_PB_18", "0");
          break;
        case 24:
          sendDcsBiosMessage("LEFT_DDI_PB_13", "0");
          break;
        case 25:
          sendDcsBiosMessage("LEFT_DDI_PB_08", "0");
          break;
        case 26: // USED BELOW
          break;
        case 27:
          sendDcsBiosMessage("RIGHT_DDI_PB_03", "0");
          break;
        case 28:
          sendDcsBiosMessage("RIGHT_DDI_PB_18", "0");
          break;
        case 29:
          sendDcsBiosMessage("RIGHT_DDI_PB_13", "0");
          break;
        case 30:
          sendDcsBiosMessage("RIGHT_DDI_PB_08", "0");
          break;
        case 31: // USED BELOW
          break;
        case 32:
          sendDcsBiosMessage("UFC_ILS", "0");
          break;
        case 33:
          sendDcsBiosMessage("LEFT_DDI_PB_02", "0");
          break;
        case 34:
          sendDcsBiosMessage("LEFT_DDI_PB_17", "0");
          break;
        case 35:
          sendDcsBiosMessage("LEFT_DDI_PB_12", "0");
          break;
        case 36:
          sendDcsBiosMessage("LEFT_DDI_PB_07", "0");
          break;
        case 37:
          sendDcsBiosMessage("UFC_COMM2_PULL", "0");
          break;
        case 38:
          sendDcsBiosMessage("RIGHT_DDI_PB_02", "0");
          break;
        case 39:
          sendDcsBiosMessage("RIGHT_DDI_PB_17", "0");
          break;
        case 40:
          sendDcsBiosMessage("RIGHT_DDI_PB_12", "0");
          break;
        case 41:
          sendDcsBiosMessage("RIGHT_DDI_PB_07", "0");
          break;
        case 42:
          sendDcsBiosMessage("MASTER_CAUTION_RESET_SW", "0");
          break;
        case 43:
          sendDcsBiosMessage("LEFT_FIRE_BTN", "0");
          sendDcsBiosMessage("LEFT_FIRE_BTN_COVER", "0");
          break;
        case 44:
          sendDcsBiosMessage("LEFT_DDI_PB_01", "0");
          break;
        case 45:
          sendDcsBiosMessage("LEFT_DDI_PB_16", "0");
          break;
        case 46:
          sendDcsBiosMessage("LEFT_DDI_PB_11", "0");
          break;
        case 47:
          sendDcsBiosMessage("LEFT_DDI_PB_06", "0");
          break;
        case 48:
          sendDcsBiosMessage("UFC_COMM1_PULL", "0");
          break;
        case 49:
          sendDcsBiosMessage("RIGHT_DDI_PB_01", "0");
          break;
        case 50:
          sendDcsBiosMessage("RIGHT_DDI_PB_16", "0");
          break;
        case 51:
          sendDcsBiosMessage("RIGHT_DDI_PB_11", "0");
          break;
        case 52:
          sendDcsBiosMessage("RIGHT_DDI_PB_06", "0");
          break;
        case 53:
          sendDcsBiosMessage("APU_FIRE_BTN", "0");
          break;
        case 54:
          sendDcsBiosMessage("RIGHT_FIRE_BTN", "0");
          sendDcsBiosMessage("RIGHT_FIRE_BTN_COVER", "0");
          break;
        case 55:
          sendDcsBiosMessage("AMPCD_PB_05", "0");
          break;
        case 56:
          sendDcsBiosMessage("AMPCD_PB_20", "0");
          break;
        case 57:
          sendDcsBiosMessage("AMPCD_PB_15", "0");
          break;
        case 58:
          sendDcsBiosMessage("AMPCD_PB_10", "0");
          break;
        case 59:
          sendDcsBiosMessage("UFC_1", "0");
          break;
        case 60:
          sendDcsBiosMessage("UFC_2", "0");
          break;
        case 61:
          sendDcsBiosMessage("UFC_3", "0");
          break;
        case 62:
          sendDcsBiosMessage("UFC_OS2", "0");
          break;
        case 63:
          sendDcsBiosMessage("UFC_AP", "0");
          break;
        case 64:
          sendDcsBiosMessage("UFC_IFF", "0");
          break;
        case 65:
          sendDcsBiosMessage("UFC_TCN", "0");
          break;
        case 66:
          sendDcsBiosMessage("AMPCD_PB_04", "0");
          break;
        case 67:
          sendDcsBiosMessage("AMPCD_PB_19", "0");
          break;
        case 68:
          sendDcsBiosMessage("AMPCD_PB_14", "0");
          break;
        case 69:
          sendDcsBiosMessage("AMPCD_PB_09", "0");
          break;
        case 70:
          sendDcsBiosMessage("UFC_4", "0");
          break;
        case 71:
          sendDcsBiosMessage("UFC_5", "0");
          break;
        case 72:
          sendDcsBiosMessage("UFC_6", "0");
          break;
        case 73:
          sendDcsBiosMessage("UFC_OS3", "0");
          break;
        case 74:
          sendDcsBiosMessage("UFC_ONOFF", "0");
          break;
        case 75:
          sendDcsBiosMessage("UFC_BCN", "0");
          break;
        case 76:
          sendDcsBiosMessage("UFC_DL", "0");
          break;
        case 77:
          sendDcsBiosMessage("AMPCD_PB_03", "0");
          break;
        case 78:
          sendDcsBiosMessage("AMPCD_PB_18", "0");
          break;
        case 79:
          sendDcsBiosMessage("AMPCD_PB_13", "0");
          break;
        case 80:
          sendDcsBiosMessage("AMPCD_PB_08", "0");
          break;
        case 81:
          sendDcsBiosMessage("UFC_7", "0");
          break;
        case 82:
          sendDcsBiosMessage("UFC_8", "0");
          break;
        case 83:
          sendDcsBiosMessage("UFC_9", "0");
          break;
        case 84:
          sendDcsBiosMessage("UFC_OS4", "0");
          break;
        case 85: // USED BELOW
          break;
        case 86:
          sendDcsBiosMessage("CMSD_DISPENSE_SW", "1");
          break;
        case 87: // EMC, IS THIS USED
          break;
        case 88:
          sendDcsBiosMessage("AMPCD_PB_02", "0");
          break;
        case 89:
          sendDcsBiosMessage("AMPCD_PB_17", "0");
          break;
        case 90:
          sendDcsBiosMessage("AMPCD_PB_12", "0");
          break;
        case 91:
          sendDcsBiosMessage("AMPCD_PB_07", "0");
          break;
        case 92:
          sendDcsBiosMessage("UFC_CLR", "0");
          break;
        case 93:
          sendDcsBiosMessage("UFC_0", "0");
          break;
        case 94:
          sendDcsBiosMessage("UFC_ENT", "0");
          break;
        case 95:
          sendDcsBiosMessage("UFC_OS5", "0");
          break;
        case 96: // USED BELOW
          break;
        case 97:
          sendDcsBiosMessage("CMSD_DISPENSE_SW", "1");
          break;
        case 98: // EMC, IS THIS USED
          break;
        case 99:
          sendDcsBiosMessage("AMPCD_PB_01", "0");
          break;
        case 100:
          sendDcsBiosMessage("AMPCD_PB_16", "0");
          break;
        case 101:
          sendDcsBiosMessage("AMPCD_PB_11", "0");
          break;
        case 102:
          sendDcsBiosMessage("AMPCD_PB_06", "0");
          break;
        case 103:
          sendDcsBiosMessage("UFC_ADF", "1");
          break;
        case 104:
          sendDcsBiosMessage("UFC_IP", "0");
          break;
        case 105:
          sendDcsBiosMessage("UFC_OS1", "0");
          break;
        case 106:
          sendDcsBiosMessage("UFC_EMCON", "0");
          break;
        case 107: // USED BELOW
          break;
        case 108:
          sendDcsBiosMessage("AUX_REL_SW", "1"); // WIRED BACKWARDS IN MY PIT
          break;
        case 109:
          sendDcsBiosMessage("SAI_TEST_BTN", "0");
          break;
        case 110:
          sendDcsBiosMessage("AMPCD_GAIN_SW", "1");
          break;
        case 111:
          sendDcsBiosMessage("AMPCD_CONT_SW", "1");
          break;
        case 112:
          sendDcsBiosMessage("AMPCD_SYM_SW", "1");
          break;
        case 113:
          sendDcsBiosMessage("AMPCD_NIGHT_DAY", "1");
          break;
        case 114:
          sendDcsBiosMessage("UFC_ADF", "1");
          break;
        case 115:
          sendDcsBiosMessage("HUD_VIDEO_BIT", "0");
          break;
        case 116: //FA-18C_hornet/FIRE_EXT_BTN
          sendDcsBiosMessage("FIRE_EXT_BTN", "0");
          break;
        case 117: //FA-18C_hornet/MASTER_MODE_AG
          sendDcsBiosMessage("MASTER_MODE_AG", "0");
          break;
        case 118: // USED BELOW
          break;
        case 119: //EMC, IS THIS USED
          break;
        case 120:
          sendDcsBiosMessage("SAI_CAGE", "0");
          break;
        case 121:
          sendDcsBiosMessage("AMPCD_GAIN_SW", "1");
          break;
        case 122:
          sendDcsBiosMessage("AMPCD_CONT_SW", "1");
          break;
        case 123:
          sendDcsBiosMessage("AMPCD_SYM_SW", "1");
          break;
        case 124:
          sendDcsBiosMessage("AMPCD_NIGHT_DAY", "1");
          break;
        case 125:
          sendDcsBiosMessage("RWR_BIT_BTN", "0");
          break;
        case 126: // USED BELOW
          break;
        case 127: //FA-18C_hornet/MASTER_MODE_AA
          sendDcsBiosMessage("MASTER_MODE_AA", "0");
          break;
        case 128: // USED BELOW
          break;
        case 129: // USED BELOW
          break;
        case 130: //EMC, IS THIS USED
          break;
        case 131: // NOT USED
          break;
        case 132:
          sendDcsBiosMessage("AMPCD_BRT_CTL", "0");
          break;
        case 133: // AMPCD, IS THIS USED
          break;
        case 134: // AMPCD, IS THIS USED
          break;
        case 135: // AMPCD, IS THIS USED
          break;
        case 136:
          sendDcsBiosMessage("RWR_OFFSET_BTN", "0");
          break;
        case 137: // USED BELOW
          break;
        case 138:
          sendDcsBiosMessage("EMER_JETT_BTN", "0");
          break;
        case 139:
          sendDcsBiosMessage("MASTER_ARM_SW", "0");
          break;
        case 140: //FA-18C_hornet/HUD_ATT_SW
          sendDcsBiosMessage("HUD_ATT_SW", "0"); //1 FOR OFF
          break;
        case 141://FA-18C_hornet/HUD_VIDEO_CONTROL_SW
          sendDcsBiosMessage("HUD_VIDEO_CONTROL_SW", "0");  //1 FOR OFF
          break;
        case 142: //FA-18C_hornet/HUD_SYM_REJ_SW
          sendDcsBiosMessage("HUD_SYM_REJ_SW", "0"); //1 FOR OFF
          break;
        case 143: //FA-18C_hornet/IFEI_MODE_BTN
          sendDcsBiosMessage("IFEI_MODE_BTN", "0");
          break;
        case 144: //FA-18C_hornet/IFEI_DWN_BTN
          sendDcsBiosMessage("IFEI_DWN_BTN", "0");
          break;
        case 145: //IFEI, IS THIS USED
          break;
        case 146: //FA-18C_hornet/MODE_SELECTOR_SW
          sendDcsBiosMessage("MODE_SELECTOR_SW", "1"); //1 FOR OFF
          break;
        case 147:
          sendDcsBiosMessage("RWR_SPECIAL_BTN", "0");
          break;
        case 148: // USED BELOW
          break;
        case 149: //FA-18C_hornet/SPIN_RECOVERY_SW
          sendDcsBiosMessage("SPIN_RECOVERY_SW", "0");
          //FA-18C_hornet/SPIN_RECOVERY_COVER
          sendDcsBiosMessage("SPIN_RECOVERY_COVER", "0");
          break;
        case 150: //FA-18C_hornet/IR_COOL_SW
          sendDcsBiosMessage("IR_COOL_SW", "1");
          break;
        case 151: //FA-18C_hornet/HUD_ATT_SW
          sendDcsBiosMessage("HUD_ATT_SW", "2"); //1 FOR OFF
          break;
        case 152: //FA-18C_hornet/HUD_VIDEO_CONTROL_SW
          sendDcsBiosMessage("HUD_VIDEO_CONTROL_SW", "2"); //1 FOR OFF
          break;
        case 153: //FA-18C_hornet/HUD_SYM_REJ_SW
          sendDcsBiosMessage("HUD_SYM_REJ_SW", "2"); //1 FOR OFF
          break;
        case 154: //FA-18C_hornet/IFEI_QTY_BTN
          sendDcsBiosMessage("IFEI_QTY_BTN", "0");
          break;
        case 155: //FA-18C_hornet/IFEI_ZONE_BTN
          sendDcsBiosMessage("IFEI_ZONE_BTN", "0");
          break;
        case 156: //FA-18C_hornet/SELECT_HMD_LDDI_RDDI
          sendDcsBiosMessage("SELECT_HMD_LDDI_RDDI", "0"); // NEEDS WORK
          break;
        case 157: //FA-18C_hornet/MODE_SELECTOR_SW
          sendDcsBiosMessage("MODE_SELECTOR_SW", "1"); //1 FOR OFF
          break;
        case 158:
          sendDcsBiosMessage("RWR_DISPLAY_BTN", "0");
          break;
        case 159: // USED BELOW
          break;
        case 160: //SPIN, IS THIS USED
          break;
        case 161: ////FA-18C_hornet/IR_COOL_SW
          sendDcsBiosMessage("IR_COOL_SW", "1");
          break;
        case 162: //FA-18C_hornet/HUD_ALT_SW
          sendDcsBiosMessage("HUD_ALT_SW", "0");
          break;
        case 163: //HUD, IS THIS USED
          break;
        case 164: //FA-18C_hornet/HUD_SYM_BRT_SELECT
          sendDcsBiosMessage("HUD_SYM_BRT_SELECT", "0");
          break;
        case 165: //FA-18C_hornet/IFEI_UP_BTN
          sendDcsBiosMessage("IFEI_UP_BTN", "0");
          break;
        case 166: //FA-18C_hornet/IFEI_ET_BTN
          sendDcsBiosMessage("IFEI_ET_BTN", "0");
          break;
        case 167:
          sendDcsBiosMessage("LEFT_DDI_CRS_SW", "1");
          break;
        case 168:
          sendDcsBiosMessage("LEFT_DDI_CRS_SW", "10");
          break;
        case 169:
          sendDcsBiosMessage("RWR_POWER_BTN", "0");
          // ######### PETE TO ADD LATCH #########
          break;
        case 170: // USED BELOW
          break;
        case 171: //FA-18C_hornet/SJ_CTR
          sendDcsBiosMessage("SJ_CTR", "0");
          break;
        case 172: //FA-18C_hornet/SJ_RI
          sendDcsBiosMessage("SJ_RI", "0");
          break;
        case 173: //FA-18C_hornet/SJ_LI
          sendDcsBiosMessage("SJ_LI", "0");
          break;
        case 174: //FA-18C_hornet/SJ_RO
          sendDcsBiosMessage("SJ_RO", "0");
          break;
        case 175: //FA-18C_hornet/SJ_LO
          sendDcsBiosMessage("SJ_LO", "0");
          break;
        case 176: // NOT USED
          break;
        case 177: // NOT USED
          break;
        case 178: // NOT USED
          break;
        case 179: // NOT USED
          break;
        default:
          sendDcsBiosMessage("LIGHTS_TEST_SW", "0");
          break;
      }
      break;
    case 1:
      switch (ind) {
        case 0:
          sendDcsBiosMessage("LEFT_DDI_PB_05", "1");
          break;
        case 1:
          sendDcsBiosMessage("LEFT_DDI_PB_20", "1");
          break;
        case 2:
          sendDcsBiosMessage("LEFT_DDI_PB_15", "1");
          break;
        case 3:
          sendDcsBiosMessage("LEFT_DDI_PB_10", "1");
          break;
        case 4:
          sendDcsBiosMessage("LEFT_DDI_BRT_SELECT", "0");
          break;
        case 5:
          sendDcsBiosMessage("RIGHT_DDI_PB_05", "1");
          break;
        case 6:
          sendDcsBiosMessage("RIGHT_DDI_PB_20", "1");
          break;
        case 7:
          sendDcsBiosMessage("RIGHT_DDI_PB_15", "1");
          break;
        case 8:
          sendDcsBiosMessage("RIGHT_DDI_PB_10", "1");
          break;
        case 9:
          sendDcsBiosMessage("RIGHT_DDI_BRT_SELECT", "0");
          break;
        case 10:
          sendDcsBiosMessage("LEFT_DDI_HDG_SW", "0");
          break;
        case 11:
          sendDcsBiosMessage("LEFT_DDI_PB_04", "1");
          break;
        case 12:
          sendDcsBiosMessage("LEFT_DDI_PB_19", "1");
          break;
        case 13:
          sendDcsBiosMessage("LEFT_DDI_PB_14", "1");
          break;
        case 14:
          sendDcsBiosMessage("LEFT_DDI_PB_09", "1");
          break;
        case 15:
          sendDcsBiosMessage("LEFT_DDI_BRT_SELECT", "1");
          break;
        case 16:
          sendDcsBiosMessage("RIGHT_DDI_PB_04", "1");
          break;
        case 17:
          sendDcsBiosMessage("RIGHT_DDI_PB_19", "1");
          break;
        case 18:
          sendDcsBiosMessage("RIGHT_DDI_PB_14", "1");
          break;
        case 19:
          sendDcsBiosMessage("RIGHT_DDI_PB_09", "1");
          break;
        case 20:
          sendDcsBiosMessage("RIGHT_DDI_BRT_SELECT", "1");
          break;
        case 21:
          sendDcsBiosMessage("LEFT_DDI_HDG_SW", "2");
          break;
        case 22:
          sendDcsBiosMessage("LEFT_DDI_PB_03", "1");
          break;
        case 23:
          sendDcsBiosMessage("LEFT_DDI_PB_18", "1");
          break;
        case 24:
          sendDcsBiosMessage("LEFT_DDI_PB_13", "1");
          break;
        case 25:
          sendDcsBiosMessage("LEFT_DDI_PB_08", "1");
          break;
        case 26:
          sendDcsBiosMessage("LEFT_DDI_BRT_SELECT", "2");
          break;
        case 27:
          sendDcsBiosMessage("RIGHT_DDI_PB_03", "1");
          break;
        case 28:
          sendDcsBiosMessage("RIGHT_DDI_PB_18", "1");
          break;
        case 29:
          sendDcsBiosMessage("RIGHT_DDI_PB_13", "1");
          break;
        case 30:
          sendDcsBiosMessage("RIGHT_DDI_PB_08", "1");
          break;
        case 31:
          sendDcsBiosMessage("RIGHT_DDI_BRT_SELECT", "2");
          break;
        case 32:
          sendDcsBiosMessage("UFC_ILS", "1");
          break;
        case 33:
          sendDcsBiosMessage("LEFT_DDI_PB_02", "1");
          break;
        case 34:
          sendDcsBiosMessage("LEFT_DDI_PB_17", "1");
          break;
        case 35:
          sendDcsBiosMessage("LEFT_DDI_PB_12", "1");
          break;
        case 36:
          sendDcsBiosMessage("LEFT_DDI_PB_07", "1");
          break;
        case 37:
          sendDcsBiosMessage("UFC_COMM2_PULL", "1");
          break;
        case 38:
          sendDcsBiosMessage("RIGHT_DDI_PB_02", "1");
          break;
        case 39:
          sendDcsBiosMessage("RIGHT_DDI_PB_17", "1");
          break;
        case 40:
          sendDcsBiosMessage("RIGHT_DDI_PB_12", "1");
          break;
        case 41:
          sendDcsBiosMessage("RIGHT_DDI_PB_07", "1");
          break;
        case 42:
          sendDcsBiosMessage("MASTER_CAUTION_RESET_SW", "1");
          break;
        case 43:
          sendDcsBiosMessage("LEFT_FIRE_BTN_COVER", "1");
          LFBCFollowupTask = true;
          timeLFBCOn = millis() + ToggleSwitchCoverMoveTime;
          break;
        case 44:
          sendDcsBiosMessage("LEFT_DDI_PB_01", "1");
          break;
        case 45:
          sendDcsBiosMessage("LEFT_DDI_PB_16", "1");
          break;
        case 46:
          sendDcsBiosMessage("LEFT_DDI_PB_11", "1");
          break;
        case 47:
          sendDcsBiosMessage("LEFT_DDI_PB_06", "1");
          break;
        case 48:
          sendDcsBiosMessage("UFC_COMM1_PULL", "1");
          break;
        case 49:
          sendDcsBiosMessage("RIGHT_DDI_PB_01", "1");
          break;
        case 50:
          sendDcsBiosMessage("RIGHT_DDI_PB_16", "1");
          break;
        case 51:
          sendDcsBiosMessage("RIGHT_DDI_PB_11", "1");
          break;
        case 52:
          sendDcsBiosMessage("RIGHT_DDI_PB_06", "1");
          break;
        case 53:
          sendDcsBiosMessage("APU_FIRE_BTN", "1");
          break;
        case 54:
          sendDcsBiosMessage("RIGHT_FIRE_BTN_COVER", "1");
          RFBCFollowupTask = true;
          timeRFBCOn = millis() + ToggleSwitchCoverMoveTime;
          break;
        case 55:
          sendDcsBiosMessage("AMPCD_PB_05", "1");
          break;
        case 56:
          sendDcsBiosMessage("AMPCD_PB_20", "1");
          break;
        case 57:
          sendDcsBiosMessage("AMPCD_PB_15", "1");
          break;
        case 58:
          sendDcsBiosMessage("AMPCD_PB_10", "1");
          break;
        case 59:
          sendDcsBiosMessage("UFC_1", "1");
          break;
        case 60:
          sendDcsBiosMessage("UFC_2", "1");
          break;
        case 61:
          sendDcsBiosMessage("UFC_3", "1");
          break;
        case 62:
          sendDcsBiosMessage("UFC_OS2", "1");
          break;
        case 63:
          sendDcsBiosMessage("UFC_AP", "1");
          break;
        case 64:
          sendDcsBiosMessage("UFC_IFF", "1");
          break;
        case 65:
          sendDcsBiosMessage("UFC_TCN", "1");
          break;
        case 66:
          sendDcsBiosMessage("AMPCD_PB_04", "1");
          break;
        case 67:
          sendDcsBiosMessage("AMPCD_PB_19", "1");
          break;
        case 68:
          sendDcsBiosMessage("AMPCD_PB_14", "1");
          break;
        case 69:
          sendDcsBiosMessage("AMPCD_PB_09", "1");
          break;
        case 70:
          sendDcsBiosMessage("UFC_4", "1");
          break;
        case 71:
          sendDcsBiosMessage("UFC_5", "1");
          break;
        case 72:
          sendDcsBiosMessage("UFC_6", "1");
          break;
        case 73:
          sendDcsBiosMessage("UFC_OS3", "1");
          break;
        case 74:
          sendDcsBiosMessage("UFC_ONOFF", "1");
          break;
        case 75:
          sendDcsBiosMessage("UFC_BCN", "1");
          break;
        case 76:
          sendDcsBiosMessage("UFC_DL", "1");
          break;
        case 77:
          sendDcsBiosMessage("AMPCD_PB_03", "1");
          break;
        case 78:
          sendDcsBiosMessage("AMPCD_PB_18", "1");
          break;
        case 79:
          sendDcsBiosMessage("AMPCD_PB_13", "1");
          break;
        case 80:
          sendDcsBiosMessage("AMPCD_PB_08", "1");
          break;
        case 81:
          sendDcsBiosMessage("UFC_7", "1");
          break;
        case 82:
          sendDcsBiosMessage("UFC_8", "1");
          break;
        case 83:
          sendDcsBiosMessage("UFC_9", "1");
          break;
        case 84:
          sendDcsBiosMessage("UFC_OS4", "1");
          break;
        case 85:
          sendDcsBiosMessage("ECM_MODE_SW", "0"); // OFF
          break;
        case 86:
          sendDcsBiosMessage("CMSD_DISPENSE_SW", "0");
          break;
        case 87: // ECM, IS THIS USED
          break;
        case 88:
          sendDcsBiosMessage("AMPCD_PB_02", "1");
          break;
        case 89:
          sendDcsBiosMessage("AMPCD_PB_17", "1");
          break;
        case 90:
          sendDcsBiosMessage("AMPCD_PB_12", "1");
          break;
        case 91:
          sendDcsBiosMessage("AMPCD_PB_07", "1");
          break;
        case 92:
          sendDcsBiosMessage("UFC_CLR", "1");
          break;
        case 93:
          sendDcsBiosMessage("UFC_0", "1");
          break;
        case 94:
          sendDcsBiosMessage("UFC_ENT", "1");
          break;
        case 95:
          sendDcsBiosMessage("UFC_OS5", "2");
          break;
        case 96:
          sendDcsBiosMessage("ECM_MODE_SW", "1"); // STBY
          break;
        case 97:
          sendDcsBiosMessage("CMSD_DISPENSE_SW", "2");
          break;
        case 98: // EMC, IS THIS USED
          break;
        case 99:
          sendDcsBiosMessage("AMPCD_PB_01", "1");
          break;
        case 100:
          sendDcsBiosMessage("AMPCD_PB_16", "1");
          break;
        case 101:
          sendDcsBiosMessage("AMPCD_PB_11", "1");
          break;
        case 102:
          sendDcsBiosMessage("AMPCD_PB_06", "1");
          break;
        case 103:
          sendDcsBiosMessage("UFC_ADF", "2");
          break;
        case 104:
          sendDcsBiosMessage("UFC_IP", "1");
          break;
        case 105:
          sendDcsBiosMessage("UFC_OS1", "1");
          break;
        case 106:
          sendDcsBiosMessage("UFC_EMCON", "1");
          break;
        case 107:
          sendDcsBiosMessage("ECM_MODE_SW", "2"); // BIT
          break;
        case 108:
          sendDcsBiosMessage("AUX_REL_SW", "0"); // WIRED BACKWARDS IN MY PIT
          break;
        case 109:
          sendDcsBiosMessage("SAI_TEST_BTN", "1");
          break;
        case 110:
          sendDcsBiosMessage("AMPCD_GAIN_SW", "2");
          break;
        case 111:
          sendDcsBiosMessage("AMPCD_CONT_SW", "2");
          break;
        case 112:
          sendDcsBiosMessage("AMPCD_SYM_SW", "2");
          break;
        case 113:
          sendDcsBiosMessage("AMPCD_NIGHT_DAY", "2");
          break;
        case 114:
          sendDcsBiosMessage("UFC_ADF", "0");
          break;
        case 115:
          sendDcsBiosMessage("HUD_VIDEO_BIT", "1");
          break;
        case 116:
          sendDcsBiosMessage("FIRE_EXT_BTN", "1");
          break;
        case 117:
          sendDcsBiosMessage("MASTER_MODE_AG", "1");
          break;
        case 118:
          sendDcsBiosMessage("ECM_MODE_SW", "3"); // REC
          break;
        case 119: // EMC, IS THIS USED
          break;
        case 120:
          sendDcsBiosMessage("SAI_CAGE", "1");
          break;
        case 121:
          sendDcsBiosMessage("AMPCD_GAIN_SW", "0");
          break;
        case 122:
          sendDcsBiosMessage("AMPCD_CONT_SW", "0");
          break;
        case 123:
          sendDcsBiosMessage("AMPCD_SYM_SW", "0");
          break;
        case 124:
          sendDcsBiosMessage("AMPCD_NIGHT_DAY", "0");
          break;
        case 125:
          sendDcsBiosMessage("RWR_BIT_BTN", "1");
          break;
        case 126:
          sendDcsBiosMessage("RWR_DIS_TYPE_SW", "3"); // "U"
          // CHECK INDIVIDUAL ASSIGNMENTS PER PIT
          break;
        case 127:
          sendDcsBiosMessage("MASTER_MODE_AA", "1");
          break;
        case 128: // MASTER ARM, IS THIS USED
          break;
        case 129:
          sendDcsBiosMessage("ECM_MODE_SW", "4"); // XMIT
          break;
        case 130: // EMC, IS THIS USED
          break;
        case 131: // NOT USED
          break;
        case 132:
          sendDcsBiosMessage("AMPCD_BRT_CTL", "1");
          break;
        case 133: // AMPCD, IS THIS USED
          break;
        case 134: // AMPCD, IS THIS USED
          break;
        case 135: // AMPCD, IS THIS USED
          break;
        case 136:
          sendDcsBiosMessage("RWR_OFFSET_BTN", "1");
          break;
        case 137:
          sendDcsBiosMessage("RWR_DIS_TYPE_SW", "2"); // "A"
          // CHECK INDIVIDUAL ASSIGNMENTS PER PIT
          break;
        case 138:
          sendDcsBiosMessage("EMER_JETT_BTN", "1");
          break;
        case 139:
          sendDcsBiosMessage("MASTER_ARM_SW", "1");
          break;
        case 140: //FA-18C_hornet/HUD_ATT_SW
          sendDcsBiosMessage("HUD_ATT_SW", "1"); //1 FOR OFF
          break;
        case 141://FA-18C_hornet/HUD_VIDEO_CONTROL_SW
          sendDcsBiosMessage("HUD_VIDEO_CONTROL_SW", "1");  //1 FOR OFF
          break;
        case 142: //FA-18C_hornet/HUD_SYM_REJ_SW
          sendDcsBiosMessage("HUD_SYM_REJ_SW", "1"); //1 FOR OFF
          break;
        case 143: //FA-18C_hornet/IFEI_MODE_BTN
          sendDcsBiosMessage("IFEI_MODE_BTN", "1");
          break;
        case 144: //FA-18C_hornet/IFEI_DWN_BTN
          sendDcsBiosMessage("IFEI_DWN_BTN", "1");
          break;
        case 145: // IFEI, IS THIS USED
          break;
        case 146: //FA-18C_hornet/MODE_SELECTOR_SW
          sendDcsBiosMessage("MODE_SELECTOR_SW", "0"); //1 FOR OFF
          break;
        case 147:
          sendDcsBiosMessage("RWR_SPECIAL_BTN", "1");
          break;
        case 148:
          sendDcsBiosMessage("RWR_DIS_TYPE_SW", "1"); // "I"
          // CHECK INDIVIDUAL ASSIGNMENTS PER PIT
          break;
        case 149://FA-18C_hornet/SPIN_RECOVERY_SW
          sendDcsBiosMessage("SPIN_RECOVERY_COVER", "1");
          SpinFollowupTask = true;
          timeSpinOn = millis() + ToggleSwitchCoverMoveTime;
          //FA-18C_hornet/SPIN_RECOVERY_COVER
          break;
        case 150: //FA-18C_hornet/IR_COOL_SW
          sendDcsBiosMessage("IR_COOL_SW", "2");
          break;
        case 151: //FA-18C_hornet/HUD_ATT_SW
          sendDcsBiosMessage("HUD_ATT_SW", "1"); //1 FOR OFF
          break;
        case 152: //FA-18C_hornet/HUD_VIDEO_CONTROL_SW
          sendDcsBiosMessage("HUD_VIDEO_CONTROL_SW", "1"); //1 FOR OFF
          break;
        case 153: //FA-18C_hornet/HUD_SYM_REJ_SW
          sendDcsBiosMessage("HUD_SYM_REJ_SW", "1"); //1 FOR OFF
          break;
        case 154: //FA-18C_hornet/IFEI_QTY_BTN
          sendDcsBiosMessage("IFEI_QTY_BTN", "1");
          break;
        case 155: //FA-18C_hornet/IFEI_ZONE_BTN
          sendDcsBiosMessage("IFEI_ZONE_BTN", "1");
          break;
        case 156: //FA-18C_hornet/SELECT_HMD_LDDI_RDDI
          sendDcsBiosMessage("SELECT_HMD_LDDI_RDDI", "1"); // NEEDS WORK
          break;
        case 157: //FA-18C_hornet/MODE_SELECTOR_SW
          sendDcsBiosMessage("MODE_SELECTOR_SW", "2"); //1 FOR OFF
          break;
        case 158:
          sendDcsBiosMessage("RWR_DISPLAY_BTN", "1");
          break;
        case 159:
          sendDcsBiosMessage("RWR_DIS_TYPE_SW", "0"); // "N"
          // CHECK INDIVIDUAL ASSIGNMENTS PER PIT
          break;
        case 160: // SPIN, IS THIS USED
          break;
        case 161: //FA-18C_hornet/IR_COOL_SW
          sendDcsBiosMessage("IR_COOL_SW", "0");
        case 162: //FA-18C_hornet/HUD_ALT_SW
          sendDcsBiosMessage("HUD_ALT_SW", "1");
          break;
        case 163: // HUD, IS THIS USED
          break;
        case 164: //FA-18C_hornet/HUD_SYM_BRT_SELECT
          sendDcsBiosMessage("HUD_SYM_BRT_SELECT", "1");
          break;
        case 165: //FA-18C_hornet/IFEI_UP_BTN
          sendDcsBiosMessage("IFEI_UP_BTN", "1");
          break;
        case 166: //FA-18C_hornet/IFEI_ET_BTN
          sendDcsBiosMessage("IFEI_ET_BTN", "1");
          break;
        case 167:
          sendDcsBiosMessage("LEFT_DDI_CRS_SW", "0");
          break;
        case 168:
          sendDcsBiosMessage("LEFT_DDI_CRS_SW", "2");
          break;
        case 169:
          sendDcsBiosMessage("RWR_POWER_BTN", "1");
          // ######### PETE TO ADD LATCH #########
          break;
        case 170:
          sendDcsBiosMessage("RWR_DIS_TYPE_SW", "4"); // "F"
          // CHECK INDIVIDUAL ASSIGNMENTS PER PIT
          break;
        case 171:
          sendDcsBiosMessage("SJ_CTR", "1");
          break;
        case 172:
          sendDcsBiosMessage("SJ_RI", "1");
          break;
        case 173:
          sendDcsBiosMessage("SJ_LI", "1");
          break;
        case 174:
          sendDcsBiosMessage("SJ_RO", "1");
          break;
        case 175:
          sendDcsBiosMessage("SJ_LO", "1");
          break;
        case 176: // NOT USED
          break;
        case 177: // NOT USED
          break;
        case 178: // NOT USED
          break;
        case 179: // NOT USED
          break;

        default:
          sendDcsBiosMessage("LIGHTS_TEST_SW", "1");
          break;
          break;
      }
  }
}

// || \\ ANALOG A INPUT PLUG // || \\
//         PINS = 6 TOTAL          \\

// HUD ANALOG INPUTS
DcsBios::Potentiometer hudSymBrt("HUD_SYM_BRT", A0);
DcsBios::Potentiometer hudAoaIndexer("HUD_AOA_INDEXER", A1);
DcsBios::Potentiometer hudBlackLvl("HUD_BLACK_LVL", A2);
DcsBios::Potentiometer hudBalance("HUD_BALANCE", A3);

// RWR ANALOG INPUTS
DcsBios::Potentiometer rwrDmrCtrl("RWR_DMR_CTRL", A4); // CHECK OWN PIT
DcsBios::Potentiometer rwrAudioCtrl("RWR_AUDIO_CTRL", A5); // CHECK OWN PIT

// || \\ ANALOG B INPUT PLUG // || \\
//         PINS = 8 TOTAL          \\

// SPIN (HMD) KNOB
DcsBios::Potentiometer hmdOffBrt("HMD_OFF_BRT", A8);

// IFEI BRIGHTNESS
DcsBios::Potentiometer ifei("IFEI", A9);

// RWR BRIGHTNESS (STANDBY PANEL)
DcsBios::Potentiometer rwrRwrIntesity("RWR_RWR_INTESITY", A10);

// LEFT DDI
DcsBios::Potentiometer leftDdiBrtCtl("LEFT_DDI_BRT_CTL", A11);
DcsBios::Potentiometer leftDdiContCtl("LEFT_DDI_CONT_CTL", A12);

// RIGHT DDI
DcsBios::Potentiometer rightDdiBrtCtl("RIGHT_DDI_BRT_CTL", A13);
DcsBios::Potentiometer rightDdiContCtl("RIGHT_DDI_CONT_CTL", A14);

// AMPCD
DcsBios::Potentiometer ampcdBrtCtl("AMPCD_BRT_CTL", A15);

// || \\ ENCODERS // || \\
// AMPCD
DcsBios::RotaryEncoder ufcComm1ChannelSelect("UFC_COMM1_CHANNEL_SELECT", "DEC", "INC", 21, 20);
DcsBios::RotaryEncoder ufcComm2ChannelSelect("UFC_COMM2_CHANNEL_SELECT", "DEC", "INC", 19, 18);

//STANDBY INST
DcsBios::RotaryEncoder saiSet("SAI_SET", "-3200", "+3200", 15, 14);
DcsBios::RotaryEncoder stbyPressAlt("STBY_PRESS_ALT", "-3200", "+3200", 16, 17); 
////||||\\\\ Investigate issue on Bens Pit ////||||\\\\

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// UFC MOVED TO RIGHT CONSOLE
//DcsBios::PotentiometerEWMA<5, 128, 5> ufcComm1Vol("UFC_COMM1_VOL", 8);
//DcsBios::PotentiometerEWMA<5, 128, 5> ufcComm2Vol("UFC_COMM2_VOL", 9);
//DcsBios::PotentiometerEWMA<5, 128, 5> ufcBrt("UFC_BRT", 10);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
    colResult[0] = (PIND & B10000000) == 0 ? 0 : 1;
    //pin 39, PG2
    colResult[1] = (PING & B00000100) == 0 ? 0 : 1;
    //pin 40, PG1
    colResult[2] = (PING & B00000010) == 0 ? 0 : 1;
    //pin 41, PG0
    colResult[3] = (PING & B00000001) == 0 ? 0 : 1;

    //pin 42, PL7
    colResult[4] = (PINL & B10000000) == 0 ? 0 : 1;
    //pin 43, PL6
    colResult[5] = (PINL & B01000000) == 0 ? 0 : 1;
    //pin 44, PL5
    colResult[6] = (PINL & B00100000) == 0 ? 0 : 1;
    //pin 45, PL4
    colResult[7] = (PINL & B00010000) == 0 ? 0 : 1;

    //pin 46, PL3
    colResult[8] = (PINL & B00001000) == 0 ? 0 : 1;
    //pin 47, PL2
    colResult[9] = (PINL & B00000100) == 0 ? 0 : 1;
    //pin 48, PL1
    colResult[10] = (PINL & B00000010) == 0 ? 0 : 1;
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
  //SPIN COVER
  if (SpinFollowupTask == true) {
    if (millis() >= timeSpinOn) {
      sendDcsBiosMessage("SPIN_RECOVERY_SW", "1");
      SpinFollowupTask = false;
    }
  }
  // LEFT FIRE BUTTON COVER
  if (LFBCFollowupTask == true) {
    if (millis() >= timeLFBCOn) {
      sendDcsBiosMessage("LEFT_FIRE_BTN", "1");
      SpinFollowupTask = false;
    }
  }
  // RIGHT FIRE BUTTON COVER
  if (RFBCFollowupTask == true) {
    if (millis() >= timeRFBCOn) {
      sendDcsBiosMessage("RIGHT_FIRE_BTN", "1");
      SpinFollowupTask = false;
    }
  }
  // END CODE
}
