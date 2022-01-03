// OPENHORNET Backlighting LEDs solid colour test.
// It will set all LEDs to the colour of your choice, set your desired brightness and colour below.

#include <FastLED.h>

// Set your desired brightness and color below

String COLOUR   =  "GREEN";   // The color name that you want to show, e.g. Green, Red, Blue, White
#define BRIGHTNESS   50      // LED Brightness 0 = Off, 255 = 100%.

// Set your power supplies 5V current limit.

#define CURRENT_LIMIT   20000   // Current in mA (1000mA = 1 Amp). Most ATX PSUs provide 20A maximum.

// Defining how many pixels each backlighting connector has connected, if a connector is not used set it to zero.

#define BL1_PIXELS    200  // BL #1 Connector pixel count

// Defining what data pin each backlighting connector is connected to.

#define BL1_PIN       16  // BL #1 Connector pin


// Some other setup information. Don't change these unless you have a reason to.

#define LED_TYPE     WS2812B  // OPENHORNET backlighting LEDs are WS2812B
#define COLOUR_ORDER GRB      // OPENHORNET backlighting LEDs are GRB (green, red, blue)
#define SOLID_SPEED  100     // The refresh rate delay in ms. Leave this at around 1000 (1 second)

// Setting up the blocks of memory that will be used for storing and manipulating the led data;

CRGB BL1[BL1_PIXELS];


char receivedChar;
boolean newData = false;

unsigned long NEXT_LED_UPDATE = 0;
unsigned long LED_POINTER = 0;

void setup() { 


  Serial.begin(250000);
  Serial.println("RBG Led Unit Test");
  Serial.println("G for all Green");
  Serial.println("R for all Red");
  Serial.println("N to move pointer forward");
  Serial.println("B to move pointer backward");
  Serial.println("0 to reset pointer to 0");
  


  // Telling the system the type, their data pin, what color order they are and how many there are;
  
  FastLED.addLeds<LED_TYPE, BL1_PIN, COLOUR_ORDER>(BL1, BL1_PIXELS); 


  // The dimming method used. We have a large number of pixels so dithering is not ideal.
  
  FastLED.setDither(false);
  
  // Set the LEDs to their defined brightness.
  
  FastLED.setBrightness(BRIGHTNESS);      

  // FastLED Power management set at 5V, and the defined current limit.
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  
  // Now apply everything we just told it about the setup.
  fill_solid( BL1, BL1_PIXELS, CRGB::Green);
  FastLED.show();
  
  NEXT_LED_UPDATE = millis() + 100;

}


void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

void showNewData() {
    if (newData == true) {
      if (receivedChar != '\n') {
        Serial.print("This just in ... ");
        Serial.println(receivedChar);
        newData = false;
        switch (receivedChar) {

          case ('0'):
            LED_POINTER = 0;
            Serial.println("Reseting pointer to " + String(LED_POINTER));
            fill_solid( BL1, BL1_PIXELS, CRGB::Blue);
            BL1[ LED_POINTER ] = CRGB::Orange;
            break;
          
          
          case ('N','n'):
          
            LED_POINTER++;
            Serial.println("Incrementing pointer to " + String(LED_POINTER));
            fill_solid( BL1, BL1_PIXELS, CRGB::Blue);
            BL1[ LED_POINTER ] = CRGB::Orange;
            break;
          
          case ('B','b'):
          
            if (LED_POINTER >= 1) 
              LED_POINTER--;
            Serial.println("Decrementing pointer to " + String(LED_POINTER));
            fill_solid( BL1, BL1_PIXELS, CRGB::Blue);
            BL1[ LED_POINTER ] = CRGB::Orange;
            break;
          case ('G','g'):
            Serial.println("Colour to Green");
            COLOUR = "GREEN";
            
            fill_solid( BL1, BL1_PIXELS, CRGB::Green);
            break;
          default:
            Serial.println("Colour to Red");
            COLOUR = "RED";
            fill_solid( BL1, BL1_PIXELS, CRGB::Red); 
            
            break;
        }
      }
    }
}


void loop() {
    recvOneChar();
    showNewData();

    if (millis() >= NEXT_LED_UPDATE) {
      FastLED.show();
      //FastLED.delay(SOLID_SPEED); 
      NEXT_LED_UPDATE = millis() + SOLID_SPEED;
    }
}
