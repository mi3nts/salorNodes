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


#define PIN  8      // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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
}





void loop() {
    // startTime  = millis();
    // Serial.println("ON");
    // Serial.println("-------------------------------------\n");
    // digitalWrite(blueLedPin, HIGH);   
    // delay(2500); 
    // digitalWrite(redLedPin, HIGH);    
    // delay(2500);
    // pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    // pixels.show();

    // Serial.println("OFF");
    // Serial.println("-------------------------------------\n");
    // digitalWrite(blueLedPin, LOW);   
    // delay(2500); 
    // digitalWrite(redLedPin, LOW);    
    // delay(2500);    
    // pixels.setPixelColor(0, pixels.Color(150, 0, 0));
    // pixels.show();
    // delay(10000);
    // pixels.clear();
    // pixels.show();
    // delay(10000);

//  Turning the SWITCH ON 
   Serial.println("ON");
   digitalWrite(unSetPin, HIGH);
   delay(10);
   digitalWrite(unSetPin, LOW);
   pixels.setPixelColor(0, pixels.Color(0, 150, 0));
   pixels.show();
   delay(10000);
   Serial.println("OFF");
   digitalWrite(setPin, HIGH);
   delay(10);
   digitalWrite(setPin, LOW);
   pixels.setPixelColor(0, pixels.Color(150, 0, 0));
   pixels.show();
   delay(10000);
  }
