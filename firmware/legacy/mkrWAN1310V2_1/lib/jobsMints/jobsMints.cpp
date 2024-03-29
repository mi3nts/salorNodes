#include "jobsMints.h"

void initializeSerialMints(){
    Serial.begin(9600);
    Serial.println("Serial Port Open");
}

void initializeSerial1Mints(){
    Serial1.begin(9600);
    Serial.println("Serial1 Port Open");
}


void sensorPrintMints(String sensor,String readings[],uint8_t numOfvals){
  Serial.print("#mintsO!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(readings[i]);Serial.print(":");
      }
      Serial.print("~");
}

byte sensorPrintBytes(String sensor,byte readings[],uint8_t numOfvals){

  Serial.print("#mintsB!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(readings[i]);Serial.print(":");
      }
      Serial.print("~");
}

void sensorPrintDoubles(String sensor,double readings[],uint8_t numOfvals){
  Serial.print("#mintsD!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(String(readings[i]));Serial.print(":");
      }
      Serial.print("~");
}


void sensorPrintFloats(String sensor,float readings[],uint8_t numOfvals){
  Serial.print("#mintsF!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(String(readings[i]));Serial.print(":");
      }
      Serial.print("~");
}

void sensorPrintUInt16s(String sensor,uint16_t readings[],uint8_t numOfvals){
  Serial.print("#mintsUInt16!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(String(readings[i]));Serial.print(":");
      }
      Serial.print("~");
}

void sensorPrintUInt8s(String sensor,uint8_t readings[],uint8_t numOfvals){
  Serial.print("#mintsUInt8!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(String(readings[i]));Serial.print(":");
      }
      Serial.print("~");
}


void sensorPrintULongs(String sensor,unsigned long readings[],uint8_t numOfvals){
  Serial.print("#mintsUL!");Serial.print(sensor);Serial.print(">");
  for (int i = 0; i < numOfvals; ++i)
      {
        Serial.print(String(readings[i]));Serial.print(":");
      }
      Serial.print("~");
}


void deleteSubString(String &completeStr, String deleteStr){

  int startIndex = completeStr.indexOf(deleteStr);

  while (startIndex != -1) {
    int endIndex = startIndex + deleteStr.length();
    completeStr.remove(startIndex, endIndex - startIndex);
    startIndex = completeStr.indexOf(deleteStr);
  }

  // // Serial.println("Modified String: " + completeStr);
  // return completeStr;
}


int countSubstrings(const String &mainString, const String &substringToCount) {
  int count = 0;
  int index = 0;

  while (true) {
    index = mainString.indexOf(substringToCount, index);
    if (index == -1) {
      break;
    }
    count++;
    index += substringToCount.length();
  }

  return count;
}

void splitString(const String &input, String substrings[], int &substringCount, String splitString) {
  int startIndex = 0;
  int endIndex = 0;
  substringCount = 0;

  while (endIndex != -1) {
    endIndex = input.indexOf(splitString, startIndex);

    if (endIndex == -1) {
      substrings[substringCount] = input.substring(startIndex);
    } else {
      substrings[substringCount] = input.substring(startIndex, endIndex);
    }
    startIndex = endIndex + 1;
    substringCount++;
  }

}





// void loraSendMints(byte sendOut[], uint8_t numOfBytes, uint8_t timeOut, uint8_t portNum){
//   Serial.println("");
//   lora.setPort(portNum);
//   lora.transferPacket(sendOut,numOfBytes,timeOut);

// }

// byte minMaxFloatMints(float inVal, float min, float max){
//   if (inVal<min) {
//     return 0;
//   }else if (inVal>max){
//     return 255;
//   }else {
//   return round(inVal);
//   }

// }

// unsigned long minMaxFloat2ULongMints(float inVal, float min, float max){
//   if (inVal<min) {
//     return 1022;
//   }else if (inVal>max){
//     return 1023;
//   }else {
//   return round(inVal);
//   }

// }

// byte sensorPrintBytes(String sensor,byte readings[],uint8_t numOfvals){

//   Serial.print("#mintsB!");Serial.print(sensor);Serial.print(">");
//   for (int i = 0; i < numOfvals; ++i)
//       {
//         Serial.print(readings[i]);Serial.print(":");
//       }
//       Serial.print("~");
// }


// void sensorPrintFloats(String sensor,float readings[],uint8_t numOfvals){
//   Serial.print("#mintsF!");Serial.print(sensor);Serial.print(">");
//   for (int i = 0; i < numOfvals; ++i)
//       {
//         Serial.print(String(readings[i]));Serial.print(":");
//       }
//       Serial.print("~");
// }

// void sensorPrintULongs(String sensor,unsigned long readings[],uint8_t numOfvals){
//   Serial.print("#mintsUL!");Serial.print(sensor);Serial.print(">");
//   for (int i = 0; i < numOfvals; ++i)
//       {
//         Serial.print(String(readings[i]));Serial.print(":");
//       }
//       Serial.print("~");
// }













