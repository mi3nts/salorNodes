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


// extern Adafruit_INA219 ina219Battery;
// extern Adafruit_INA219 ina219Solar;
// bool initializeINA219DuoMints();
// void readINA219DuoMints();
// void readINA219DuoMintsMax();

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

bool initializeRG15();
int readRG15();

extern Adafruit_INA219 ina219Battery;
extern Adafruit_INA219 ina219Solar;
bool initializeMLRPS001();
int readMLRPS001(bool initCheck);
// uint8_t sizeIn = 7;
// unsigned long valuesPC[7];

// uint8_t sizeInBytesPC = sizeof(valuesPC);   
// uint8_t sizeInBytesPM = sizeof(valuesPM);   
// uint8_t sizeInBytes = sizeof(valuesPC) + sizeof(valuesPM);  

// uint8_t getPowerMode(uint8_t powerPin);
// uint32_t getPeriod(uint8_t getPowerMode, String sensorID);


#endif