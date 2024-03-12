#include <bluefruit.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include <Arduino.h>


uint16_t sensingPeriod = 2000;
uint16_t initPeriod = 1500;

unsigned long startTime;
uint16_t redLedPin = 3;  
uint16_t blueLedPin = 4;  

uint16_t setPin = 5;  
uint16_t unSetPin = 6;  

// Have a couple of timers - 
// Anyway reboot after 6 + 2 hours 

// Every hour check for pings from the arduino 

// After 2 days - Auto reboot 


#define PIN  8      // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 1 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Initial Setup
unsigned long resetTimeMillis = 18000000 + random(1000)*60;
unsigned long checkTimeMillis = 18000000 + random(1000)*60;
unsigned long startTimeMillis = millis();



void setup() {

  delay(initPeriod);
  initializeSerialMints();

  Serial.println("MINTS Power Cycler");
  Serial.println("-------------------------------------\n");
  pinMode(redLedPin, OUTPUT); 
  pinMode(blueLedPin, OUTPUT); 
  pinMode(setPin, OUTPUT); 
  pinMode(unSetPin, OUTPUT); 
  
  
    pixels.begin();
    delay(5000);
    pixels.clear(); // Set all pixel colors to 'off'
    delay(10);
    pixels.show();
    delay(10);
    ledsOff();
    delay(10);
}


void loop() {

  powerCycle();
  delay(20000);
}

