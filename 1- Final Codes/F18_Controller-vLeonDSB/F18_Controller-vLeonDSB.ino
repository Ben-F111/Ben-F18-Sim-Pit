


#include <Nextion.h>          // Nextion Library
//#include <SoftwareSerial1.h>   // Library for Software Serial Port
#include <Keyboard.h>         // Keyboard Emulation
#include "NexTouch.h"
#include <Mouse.h>
//SoftwareSerial HMISerial(0,11);

// delays

int KPD = 20; // KEYPRESS DELAY
int KPR = 50; // KEYPRESS RELEASE

int horzPin = A0;  // Analog output of horizontal joystick pin
int vertPin = A1;  // Analog output of vertical joystick pin
int selPin = 7;  // select button pin of joystick
// int brightness = analogRead(A4);

int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
const int sensitivity = 500;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouseClickFlag = 0;

int invertMouse = 1;        //Invert joystick based on orientation
//int invertMouse = -1;         //Noninverted joystick based on orientation
#define KEY_PAUSE (76+136)

#define SafetyTimeOut 10000 // 10 Sec
#define WaitOneHour 3600000
long GlobalLastkeyPressed = WaitOneHour;

NexButton m444 = NexButton(5, 10, "b2"); // TEST

NexTouch *nex_listen_list[] = {

&m444,

  NULL
};

void m444PressCallback(void *ptr) { //press
  Keyboard.press('k');
  GlobalLastkeyPressed = millis();
}
void m444ReleaseCallback(void *ptr) { //release
  Keyboard.releaseAll();
}

// Main Setup
void setup() {

  Serial1.begin(9600);  // Start Software Serial at 9600bauds

  nexInit();  //  Nextion Display initalize

  m444.attachPush(m444PressCallback, &m444); //press
  m444.attachPop(m444ReleaseCallback, &m444); //release

  Keyboard.releaseAll();

  pinMode(horzPin, INPUT);  // Set both analog pins as inputs
  pinMode(vertPin, INPUT);
  pinMode(selPin, INPUT);  // set button select pin as input
  digitalWrite(selPin, HIGH);  // Pull button select pin high
  delay(10);  // short delay to let outputs settle
  vertZero = analogRead(vertPin);  // get the initial values
  horzZero = analogRead(horzPin);  // Joystick should be in neutral position when reading these
  //Mouse.begin(); //Init mouse emulation

  Serial1.print("dim=100");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(500);
}
// Main Loop
void loop() {

  if (millis() >= (GlobalLastkeyPressed + SafetyTimeOut)) {
    Keyboard.releaseAll();
    GlobalLastkeyPressed = millis() + WaitOneHour;
  }

  nexLoop(nex_listen_list);  // Check for any touch event and run the associated function

  // PAGEVIEWS();

  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

  if (vertValue != 0)
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
  if (horzValue != 0)
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis

  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT);  // click the left button down
  }
  else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_LEFT);  // release the left button
  }
  
     {
     int brightness = analogRead(A4);
      int bright = map(brightness, 10, 1100, 2, 100);
      String dim = "dim=" + String(bright);
      brightness = bright;
      Serial1.print(dim.c_str());
      Serial1.write("\xFF\xFF\xFF");
  
}
}
