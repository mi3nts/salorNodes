// # ***************************************************************************
// #   Salor Nodes
// #   ---------------------------------
// #   Written by: Lakitha Omal Harindha Wijeratne
// #   - for -
// #   MINTS:  Multi-scale Integrated Sensing and Simulation
// #   &  
// #   TRECIS: Texas Research and Education Cyberinfrastructure Services
// #   ---------------------------------
// #   Date: October 9th, 2023
// #   ---------------------------------
// #   This module is written for generic implimentation of LoRaWAN MINTS projects
// #   --------------------------------------------------------------------------
// #   https://github.com/mi3nts
// #   https://mints.utdallas.edu/
// #   https://trecis.cyberinfrastructure.org/
// #  ***************************************************************************

// ADD LINKS OF ALL LIBRARIES USED 

#include <Arduino.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include "loRaMints.h"

LoRaModem modem;

Adafruit_BME280 bme280; // I2C
bool BME280Online;

SCD30 scd30;
bool SCD30Online;

AS7265X as7265x;
bool AS7265XOnline;

bool RG15Online;

// IPS7100 
bool IPS7100Online;
IpsSensor ips7100;
uint32_t IPS7100ResetTime = 30;
uint32_t IPS7100Period   ;

Adafruit_INA219 ina219Battery(0x40);
Adafruit_INA219 ina219Solar(0x41);
bool MLRPS001Online;
powerStatus currentPowerStatus;

Adafruit_GPS pa1010d(&Wire);
bool PA1010DOnline;

// Initial Setup

// powerStatus = 

void setup() {

  // Initiating Serial Communications for debugging purposes
  initializeSerialMints();
  initializeSerial1Mints();

  Serial.print("==========================================");
  Serial.print("========== MINTS SALOR  NODES ============");
  Serial.print("==========================================");
  
  MLRPS001Online = initializeMLRPS001();
  if (MLRPS001Online){
     readMLRPS001(false);
  }

  loraInitMints();
  resetLoRaMints(10,1);

  BME280Online   = initializeBME280();
  SCD30Online    = initializeSCD30();
  AS7265XOnline  = initializeAS7265X();
  RG15Online     = initializeRG15();
  IPS7100Online  = initializeIPS7100();
  PA1010DOnline  = initializePA1010D();
  // resetIPS7100(IPS7100ResetTime);  

}

void loop() {
  
  // Serial.println("Reading IPS7100");
  // readIPS7100();
  // delay(5000);

  // Serial.println("Reading BME280");
  // readBME280();
  // delay(5000);

  // Serial.println("Reading SCD30");
  // readSCD30();
  // delay(5000);

  // Serial.println("Reading AS7265X");
  // readAS7265X();
  // delay(5000);

  // Serial.println("Reading RG15");
  // readRG15()  ;
  // delay(5000);
  
  // Serial.println("Reading MLRPS001");
  // readMLRPS001(true)  ;
  // delay(5000);

  Serial.println("Reading PA1010D");
  readPA1010D()  ;
  delay(1000);

}



//   while (!Serial);




//     delayTime = 1000;
//     Serial.println();

//  if (scd.begin()) 
//     {
//     delay(10);
//     scd.setMeasurementInterval(5);
//     delay(1);
//     SerialUSB.println("SCD30 Initiated");
//     SCD30Online = true;
//     }else{
//       SerialUSB.println("SCD30 not found");
//       delay(1);
//     SCD30Online =  false;
//     }


//   if (as7265x.begin() == false)
//   {
//     Serial.println("Sensor does not appear to be connected. Please check wiring. Freezing...");
//     while (1)
//       ;
//   }else{

//     as7265x.enableBulb(AS7265x_LED_WHITE);
//     as7265x.enableBulb(AS7265x_LED_IR);
//     as7265x.enableBulb(AS7265x_LED_UV);
//     as7265x.setIntegrationCycles(1);
//     delay(2500);
//     as7265x.disableBulb(AS7265x_LED_WHITE);
//     as7265x.disableBulb(AS7265x_LED_IR);
//     as7265x.disableBulb(AS7265x_LED_UV);
//     delay(1000);
//     as7265x.disableIndicator();
//     delay(2500);
//     Serial.println("AS7265x found");
//   }




// // IPS7100
//   IPS7100Online =  initializeIPS7100Mints();
//   IPS7100Period  = 10;
//   SerialUSB.print("IPS Period: ");
//   SerialUSB.println(IPS7100Period);

// }




// void loop() {


//   //   while (Serial1.available() == 0)
//   // {
//   //   Serial.println("Reading Rain Sensor");
//   //   Serial.println(Serial1.readString());
//   // }

//     Serial.println(Serial1.readString());


//     // resetLoRaMints(numOfTries,powerMode);

//     Serial.print("Temperature = ");
//     Serial.print(bme.readTemperature());
//     Serial.println(" °C");

//     Serial.print("Pressure = ");
//     Serial.print(bme.readPressure() / 100.0F);
//     Serial.println(" hPa");

//     Serial.print("Approx. Altitude = ");
//     Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//     Serial.println(" m");

//     Serial.print("Humidity = ");
//     Serial.print(bme.readHumidity());
//     Serial.println(" %");

//     Serial.println();
//     delay(1000);

//     Serial.print("CO2 = ");
//     Serial.print( scd.getCO2());
//     Serial.println(" °ppm");
    
//     Serial.print("Temperature = ");
//     Serial.print( scd.getTemperature());
//     Serial.println(" °C");
    
//     Serial.print("Humidity = ");
//     Serial.print( scd.getHumidity());
//     Serial.println(" %");
    
//     delay(1000);
//     Serial.println();
//     ips_sensor.update();
    
//     Serial.print("Particle Counts");
//     Serial.println(ips_sensor.getPC01());
//     Serial.println(ips_sensor.getPC03());
//     Serial.println(ips_sensor.getPC05());  
//     Serial.println(ips_sensor.getPC10());
//     Serial.println(ips_sensor.getPC25());
//     Serial.println(ips_sensor.getPC50());  
//     Serial.println(ips_sensor.getPC100());
    
//     Serial.println("Particle Concentrations");
//     Serial.println(ips_sensor.getPM01());
//     Serial.println(ips_sensor.getPM03());
//     Serial.println(ips_sensor.getPM05());  
//     Serial.println(ips_sensor.getPM10());
//     Serial.println(ips_sensor.getPM25());
//     Serial.println(ips_sensor.getPM50());  
//     Serial.println(ips_sensor.getPM100());    
    
//     Serial.println();
//     delay(1000);

//     Serial.print("Shunt Voltage Battery  = ");
//     Serial.print(ina219Battery.getShuntVoltage_mV());
//     Serial.println(" mV");

//     Serial.print("Bus Voltage Battery  = ");
//     Serial.print(ina219Battery.getBusVoltage_V());
//     Serial.println(" V");

//     Serial.print("Current Battery = ");
//     Serial.print(ina219Battery.getCurrent_mA());
//     Serial.println(" mA");

//     Serial.print("Power Battery = ");
//     Serial.print(ina219Battery.getPower_mW());
//     Serial.println("   V");


//     Serial.print("Shunt Voltage Solar = ");
//     Serial.print(ina219Solar.getShuntVoltage_mV());
//     Serial.println(" mV");

//     Serial.print("Bus Voltage Solar = ");
//     Serial.print(ina219Solar.getBusVoltage_V());
//     Serial.println(" V");

//     Serial.print("Current Solar = ");
//     Serial.print(ina219Solar.getCurrent_mA());
//     Serial.println(" mA");

//     Serial.print("Power = ");
//     Serial.print(ina219Solar.getPower_mW());
//     Serial.println("   V");
    
//     delay(1000);
//     Serial.println("");
    
//     as7265x.takeMeasurements(); //This is a hard wait while all 18 channels are measured

//     Serial.print(as7265x.getCalibratedA()); //410nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedB()); //435nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedC()); //460nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedD()); //485nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedE()); //510nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedF()); //535nm
//     Serial.print(",");

//     Serial.print(as7265x.getCalibratedG()); //560nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedH()); //585nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedR()); //610nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedI()); //645nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedS()); //680nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedJ()); //705nm
//     Serial.print(",");

//     Serial.print(as7265x.getCalibratedT()); //730nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedU()); //760nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedV()); //810nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedW()); //860nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedK()); //900nm
//     Serial.print(",");
//     Serial.print(as7265x.getCalibratedL()); //940nm
//     Serial.print(",");

//     Serial.println();

// }


  // change this to your regional band (eg. US915, AS923, ...)
  // if (!modem.begin(US915)) {
  //   Serial.println("Failed to start module");
  //   while (1) {}
  // };
  // Serial.print("Your module version is: ");
  // Serial.println(modem.version());
  // Serial.print("Your device EUI is: ");
  // Serial.println(modem.deviceEUI());

//   int connected = modem.joinOTAA(appEui, appKey);
//   if (!connected) {
//     Serial.println("Something went wrong; are you indoor? Move near a window and retry");
//     while (1) {}
//   }
 // Serial.println();
  // Serial.print("Sending: " + msg + " - ");
  // for (unsigned int i = 0; i < msg.length(); i++) {
  //   Serial.print(msg[i] >> 4, HEX);
  //   Serial.print(msg[i] & 0xF, HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();

//   int err;
//   modem.beginPacket();
//   modem.print(msg);
//   err = modem.endPacket(true);
//   if (err > 0) {
//     Serial.println("Message sent correctly!");
//   } else {
//     Serial.println("Error sending message :(");
//     Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
//     Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
//   }
//   delay(1000);
//   if (!modem.available()) {
//     Serial.println("No downlink message received at this time.");
//     return;
//   }
//   char rcv[64];
//   int i = 0;
//   while (modem.available()) {
//     rcv[i++] = (char)modem.read();
//   }
//   Serial.print("Received: ");
//   for (unsigned int j = 0; j < i; j++) {
//     Serial.print(rcv[j] >> 4, HEX);
//     Serial.print(rcv[j] & 0xF, HEX);
//     Serial.print(" ");
//   }
//   Serial.println();