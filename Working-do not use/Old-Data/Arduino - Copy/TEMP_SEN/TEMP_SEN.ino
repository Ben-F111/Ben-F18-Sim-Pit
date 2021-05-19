

#include <dht.h>

dht DHT;

#define DHT11_PIN 5

void setup()
{
  Serial.begin(9600);
 }

void loop()
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
