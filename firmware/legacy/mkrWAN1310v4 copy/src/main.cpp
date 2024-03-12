
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

// Every 5 + 0 to 1 hour reboot
unsigned long resetTimeMillis =  18000000;
unsigned long startTimeMillis = millis();

// Every 2 hours sending a confirmed data packet
unsigned long resetTimeConfirmedMillis = 7200000;
unsigned long startTimeConfirmedMillis = millis();

// // Every 5 + 0 to 1 hour reboot
// unsigned long resetTimeMillis =  120000;
// unsigned long startTimeMillis = millis();

// // Every 2 hours sending a confirmed data packet
// unsigned long resetTimeConfirmedMillis = 60000;
// unsigned long startTimeConfirmedMillis = millis();



void setup() {

  // delay(2000);  // Debugging Purposes only

  // Initiating Serial Communications for debugging purposes
  initializeSerialMints();
  initializeSerial1Mints();

  Serial.println();
  Serial.println("==========================================");
  Serial.println("========== MINTS SALOR  NODES ============");
  Serial.println("==========================================");
  
  Serial.print("Enabled the Watchdog tmer with max countdown of ");
  Serial.print(countdownMS, DEC);
  Serial.println(" milliseconds!");
  Serial.println();

  Serial.print("Reset time (ms) = ");
  Serial.println(resetTimeMillis);
  Serial.println();

  Serial.print("Confirmed data check time (ms) = ");
  Serial.println(resetTimeConfirmedMillis);
  Serial.println();

  MLRPS001Online = initializeMLRPS001();
  if (MLRPS001Online){
     readMLRPS001(false);
  }

  loraInitMints();
  resetLoRaMints(10,1,true);

  BME280Online   = initializeBME280();
  SCD30Online    = initializeSCD30();
  AS7265XOnline  = initializeAS7265X();
  RG15Online     = initializeRG15();
  IPS7100Online  = initializeIPS7100();
  PA1010DOnline  = initializePA1010D();
   
  resetIPS7100(IPS7100ResetTime);

}

void loop() {
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunner(  readBME280,  "BME280",  BME280Online, sensingPeriod);
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunner(   readSCD30,   "SCD30",   SCD30Online, sensingPeriod);
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunner(readAS7265X1,"AS7265X1", AS7265XOnline, sensingPeriod);
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunner(readAS7265X2,"AS7265X2", AS7265XOnline, sensingPeriod);
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunner(    readRG15,    "RG15",    RG15Online, sensingPeriod);
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunner( readPA1010D, "PA1010D", PA1010DOnline, sensingPeriod);
  readRunner( readIPS7100, "IPS7100", IPS7100Online, sensingPeriod);
  readRunnerBool(readMLRPS001, "PA1010D", PA1010DOnline, sensingPeriod, true);

  if ( millis()- startTimeConfirmedMillis  >=resetTimeConfirmedMillis) {
    Watchdog.reset();
    Serial.println("Sending a confirmation data packed");
    resetLoRaMints(10,2,true);
    startTimeConfirmedMillis = millis();
    delay(sensingPeriod);
  }
  
  if ( millis()- startTimeMillis >=resetTimeMillis) {
    Serial.println("Resetting node");
    delay(100000);
  }


}
