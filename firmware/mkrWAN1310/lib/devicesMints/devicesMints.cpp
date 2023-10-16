
#include "devicesMints.h"

//  IPS7100
void resetIPS7100(uint32_t secondsIn){
  Serial.println("Resetting IPS7100");
  for (uint16_t  cT = 1 ;cT<secondsIn ; cT++){

    ips7100.update();
    delay(1000);
  } 
}

bool initializeIPS7100(){
    Serial.println("IPS7100 Initiated");
   return true;
}

int readIPS7100(){

  Serial.println("IPS7100 reader");
  uint8_t sizeIn = 7;
  uint8_t portIn = 15;
  
  String sensorName = "IPS7100" ;
  ips7100.update();
  unsigned long valuesPC[sizeIn]  = {
                      ips7100.getPC01(),
                      ips7100.getPC03(),
                      ips7100.getPC05(),
                      ips7100.getPC10(),
                      ips7100.getPC25(),
                      ips7100.getPC50(),                      
                      ips7100.getPC100()
  };

  float valuesPM[sizeIn]  = {
                      ips7100.getPM01(),
                      ips7100.getPM03(),
                      ips7100.getPM05(), 
                      ips7100.getPM10(),
                      ips7100.getPM25(),
                      ips7100.getPM50(),                      
                      ips7100.getPM100()
                      };

  uint8_t sizeInBytesPC = sizeof(valuesPC);   
  uint8_t sizeInBytesPM = sizeof(valuesPM);   
  uint8_t sizeInBytes = sizeof(valuesPC) + sizeof(valuesPM);  

  byte sendOutPC[sizeInBytesPC];
  byte sendOutPM[sizeInBytesPM];
  byte sendOut[sizeInBytes];

  memcpy(sendOutPC,&valuesPC,sizeof(valuesPC));
  memcpy(sendOutPM,&valuesPM,sizeof(valuesPM));
  memcpy(sendOut, &sendOutPC, sizeof(valuesPC));
  memcpy(sendOut + sizeInBytesPC, &sendOutPM, sizeof(valuesPC));

  sensorPrintULongs(sensorName,valuesPC,sizeIn);
        Serial.println(" ");
  sensorPrintFloats(sensorName,valuesPM,sizeIn);
        Serial.println(" ");
  sensorPrintBytes(sensorName,sendOutPC,sizeInBytesPC);
        Serial.println(" ");
  sensorPrintBytes(sensorName,sendOutPM,sizeInBytesPM);
        Serial.println(" ");                 
  sensorPrintBytes(sensorName,sendOut,sizeInBytes);
     Serial.println(" ");  
    if (valuesPC[0] >  0 ){
      return loRaSendMints(sendOut,sizeInBytes, portIn);
    }
    else {
      return -100;
    }
}


bool initializeBME280()
{
    unsigned status = bme280.begin();  
    if (status) {
        Serial.println("BME280 Initiated");
        return true;
    }else{        
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme280.sensorID(),16);
        Serial.print("ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("ID of 0x60 represents a BME 280.\n");
        Serial.print("ID of 0x61 represents a BME 680.\n");
        return false;
    }
}

int readBME280()
{
  Serial.println("BME280 reader");
  uint8_t sizeIn = 5;
  uint8_t portIn = 22;
  String sensorName = "BME280V2" ;
  float temperature = bme280.readTemperature();
  float pressure = bme280.readPressure()/100;
  float humidity = bme280.readHumidity();
  float A = (100*pressure) / 101325;
  float B = 1 / 5.25588;
  float C = pow(A, B);
        C = 1.0 - C;
  float altitude = C / 0.0000225577;
  float dewPoint = 243.04 * (log(humidity/100.0) +((17.625 * temperature)/(243.04 + temperature)))/
          (17.625 - log(humidity/100.0) -
         ((17.625 * temperature)/(243.04 + temperature)));

  float values[sizeIn]  = {
                      temperature,
                      pressure,
                      humidity,
                      dewPoint,
                      altitude
                      };
  uint8_t sizeInBytes =sizeof(values);                    
  byte sendOut[sizeInBytes];
  memcpy(sendOut,&values,sizeof(values));
  sensorPrintFloats(sensorName,values,sizeIn);
  Serial.println(" ");   
  sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  Serial.println(" ");   
  return loRaSendMints(sendOut,sizeInBytes, portIn);
}

// SCD30 ---------------------------------------
bool initializeSCD30(){
  if (scd30.begin()) {
    delay(10);
    scd30.setMeasurementInterval(10);
    delay(1);
    Serial.println("SCD30 Initiated");
    return true;
  }else{
    Serial.println("SCD30 not found");
    delay(1);
    return false;
  }
}

int readSCD30(){
  Serial.println("SCD30 reader");
  uint8_t sizeIn = 3;
  uint8_t portIn = 33;
  String sensorName = "SCD30" ;
  float values[sizeIn]  = {
                      float(scd30.getCO2()),
                      scd30.getTemperature(),
                      scd30.getHumidity()
                      };


    uint8_t sizeInBytes = sizeof(values);                    
    byte sendOut[sizeInBytes];
    memcpy(sendOut,&values,sizeof(values));
    sensorPrintFloats(sensorName,values,sizeIn);
    Serial.println(" ");   
    sensorPrintBytes(sensorName,sendOut,sizeInBytes);
    Serial.println(" ");   
    if (values[0] >  0 ){
        return loRaSendMints(sendOut,sizeInBytes, portIn);
    }else{
      return -100;
    }
  }

// SCD30 ---------------------------------------
bool initializeAS7265X(){
  if (as7265x.begin()) {
    as7265x.setIntegrationCycles(1);
    as7265x.disableBulb(AS7265x_LED_WHITE);
    as7265x.disableBulb(AS7265x_LED_IR);
    as7265x.disableBulb(AS7265x_LED_UV);
    delay(1);
    as7265x.disableIndicator();
    delay(1);
    Serial.println("AS7265x Initiated");
    return true;
  }else{
    Serial.println("AS7265x not found");
    delay(1);
    return false;
  }
}

int readAS7265X(){
  Serial.println("AS7265X reader");
  uint8_t sizeIn = 18;
  uint8_t portIn = 51;
  String sensorName = "AS7265X" ;
  float values[sizeIn]  = {
                      as7265x.getCalibratedA(),
                      as7265x.getCalibratedB(),
                      as7265x.getCalibratedC(),
                      as7265x.getCalibratedD(),
                      as7265x.getCalibratedE(),
                      as7265x.getCalibratedF(),
                      as7265x.getCalibratedG(),
                      as7265x.getCalibratedH(),
                      as7265x.getCalibratedR(),
                      as7265x.getCalibratedI(),
                      as7265x.getCalibratedS(),
                      as7265x.getCalibratedJ(),
                      as7265x.getCalibratedT(),
                      as7265x.getCalibratedU(),
                      as7265x.getCalibratedV(),
                      as7265x.getCalibratedW(),
                      as7265x.getCalibratedK(),
                      as7265x.getCalibratedL()
                      };


    uint8_t sizeInBytes = sizeof(values);                    
    byte sendOut[sizeInBytes];
    memcpy(sendOut,&values,sizeof(values));
    sensorPrintFloats(sensorName,values,sizeIn);
    Serial.println(" ");   
    sensorPrintBytes(sensorName,sendOut,sizeInBytes);
    Serial.println(" ");   
    if (values[0] >  0 ){
        return loRaSendMints(sendOut,sizeInBytes, portIn);
    }else{
      return -100;
    }
  }



//  Serial.println(Serial1.readString());
bool initializeRG15(){

  Serial1.readString();
  Serial1.println('R');
  delay(10);
  Serial1.println('O');
  delay(10);
  Serial1.println('R');
  delay(10);
  Serial1.println('H');
  delay(10);
  Serial1.println('M');
  delay(10);
  Serial1.println('P');  
  delay(10);

  String rainBuffer = Serial1.readString();

  if (rainBuffer.indexOf("Acc") != -1) {
    Serial.println("RG15 Initiated");
    // Serial.println(rainBuffer);
    return true;
  }else{
    Serial.println("RG15 not found");
    delay(1);
    return false;
  }
}

int readRG15(){
  uint8_t sizeIn = 4;
  uint8_t portIn = 61;
  String sensorName = "RG15" ;
  
  Serial1.println('R');
  delay(10);
  String rainBuffer = Serial1.readString();
  int numOfCommas = countSubstrings(rainBuffer,",");

  if (numOfCommas ==3 && rainBuffer.indexOf("Acc") != -1){
      Serial.println(rainBuffer); 
      Serial.println(numOfCommas); 
      deleteSubString(rainBuffer," ");
      Serial.println(rainBuffer); 
      deleteSubString(rainBuffer,"EventAcc");
      deleteSubString(rainBuffer,"TotalAcc");
      deleteSubString(rainBuffer,"RInt");
      deleteSubString(rainBuffer,"mmph");
      deleteSubString(rainBuffer,"mm");
      deleteSubString(rainBuffer,"Acc");
      Serial.println(rainBuffer); 
      // at this point make sure that you 
      int subStringCount = numOfCommas +1;
      String subStrings[subStringCount];
      splitString(rainBuffer, subStrings,subStringCount,",") ;
      float values[sizeIn]  = {
                   subStrings[0].toFloat(),
                   subStrings[1].toFloat(),
                   subStrings[2].toFloat(),
                   subStrings[3].toFloat()
                          };

    uint8_t sizeInBytes = sizeof(values);                    
    byte sendOut[sizeInBytes];
    memcpy(sendOut,&values,sizeof(values));
    sensorPrintFloats(sensorName,values,sizeIn);
    Serial.println(" ");   
    sensorPrintBytes(sensorName,sendOut,sizeInBytes);
    Serial.println(" ");   
    return loRaSendMints(sendOut,sizeInBytes, portIn);
    }
    else{
      return -100;
    }
  }

// INS219s ---------------------------------------
bool initializeMLRPS001(){
  bool batteryINA219Status = ina219Battery.begin();
  bool solarINA219Status   = ina219Solar.begin();

  if (batteryINA219Status) {
      Serial.println("Battery INA219 Initiated");
  }else{
    Serial.println("Battery INA219 not found");
    delay(1);
  }
  if (solarINA219Status) {
      Serial.println("Solar INA219 Initiated");
  }else{
    Serial.println("Solar INA219 not found");
    delay(1);
  }

  return (batteryINA219Status && solarINA219Status);
}


int readMLRPS001(bool initCheck){
  Serial.println("MLRPS001 reader");
  uint8_t sizeIn = 3;
  uint8_t portIn = 33;
  String sensorName = "MLRPS001" ;

  float internalBatteryVoltage = (analogRead(A0) / 4095.0) * 3.3*2;
  Serial.println(internalBatteryVoltage);
  // float values[sizeIn]  = {
  //                     scd30.getCO2(),
  //                     scd30.getTemperature(),
  //                     scd30.getHumidity()
  //                     };
  //   uint8_t sizeInBytes = sizeof(values);                    
  //   byte sendOut[sizeInBytes];
  //   memcpy(sendOut,&values,sizeof(values));
  //   sensorPrintFloats(sensorName,values,sizeIn);
  //   Serial.println(" ");   
  //   sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  //   Serial.println(" ");   
  //   if (values[0] >  0 ){
  //       return loRaSendMints(sendOut,sizeInBytes, portIn);
  //   }else{
  //     return -100;
  //   }
  return 1;
  }









  // for (int i = 0; i < subStringCount; i++) {
  //   Serial.println(subStrings[i]);
  // }

  // if (rainBuffer.indexOf("Acc") != -1) {
  //   int firstComma = rainBuffer.indexOf(',');
  //   int secondComma = rainBuffer.indexOf(',', firstComma + 1);
  //   int thirdComma = rainBuffer.indexOf(',', secondComma + 1);

  //   if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
  //     subString1 = rainBuffer.substring(4, firstComma);
  //     subString2 = rainBuffer.substring(firstComma + 11, secondComma);
  //     subString3 = rainBuffer.substring(secondComma + 11, thirdComma);
  //     subString4 = rainBuffer.substring(thirdComma + 7);
  //     Serial.println(subString1);
  //     Serial.println(subString2);
  //     Serial.println(subString3);
  //     Serial.println(subString4);
  //   }
  // }

  //   return 1;
// }


  // Serial.println(Serial1.readString());
  // Serial.println("RG15 reader");
  // uint8_t sizeIn = 18;
  // uint8_t portIn = 51;
  // String sensorName = "RG15" ;
  // float values[sizeIn]  = {
  //                     as7265x.getCalibratedA(),
  //                     as7265x.getCalibratedB(),
  //                     as7265x.getCalibratedC(),
  //                     as7265x.getCalibratedD(),
  //                     as7265x.getCalibratedE(),
  //                     as7265x.getCalibratedF(),
  //                     as7265x.getCalibratedG(),
  //                     as7265x.getCalibratedH(),
  //                     as7265x.getCalibratedR(),
  //                     as7265x.getCalibratedI(),
  //                     as7265x.getCalibratedS(),
  //                     as7265x.getCalibratedJ(),
  //                     as7265x.getCalibratedT(),
  //                     as7265x.getCalibratedU(),
  //                     as7265x.getCalibratedV(),
  //                     as7265x.getCalibratedW(),
  //                     as7265x.getCalibratedK(),
  //                     as7265x.getCalibratedL()
  //                     };


  //   uint8_t sizeInBytes = sizeof(values);                    
  //   byte sendOut[sizeInBytes];
  //   memcpy(sendOut,&values,sizeof(values));
  //   sensorPrintFloats(sensorName,values,sizeIn);
  //   Serial.println(" ");   
  //   sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  //   Serial.println(" ");   
  //   if (values[0] >  0 ){
  //       return loRaSendMints(sendOut,sizeInBytes, portIn);
  //   }else{
  //     return -100;
  //   }







// bool initializeGPGGALRMints()
// {
//   return true;
// }

// void readGPGGALRMintsMax()
// {
//   unsigned long waitTime = 5000;  
//   unsigned long start = millis();
//   Serial.begin(9600); 
//   do 
//   {
//     while (Serial.available())
//       gpggalr.encode(Serial.read());
//   } while (millis() - start < waitTime);

// Serial.print("Num of Satelites Validity: ");
// Serial.println(gpggalr.satellites.isValid());
// Serial.print("Num of Satelites: ");
// Serial.println(gpggalr.satellites.value());

// Serial.print("HDOP Validity: ");
// Serial.println(gpggalr.hdop.isValid());
// Serial.print("HDOP: ");
// Serial.println(gpggalr.hdop.hdop());

// Serial.print("Location Validity: ");
// Serial.println(gpggalr.location.isValid());
// Serial.print("Latitude: ");
// Serial.println(gpggalr.location.lat(),10);
// Serial.print("Longitude: ");
// Serial.println(gpggalr.location.lng(),10);
// Serial.print("Location Age: ");
// Serial.println(gpggalr.location.age());

// Serial.print("Date: ");
// Serial.print(gpggalr.date.year());
// Serial.print(":");
// Serial.print(gpggalr.date.month());
// Serial.print(":");
// Serial.println(gpggalr.date.day());  

// Serial.print("Time: ");
// Serial.print(gpggalr.time.hour());
// Serial.print(":");
// Serial.print(gpggalr.time.minute());
// Serial.print(":");
// Serial.println(gpggalr.time.second());

// Serial.print("Altitude Validity: ");
// Serial.println(gpggalr.altitude.isValid());
// Serial.print("Altitude: ");
// Serial.println(gpggalr.altitude.meters());

// Serial.print("Course Validity: ");
// Serial.println(gpggalr.course.isValid());
// Serial.print("Course: ");
// Serial.println(gpggalr.course.deg());

// Serial.print("Speed Validity: ");
// Serial.println(gpggalr.course.isValid());
// Serial.print("Speed: ");
// Serial.println(gpggalr.speed.kmph());

// Serial.print("Altitude: ");
// Serial.println(gpggalr.altitude.meters());

// if (gpggalr.location.isValid())
//   {
//     uint8_t portIn       = 5;
//     uint8_t sizeInDouble = 6;
//     uint8_t sizeInUint16 = 1;
//     uint8_t sizeInUint8  = 5;
    
//     String sensorName = "GPGGALR" ;
//     double valuesDouble[sizeInDouble]  = {
//                    gpggalr.location.lat(),
//                    gpggalr.location.lng(),
//                    gpggalr.speed.kmph(),
//                    gpggalr.altitude.meters(),
//                    gpggalr.course.deg(),
//                    gpggalr.hdop.hdop()
//     };// 42 bytes 

//     uint16_t valuesUint16[sizeInUint16]  = {
//                  gpggalr.date.year()
//     };
//     uint8_t valuesUint8[sizeInUint8]  = {
//                      gpggalr.date.month(),
//                      gpggalr.date.day(),
//                      gpggalr.time.hour(),
//                      gpggalr.time.minute(),
//                      gpggalr.time.second(),
//                   };  

//   uint8_t sizeInBytesDouble = sizeof(valuesDouble);   
//   uint8_t sizeInBytesUint16 = sizeof(valuesUint16);   
//   uint8_t sizeInBytesUint8  = sizeof(valuesUint8);
//   uint8_t sizeInBytes       =  sizeInBytesDouble + sizeInBytesUint16+ sizeInBytesUint8;

//   byte sendOutDouble[sizeInBytesDouble];
//   byte sendOutUint16[sizeInBytesUint16];
//   byte sendOutUint8[sizeInBytesUint8];
  
//   byte sendOut[sizeInBytes];

//   memcpy(sendOutDouble,&valuesDouble,sizeof(valuesDouble));
//   memcpy(sendOutUint16,&valuesUint16,sizeof(valuesUint16));
//   memcpy(sendOutUint8,&valuesUint8,sizeof(valuesUint8));

//   memcpy(sendOut, &sendOutDouble, sizeof(valuesDouble));
//   memcpy(sendOut + sizeInBytesDouble, &sendOutUint16, sizeof(valuesUint16));
//   memcpy(sendOut + sizeInBytesDouble + sizeInBytesUint16, &sendOutUint8 ,sizeof(valuesUint8));

//   loRaSendMints(sendOut,sizeInBytes,5,portIn);

//  }

// }


// //  INA 219 
// bool initializeINA219DuoMints()
// {

//   if (ina219Battery.begin()&ina219Solar.begin()) 
//   {
//         Serial.println("INA219s Initiated");
//         delay(1);
//         return true;
//   }
//       else
//       {
//       Serial.println("INA219s not found");
//       delay(1);

//       return false;
//       }
// }



// uint8_t getPowerMode(uint8_t powerPin)
// {
//   float busVoltageBat    = ina219Battery.getBusVoltage_V();
//   float busVoltageSol    = ina219Solar.getBusVoltage_V();
//   String readings[2] = {  
//                           String( busVoltageBat,2), 
//                           String( busVoltageSol,2)};
//   sensorPrintMints("INA219Duo",readings,2);

//   if (abs(busVoltageBat) <= 3.3 ) 
//   {
//     rebootBoard(powerPin);
//     return 0;
//   }

//   if ((abs(busVoltageBat))> 3.3 && (abs(busVoltageBat)<= 3.5)) {
//     return 4;
//   }

//   if (abs(busVoltageBat) > 4.00)
//   {
//     if (abs(busVoltageSol) > 5 ){
//         return 3;
//     }else{
//       return 7;
//     }
//   }


//   if (abs(busVoltageBat)> 3.5 && abs(busVoltageBat)<= 4)  {
//     if (abs(busVoltageSol)<=5)
//     {
//       return 5;
//     }else
//     {
//       return 6;
//     }
//   }
//     rebootBoard(powerPin);
//     return 0 ;
       
// }



// uint32_t getPeriod(uint8_t powerMode, String sensorID)
// {
//   if (sensorID == "IPS7100") {
//     if (powerMode == 3){
//     return 60000; 
//     }else
//     {
//     return 120000; 
//     }
//   }
//   if (sensorID == "BME280") {
//     if (powerMode == 3 ){
//     return 60000; 
//     }else
//     {
//     return 120000; 
//     }
//   }

//   if (sensorID == "INA219Duo") {
//     if (powerMode == 3 ){
//     return 180000; 
//     }else
//     {
//     return 240000; 
//     }
//   }

//   if (sensorID == "GPGGALR") {
//     if (powerMode == 3 ){
//     return 180000; 
//     }else
//     {
//     return 240000; 
//     }
//   }
// return 3000000;

// } // End of Period Seek 
 


// void readINA219DuoMints(){

//   float shuntVoltageBat  = ina219Battery.getShuntVoltage_mV();
//   float busVoltageBat    = ina219Battery.getBusVoltage_V();
//   float currentMaBat     = ina219Battery.getCurrent_mA();
//   float powerMwBat       = ina219Battery.getPower_mW();
  
//   float shuntVoltageSol  = ina219Solar.getShuntVoltage_mV();
//   float busVoltageSol    = ina219Solar.getBusVoltage_V();
//   float currentMaSol     = ina219Solar.getCurrent_mA();
//   float powerMwSol       = ina219Solar.getPower_mW();

//   String readings[8] = { String(shuntVoltageBat,2) , String( busVoltageBat,2), String(currentMaBat ,2) , String(powerMwBat,2) ,
//   String(shuntVoltageSol,2) , String( busVoltageSol,2), String(currentMaSol ,2) , String(powerMwSol,2)};
//   sensorPrintMints("INA219Duo",readings,8);
// }

// void readINA219DuoMintsMax(){
//   uint8_t sizeIn = 8;
//   uint8_t portIn = 3;
//   String sensorName = "INA219Duo" ;
//   float values[sizeIn]  = {
//                       ina219Battery.getShuntVoltage_mV(),
//                       ina219Battery.getBusVoltage_V(),
//                       ina219Battery.getCurrent_mA(),
//                       ina219Battery.getPower_mW(),
//                       ina219Solar.getShuntVoltage_mV(),
//                       ina219Solar.getBusVoltage_V(),
//                       ina219Solar.getCurrent_mA(),
//                       ina219Solar.getPower_mW()
//                       };

//   uint8_t sizeInBytes =sizeof(values);                    
//   byte sendOut[sizeInBytes];
//   memcpy(sendOut,&values,sizeof(values));
//   sensorPrintFloats(sensorName,values,sizeIn);
//   sensorPrintBytes(sensorName,sendOut,sizeInBytes);
//   loRaSendMints(sendOut,sizeInBytes,5,portIn); 
// }


// //  BME280



// // // MGS001  ---------------------------------------

// bool initializeMGS001Mints(){

//   gas.begin(0x04);//the default I2C address of the slave is 0x04
//   gas.powerOn();
//   Serial.println("MGS001 Initiated");
//   Serial.print("MGS001 Firmware Version = ");
//   Serial.println(gas.getVersion());
//   delay(1);

// return true;
// }

// void readMGS001MintsMax(){
//   uint8_t sizeIn = 8;
//   uint8_t portIn = 31;
//   String sensorName = "MGS001" ;
//   float values[sizeIn]  = {
//                         gas.measure_C2H5OH(),
//                         gas.measure_C3H8(),
//                         gas.measure_C4H10(),
//                         gas.measure_CH4(),
//                         gas.measure_CO(),
//                         gas.measure_H2(),
//                         gas.measure_NH3(),
//                         gas.measure_NO2()
//                         };

//   uint8_t sizeInBytes = sizeof(values);                    
//   byte sendOut[sizeInBytes];
//   memcpy(sendOut,&values,sizeof(values));
//   sensorPrintFloats(sensorName,values,sizeIn);
//   sensorPrintBytes(sensorName,sendOut,sizeInBytes);
//   loRaSendMints(sendOut,sizeInBytes,5,portIn); 
// }


// // SCD30 ---------------------------------------
// bool initializeSCD30Mints(uint16_t scdPeriod ){
//   if (scd30.begin()) {
//     delay(10);
//     scd30.setMeasurementInterval(scdPeriod);
//     delay(1);
//     Serial.println("SCD30 Initiated");
//     return true;
//   }else{
//     Serial.println("SCD30 not found");
//     delay(1);
//     return false;
//   }
// }

// void readSCD30MintsMax(){
//   uint8_t sizeIn = 3;
//   uint8_t portIn = 33;
//   String sensorName = "SCD30" ;
//   float values[sizeIn]  = {
//                       scd30.getCO2(),
//                       scd30.getTemperature(),
//                       scd30.getHumidity()
//                       };


//     uint8_t sizeInBytes = sizeof(values);                    
//     byte sendOut[sizeInBytes];
//     memcpy(sendOut,&values,sizeof(values));
//     sensorPrintFloats(sensorName,values,sizeIn);
//     sensorPrintBytes(sensorName,sendOut,sizeInBytes);
//     if (values[0] >  0 ){
//       loRaSendMints(sendOut,sizeInBytes,5,portIn); 
//     }
//   }


// // IPS7100 ---------------------------------------
// bool initializeIPS7100Mints(){
//     // ips7100.begin();
//     Serial.println("IPS7100 Initiated");
//    return true;
// }

// int readIPS7100(){
//   ips7100.update();
//   // ips7100.updatePC();
//   // ips7100.updatePM();
//   uint8_t sizeIn = 7;
//   uint8_t portIn = 15;
  
//   String sensorName = "IPS7100" ;

//   unsigned long valuesPC[sizeIn]  = {
//                       ips7100.getPC01(),
//                       ips7100.getPC03(),
//                       ips7100.getPC05(),
//                       ips7100.getPC10(),
//                       ips7100.getPC25(),
//                       ips7100.getPC50(),                      
//                       ips7100.getPC100()
//   };

//   float valuesPM[sizeIn]  = {
//                       ips7100.getPM01(),
//                       ips7100.getPM03(),
//                       ips7100.getPM05(), 
//                       ips7100.getPM10(),
//                       ips7100.getPM25(),
//                       ips7100.getPM50(),                      
//                       ips7100.getPM100()
                      // };


//   uint8_t sizeInBytesPC = sizeof(valuesPC);   
//   uint8_t sizeInBytesPM = sizeof(valuesPM);   
//   uint8_t sizeInBytes = sizeof(valuesPC) + sizeof(valuesPM);  

//   byte sendOutPC[sizeInBytesPC];
//   byte sendOutPM[sizeInBytesPM];
//   byte sendOut[sizeInBytes];

//   memcpy(sendOutPC,&valuesPC,sizeof(valuesPC));
//   memcpy(sendOutPM,&valuesPM,sizeof(valuesPM));



//   memcpy(sendOut, &sendOutPC, sizeof(valuesPC));
//   memcpy(sendOut + sizeInBytesPC, &sendOutPM, sizeof(valuesPC));

//   sensorPrintULongs(sensorName,valuesPC,sizeIn);
//         Serial.println(" ");
//   sensorPrintFloats(sensorName,valuesPM,sizeIn);
//         Serial.println(" ");
//   sensorPrintBytes(sensorName,sendOutPC,sizeInBytesPC);
//         Serial.println(" ");
//   sensorPrintBytes(sensorName,sendOutPM,sizeInBytesPM);
//             Serial.println(" ");                 

//   sensorPrintBytes(sensorName,sendOut,sizeInBytes);
  
//     if (valuesPC[0] >  0 ){
//       loRaSendMints(sendOut,sizeInBytes,5,portIn); 
//     }

// }


// void resetIPS7100Mints(uint32_t secondsIn){
//   Serial.println("Resetting IPS7100");
//   for (uint16_t  cT = 1 ;cT<secondsIn ; cT++){
//     ips7100.update();
//     delay(1010);
//   } 
// }


