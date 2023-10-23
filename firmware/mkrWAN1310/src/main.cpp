

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

LoRaModem modem;

Adafruit_BME280 bme280; // I2C
bool BME280Online;

SCD30 scd30;
bool SCD30Online;

AS7265X as7265x;
bool AS7265XOnline;

bool RG15Online;

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

// powerStatus = 

void setup() {

  // Initiating Serial Communications for debugging purposes
  initializeSerialMints();
  initializeSerial1Mints();

  Serial.print("==========================================");
  Serial.print("========== MINTS SALOR  NODES ============");
  Serial.print("==========================================");
  
  MLRPS001Online = initializeMLRPS001();
  if (MLRPS001Online){
     readMLRPS001(false);
  }

  loraInitMints();
  resetLoRaMints(10,1);

  BME280Online   = initializeBME280();
  SCD30Online    = initializeSCD30();
  AS7265XOnline  = initializeAS7265X();
  RG15Online     = initializeRG15();
  IPS7100Online  = initializeIPS7100();
  PA1010DOnline  = initializePA1010D();
  resetIPS7100(IPS7100ResetTime);  

}

void loop() {
  
  Serial.println("Reading IPS7100");
  readIPS7100();
  delay(5000);

  Serial.println("Reading BME280");
  readBME280();
  delay(5000);

  Serial.println("Reading IPS7100");
  readIPS7100();
  delay(5000);

  Serial.println("Reading SCD30");
  readSCD30();
  delay(5000);

  Serial.println("Reading IPS7100");
  readIPS7100();
  delay(5000);

  Serial.println("Reading AS7265X1");
  readAS7265X1();
  delay(5000);

  Serial.println("Reading IPS7100");
  readIPS7100();
  delay(5000);

  Serial.println("Reading AS7265X2");
  readAS7265X2();
  delay(5000);

  Serial.println("Reading RG15");
  readRG15()  ;
  delay(5000);

  Serial.println("Reading IPS7100");
  readIPS7100();
  delay(5000);

  Serial.println("Reading MLRPS001");
  readMLRPS001(true)  ;
  delay(5000);

  Serial.println("Reading IPS7100");
  readIPS7100();
  delay(5000);

  Serial.println("Reading PA1010D");
  readPA1010D()  ;
  delay(5000);

}

