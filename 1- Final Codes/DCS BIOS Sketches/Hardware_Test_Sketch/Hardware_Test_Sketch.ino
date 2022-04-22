

// Max 7219 Library 
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.

 LedControl lc=LedControl(12,11,10,1);
 */
LedControl lc=LedControl(4,6,5,2);                      // Landing and Flight Displasy
LedControl lc_2=LedControl(8,10,9,1);                   // Individual Leds and LE Devices

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

unsigned long sdelaytime=20;

#define filename "Hardware_Test_Sketch_20171028b"

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 
  0xA9,0xE7,0x3E,0xCA,0x34,0x1f};
IPAddress ip(192,168,1,205);
IPAddress TargetIPAddress(192,168,1,111);
const unsigned int localport = 13135;

EthernetUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to store the incoming data


const unsigned int listenport = 9920;
EthernetUDP rxUdp;
char receivePacketBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to store the incoming data

//TMS Display
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3


TM1637Display display(CLK, DIO);
// The amount of time (in milliseconds) between tests
#define TEST_DELAY   1

#include <Servo.h> 
#define starterOneServoPin 13
#define starterTwoServoPin 12

Servo starterOneServo;
Servo starterTwoServo;

#define apuPin 7
#define spareRelayPin 11

boolean APU_Starting = 0;
boolean APU_Stopping = 0;
boolean AC_Power = false;
boolean DC_Power = false;

boolean Debug_Display = false;

#define Flight_Altitude_Max7219 0
#define Landing_Altitude_Max7219 1

int LAND_ALT_STORE = 0;
int FLT_ALT_STORE = 0;

#include <Wire.h>          // *** I2C Mode 
//  Board  I2C / TWI pins
//  Uno, Ethernet A4 (SDA), A5 (SCL)
//  Mega2560  20 (SDA), 21 (SCL)
//  Leonardo  2 (SDA), 3 (SCL)
//  Due 20 (SDA), 21 (SCL), SDA1, SCL1
#include <SPI.h>


#define OLED_Address 0x3c
#define OLED_Command_Mode 0x80
#define OLED_Data_Mode 0x40

#define cmd_CLS 0x01

#define STX 0x02
#define ETX 0x03



#define minoledPtr 1
#define maxoledPtr 2

#define minLinePtr 1
#define maxLinePtr 2




char byteIn = 0;

char currentoled = 0;

int loopcounter = 0;

void setup() {
  // put your setup code here, to run once:
    /*
     The MAX72XX is in power-saving mode on startup,
      we have to do a wakeup call
    */
    lc.shutdown(0,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0,8);
    /* and clear the display */
    lc.clearDisplay(0);
    
    lc.shutdown(1,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(1,8);
    /* and clear the display */
    lc.clearDisplay(1);
    
    lc.shutdown(2,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(2,8);
    /* and clear the display */
    lc.clearDisplay(2);
    
    lc.shutdown(3,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(3,8);
    /* and clear the display */
    lc.clearDisplay(3);

    lc_2.shutdown(0,false);
    /* Set the brightness to a medium values */
    lc_2.setIntensity(0,8);
    /* and clear the display */
    lc_2.clearDisplay(0);

    digitalWrite(apuPin, HIGH);
    digitalWrite(spareRelayPin, HIGH);
    
    Serial.begin(115200); 
    Serial.println(filename);
    
    pinMode(apuPin, OUTPUT);
    pinMode(spareRelayPin, OUTPUT);
  


    // Sending Infrastructure
    Serial.println("Starting Network");
    Ethernet.begin( mac, ip);

  
    //Receiving Infrastructure

    rxUdp.begin( listenport );
    Serial.println("Network Initialised");

    Serial.println("Start LED Display");
    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    display.setBrightness(0x0f);
    
    // All segments on
    display.setSegments(data);
    Serial.println("Display Initialised");

    // Servo
    Serial.println("Start Servo");

    starterOneServo.attach(starterOneServoPin);
    starterTwoServo.attach(starterTwoServoPin);
    
    starterOneServo.write(60);
    delay(500);
    starterOneServo.write(200);
    delay(500);
    starterOneServo.write(60);

    starterTwoServo.write(60);
    delay(500);
    starterTwoServo.write(200);
    delay(500);
    starterTwoServo.write(60);
  
    Serial.println("Servo Initialised");

    Serial.println("Start Relay");
    digitalWrite(apuPin, LOW);
    delay(1000);
    digitalWrite(apuPin, HIGH);

    digitalWrite(spareRelayPin, LOW);
    delay(1000);
    digitalWrite(spareRelayPin, HIGH);


    Serial.println("Relay Initialised");


  
  Wire.begin();          // Start the I2C bus
  

 
 
 // *** I2C initial *** //
  delay(100);
   sendCommand(0x2A);  // **** Set "RE"=1  00101010B
   sendCommand(0x71);
   sendCommand(0x5C);
   sendCommand(0x28);
  
   sendCommand(0x08); // **** Set Sleep Mode On
   sendCommand(0x2A); // **** Set "RE"=1  00101010B
   sendCommand(0x79); // **** Set "SD"=1  01111001B
  
   sendCommand(0xD5);
   sendCommand(0x70);
   sendCommand(0x78); // **** Set "SD"=0
  
   sendCommand(0x08); // **** Set 5-dot, 3 or 4 line(0x09), 1 or 2 line(0x08)
  
   sendCommand(0x06); // **** Set Com31-->Com0  Seg0-->Seg99
  
   // **** Set OLED Characterization *** //
   sendCommand(0x2A);   // **** Set "RE"=1 
   sendCommand(0x79);   // **** Set "SD"=1
  
   // **** CGROM/CGRAM Management *** //
   sendCommand(0x72);   // **** Set ROM
   sendCommand(0x00);   // **** Set ROM A and 8 CGRAM
  
  
   sendCommand(0xDA);   // **** Set Seg Pins HW Config
   sendCommand(0x10);   
  
   sendCommand(0x81);   // **** Set Contrast
   sendCommand(0xFF); 
  
   sendCommand(0xDB);   // **** Set VCOM deselect level
   sendCommand(0x30);   // **** VCC x 0.83
  
   sendCommand(0xDC);   // **** Set gpio - turn EN for 15V generator on.
   sendCommand(0x03);
  
   sendCommand(0x78);   // **** Exiting Set OLED Characterization
   sendCommand(0x28); 
   sendCommand(0x2A); 
   //sendCommand(0x05);   // **** Set Entry Mode
   sendCommand(0x06);   // **** Set Entry Mode
   sendCommand(0x08);  
   sendCommand(0x28);   // **** Set "IS"=0 , "RE" =0 //28
   sendCommand(0x01); 
   sendCommand(0x80);   // **** Set DDRAM Address to 0x80 (line 1 start)
  
   delay(100);
   sendCommand(0x0C);   // **** Turn on Display  

 
  sendCommand(0x80);
  send_string("                ");
  sendCommand(0xC0);
  send_string("   00001        ");
    

    //Clear the UDP Buffer
    for(int i=0;i<UDP_TX_PACKET_MAX_SIZE;i++) receivePacketBuffer[i] = 0;


  
}



/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  lc.setRow(0,0,0x05);
  delay(delaytime);
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  lc.setRow(0,0,0x1c);
  delay(delaytime);
  lc.setRow(0,0,B00010000);
  delay(delaytime);
  lc.setRow(0,0,0x15);
  delay(delaytime);
  lc.setRow(0,0,0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  delay(delaytime);
} 

void writeStaticOn7Segment() {
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  lc.setRow(0,0,0x05);
  delay(delaytime);
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  lc.setRow(0,0,0x1c);
  delay(delaytime);
  lc.setRow(0,0,B00010000);
  delay(delaytime);
  lc.setRow(0,0,0x15);
  delay(delaytime);
  lc.setRow(0,0,0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  for(int i=0;i<13;i++) {

    lc.setDigit(0,7,i,false);
    lc.setDigit(0,6,i,false);
    lc.setDigit(0,5,i,false);
    lc.setDigit(0,4,i,false);
    lc.setDigit(0,3,i,false);
    lc.setDigit(0,2,i+1,false);
    lc.setDigit(0,1,i+2,false);
    lc.setDigit(0,0,i+3,false);

    lc.setDigit(1,7,i,false);
    lc.setDigit(1,6,i,false);
    lc.setDigit(1,5,i,false);
    lc.setDigit(1,4,i,false);
    lc.setDigit(1,3,i,false);
    lc.setDigit(1,2,i+1,false);
    lc.setDigit(1,1,i+2,false);
    lc.setDigit(1,0,i+3,false);

    lc.setDigit(2,7,i,false);
    lc.setDigit(2,6,i,false);
    lc.setDigit(2,5,i,false);
    lc.setDigit(2,4,i,false);
    lc.setDigit(2,3,i,false);
    lc.setDigit(2,2,i+1,false);
    lc.setDigit(2,1,i+2,false);
    lc.setDigit(2,0,i+3,false);

    lc.setDigit(3,7,i,false);
    lc.setDigit(3,6,i,false);
    lc.setDigit(3,5,i,false);
    lc.setDigit(3,4,i,false);
    lc.setDigit(3,3,i,false);
    lc.setDigit(3,2,i+1,false);
    lc.setDigit(3,1,i+2,false);
    lc.setDigit(3,0,i+3,false);
    delay(delaytime);
  }
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  delay(delaytime);
}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(sdelaytime);
    lc_2.setRow(0,row,B10100000);
    delay(sdelaytime);
    lc_2.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(sdelaytime);
      lc_2.setRow(0,row,B10100000);
      delay(sdelaytime);
      lc_2.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(sdelaytime);
    lc_2.setColumn(0,col,B10100000);
    delay(sdelaytime);
    lc_2.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(sdelaytime);
      lc_2.setColumn(0,col,B10100000);
      delay(sdelaytime);
      lc_2.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      Serial.println("new Row Col " + String(row) + " " +  String(col) + " Position:" + String( row * 8 +  col));
      PrintMapping(row * 8 +  col);
      delay(sdelaytime);
      lc_2.setLed(0,row,col,true);
      delay(sdelaytime);
      for(int i=0;i<8;i++) {
        lc_2.setLed(0,row,col,false);
        delay(sdelaytime);
        lc_2.setLed(0,row,col,true);
        delay(sdelaytime);
      }
      lc_2.setLed(0,row,col,false);
    }
  }
}




void sendData(unsigned char data)
{
    Wire.beginTransmission(OLED_Address);    // **** Start I2C 
    Wire.write(OLED_Data_Mode);         // **** Set OLED Data mode
    Wire.write(data);
    Wire.endTransmission();                     // **** End I2C 
    
}

void sendCommand(unsigned char command)
{
    Wire.beginTransmission(OLED_Address);    // **** Start I2C 
    Wire.write(OLED_Command_Mode);         // **** Set OLED Command mode
    Wire.write(command);
    Wire.endTransmission();                    // **** End I2C 
    
}

void send_string(const char *String)
{
    unsigned char i=0;
    while(String[i])
    {
        sendData(String[i]);      // *** Show String to OLED
        i++;
    }

}



void PrintMapping (int ledPos)
{
  if (ledPos == 0) Serial.println( "I_OH_LEDEVICES_TRANS_FLAP3");
  if (ledPos == 1) Serial.println( "I_OH_LEDEVICES_TRANS_FLAP4");
  if (ledPos == 2) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT5");
  if (ledPos == 3) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT6");
  if (ledPos == 4) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT7");
  if (ledPos == 5) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT8");
  if (ledPos == 6) Serial.println( "I_OH_IRS_DCFAIL_L");
  if (ledPos == 7) Serial.println( "I_OH_REVERSER1");
  if (ledPos == 8) Serial.println( "I_OH_LEDEVICES_EXT_FLAP3");
  if (ledPos == 9) Serial.println( "I_OH_LEDEVICES_EXT_FLAP4");
  if (ledPos == 10) Serial.println( "I_OH_LEDEVICES_EXT_SLAT5");
  if (ledPos == 11) Serial.println( "I_OH_LEDEVICES_EXT_SLAT6");
  if (ledPos == 12) Serial.println( "I_OH_LEDEVICES_EXT_SLAT7");
  if (ledPos == 13) Serial.println( "I_OH_LEDEVICES_EXT_SLAT8");
  if (ledPos == 14) Serial.println( "I_OH_IRS_FAULT_L");
  if (ledPos == 15) Serial.println( "I_OH_GEAR_NOSE_DOWN");
  if (ledPos == 16) Serial.println( "I_OH_LEDEVICES_EXT_FLAP3");
  if (ledPos == 17) Serial.println( "I_OH_LEDEVICES_EXT_FLAP4");
  if (ledPos == 18) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT5");
  if (ledPos == 19) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT6");
  if (ledPos == 20) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT7");
  if (ledPos == 21) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT8");
  if (ledPos == 22) Serial.println( "I_OH_IRS_ONDC_R");
  if (ledPos == 23) Serial.println( "I_OH_ENGINE_CONTROL2");
  if (ledPos == 24) Serial.println( "I_OH_LEDEVICES_TRANS_FLAP2");
  if (ledPos == 25) Serial.println( "I_OH_LEDEVICES_TRANS_FLAP1");
  if (ledPos == 26) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT4");
  if (ledPos == 27) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT3");
  if (ledPos == 28) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT2");
  if (ledPos == 29) Serial.println( "I_OH_LEDEVICES_TRANS_SLAT1");
  if (ledPos == 30) Serial.println( "I_OH_IRS_ALIGN_R");
  if (ledPos == 31) Serial.println( "I_OH_ENGINE_CONTROL1");
  if (ledPos == 32) Serial.println( "I_OH_LEDEVICES_EXT_FLAP2");
  if (ledPos == 33) Serial.println( "I_OH_LEDEVICES_EXT_FLAP1");
  if (ledPos == 34) Serial.println( "I_OH_LEDEVICES_EXT_SLAT4");
  if (ledPos == 35) Serial.println( "I_OH_LEDEVICES_EXT_SLAT3");
  if (ledPos == 36) Serial.println( "I_OH_LEDEVICES_EXT_SLAT2");
  if (ledPos == 37) Serial.println( "I_OH_LEDEVICES_EXT_SLAT1");
  if (ledPos == 38) Serial.println( "I_OH_IRS_ALIGN_L");
  if (ledPos == 39) Serial.println( "I_OH_GEAR_LEFT_DOWN");
  if (ledPos == 40) Serial.println( "I_OH_LEDEVICES_EXT_FLAP2");
  if (ledPos == 41) Serial.println( "I_OH_LEDEVICES_EXT_FLAP1");
  if (ledPos == 42) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT4");
  if (ledPos == 43) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT3");
  if (ledPos == 44) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT2");
  if (ledPos == 45) Serial.println( "I_OH_LEDEVICES_FULLEXT_SLAT1");
  if (ledPos == 46) Serial.println( "I_OH_IRS_DCFAIL_R");
  if (ledPos == 47) Serial.println( "I_OH_REVERSER2");
  if (ledPos == 54) Serial.println( "I_OH_IRS_FAULT_R");
  if (ledPos == 55) Serial.println( "I_OH_GEAR_RIGHT_DOWN");
  if (ledPos == 61) Serial.println( "I_OH_GPS");
  if (ledPos == 62) Serial.println( "I_OH_IRS_ONDC_L");
  if (ledPos == 63) Serial.println( "I_OH_PSEU");
}


void printNumber(long v, int maxArrayNo) {
    int ones;
    int tens;
    int hundreds;
    int thousands;
    int tenThousands;
    boolean negative;  
    int negativePos;
//
//    Serial.print(v);
//    Serial.print("-");

    if (maxArrayNo < 0 || maxArrayNo > 1)
    {
      Serial.print("Unsupport max7219 no 0-1 supported: ");
      Serial.print(maxArrayNo);
      Serial.println(" passed");
      return;
      
    }
    

    if(v < -99999 || v > 99999) 
    {
      Serial.print("Unsupport max7219 no -99999-99999 supported: ");
      Serial.print(v);
      Serial.println(" passed");
      return;
    }
    
    if(v<0) {
        // Serial.println("Negative passed");
        negative=true;
        v=v*-1;
    }
        else
    {
        negative = false;
    }

    //Determine location if -
    if (v > 9999) { negativePos = 6; }
    else if ( v > 999) {negativePos=5;}
    else if (v > 99)  {negativePos=4;}
    else if (v > 9)  {negativePos=3;} 
    else {negativePos=2;}

    //Serial.println(negativePos);
    
    ones=v%10;
    v=v/10;
    tens=v%10;
    v=v/10;
    hundreds=v%10;
    v=v/10;
    thousands=v%10;
    v=v/10;
    tenThousands=v%10;     

    //Now print the number digit by digit
    if (negativePos < 6)  { lc.setChar(maxArrayNo,5,' ',false);}
    else  { lc.setDigit(maxArrayNo,5,(byte)tenThousands,false); }

    if (negativePos < 5)  { 
      lc.setChar(maxArrayNo,4,' ',false);} 
    else {
      lc.setDigit(maxArrayNo,4,(byte)thousands,false);
    }
    if (negativePos < 4)  { 
      lc.setChar(maxArrayNo,3,' ',false);}
    else {
      lc.setDigit(maxArrayNo,3,(byte)hundreds,false);
    }
    if (negativePos < 3)  { 
      lc.setChar(maxArrayNo,2,' ',false);}
    else { 
      lc.setDigit(maxArrayNo,2,(byte)tens,false);
    }
    
    lc.setDigit(maxArrayNo,1,(byte)ones,false);

    if (negative) {
       //print character '-' in the leftmost column 
       lc.setChar(maxArrayNo,negativePos,'-',false);
    }
    else {
       //print a blank in the sign column
       lc.setChar(maxArrayNo,negativePos,' ',false);
    }

}

void loop() {



  String testString;
  testString  = String(loopcounter);
  testString = testString + " Helloworld";
  testString.toCharArray(packetBuffer, 20);
  sendCommand(0xC0);
  send_string(packetBuffer);


//  Serial.println("rows");
//  rows();
//  Serial.println("columns");
//  columns();

//  Serial.println("showNumberDec");
//  for(int k = 0; k < 1000; k += 1) {
//    display.showNumberDec(k, 0);
//    delay(TEST_DELAY);
//  }
//
//  Serial.println("writeArduinoOn7Segment");
//  writeArduinoOn7Segment();
//
//  Serial.println("scrollDigits");
//  scrollDigits();
//
//  Serial.println("writeStaticOn7Segment");
//  writeStaticOn7Segment();
  
  Serial.println("Getting serious with 7 Seg");
 lc.clearDisplay(0);
//  for(long i=-11000;i<100000;i++) {
//    printNumber(i,Flight_Altitude_Max7219);
//
//  }
  // Flight Altitude - Max7219-0 Use Digits 5-1 38000
  // Landing Altitude - Max7219-1 Use Digits 5-1 10000

  lc.clearDisplay(0);
  printNumber(25980,Flight_Altitude_Max7219 );
  printNumber(-100,Landing_Altitude_Max7219 );
   
  single();


  
  loopcounter = loopcounter + 1;
  lc.clearDisplay(0);
  lc.clearDisplay(1);

}
