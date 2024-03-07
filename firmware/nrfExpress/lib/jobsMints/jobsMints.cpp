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

