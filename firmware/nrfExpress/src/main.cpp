#include <bluefruit.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include <Arduino.h>


uint16_t sensingPeriod = 2000;
uint16_t initPeriod = 1500;

unsigned long startTime;


sensorPacket liveDataPacket;

void setup() {

  delay(initPeriod);
  initializeSerialMints();

  Serial.println("MINTS Power Cycler");
  Serial.println("-------------------------------------\n");

}


void loop() {
    startTime  = millis();

    Serial.println("MINTS Power Cycler");
    Serial.println("-------------------------------------\n");

    delayMints(millis() - startTime,100);
  
  }
