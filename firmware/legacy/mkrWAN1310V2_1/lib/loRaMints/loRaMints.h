#ifndef LORA_MINTS_H
#define LORA_MINTS_H

#include <Arduino.h>
#include "credentials.h"
#include <MKRWAN.h>
#include <Adafruit_SleepyDog.h>


extern LoRaModem modem;

// #include "LoRaWan.h"
void loraInitMints();
void join(u_int8_t  trialIndex);

//  Make this a boolean call
void resetLoRaMints(uint8_t numOfTrysIn,uint8_t powerMode,bool confirmation);

int loRaSendMints(byte sendOut[],uint8_t numOfBytes,uint8_t portNum);

int loRaSendMintsConfirmed(byte sendOut[],uint8_t numOfBytes,uint8_t portNum);

// extern uint8_t rebootPin; 
// 
// void sensorPrintULongs(String sensor,unsigned long readings[],uint8_t numOfvals);
// void sensorPrintFloats(String sensor,float readings[],uint8_t numOfvals);
// byte sensorPrintBytes(String sensor,byte readings[],uint8_t numOfvals);
// 
// byte minMaxFloatMints(float inVal, float min, float max);
// unsigned long minMaxFloat2ULongMints(float inVal, float min, float max);

// void initializeReboot(uint8_t rebootPin);
// void rebootBoard(uint8_t rebootPin);
// void checkReboot(uint8_t powerModeIn,uint8_t rebootPinIn);
// void checkRebootCycle(uint8_t powerModeIn,uint8_t rebootPinIn,uint8_t numOfCycles);
// bool readNow(bool sensorOnline,uint32_t startIn,uint32_t periodIn,bool initialIn);
#endif