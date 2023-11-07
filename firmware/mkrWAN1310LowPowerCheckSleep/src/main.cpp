
// # ***************************************************************************
// #   Salor Nodes
// #   ---------------------------------
// #   Written by: Lakitha Omal Harindha Wijeratne
// #   - for -
// #   MINTS:  Multi-scale Integrated Sensing and Simulation
// #   &  
// #   TRECIS: Texas Research and Education Cyberinfrastructure Services
// #   ---------------------------------
// #   Date: October 9th, 2023
// #   ---------------------------------
// #   This module is written for generic implimentation of LoRaWAN MINTS projects
// #   --------------------------------------------------------------------------
// #   https://github.com/mi3nts
// #   https://mints.utdallas.edu/
// #   https://trecis.cyberinfrastructure.org/
// #  ***************************************************************************

// ADD LINKS OF ALL LIBRARIES USED 

#include <Arduino.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include "loRaMints.h"

unsigned long sensingPeriod = SENSING_PERIOD  ;

LoRaModem modem;

Adafruit_BME280 bme280; // I2C
bool BME280Online;

SCD30 scd30;
bool SCD30Online;

AS7265X as7265x;
bool AS7265XOnline;

bool RG15Online;

    // Watchdog.reset();

// IPS7100 
bool IPS7100Online;
IpsSensor ips7100;
uint32_t IPS7100ResetTime = 30;
uint32_t IPS7100Period   ;

Adafruit_INA219 ina219Battery(0x40);
Adafruit_INA219 ina219Solar(0x41);
bool MLRPS001Online;
powerStatus currentPowerStatus;

Adafruit_GPS pa1010d(&Wire);
bool PA1010DOnline;

// Initial Setup
unsigned long resetTimeMillis =  18000000 + random(1000)*60;
unsigned long startTimeMillis = millis();

void setup() {

  // Initiating Serial Communications for debugging purposes
  initializeSerialMints();
  initializeSerial1Mints();

  Serial.print("==========================================");
  Serial.print("========== MINTS SALOR  NODES ============");
  Serial.print("==========================================");
  
  Serial.println("Reset time (ms) = ");
  Serial.println(resetTimeMillis);


  MLRPS001Online = initializeMLRPS001();
  if (MLRPS001Online){
     readMLRPS001(false);
  }

  loraInitMints();
  resetLoRaMints(10,1);
  // Serial.println((analogRead(A0) / 1023.0)*3.3*2);
  BME280Online   = initializeBME280();
  SCD30Online    = initializeSCD30();
  AS7265XOnline  = initializeAS7265X();
  RG15Online     = initializeRG15();
  IPS7100Online  = initializeIPS7100();
  PA1010DOnline  = initializePA1010D();
   
  resetIPS7100(IPS7100ResetTime);

  int countdownMS = Watchdog.enable(16000);
  Serial.print("Enabled the watchdog with max countdown of ");
  Serial.print(countdownMS, DEC);
  Serial.println(" milliseconds!");
  Serial.println();
   

  // Serial.println(BOARD_TYPE);
}

void loop() {
  
  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading BME280");
  readBME280();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());
  
  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());
  
  Serial.println("Reading SCD30");
  readSCD30();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading AS7265X1");
  readAS7265X1();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading AS7265X2");
  readAS7265X2();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading RG15");
  readRG15()  ;
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading MLRPS001");
  readMLRPS001(true)  ;
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading PA1010D");
  readPA1010D()  ;
  Watchdog.reset();
  LowPower.sleep(sensingPeriod);
  Serial.println(millis());

  if ( millis()- startTimeMillis >=resetTimeMillis) {
    // It's time to reset
    Serial.println("Resetting node");
    LowPower.sleep(100000);
  }


}

