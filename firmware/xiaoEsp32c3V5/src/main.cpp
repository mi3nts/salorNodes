#include <Arduino.h>
#include "jobsMints.h"
#include "devicesMints.h"


#define mS_TO_S_FACTOR 1000  /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

bool debuggingState = false;

uint16_t sensingPeriod = 2000;
uint16_t initPeriod = 1500;

unsigned long startTime;


uint16_t checkPin = D0;  
uint16_t unSetPin = D2;  
uint16_t setPin   = D4;  



// Timers Setup 

unsigned long pingPeriod       =   3600000; // It looks for a ping every hour
unsigned long waitTimePing     =   900000 ; // It listens to pings if not there for 15 minutes

unsigned long pingTimeStartMillis = millis();
unsigned long rebootTimeStartMillis = millis();


void setup(){
  initializeSerialMints();
  delay(10);

  pinMode(setPin, OUTPUT); 
  delay(15);
  pinMode(unSetPin, OUTPUT); 
  delay(15);
  pinMode(checkPin, INPUT); //
  delay(15);

  delay(10000);
  switchOn();

  Serial.println();

  if (debuggingState) {
    Serial.println("DEBUGGING ON");
    pingPeriod       =   60000;  // It looks for a ping every minute
    waitTimePing     =   30000;  // Each chage of state is expected withing 30 seconds
  }
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(pingPeriod * mS_TO_S_FACTOR); // This is in Micro Seconds 

  if (bootCount == 1) 
  {
  Serial.print("Initial Boot - Letting it run for ");
  Serial.print(pingPeriod);
  Serial.print(" Micro Seconds");
  
  } else if (bootCount <= 7){
    if (checkPing(waitTimePing)){
      Serial.println("Ping heard - Salor node alive");
    }else{
      Serial.println("Ping not heard - Salor node is dead - Power cycling");
      powerCycle();
    }
  } else {
    bootCount = 0;
    delay(1000);
    powerCycle();
  }
    Serial.println();
    delay(1000);
    Serial.println("Going to sleep now");
    delay(1000);
    Serial.flush(); 
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}



void loop() {
    // powerCycle();
    //     delay(10000);
}

