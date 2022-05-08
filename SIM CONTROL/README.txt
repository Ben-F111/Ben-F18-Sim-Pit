 NOTES - 
 Thanks for downloading
 The Sketch below required the following library
 you must delete any of the current "Nextion" Library's already installed
 This will not compile or work without this Library
 "ITEADLIB_Arduino_Nextion-master - Version 0.9.0"
 the library is located in the same folder you got this sketch from
 
 if you download the file from ITEAD" you will need to make the below 3 changes, 
 these have been made in the library located with this Sketch already
 In NexConfig.h  
 
 Current #define DEBUG_SERIAL_ENABLE
 Change //#define DEBUG_SERIAL_ENABLE ("//" add to the begining, remark this line out)
 
 Current #define dbSerial Serial
 Change //#define dbSerial Serial ("//" add to the begining, remark this line out)
 
 Current #define nexSerial Serial //
 Change #define nexSerial Serial1 // (Add "1" to Serial)
 
 This file currently works with Version of the Nextion file
 
 "SIM_CONTROL_V2.1.HMI"
 
 Arduino need to be an ARDUINO LEONARDO or other ATmega32u4 Board
 it will not work on an ARDUINO MEGA, due to the Keyboard output requirement
 
 the screen is a 7" Nextion can be purchased from
 https://itead.cc/product/nx8048k070-nextion-7-0-enhanced-series-hmi-touch-display/
 the file has only been compiled for this version
 
 the "joystick Mouse" is a 5 wire, I used this from Jaycar (Aus)
 
 https://www.jaycar.com.au/arduino-compatible-x-and-y-axis-joystick-module/p/XC4422
 
 horzPin = A0;  // Analog output of horizontal joystick pin
 vertPin = A1;  // Analog output of vertical joystick pin
 int selPin = 7;  // select button pin of joystick 
 
 Dimmer is a standard 10K Pot and switch, Dimmer is Pin A4
 
 Keyboard mapping is standard DCS F18, I have not added any special key bindings
 
 note this is the early versions and new files will be required
 
 Mount can be located here, however it needs work and is not perfect yet
 SIM CONTROL\Mount
