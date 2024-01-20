#include "loRaMints.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void loraInitMints()
{
  delay(10);
  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1) {
        Serial.println("Rebooting the Arduino");
        // Watchdog.enable(16000);  // Not desabled yet
        delay(100000);
    }
  };

  Serial.println("Rebooting the LoRaWAN radio");
  modem.restart();
  delay(1000);
  Serial.println("Rebooted the LoRaWAN radio");
  Serial.println();

  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1) {
        Serial.println("Rebooting the Arduino");
        // Watchdog.enable(16000); 
        delay(100000);
    }
  };

  delay(1000);

  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  Serial.print("Your device app key is: ");
  Serial.println(appKey);  
  Serial.println();

  Serial.println("MKR WAN 1310 channel masking:");
  Serial.println(modem.getChannelMask());
  delay(10);

  // MASK MATCHING MINTS LORAWAN CONFIGARATION 

  modem.sendMask("000000000000ff0000000000");
  Serial.println(modem.getChannelMask());
  Serial.println();

  //  Adaptive data rates setting up
  Serial.println("Setting adaptive data rates");
  modem.setADR(true);  

  Serial.println();
  Serial.println("Disabling the watchdog timer");
  Watchdog.disable();
  Serial.println("Trying to connect to the MINTS LoRaWAN Network."); 
  join(0);
  Serial.println("Enabling the watchdog timer");
  Watchdog.enable(16000); 
}


void join(u_int8_t trialIndex) {
  trialIndex++;
  Serial.print("Trial index: ");
  Serial.println(trialIndex);
  
  int connected = modem.joinOTAA(appEui, appKey);

  Serial.print("Join status: ");
  Serial.println(connected);

  if (!connected) {
    Serial.println("Unable to find the MINTS LoRaWAN Network, trying again");
    delay(10);
    if (trialIndex < 3) {
      join(trialIndex);
    }else{
      Serial.println("MINTS LoRaWAN Network not found. Rebooting node");
      Watchdog.enable(16000); 
      delay(100000);
    }
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
    Serial.print("Error code: ");
    Serial.println(err);
    Watchdog.reset();
    delay(1000);
    return err;
}

int loRaSendMintsConfirmed(byte sendOut[], uint8_t numOfBytes, uint8_t portNum){
    int err=-1;
    modem.setPort(portNum);
    modem.beginPacket();
    modem.write(sendOut,numOfBytes);
    err = modem.endPacket(true);
    Serial.print("Error code: ");
    Serial.println(err);
    Watchdog.reset();
    delay(1000);
    return err;
}



void resetLoRaMints(uint8_t numOfTrysIn,uint8_t powerMode,bool confirmation)
{
  int err = -1;
  Serial.println("Sending data packets");
  byte sendOut[1];
  uint8_t portIn = 102;
  modem.setPort(portIn);
  memcpy(sendOut,&powerMode,1);

  for (uint16_t  cT = 1 ;cT<numOfTrysIn ; cT++){
    Watchdog.reset();
    if (confirmation){
      err = loRaSendMintsConfirmed(sendOut,1, portIn);
    }else{
      err = loRaSendMints(sendOut,1, portIn);
    }
    if(err>0){
      Serial.println("Gateway contacted");
      Serial.println();
      return;
    }
  }

  Serial.println("Connection to the gateway is lost, reeboting node");
  delay(100000);
  return;
}
