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

// Anyway reboot after 6 hours 

// Every hour check for pings from the arduino 


// Timers Setup 

unsigned long pingPeriod       =   3600000; // It looks for a ping every hour
unsigned long rebootPeriod     =   28800000; // It reboots what ever happends every 8 hours

unsigned long waitTimePing     =   900000 ; // It listens to pings if not there for 15 minutes

unsigned long pingTimeStartMillis = millis();
unsigned long rebootTimeStartMillis = millis();

void setup() {
  delay(2000);
  initializeSerialMints();
  delay(2000);
  Serial.println("MINTS Salor Nodes");
  Serial.println("-------------------------------------");


  Serial.println("MINTS Power Cycler");
  Serial.println("-------------------------------------");
  pinMode(redLedPin, OUTPUT); 
  pinMode(blueLedPin, OUTPUT); 
  pinMode(setPin, OUTPUT); 
  pinMode(unSetPin, OUTPUT); 

  digitalWrite(unSetPin, LOW);
  digitalWrite(setPin, LOW);
  
  pinMode(checkPin, INPUT); //


  if (debuggingState) {
    Serial.println("DEBUGGING ON");
    Serial.println("-------------------------------------");

    pingPeriod       =   360000; // It looks for a ping every 6 minutes
    rebootPeriod     =   420000; // It reboots what ever happends every 7 minutes
    waitTimePing     =   300000;  // Each chage of state is expected withing 5 minutes

    delay(10);
    powerCycle();

  }

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

    Serial.println("Waiting for ping check time");
    if (elapsedTime(pingTimeStartMillis,pingPeriod)){
       Serial.println("Check ping function activated");
       if (checkPing(waitTimePing)){
        Serial.println("Ping heard - Salor node alive");
       }else{
        Serial.println("Ping not heard - Salor node is dead - Power cycling");
        powerCycle();
       }
       pingTimeStartMillis = millis();
    }

    if (elapsedTime(rebootTimeStartMillis,rebootPeriod)) {
        Serial.println("Its time for a reboot");
        powerCycle();
        rebootTimeStartMillis = millis();
    }
    
    delay(10000);

}

