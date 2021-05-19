#include "Nextion.h"

// vars
int buttonvalue = 0;
char buttonvalue_char[2];

NexButton b0 = NexButton(0, 1, "b0");

NexTouch *nex_listen_list[] = 
{
    &b0,
    NULL
};

void b0PopCallback(void *ptr)
{
  // JUST PRINT SOMETHING THAT WE KNOW THE BUTTON IS PRESSED AND THE FEEDBACK IS OK
    Serial.println("button pressed");

// INCREASE THE VALUE WE WANT TO SET AS TEXT TO THE BUTTON
    buttonvalue++;
    
// CREATE A CHAR OF THE INTEGER TO SEND IT TO THE BUTTON AS TEXT
itoa(buttonvalue, buttonvalue_char, 10);

// SET THE TEXT TO THE BUTTON
b0.setText(buttonvalue_char);
    
}

void setup(void)
{   
  // SHOULD HAVE bauds=115200 AS PREINITIALIZE EVENT IN NEXTION EDITOR 
  Serial.begin(9600);
    nexInit();
    b0.attachPop(b0PopCallback, &b0);
    Serial.println("setup done"); 
}

void loop(void)
{   
    nexLoop(nex_listen_list);
}
