#include <bluefruit.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include <Arduino.h>

bool debuggingState = false;

uint16_t sensingPeriod = 2000;
uint16_t initPeriod = 1500;

unsigned long startTime;
uint16_t redLedPin = 3;  
uint16_t blueLedPin = 4;  

uint16_t unSetPin = 6;  
uint16_t setPin   = 5;  

uint16_t checkPin = 10;  


#define PIN  8      // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 1 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint16_t currentState  = LOW;


// Have a couple of timers - 
// Anyway reboot after 6 + 2 hours 

// Every hour check for pings from the arduino 

// After 2 days - Auto reboot 

// Timers Setup 

unsigned long pingPeriod       =   3600000;
unsigned long rebootPeriod     =   18000000 + random(1000)*60;

unsigned long waitTimePing     =   1200000;

unsigned long pingTimeStartMillis = millis();
unsigned long rebootTimeStartMillis = millis();

void setup() {

  delay(1000);
  initializeSerialMints();

  if (debuggingState) {
    delay(2000);
    Serial.println("DEBUGGING ON");
    Serial.println("-------------------------------------\n");

    pingPeriod       =   100000;
    rebootPeriod     =   300000;
    waitTimePing     =   10000;

  }

  Serial.println("MINTS Power Cycler");
  Serial.println("-------------------------------------\n");
  pinMode(redLedPin, OUTPUT); 
  pinMode(blueLedPin, OUTPUT); 
  pinMode(setPin, OUTPUT); 
  pinMode(unSetPin, OUTPUT); 
  pinMode(checkPin, INPUT); //
  
  delay(10);
  switchOn();
  pixels.begin();
  delay(10);
  pixels.clear(); // Set all pixel colors to 'off'
  delay(10);
  pixels.show();
  delay(10);
  ledsOff();
  delay(10);

}

void loop() {
    
    if (elapsedTime(pingTimeStartMillis,pingPeriod)){
       if (checkPing(waitTimePing)){
        Serial.println("Ping Heard - Salor Node Alive");
       }else{
        Serial.println("Ping Not Heard - Salor Node is dead - Power Cycling");
        powerCycle();
       }
       pingTimeStartMillis = millis();
    }

    if (elapsedTime(rebootTimeStartMillis,rebootPeriod)) {
        Serial.println("Its time for a reboot");
        powerCycle();
        rebootTimeStartMillis = millis();
    }
}

