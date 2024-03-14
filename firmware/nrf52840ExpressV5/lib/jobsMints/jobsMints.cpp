#include "jobsMints.h"

void initializeSerialMints(){
    Serial.begin(9600);
    Serial.println("Serial Port Open");
}

void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals){
  Serial.print("#mintsO!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(readings[i]);Serial.print(":");
      }
      Serial.print("~");
}

void delayMints(unsigned int timeSpent,unsigned int loopInterval){
  unsigned int loopIntervalReal = loopInterval+ 30 ;
  unsigned int waitTime;
  if(loopIntervalReal>timeSpent){
    waitTime = loopIntervalReal - timeSpent;
    delay(waitTime);
  }

}

void switchOn(){
  Serial.println("Switch ON");
  digitalWrite(setPin, LOW);
  delay(10);
  digitalWrite(unSetPin, HIGH);
  //  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  //  pixels.show();
  //  delay(100);
  //  pixels.clear();
  //  pixels.show();
     
}


void switchOff(){
  Serial.println("Switch Off");
  digitalWrite(unSetPin, LOW);
  delay(10);
  digitalWrite(setPin, HIGH);
  
  //  pixels.setPixelColor(0, pixels.Color( 150,0, 0));
  //  pixels.show();
  //  delay(100);
  //  pixels.clear();
  //  pixels.show();
}


void ledsOn(){
    Serial.println("LEDS ON");
    Serial.println("-------------------------------------\n");
    digitalWrite(blueLedPin, HIGH);   
    delay(2500); 
    digitalWrite(redLedPin, HIGH);    
    delay(2500);
}



void ledsOff(){
    Serial.println("LEDS OFF");
    Serial.println("-------------------------------------\n");
    digitalWrite(blueLedPin, LOW);   
    delay(2500); 
    digitalWrite(redLedPin, LOW);    
    delay(2500);    

}



void powerCycle(){
 switchOff();
 delay(5000);
 switchOn(); 
 }


bool checkPing(unsigned long waitTimePing){
  Serial.print("Checking Ping! ");
  unsigned long beginTime = millis();
  unsigned long pulseTime = millis();
  uint16_t previousState = LOW; // Variable to store the previous state of the pin
  uint32_t risingEdgeCount = 0; // Counter variable for rising edges

  while(millis() - beginTime <  waitTimePing)
  {
    int currentState = digitalRead(checkPin); // Read the current state of the pin
    delay(10);
    if (currentState == HIGH && previousState == LOW) {
      Serial.print("Rising edge detected! ");
      Serial.println(risingEdgeCount);
      pulseTime = millis();
      if(millis() - pulseTime < 500){
        risingEdgeCount++; // Increment the rising edge counter
        Serial.println("Pulse Detected ");
      }
    }
    delay(10);
    previousState = currentState; 
  
    if (risingEdgeCount >= 5){
      Serial.print("Resetting System: ");
      Serial.println();
      risingEdgeCount  = 0;
      return true;
    }
  }
  return false;
}

bool elapsedTime(unsigned long startIn, unsigned long periodIn){
  return millis() - startIn > periodIn ;
}
