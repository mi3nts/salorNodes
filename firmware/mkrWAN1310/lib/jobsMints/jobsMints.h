#ifndef JOBS_MINTS_H
#define JOBS_MINTS_H

#include <Arduino.h>

void initializeSerialMints();
void initializeSerial1Mints();
void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals);


void sensorPrintFloats(String sensor,float readings[],uint8_t numOfvals);
void sensorPrintULongs(String sensor,unsigned long readings[],uint8_t numOfvals);
byte sensorPrintBytes(String sensor,byte readings[],uint8_t numOfvals);

void deleteSubString(String &completeStr, String deleteStr);
int countSubstrings(const String &mainString, const String &substringToCount);
void splitString(const String &input, String substrings[], int &substringCount, String splitString);

// void loraSendMints(byte sendOut[],uint8_t numOfBytes,uint8_t timeOut,uint8_t portNum);
// byte minMaxFloatMints(float inVal, float min, float max);
// unsigned long minMaxFloat2ULongMints(float inVal, float min, float max);
#endif