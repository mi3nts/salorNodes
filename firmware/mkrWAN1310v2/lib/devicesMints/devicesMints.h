#ifndef DEVICES_MINTS_H
#define DEVICES_MINTS_H

#include <math.h>
#include "loRaMints.h"
#include "jobsMints.h"

#include "IpsI2C.h"
#include "Adafruit_BME280.h"
#include "Adafruit_INA219.h"
#include "SparkFun_SCD30_Arduino_Library.h"
#include "SparkFun_AS7265X.h"
#include "Adafruit_GPS.h"



struct powerStatus
 {
  float batteryShuntVoltage;          
  float batteryBusVoltage;   
  float batteryCurrent;   
  float batteryPower;   
  float solarShuntVoltage;          
  float solarBusVoltage;   
  float solarCurrent;   
  float solarPower; 
  float internalBatteryVoltage;   
} ;

extern IpsSensor ips7100;
bool initializeIPS7100();
int readIPS7100();
void resetIPS7100(uint32_t secondsIn);

extern Adafruit_BME280 bme280;
#define SEALEVELPRESSURE_HPA (1013.25)
bool initializeBME280();
int readBME280();

extern SCD30 scd30;
bool initializeSCD30();
int readSCD30();

extern AS7265X as7265x;
bool initializeAS7265X();
int readAS7265X();
int readAS7265X1();
int readAS7265X2();

bool initializeRG15();
int readRG15();

extern Adafruit_INA219 ina219Battery;
extern Adafruit_INA219 ina219Solar;
bool initializeMLRPS001();
int readMLRPS001(bool initCheck);
extern powerStatus currentPowerStatus;

extern Adafruit_GPS pa1010d;
bool initializePA1010D();
int readPA1010D();
#define GPSECHO false



#endif