#include "Nextion.h" 

#include <dht.h>

dht DHT;

#define DHT11_PIN 5

// Declare a text object [page id:0,component id:1, component name: "t0"]. 
NexText t8 = NexText(0, 18, "t8"); 

char buffer[100] = {0}; 
  
void setup(void) 
{ 
    /* Set the baudrate which is for debug and communicate with Nextion screen. */ 
    nexInit(); 
    dbSerialPrintln("setup done"); 
      Serial.begin(9600);
} 

void loop(void) 
{ 
  uint16_t dht11val;
  memset(buffer,0,sizeof(buffer));
{
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
  }

  //Serial.print(DHT.humidity, 0);
//  Serial.print(",\t");
Serial.println(DHT.temperature, 1);
delay(250);
}
  dht11val = DHT.temperature;  
  itoa(dht11val,buffer,10);
  t8.setText(buffer);
} 
