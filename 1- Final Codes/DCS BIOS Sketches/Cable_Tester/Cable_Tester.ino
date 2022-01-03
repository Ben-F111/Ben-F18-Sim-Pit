/* 

Cable Tester

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




unsigned long currentMillis = 0;
unsigned long previousMillis = 0;




// Note Pin 4 and Pin 10 cannot be used for other purposes.
//Arduino communicates with both the W5500 and SD card using the SPI bus (through the ICSP header). 
//This is on digital pins 10, 11, 12, and 13 on the Uno and pins 50, 51, and 52 on the Mega. 
//On both boards, pin 10 is used to select the W5500 and pin 4 for the SD card. These pins cannot be used for general I/O. 
//On the Mega, the hardware SS pin, 53, is not used to select either the W5500 or the SD card, 
//but it must be kept as an output or the SPI interface won't work.

const int inputPinOffset = 37;
const int outputPinOffset = 21;

const int waitForOutputstoSettle = 5;

void setup() {

  Serial.begin(250000);
  Serial.println("Cable Tester");
  Serial.setTimeout(5000);

  // Set the output ports to output
  for( int portId = 1; portId < 17; portId ++ )
  {
    pinMode( portId + outputPinOffset, OUTPUT );
    digitalWrite( portId + outputPinOffset, LOW);
  }


  // Set the input ports to input - port 50-53 used by Ethernet Shield
  for( int portId = 1; portId < 13; portId ++ )
  {
    // Even though we've already got 10K external pullups
    pinMode( portId + inputPinOffset , INPUT_PULLUP);
  }
}

void SetOnlyOnePort(int portToSet){

  for( int portId = 1; portId < 17; portId ++ )
  {
    digitalWrite( portId + outputPinOffset, HIGH);
  }
  digitalWrite( portToSet + outputPinOffset, LOW);
  delay(waitForOutputstoSettle);

}


void CheckPin(int pinToTest) {
  int currentPin = 1;

  
  Serial.println("Jammed Low Test");
  // Set all outputs to high
  for( int portId = 1; portId < 17; portId ++ )
  {
    digitalWrite( portId + outputPinOffset, HIGH);
  }
  delay(waitForOutputstoSettle);
  
  // Check all inputs are high
  bool allInputsLow = true;
  for( int portId = 1; portId < 13; portId ++ )
  {
    if (digitalRead(portId +inputPinOffset ) == 0) {
      allInputsLow = false; 
      Serial.println("Warning - pin " + String( portId) + " jammed low");
    }
  }



  
  // Check ports are not crossed over
  Serial.println("Running Crossover Test");
  for( int portId = 1; portId <= pinToTest; portId ++) {
    
    SetOnlyOnePort(portId);

    
    for( int portToRead = 1; portToRead < 13; portToRead++) {
      if (digitalRead(portToRead + inputPinOffset) == 0 && portToRead != portId) {
        Serial.println("Crossover Test - Pin High :" + String(portToRead));
      }
      if (digitalRead(portToRead + inputPinOffset) == 1 && portToRead == portId) {
        Serial.println("Crossover Test - Pin Open :" + String(portToRead));
      }      
    
    }
  }



  
}


void loop() {

  int highestConnectedPin = 0;

  Serial.println("Time " + String(millis()));
  Serial.println("Pausing for 2 seconds");
  Serial.end();
  delay(2000);
  Serial.begin(250000);


  for( int portId = 1; portId < 17; portId ++ )
  {
    digitalWrite( portId + outputPinOffset, LOW);
  }
  delay(waitForOutputstoSettle);


  bool inputPulledLow = false;
  // Scan to see if a cable if connected
  for( int portId = 1; portId < 13; portId ++ )
  {
    Serial.print("Port " + String(portId) + " :");
    if (digitalRead(portId +inputPinOffset ) == 0) {
      Serial.println(" connected");
      highestConnectedPin = portId;
    } else {
      Serial.println(" open");
    }
  }


  if (highestConnectedPin >= 1) {
    // At least one of the pins is connect
    Serial.println("Testing up to pin: " + String(highestConnectedPin));
    CheckPin(highestConnectedPin);
  }


 
            

         

  currentMillis = millis();


}
