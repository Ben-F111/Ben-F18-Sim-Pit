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

  inputRole[0] = "HUD VID SIM BALANCE";
  inputRole[1] = "HUD AOA BRIGHT";
  inputRole[2] = "HUB BLACKLEVEL";
  inputRole[3] = "HUD BRIGHTNESS";
  inputRole[4] = "RWR AUDIO";
  inputRole[5] = "RWR DIMMER";
  inputRole[6] = "UNUSED";
  inputRole[7] = "UNUSED";
  inputRole[8] = "HMD BRIGHTNESS (SPIN PANEL)";
  inputRole[9] = "IFEI BRIGHTNESS";
  inputRole[10] = "STANDBY ANALOG";
  inputRole[11] = "LEFT DDI CONSTRAST";
  inputRole[12] = "LEFT DDI BRIGHT";
  inputRole[13] = "RIGHT DDI CONTRAST";
  inputRole[14] = "RIGHT DDI BRIGHT";
  inputRole[15] = "AMPCD BRIGHT";


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
