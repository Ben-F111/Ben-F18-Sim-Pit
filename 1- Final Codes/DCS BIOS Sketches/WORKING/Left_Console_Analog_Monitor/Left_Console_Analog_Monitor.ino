/*
 * 
 * Left Console Ananlog Monitor
 * 
 * Scans the Analog ports and prints the current value.
 * Used for testing
 */

int pinUnderTest = 0;
const int maxPins = 16;
int analogValue = 0;
long currentTime = 0;

String inputRole[maxPins];


 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);

  inputRole[0] = "Comm - VOX";
  inputRole[1] = "Comm - ICS";
  inputRole[2] = "Comm - RWR";
  inputRole[3] = "Comm - MIDS A";
  inputRole[4] = "Comm - MIDS B";
  inputRole[5] = "Comm - TCN";
  inputRole[6] = "";
  inputRole[7] = "";
  inputRole[8] = "Ext Lights Formation";
  inputRole[9] = "";
  inputRole[10] = "";
  inputRole[11] = "";
  inputRole[12] = "Comm - Aux";
  inputRole[13] = "Comm - WPN";
  inputRole[14] = "";
  inputRole[15] = "Ext Lights Position";


}

void loop() {
  // put your main code here, to run repeatedly:

  for (pinUnderTest = 0; pinUnderTest < maxPins; ++pinUnderTest) {
      if (inputRole[pinUnderTest].length() > 0) {
        analogValue = analogRead(pinUnderTest);
        Serial.print(inputRole[pinUnderTest] + " = ");
        Serial.println(String(analogValue));
      }  
    
  }

  Serial.println("Uptime " + String(millis()));
  delay(500);

}
