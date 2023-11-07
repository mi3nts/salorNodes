#include "loRaMints.h"


String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void loraInitMints()
{
  delay(10);
  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1) {}
  };

  delay(1000);
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  Serial.print("Your device App Key is: ");
  Serial.println(appKey);  
  Serial.println("--------------------------------");
  Serial.println("- MKR WAN 1310 Channel Masking -");
  Serial.println("--------------------------------");
  Serial.println(modem.getChannelMask());
  delay(10);

  // MASK MATCHING MINTS LORAWAN CONFIGARATION 

  modem.sendMask("000000000000ff0000000000");
  Serial.println(modem.getChannelMask());

  //  Adaptive data rates setting up
  Serial.println("Setting Adaptive Data  Rates");
  modem.setADR(true);  

  Serial.println("Trying to connect into the mints network");
  join();

}



void join() {
  // Try to connect
  int connected = modem.joinOTAA(appEui, appKey);
  Serial.println("Trying to connect to the MINTS LoRaWAN Network.");    
  if (!connected) {
    Serial.println("Unable to find the mints network, trying again");
    join();
  }else{
    Serial.println("Connected to the MINTS LoRaWAN Network.");
  }
}

int loRaSendMints(byte sendOut[], uint8_t numOfBytes, uint8_t portNum){
    int err=-1;
    modem.setPort(portNum);
    modem.beginPacket();
    modem.write(sendOut,numOfBytes);
    err = modem.endPacket(false);
    SerialUSB.println("Error Code:");
    SerialUSB.println(err);
    Watchdog.reset();
    // LowPower.deepSleep(5000);
    // Watchdog.reset();
    return err;
}

void resetLoRaMints(uint8_t numOfTrysIn,uint8_t powerMode){

  int err=-1;
  SerialUSB.println("Sending Initial Packets");
  byte sendOut[1];
  uint8_t portIn = 102;
  modem.setPort(portIn);
  memcpy(sendOut,&powerMode,1);



  for (uint16_t  cT = 1 ;cT<numOfTrysIn ; cT++){
      err = loRaSendMints(sendOut,1, portIn);
      if(err>0){
        SerialUSB.println("Gateway Contacted");
      break;
     }
  } 
}



// void loraInitMints(char* keyIn,uint8_t powerMode,uint8_t rebootPin)
// {  SerialUSB.println(" ");
//   char buffer[256];
//   SerialUSB.println("Initializing LoRa");
//   lora.init();
//   lora.setDeviceReset();
//   lora.setDeviceDefault();

//   SerialUSB.println("Module Version:");
//   memset(buffer, 0, 256);
//   lora.getVersion(buffer, 256, 1);
//   SerialUSB.print(buffer);

//   SerialUSB.println("Lora Credentials:");
//   memset(buffer, 0, 256);
//   lora.getId(buffer, 256, 1);
//   SerialUSB.println("LoRa IDs: ");
//   SerialUSB.print(buffer);

//   SerialUSB.println("Lora App Key:");
//   lora.setKey(NULL, NULL, keyIn);
 
//   SerialUSB.println("Network Settings:");
//   lora.setDeciveMode(LWOTAA);
//   lora.setDataRate(DR2, US915);
  
//   lora.setChannel(0, 913.5);
//   lora.setChannel(1, 913.7);
//   lora.setChannel(2, 913.9);
//   lora.setChannel(3, 914.1);
//   lora.setChannel(4, 914.3);
//   lora.setChannel(5, 914.5);
//   lora.setChannel(6, 914.7);
//   lora.setChannel(7, 914.9);
//   lora.setReceiceWindowFirst(0,  927.5);
//   lora.setReceiceWindowSecond(927.5, DR8);
//   lora.setJoinDutyCycle(false);

//   lora.setPower(14);  //###
//   lora.setPort(1);
//   lora.setClassType(CLASS_A);

//   SerialUSB.println("Starting Join");
 
//   for (uint8_t i = 1; i <= 10; i++) {{
//      if(lora.setOTAAJoin(JOIN, 10)){
//       SerialUSB.println("Setup Complete");
//       return;
//   }}}
//     SerialUSB.println("No gateway found: Step 1");
//     SerialUSB.println("Delay 1: 1 Minute");
//     delay (60000);
//   for (uint8_t i = 1; i <= 10; i++) {{
//      if(lora.setOTAAJoin(JOIN, 10)){
//       SerialUSB.println("Setup Complete");
//       return;
//   }}} 
//     SerialUSB.println("No gateway found: Step 2");
//     SerialUSB.println("Delay 2: 2 Minutes");
    
//     if(powerMode == 0){
//       rebootBoard(rebootPin);
//     }
//     if(powerMode == 4){
//       rebootBoard(rebootPin);
//     }
//     if(powerMode == 5){
//       rebootBoard(rebootPin);
//     }


//     delay (120000);
//   for (uint8_t i = 1; i <= 10; i++) {{
//      if(lora.setOTAAJoin(JOIN, 10)){
//       SerialUSB.println("Setup Complete");
//       return;
//   }}} 
//   SerialUSB.println("No gateway found: Step 3");
//   SerialUSB.println("Sleeping Module");
//   rebootBoard(rebootPin);
//   delay (120000);
// }







// byte sensorPrintBytes(String sensor,byte readings[],uint8_t numOfvals){

//   SerialUSB.print("#mintsB!");SerialUSB.print(sensor);SerialUSB.print(">");
//   for (int i = 0; i < numOfvals; ++i)
//       {
//         SerialUSB.print(readings[i]);SerialUSB.print(":");
//       }
//       SerialUSB.print("~");
// }


// void sensorPrintFloats(String sensor,float readings[],uint8_t numOfvals){
//   SerialUSB.print("#mintsF!");SerialUSB.print(sensor);SerialUSB.print(">");
//   for (int i = 0; i < numOfvals; ++i)
//       {
//         SerialUSB.print(String(readings[i]));SerialUSB.print(":");
//       }
//       SerialUSB.print("~");
// }

// void sensorPrintULongs(String sensor,unsigned long readings[],uint8_t numOfvals){
//   SerialUSB.print("#mintsUL!");SerialUSB.print(sensor);SerialUSB.print(">");
//   for (int i = 0; i < numOfvals; ++i)
//       {
//         SerialUSB.print(String(readings[i]));SerialUSB.print(":");
//       }
//       SerialUSB.print("~");
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

// //  Booting Menu 

// void initializeReboot(uint8_t rebootPin){
//   pinMode(rebootPin, OUTPUT); 
// }

// void rebootBoard(uint8_t rebootPin){
//     SerialUSB.println();
//     SerialUSB.println("Rebooting Board");
//     digitalWrite(rebootPin, HIGH);
//     delay(1);
//     digitalWrite(rebootPin, LOW);
//     delay(1);
//     // delay(1000000);
// }

// bool readNow(bool sensorOnline,uint32_t startIn,uint32_t periodIn,bool initialIn){
//     if (initialIn){
//       return sensorOnline;
//     }else{
//       return ((((millis() - startIn)> periodIn)&& sensorOnline));
//     }
// }

// void checkReboot(uint8_t powerModeIn,uint8_t rebootPinIn){
//   if((powerModeIn == 1) && (millis() > 360000) ){
//     rebootBoard(rebootPinIn); // Halt the board in the day after 6 minutes
//   }
//   if((powerModeIn == 2) && (millis() > 300000) ){
//     rebootBoard(rebootPinIn); // Halt the board in the day after 5 minutes
//   }
// }

// void checkRebootCycle(uint8_t powerModeIn,uint8_t rebootPinIn,uint8_t numOfCycles){

//   if(powerModeIn == 0){
//     rebootBoard(rebootPinIn); // Halt the board in the day after 6 minutes
//   }
//   if((powerModeIn == 1) && (millis() > 360000) ){
//     rebootBoard(rebootPinIn); // Halt the board in the day after 6 minutes
//   }
//   if((powerModeIn == 2) && (millis() > 300000) ){
//     rebootBoard(rebootPinIn); // Halt the board in the day after 5 minutes
//   }
//   if((powerModeIn == 3) && (millis() > 900000) ){
//     delay(900000);// Reboot the board after 15 minutes
//     rebootBoard(rebootPinIn); // Halt the board in the day after 15 minutes
//   }
//   if((powerModeIn == 4) && (numOfCycles >0))
//   {
//     rebootBoard(rebootPinIn); // Halt the board after 1 cycle
//   }
//   if((powerModeIn == 5) && (numOfCycles >0))
//   {
//     rebootBoard(rebootPinIn); // Halt the board after 1 cycle
//   }
//   if((powerModeIn == 6) && (numOfCycles >1))
//   {
//     rebootBoard(rebootPinIn); // Halt the board after 2 cycles
//   }
//   if((powerModeIn == 7) && (numOfCycles >1))
//   {    
//     rebootBoard(rebootPinIn); // Halt the board after 2 cycles
//   }

  
// }