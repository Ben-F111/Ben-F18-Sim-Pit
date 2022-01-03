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


void setup() {

  Serial.begin(250000);

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
          Serial.print("Pressed ");

        }  
        else {   
          Serial.print("Released ");
        }
        
        Serial.println(stringind);

        prevjoyReport.button[ind] = joyReport.button[ind]; 


      }
      
    }

}




void loop() {



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


  currentMillis = millis();


}
