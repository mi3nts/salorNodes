#ifndef LORA_MINTS_H
#define LORA_MINTS_H

#include <Arduino.h>
#include "credentials.h"
#include <MKRWAN.h>
#include <Adafruit_SleepyDog.h>


extern LoRaModem modem;

// #include "LoRaWan.h"


void loraInitMints();
void joinMints(u_int8_t trialIndex);

//  Make this a boolean call
void resetLoRaMints(uint8_t numOfTrysIn,uint8_t powerMode,bool confirmation);

int loRaSendMints(byte sendOut[],uint8_t numOfBytes,uint8_t portNum);

int loRaSendMintsConfirmed(byte sendOut[], uint8_t numOfBytes, uint8_t portNum);

#endif