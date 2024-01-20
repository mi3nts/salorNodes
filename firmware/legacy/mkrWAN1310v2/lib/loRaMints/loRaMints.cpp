#include "loRaMints.h"


String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void loraInitMints()
{

  Watchdog.reset();

  
  Serial.println("Rebooting the LoRaWAN Radio");
  modem.restart();
  delay(500);

  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1) {
      Serial.println("Rebooting the arduino");
    }
  };

  delay(500);
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
  Serial.println("Setting Adaptive Data Rates");
  modem.setADR(true);  

  Serial.println("Trying to connect into the mints network");

  Watchdog.disable();
  join(0);
  int countdownMS = Watchdog.enable(16000); 
}

void join(u_int8_t trialIndex) {
  // Try to connect --> CHECK How many seconds it takes to connect
  trialIndex++;
  Serial.print("Trial Index: ");
  Serial.println(trialIndex);
  
  int connected = modem.joinOTAA(appEui, appKey);

  Serial.println("Trying to connect to the MINTS LoRaWAN Network."); 
  Serial.print("Join Status: ");
  Serial.println(connected);

  if (!connected) {
    Serial.println("Unable to find the MINTS LoRaWAN Network, trying again");
    delay(10);
    if (trialIndex < 10) {
      join(trialIndex);
    }else{
      //  Rebooting sequence
      int countdownMS = Watchdog.enable(16000); 
      delay(100000);
    }
  }else{
    Serial.println("Connected to the MINTS LoRaWAN Network.");
    return;
  }
}



int loRaSendMints(byte sendOut[], uint8_t numOfBytes, uint8_t portNum){
    Watchdog.reset();
    int err=-1;
    modem.setPort(portNum);
    modem.beginPacket();
    modem.write(sendOut,numOfBytes);
    err = modem.endPacket(false);
    Serial.println("Error Code:");
    Serial.println(err);
 
    delay(5000);
    Watchdog.reset();
    return err;
}

void resetLoRaMints(uint8_t numOfTrysIn,uint8_t powerMode){

  int err=-1;
  Serial.println("Sending Initial Packets");
  byte sendOut[1];
  uint8_t portIn = 102;
  modem.setPort(portIn);
  memcpy(sendOut,&powerMode,1);

  for (uint16_t  cT = 1 ;cT<numOfTrysIn ; cT++){
      err = loRaSendMints(sendOut,1, portIn);
      if(err>0){
        Serial.println("Gateway Contacted");
      break;
     }
  } 
}

