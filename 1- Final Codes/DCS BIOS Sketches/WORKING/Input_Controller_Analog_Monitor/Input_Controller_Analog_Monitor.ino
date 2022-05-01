/*
 * 
 * Input Controller Ananlog Monitor
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

  inputRole[0] = "00";
  inputRole[1] = "01";
  inputRole[2] = "02";
  inputRole[3] = "03";
  inputRole[4] = "04";
  inputRole[5] = "05";
  inputRole[6] = "06";
  inputRole[7] = "07";
  inputRole[8] = "08";
  inputRole[9] = "09";
  inputRole[10] = "10";
  inputRole[11] = "11";
  inputRole[12] = "12";
  inputRole[13] = "13";
  inputRole[14] = "14";
  inputRole[15] = "15";


}

void loop() {

  // If the Role has a description - grab the analog value and print reslut
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
