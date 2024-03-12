#ifndef JOBS_MINTS_H
#define JOBS_MINTS_H

#include <Arduino.h>
#include <Adafruit_SleepyDog.h>

void readRunner(int (*readFunction)(), String sensorID, bool sensorOnline, uint32_t sensingPeriod);
void readRunnerBool(int (*readFunction)(bool), String sensorID, bool sensorOnline, uint32_t sensingPeriod,bool boolInput);
void initializeSerialMints();
void initializeSerial1Mints();

void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals);

void sensorPrintFloats(String sensor,float readings[],uint8_t numOfvals);
void sensorPrintDoubles(String sensor,double readings[],uint8_t numOfvals);
void sensorPrintUInt16s(String sensor,uint16_t readings[],uint8_t numOfvals);
void sensorPrintUInt8s(String sensor,uint8_t readings[],uint8_t numOfvals);

void sensorPrintULongs(String sensor,unsigned long readings[],uint8_t numOfvals);
void sensorPrintBytes(String sensor,byte readings[],uint8_t numOfvals);

void deleteSubString(String &completeStr, String deleteStr);
int countSubstrings(const String &mainString, const String &substringToCount);
void splitString(const String &input, String substrings[], int &substringCount, String splitString);

void sendHBPulse(uint8_t hbPin,  unsigned long HighLowPeriodHB, unsigned long maxTimeHB);

#endif