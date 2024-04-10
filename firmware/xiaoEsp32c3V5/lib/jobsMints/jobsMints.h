#ifndef JOBS_MINTS_H
#define JOBS_MINTS_H

#include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
// #include "devicesMints.h"


extern uint16_t unSetPin;
extern uint16_t setPin;
extern uint16_t redLedPin;
extern uint16_t blueLedPin;
// extern Adafruit_NeoPixel pixels;
extern uint16_t checkPin;

void print_wakeup_reason();


void initializeSerialMints();
void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals);
void delayMints(unsigned int timeSpent,unsigned int loopInterval);
void switchOff();
void switchOn();
// void ledsOff();
// void ledsOn();
void powerCycle();
bool checkPing(unsigned long waitTimePing);
bool elapsedTime(unsigned long startIn, unsigned long periodIn);
// extern bool stringComplete;
// extern String inputString;
// void commandReadMints();
//
// void serialEvent();







#endif
