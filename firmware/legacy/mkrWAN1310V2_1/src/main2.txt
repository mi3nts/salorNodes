/*
  Read the 18 channels of spectral light over I2C using the Spectral Triad
  By: Nathan Seidle
  SparkFun Electronics
  Date: October 25th, 2018
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  This example takes all 18 readings, 372nm to 966nm, over I2C and outputs
  them to the serial port.
  
  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15050

  Hardware Connections:
  Plug a Qwiic cable into the Spectral Triad and a BlackBoard
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  Open the serial monitor at 115200 baud to see the output
*/

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

// Remove Initial Delay
// Check All Inits for watchdog resets 
// Get some scenarios 



#include <Arduino.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include "loRaMints.h"

int countdownMS = Watchdog.enable(16000); 


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

// Confirmed data send
unsigned long resetTimeConfirmedMillis = 150000;
unsigned long startTimeConfirmedMillis = millis();


void setup() {
  delay(2000);  // Debugging Purposes only

  // Initiating Serial Communications for debugging purposes
  initializeSerialMints();
  initializeSerial1Mints();
  Serial.println();
  Serial.println();
  Serial.println("==========================================");
  Serial.println("========== MINTS SALOR  NODES ============");
  Serial.println("==========================================");

  
  Serial.print("Enabled the watchdog with max countdown of ");
  Serial.print(countdownMS, DEC);
  Serial.println(" milliseconds!");
  Serial.println();
   
  Serial.print("Reset time (ms) = ");
  Serial.println(resetTimeMillis);
  Serial.println();

  MLRPS001Online = initializeMLRPS001();
  if (MLRPS001Online){
     readMLRPS001(false);
  }

  loraInitMints();

  resetLoRaMints(10,1,true);
  // Serial.println((analogRead(A0) / 1023.0)*3.3*2);
  BME280Online   = initializeBME280();
  SCD30Online    = initializeSCD30();
  AS7265XOnline  = initializeAS7265X();
  RG15Online     = initializeRG15();
  IPS7100Online  = initializeIPS7100();
  PA1010DOnline  = initializePA1010D();
   
  resetIPS7100(IPS7100ResetTime);



  // Serial.println(BOARD_TYPE);
}

void loop() {
  
  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading BME280");
  readBME280();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());
  
  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());
  
  Serial.println("Reading SCD30");
  readSCD30();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading AS7265X1");
  readAS7265X1();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading AS7265X2");
  readAS7265X2();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading RG15");
  readRG15()  ;
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading MLRPS001");
  readMLRPS001(true)  ;
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading IPS7100");
  readIPS7100();
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  Serial.println("Reading PA1010D");
  readPA1010D()  ;
  Watchdog.reset();
  delay(sensingPeriod);
  Serial.println(millis());

  if ( millis()- startTimeMillis >=resetTimeMillis) {
    // It's time to reset
    Serial.println("Resetting node");
    delay(100000);
  }

  if ( millis()- startTimeConfirmedMillis  >=resetTimeConfirmedMillis) {
    Watchdog.reset();
    Serial.println("Sending a confirmation data packed");
    resetLoRaMints(10,2,true);
  }



}