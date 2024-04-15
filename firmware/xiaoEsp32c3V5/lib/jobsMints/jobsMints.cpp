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


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void delayMints(unsigned int timeSpent,unsigned int loopInterval){
  unsigned int loopIntervalReal = loopInterval+ 30 ;
  unsigned int waitTime;
  if(loopIntervalReal>timeSpent){
    waitTime = loopIntervalReal - timeSpent;
    delay(waitTime);
  }

}



void switchOff(){
  Serial.println("Switch Off");
  
  delay(10);
  digitalWrite(unSetPin, LOW);
  delay(10);
  digitalWrite(setPin, HIGH);
  delay(10);
  digitalWrite(setPin, LOW);
  delay(10);
}

void switchOn(){
  Serial.println("Switch ON");
  delay(10);
  digitalWrite(setPin, LOW);
  delay(10);
  digitalWrite(unSetPin, HIGH);
  delay(10);
  digitalWrite(unSetPin, LOW);
  delay(10);
     
}



void powerCycle(){
 switchOff();
 delay(5000);
 switchOn(); 
 delay(5000);
 }


bool checkPing(unsigned long waitTimePing){
  Serial.print("Checking Ping! ");
  unsigned long beginTime = millis();
  unsigned long pulseTime = millis();
  int previousState = LOW; // Variable to store the previous state of the pin
  uint32_t risingEdgeCount = 0; // Counter variable for rising edges
  pulseTime = millis();
 
  while(millis() - beginTime <  waitTimePing)
  {
    int currentState = digitalRead(checkPin); // Read the current state of the pin
    delay(20);
    if (currentState == HIGH && previousState == LOW) {

      Serial.println();
      Serial.print("Rising edge detected! ");
      Serial.println();

      if(millis() - pulseTime < 500){
        risingEdgeCount++; // Increment the rising edge counter
        Serial.println();
        Serial.print("Pulse Detected - Pulse Count: ");
        Serial.println(risingEdgeCount); // Increment the rising edge counter
      }else{
        risingEdgeCount = 0; // Increment the rising edge counter
        Serial.println();
        Serial.println("New Pulse Detected ");
      }
      pulseTime = millis();

    }

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
