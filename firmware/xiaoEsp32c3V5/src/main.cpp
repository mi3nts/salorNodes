#include <Arduino.h>
#include "jobsMints.h"
#include "devicesMints.h"


RTC_DATA_ATTR int bootCount = 0;

bool debuggingState = false;

unsigned long startTime;


uint16_t checkPin = D0;  
uint16_t unSetPin = D2;  
uint16_t setPin   = D4;  

uint16_t bootAfter = 5;  

// Timer Setup 
unsigned long long pingPeriod       =   3600000; // It looks for a ping every hour
unsigned long long waitTimePing     =    900000 ; // It listens to pings if not there for 15 minutes

void setup(){
  initializeSerialMints();
  delay(10);

  pinMode(setPin, OUTPUT); 
  delay(15);
  pinMode(unSetPin, OUTPUT); 
  delay(15);
  pinMode(checkPin, INPUT); //
  delay(15);

  delay(5000);
  switchOn();

  Serial.println();

  if (debuggingState) {
    Serial.println("DEBUGGING ON T1");
    pingPeriod         =   3600000 ;  // It looks for a ping half an hour
    waitTimePing       =    900000 ;  // Each chage of state is expected withing 15 minutes at most
  }

  //Increment boot number and print it every reboot

  Serial.println("Boot number: " + String(bootCount));
  Serial.println();

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  delay(1000);
  esp_sleep_enable_timer_wakeup(1000ULL * pingPeriod); // This is in Micro Seconds //This is just defining the timer
  delay(1000);

switch (bootCount) 
  {
  case 0:
    Serial.print("Initial Boot - Letting it run for ");
    Serial.print(pingPeriod);
    Serial.print(" Micro Seconds");
    break;

  case 8:
    delay(1000);
    Serial.println("Forced Power Cycling");
    delay(1000);
    bootCount = 1 ;
    delay(1000);
    powerCycle();
    break;

  default:
    if (checkPing(waitTimePing)) {
      Serial.println("Ping heard - Salor node alive");
    } else {
      Serial.println("Ping not heard - Salor node is dead - Power cycling");
      powerCycle();
    }
    break;
  }
    Serial.println();
    delay(1000);
    Serial.println("Going to sleep now");
    delay(1000);
    ++bootCount;
    delay(1000);
    if(Serial)
    {
      Serial.flush();
    }
    esp_deep_sleep_start();
    Serial.println("This will never be printed");

}


void loop() {

}
