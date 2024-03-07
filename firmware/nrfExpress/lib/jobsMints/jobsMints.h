#ifndef JOBS_MINTS_H
#define JOBS_MINTS_H

#include <Arduino.h>
// #include "devicesMints.h"


void initializeSerialMints();
void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals);
void delayMints(unsigned int timeSpent,unsigned int loopInterval);
// extern bool stringComplete;
// extern String inputString;
// void commandReadMints();
//
// void serialEvent();




#endif
