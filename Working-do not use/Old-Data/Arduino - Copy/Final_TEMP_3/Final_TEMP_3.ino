#include <Arduino.h>
#include "Nextion.h"
#include "NexTouch.h"

#include <OneWire.h>
#include <DallasTemperature.h> // TEMP Sensors
#define ONE_WIRE_BUS_PIN 3 // Data wire for onewire is plugged into pin 3 on the Arduino
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

DeviceAddress Probe01 = { 0x28, 0xFF, 0x7C, 0xB0, 0x86, 0x16, 0x05, 0x5F }; // probe one address
DeviceAddress Probe02 = { 0x28, 0xFF, 0xD1, 0x1C, 0x87, 0x16, 0x05, 0x90 }; // probe two address

#include <dht.h> // TEMP Hum Sensor
dht DHTA;
#define DHT11a_PIN 5 //***** Temp Sen Pin *****

// parameters  page #, component .id, component .objname
NexText t0 = NexText(0, 37, "t0");
NexText t1 = NexText(0, 37, "t1");
NexText t2 = NexText(0, 37, "t2");
NexText t3 = NexText(0, 38, "t3");
//********************************************************************* Control Buttons link to Nextion Controls *************************************************************
NexDSButton pump1btn = NexDSButton(0, 2, "bt0"); // Aqua Medic Pump 1
NexDSButton pump2btn = NexDSButton(0, 3, "bt1"); // Aqua Medic Pump 2
NexDSButton uvbtn = NexDSButton(0, 4, "bt2"); // UV On or Off
NexDSButton chillbtn = NexDSButton(0, 5, "bt3"); // Chiller On or Off
NexDSButton skimbtn = NexDSButton(0, 12, "bt4"); // Skimmer On or Off
NexDSButton drainbtn = NexDSButton(0, 6, "bt5"); // drain sump On or Off
NexDSButton fillatobtn = NexDSButton(0, 7, "bt6"); // fill ATO 30 ltr On or Off
NexDSButton fillsaltbtn = NexDSButton(0, 8, "bt7"); // Fill Sump Salt water On or Off
NexDSButton fullautobtn = NexDSButton(0, 9, "bt8"); // Tank Auto On or Off (to be removed not used)
NexDSButton feedbtn = NexDSButton(0, 10, "bt9"); // Feed the tank 10 mins On or Off
NexDSButton waterchgbtn = NexDSButton(0, 11, "bt10"); // Auto Water change 100 ltrs On or Off

uint32_t digitalstate0, digitalstate1, digitalstate2, digitalstate3, digitalstate4,
         digitalstate5, digitalstate6, digitalstate7, digitalstate8, digitalstate9, digitalstate10 = 0;

int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
//**************************************************************** Relay Outputs 13-6 - 8 total ****************************************************************************
int pump1Pin = 13;
int pump2Pin = 12;
int uvPin = 11;
int chillPin = 10;
int skimPin = 9;
int drainPin = 8;
int atoPin = 7;
int saltPin = 6;
//**************************************************************** Tank Inputs water sensors ****************************************************************************
int salthigh = 22; // pre mised salt tank full
int saltlow = 24; // pre mixed salt tank low
int atohigh = 26; // RODI 100 ltr tank High
int atolow = 28; // RODI 100 ltr tank low
int topoff = 30; // 30 ltrs Top off tank full
int topofffull = 42; // 30 ltrs Top off tank low
int sumpchg = 32; // sump tank over full (water change mode)
int sumphigh = 34; // sump tank normal
int sumplow = 36; // sumptank low (waterchange mode)
int sumplvl = 38; // water level at pump critical low
int socks = 40; // sock are blocked (water level high in compartment)
//**************************************************************** one wire temp sensors ********************************************************************************
float S1Val; //DS18B20 - Sensor 1
float S2Val; //DS18B20 - Sensor 2
//**************************************************************** Internal code bits ***********************************************************************************
byte alarm;
byte pump1;
byte pump1sw = 0;
byte lastPump1sw = 0;
byte pump2;
byte pump2sw = 0;
byte lastPump2sw = 0;
byte uv;
byte uvsw;
byte chill;
byte chillsw;
byte skim;
byte skimsw;
byte drain;
byte drainsw;
byte ato;
byte atosw;
byte salt;
byte saltsw;
byte fullauto;
byte fullautosw;
byte waterchange;
byte waterchangesw = 0;
byte lastWaterChangesw = 0;
byte feedtank;
byte feedtanksw;
byte sumpwchg = 0;
byte lastSumpWchg = 1;
byte sumpfull = 0;
byte lastSumpFull = 1;
byte sumpempty = 0;
byte lastSumpEmpty = 1;
byte sump = 0;
byte lastSump = 1;
byte temphigh;
byte templow;
byte saltfull = 0;
byte lastSaltFull = 1;
byte saltempty = 0;
byte lastSaltEmpty = 1;
byte atofull = 0;
byte lastAtoFull = 1;
byte atoempty = 0;
byte lastAtoEmpty = 1;
byte topfull = 0;
byte lastTopFull = 1;
byte topover = 0;
byte lastTopOver = 1;
byte topempty;
byte socksblk = 0;
byte lastSocksBlk = 1;
byte drainsump;
byte sumpdraintog;
byte sumpfill;
byte sumpfilltog;
//byte RadionsLED; // check if needed
//byte T5lamps; // lights removed for now
//byte LEDon = 0;  // lights removed for now
//byte lastLEDon = 1;  // lights removed for now
//byte T5on = 0;  // lights removed for now
//byte lastT5on = 1;  // lights removed for now
//********************************************************************* Control Buttons Arduino code **********************************************************************
NexTouch *nex_listen_list[] = {
  &pump1btn, &pump2btn, &uvbtn, &chillbtn, &skimbtn, &drainbtn,
  &fillatobtn, &fillsaltbtn, &fullautobtn, &feedbtn, &waterchgbtn,
  //&LEDbtn, &T5btn, ***lights removed for now***
  NULL
};
//********************************************************************* converts button to bits Ardunio ******************************************************************
//****************************************************  critcal bits are set LOW when not used, fail safe for pumps to run ***********************************************
void pump1btnReleaseCallback(void *ptr) {
  digitalstate0 = 1 - digitalstate0;
  if (digitalstate0 == 1) {
    pump1sw = LOW;
  } else {
    pump1sw = HIGH;
  }
}
void pump2btnReleaseCallback(void *ptr) {
  digitalstate1 = 1 - digitalstate1;
  if (digitalstate1 == 1) {
    pump2sw = HIGH;
  } else {
    pump2sw = LOW;
  }
}
void uvbtnReleaseCallback(void *ptr) {
  digitalstate2 = 1 - digitalstate2;
  if (digitalstate2 == 1) {
    uvsw = HIGH;
  } else {
    uvsw = LOW;
  }
}  void chillbtnReleaseCallback(void *ptr) {
  digitalstate3 = 1 - digitalstate3;
  if (digitalstate3 == 1) {
    chillsw = HIGH;
  } else {
    chillsw = LOW;
  }
}  void skimbtnReleaseCallback(void *ptr) {
  digitalstate4 = 1 - digitalstate4;
  if (digitalstate4 == 1) {
    skimsw = HIGH;
  } else {
    skimsw = LOW;
  }
}  void drainbtnReleaseCallback(void *ptr) {
  digitalstate5 = 1 - digitalstate5;
  if (digitalstate5 == 1) {
    drainsw = HIGH;
  } else {
    drainsw = LOW;
  }
}  void fillatobtnReleaseCallback(void *ptr) {
  digitalstate6 = 1 - digitalstate6;
  if (digitalstate6 == 1) {
    atosw = HIGH;
  } else {
    atosw = LOW;
  }
}  void fillsaltbtnReleaseCallback(void *ptr) {
  digitalstate7 = 1 - digitalstate7;
  if (digitalstate7 == 1) {
    saltsw = HIGH;
  } else {
    saltsw = LOW;
  }
  //************************************************* Non critcal bits are set HIGH when not used, fail safe ******************************************************************
}  void fullautobtnReleaseCallback(void *ptr) {
  digitalstate8 = 1 - digitalstate8;
  if (digitalstate8 == 1) {
    fullautosw = LOW;
  } else {
    fullautosw = HIGH;
  }
}  void feedbtnReleaseCallback(void *ptr) {
  digitalstate9 = 1 - digitalstate9;
  if (digitalstate9 == 1) {
    feedtanksw = LOW;
  } else {
    feedtanksw = HIGH;
  }
}  void waterchgbtnReleaseCallback(void *ptr) {
  digitalstate10 = 1 - digitalstate10;
  if (digitalstate10 == 1) {
    waterchangesw = LOW;
  } else {
    waterchangesw = HIGH;
  }
  /* Lights removed for now
    }  void LEDbtnReleaseCallback(void *ptr) {
    digitalstate11 = 1 - digitalstate11;
    if (digitalstate11 == 1) {
      RadionsLED = LOW;
    } else {
      RadionsLED = HIGH;
    }
    }  void T5btnReleaseCallback(void *ptr) {
    digitalstate12 = 1 - digitalstate12;
    if (digitalstate12 == 1) {
      T5lamps = LOW;
    } else {
      T5lamps = HIGH;
    }
    }
  */
}
boolean running = false;
//***************************************************************** TIMERS ********************************************************************************
unsigned long prevSenMillis = 0;          // Nextion Indications
unsigned long refreshSenInterval = 5000;  // Nextion Indications

unsigned long prevTempResetMillis = 0;      // Check Temperatures
unsigned long tempResetInterval = 10000;    // Check Temperatures

unsigned long prevFEEDResetMillis = 0;      // Feed Tank
unsigned long feedResetInterval = 5000;     // Feed Tank
char buffer[10] = {0};
//*********************************************************************************************************************************************************
//***************************************************************** FUNCTION 1 ****************************************************************************
void doFunction1()
//***************************************************************** FUNCTION 1 ****************************************************************************
//*********************************************************************************************************************************************************
{
  //***************************************************************** ALL WATER LEVELS ********************************************************************

  //****************************************************************** LIGHTS *****************************************************************************
  // Lights romved for now
  /*  LEDon = digitalRead(RadionPin);
    T5on = digitalRead(T5tubesPin);
    if (LEDon == LOW) {
      sendCommand("bt12.pic0=66");
      sendCommand("bt12.pic1=66");
    } else {
      sendCommand("bt12.pic0=64");
      sendCommand("bt12.pic1=64");
    }
    if (T5on == LOW) {
      sendCommand("bt13.pic0=67");
      sendCommand("bt13.pic1=67");
    } else {
      sendCommand("bt13.pic0=65");
      sendCommand("bt13.pic1=65");
    }
  */
  //****************************************************************** SALT TANK LEVEL **********************************************************************
  saltfull = digitalRead(salthigh);
  saltempty = digitalRead(saltlow);
  if (saltfull != lastSaltFull) {
    if (saltfull == LOW) {
      sendCommand("bt22.pic0=44");
    } else {
      sendCommand("bt22.pic0=47");
    }
    lastSaltFull = saltfull;
  }
  if (saltempty != lastSaltEmpty) {
    if (saltempty == LOW) {
      sendCommand("bt21.pic0=43");
    } else {
      sendCommand("bt21.pic0=47");
    }
    lastSaltEmpty = saltempty;
  }
  //****************************************************************** SUMP MAIN LEVEL **********************************************************************
  sumpwchg = digitalRead(sumpchg);
  sumpfull = digitalRead(sumphigh);
  sumpempty = digitalRead(sumplow);
  if (sumpwchg != lastSumpWchg) {
    if (sumpwchg == LOW) {
      sendCommand("bt103.pic0=46");
    } else {
      sendCommand("bt103.pic0=47");
    }
    lastSumpWchg = sumpwchg;
  }
  if (sumpfull != lastSumpFull) {
    if (sumpfull == LOW) {
      sendCommand("bt102.pic0=44");
    } else {
      sendCommand("bt102.pic0=47");
    }
    lastSumpFull = sumpfull;
  }
  if (sumpempty != lastSumpEmpty) {
    if (sumpempty == LOW) {
      sendCommand("bt101.pic0=43");
    } else {
      sendCommand("bt101.pic0=47");
    }
    lastSumpEmpty = sumpempty;
  }
  //****************************************************************** ATO TANK LEVEL ***********************************************************************
  atofull = digitalRead(atohigh);
  atoempty = digitalRead(atolow);
  if (atofull != lastAtoFull) {
    if (atofull == LOW) {
      sendCommand("bt32.pic0=44");
    } else {
      sendCommand("bt32.pic0=47");
    }
    lastAtoFull = atofull;
  }
  if (atoempty != lastAtoEmpty) {
    if (atoempty == LOW) {
      sendCommand("bt31.pic0=43");
    } else {
      sendCommand("bt31.pic0=47");
    }
    lastAtoEmpty = atoempty;
  }
  //****************************************************************** TOP OFF LEVEL ************************************************************************
  topfull = digitalRead(topoff);
  topover = digitalRead(topofffull);
  if (topfull != lastTopFull) {
    if (topfull == LOW) {
      sendCommand("bt41.pic0=44");
    } else {
      sendCommand("bt41.pic0=47");
    }
    lastTopFull = topfull;
  }
  if (topover != lastTopOver) {
    if (topover == LOW) {
      sendCommand("bt42.pic0=46");
      Serial.print("Top OFF 1");
    } else {
      sendCommand("bt42.pic0=47");
      Serial.print("Top OFF 2");
    }
    lastTopOver = topover;
  }
  //****************************************************************** FILTER SOCKS *************************************************************************
  socksblk = digitalRead(socks);
  if (socksblk != lastSocksBlk) {
    if (socksblk == LOW) {
      sendCommand("bt50.pic0=36");
    } else {
      sendCommand("bt50.pic0=35");
    }
    lastSocksBlk = socksblk;
  }
  //****************************************************************** FINAL SUMP LEVEL *********************************************************************
  sump = digitalRead(sumplvl);
  if (sump != lastSump) {
    if (sump == LOW) {
      sendCommand("bt68.pic0=22");
    } else {
      sendCommand("bt68.pic0=21");
    }
    lastSump = sump;
  }
  //***************************************************************** TANK ALARM ***************************************************************************
  // Tank OK
  if (sump == LOW || socksblk == LOW || topfull == HIGH || pump1sw == HIGH || pump2sw == HIGH ) {
    sendCommand("bt111.pic0=1");
    // sendCommand("bt111.pic1=1");
  }
  else {
    sendCommand("bt111.pic0=26");
  }
} // END FUNCTION1
//***************************************************************** END FUNCTION 1 ************************************************************************
//*********************************************************************************************************************************************************

//*********************************************************************************************************************************************************
//***************************************************************** FUNCTION 2 ****************************************************************************
void doFunction2()
{
  {
    {
      // Temp Outside
      uint16_t dht11val;
      memset(buffer, 0, sizeof(buffer));
      {
        int chk = DHTA.read11(DHT11a_PIN);
        Serial.print("A Temperature = ");
        Serial.println(DHTA.temperature);
        Serial.print("A Humidity = ");
        Serial.println(DHTA.humidity);
      }
      dht11val = DHTA.temperature;
      itoa(dht11val, buffer, 10);
      //t2.setText(buffer);
      t2.setText(buffer);
      dht11val = DHTA.humidity;
      itoa(dht11val, buffer, 10);
      t3.setText(buffer);
      yield();
    }
    {
      // Temp water
      uint16_t dht11val;
      memset(buffer, 0, sizeof(buffer));
    }
    Serial.println();
    Serial.print("Number of Devices found on bus = ");
    Serial.println(sensors.getDeviceCount());
    Serial.print("Getting temperatures... ");
    Serial.println();

    // Command all devices on bus to read temperature
    // Sensor No. 1
    sensors.requestTemperatures();
    char buffer[10];
    Serial.print("Probe 01 temperature is:   ");
    Serial.println(sensors.getTempCByIndex(0));
    Serial.println();
    S1Val = sensors.getTempCByIndex(0);
    dtostrf(S1Val, 2, 1, buffer);
    t1.setText(buffer);
    // Sensor No. 2
    Serial.print("Probe 02 temperature is:   ");
    Serial.println(sensors.getTempCByIndex(1));
    Serial.println();
    S2Val = sensors.getTempCByIndex(1);
    itoa(S2Val, buffer, 10);
    t0.setText(buffer);
    yield();
  }
}
//***************************************************************** END FUNCTION 2 ************************************************************************
//*********************************************************************************************************************************************************

//*********************************************************************************************************************************************************
//***************************************************************** FUNCTION 3 ****************************************************************************
void doFunction3() // feed tank timer
{
  {
    sendCommand("bt64.pic0=24"); // Skimmer
    sendCommand("bt9.pic0=14"); // Feed Tank
    sendCommand("bt9.pic1=14"); // Feed Tank
    digitalWrite(skimPin, LOW);
    feedtanksw = HIGH;
  } delay(5000);
  {
    sendCommand("bt64.pic0=27"); // Skimmer
    sendCommand("bt9.pic0=16"); // Feed Tank
    sendCommand("bt9.pic1=16"); // Feed Tank
    digitalWrite(skimPin, HIGH);
    feedtanksw = LOW;
  }
}
//***************************************************************** END FUNCTION 3 ************************************************************************
//*********************************************************************************************************************************************************
//*********************************************************************************************************************************************************
//***************************************************************** VOID SETUP ****************************************************************************
void setup(void) {
  //*************************************************************** VOID SETUP ****************************************************************************
  //*******************************************************************************************************************************************************
  sensors.begin();
  // set the resolution to 9 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 9);
  sensors.setResolution(Probe02, 9);
  //***************************************************************** SET RED INDICATIONS ONCE *************************************************************

  //**************************************************************** OUTPUT BITS ***************************************************************************
  pinMode(pump1Pin, OUTPUT);
  pinMode(pump2Pin, OUTPUT);
  pinMode(uvPin, OUTPUT);
  pinMode(chillPin, OUTPUT);
  pinMode(skimPin, OUTPUT);
  pinMode(drainPin, OUTPUT);
  pinMode(atoPin, OUTPUT);
  pinMode(saltPin, OUTPUT);
  // pinMode(RadionPin, OUTPUT);
  // pinMode(T5tubesPin, OUTPUT);
  // int Radion = 21; // LED for now
  // int T5tubes = 23; // LED for now
  //***************************************************************** INPUT BITS ***************************************************************************
  pinMode(salthigh, INPUT);
  pinMode(saltlow, INPUT);
  pinMode(atohigh, INPUT);
  pinMode(atolow, INPUT);
  pinMode(topoff, INPUT);
  pinMode(topofffull, INPUT);
  pinMode(sumpchg, INPUT);
  pinMode(sumphigh, INPUT);
  pinMode(sumplow, INPUT);
  pinMode(sumplvl, INPUT);
  pinMode(socks, INPUT);
  //***************************************************************** SET INPUT BITS ***********************************************************************
  digitalWrite(salthigh, HIGH);
  digitalWrite(saltlow, HIGH);
  digitalWrite(atohigh, HIGH);
  digitalWrite(atolow, HIGH);
  digitalWrite(topoff, HIGH);
  digitalWrite(topofffull, HIGH);
  digitalWrite(sumpchg, HIGH);
  digitalWrite(sumphigh, HIGH);
  digitalWrite(sumplow, HIGH);
  digitalWrite(sumplvl, HIGH);
  digitalWrite(socks, HIGH);
  //digitalWrite(sumpfilltog, LOW);         //******* CHECK ON SIM *******************
  digitalWrite(pump1Pin, HIGH);
  digitalWrite(pump2Pin, HIGH);
  digitalWrite(uvPin, HIGH);
  digitalWrite(chillPin, HIGH);
  digitalWrite(skimPin, LOW);
  digitalWrite(drainPin, LOW);
  digitalWrite(atoPin, LOW);
  digitalWrite(saltPin, LOW);
  // digitalWrite(RadionPin, HIGH);
  // digitalWrite(T5tubesPin, HIGH);

  // **** Sets bits back to normal arfter a restart****
  nexInit();
  pump1btn.attachPop(pump1btnReleaseCallback, &pump1btn);
  pump2btn.attachPop(pump2btnReleaseCallback, &pump2);
  uvbtn.attachPop(uvbtnReleaseCallback, &uvbtn);
  chillbtn.attachPop(chillbtnReleaseCallback, &chillbtn);
  skimbtn.attachPop(skimbtnReleaseCallback, &skimbtn);
  drainbtn.attachPop(drainbtnReleaseCallback, &drainbtn);
  fillatobtn.attachPop(fillatobtnReleaseCallback, &fillatobtn);
  fillsaltbtn.attachPop(fillsaltbtnReleaseCallback, &fillsaltbtn);
  fullautobtn.attachPop(fullautobtnReleaseCallback, &fullautobtn);
  feedbtn.attachPop(feedbtnReleaseCallback, &feedbtn);
  waterchgbtn.attachPop(waterchgbtnReleaseCallback, &waterchgbtn);
  // LEDbtn.attachPop(LEDbtnReleaseCallback, &LEDbtn);
  // T5btn.attachPop(T5btnReleaseCallback, &T5btn);
  //***************************************************************** SERIAL 115200 ************************************************************************
  Serial.begin(115200);
  dbSerialPrintln("setup done");
  // Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin();
}
void serialFlush() {
  while (Serial.available() > 0) {
  }
}
//*********************************************************************************************************************************************************
//******************************************************************* VOID LOOP ***************************************************************************
void loop()
{
  //***************************************************************** VOID LOOP ****************************************************************************
  //********************************************************************************************************************************************************

  //***************************************************************** LOOP 1 TIMER *************************************************************************
  //***************************************************************** Nextion Indications ******************************************************************
  unsigned long currentMillis = millis();
  if (currentMillis - prevSenMillis >= refreshSenInterval)
  {
    prevSenMillis = currentMillis; // save the last time we checked
    yield();
    doFunction1();
  }
  //***************************************************************** LOOP 2 TIMER *************************************************************************
  //***************************************************************** Check Tempratures ********************************************************************
  if (currentMillis - prevTempResetMillis >= tempResetInterval)
  {
    prevTempResetMillis = currentMillis; // save the last time we checked
    yield();
    doFunction2();
  }
  //***************************************************************** LOOP 3 ******************************************************************************
  //***************************************************************** Feed Tank Skimmer *******************************************************************
  // feedtanksw
  if (feedtanksw == HIGH && waterchangesw == LOW) {
    yield();
    doFunction3();
  }
  //***************************************************************** WATER CHANGE BUTTON ******************************************************************
  if (waterchangesw != lastWaterChangesw) {
    if (waterchangesw == HIGH) {
      sendCommand("bt10.pic0=41");
    } else {
      sendCommand("bt10.pic0=40");
    }
    lastWaterChangesw = waterchangesw;
  }
  //***************************************************************** PUMP 1 OUTPUT ************************************************************************
  if (pump1sw == LOW && waterchangesw == LOW) {
    sendCommand("bt60.pic0=27"); // Pump 1
    digitalWrite(pump1Pin, HIGH);
  } else {
    sendCommand("bt60.pic0=24"); // Pump 1
    digitalWrite(pump1Pin, LOW);
  }
  //***************************************************************** PUMP 2 OUTPUT ************************************************************************
  if (pump2sw == LOW && waterchangesw == LOW) {
    sendCommand("bt61.pic0=27");// Pump 2
    digitalWrite(pump2Pin, HIGH);
  } else {
    sendCommand("bt61.pic0=24"); // Pump 2
    digitalWrite(pump2Pin, LOW);
  }
  //***************************************************************** UV OUTPUT ****************************************************************************
  if (uvsw == LOW && waterchangesw == LOW && pump2sw == LOW) {
    sendCommand("bt63.pic0=27"); //UV
    digitalWrite(uvPin, HIGH);
  } else {
    sendCommand("bt63.pic0=24"); // UV
    digitalWrite(uvPin, LOW);
  }
  //***************************************************************** CHILLER OUTPUT ************************************************************************
  if (chillsw == LOW && waterchangesw == LOW && pump1sw == LOW) {
    sendCommand("bt62.pic0=27"); // Chiller
    digitalWrite(chillPin, HIGH);
  } else {
    sendCommand("bt62.pic0=24"); // Chiller
    digitalWrite(chillPin, LOW);
  }
  //***************************************************************** SKIMMER OUTPUT ************************************************************************
  if (skimsw == LOW && waterchangesw == LOW) {
    sendCommand("bt64.pic0=27"); // Skimmer
    digitalWrite(skimPin, HIGH);
  } else {
    sendCommand("bt64.pic0=24"); // Skimmer
    digitalWrite(skimPin, LOW);
  }
  //******************************************************************** Radions ***************************************************************************
  /* Lights romved for now
    if (RadionsLED == HIGH) {
      digitalWrite(RadionPin, HIGH);
    } else {
      digitalWrite(RadionPin, LOW);
    }
    if (T5lamps == HIGH) {
      digitalWrite(T5tubesPin, HIGH);
    } else {
      digitalWrite(T5tubesPin, LOW);
    }
  */
  //***************************************************************** WATER CHANGE ************************************************************************
  //byte drainsump;
  //byte sumpdraintog;
  //byte sumpfill;
  //byte sumpfilltog;
  //***************************************************************** DRAIN SUMP AUTO *********************************************************************
  if (waterchangesw == HIGH && sumpfill == LOW) {
    drainsump = HIGH;
  }
  if (drainsump == HIGH && sumpwchg == LOW) {
    sumpdraintog = HIGH;
  }
  if (sumpdraintog == HIGH && sumpempty == LOW) {
    digitalWrite(drainPin, HIGH);
    sendCommand("bt65.pic0=27"); // Sumpdrain
  }
  //***************************************************************** FILL DRAIN CUTOFF *******************************************************************
  if (sumpdraintog == HIGH && sumpempty == HIGH) {
    digitalWrite(drainPin, LOW);
    sendCommand("bt65.pic0=25"); // sumpdrain
    sumpdraintog = LOW;
    drainsump = LOW;
    sumpfill = HIGH;
  } else {
  }
  if (sumpfill == HIGH && sumpwchg == HIGH) {
    sumpfilltog = HIGH;
  }
  //***************************************************************** FILL SUMP AUTO **********************************************************************
  if (sumpfilltog == HIGH) {
    digitalWrite(saltPin, HIGH);
    sendCommand("bt66.pic0=27"); // Salt
  }
  //***************************************************************** FILL SUMP CUTOFF ********************************************************************
  if (sumpfilltog == HIGH && sumpwchg == LOW) {
    digitalWrite(saltPin, LOW);
    sendCommand("bt66.pic0=25"); // Salt
    waterchangesw = LOW;
    sumpfilltog = LOW;
    sumpfill = LOW;
  }
  //***************************************************************** END WATER CHANGE *********************************************************************

  //***************************************************************** DRAIN OUTPUT MANUAL ******************************************************************
  if (drainsw == HIGH && waterchangesw == LOW && sumpempty == LOW) {
    sendCommand("bt65.pic0=27"); // Drain
    digitalWrite(drainPin, HIGH);
  }
  if (drainsw == LOW  && waterchangesw == LOW || sumpempty == HIGH) {
    sendCommand("bt65.pic0=25"); // Drain
    digitalWrite(drainPin, LOW);
    drainsw = LOW;
  }
  //***************************************************************** SUMP FILL OUTPUT MANUAL *************************************************************
  if (saltsw == HIGH && waterchangesw == LOW && saltempty == LOW && sumpwchg == HIGH)  {
    sendCommand("bt66.pic0=27"); // Saltwater
    digitalWrite(saltPin, HIGH);
  }
  if (saltsw == LOW && waterchangesw == LOW || sumpwchg == LOW || saltempty == HIGH) {
    sendCommand("bt66.pic0=25"); // saltwater
    digitalWrite(saltPin, LOW);
    saltsw = LOW;
  }
  //***************************************************************** ATO FILL OUTPUT MANUAL ***************************************************************
  if (atosw == HIGH && atoempty == LOW) {
    sendCommand("bt67.pic0=27"); // Saltwater
    digitalWrite(atoPin, HIGH);
  }
  if (atosw == LOW || atoempty == HIGH || topover == LOW) {
    sendCommand("bt67.pic0=25"); // saltwater
    digitalWrite(atoPin, LOW);
    atosw = LOW;
  }
  nexLoop(nex_listen_list);
}
//***************************************************************** END END END END ***********************************************************************
//***************************************************************** END END END END ***********************************************************************
//***************************************************************** END END END END ***********************************************************************

