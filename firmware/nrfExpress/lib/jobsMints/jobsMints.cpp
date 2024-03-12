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
   digitalWrite(unSetPin, HIGH);
   delay(10);
   digitalWrite(unSetPin, LOW);
   pixels.setPixelColor(0, pixels.Color(0, 150, 0));
   pixels.show();
   delay(100);
   pixels.clear();
   pixels.show();
     
}


void switchOff(){
   Serial.println("Switch Off");
   digitalWrite(setPin, HIGH);
   delay(10);
   digitalWrite(setPin, LOW);
   pixels.setPixelColor(0, pixels.Color( 150,0, 0));
   pixels.show();
   delay(100);
   pixels.clear();
   pixels.show();
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








