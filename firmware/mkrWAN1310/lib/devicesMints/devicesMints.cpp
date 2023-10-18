
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
      // Serial.println(rainBuffer); 
      // Serial.println(numOfCommas); 
      deleteSubString(rainBuffer," ");
      // Serial.println(rainBuffer); 
      deleteSubString(rainBuffer,"EventAcc");
      deleteSubString(rainBuffer,"TotalAcc");
      deleteSubString(rainBuffer,"RInt");
      deleteSubString(rainBuffer,"mmph");
      deleteSubString(rainBuffer,"mm");
      deleteSubString(rainBuffer,"Acc");
      // Serial.println(rainBuffer); 
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
  uint8_t sizeIn = 8;
  uint8_t portIn = 103;
  String sensorName = "MLRPS001" ;


  Serial.println("Battery Voltage");
  currentPowerStatus.internalBatteryVoltage = (analogRead(A0) / 4095.0)*3.3*2;
  Serial.println(currentPowerStatus.internalBatteryVoltage);
  if (initCheck)
  {
    currentPowerStatus.batteryShuntVoltage   = ina219Battery.getShuntVoltage_mV();
    currentPowerStatus.batteryBusVoltage     = ina219Battery.getBusVoltage_V();
    currentPowerStatus.batteryCurrent        = ina219Battery.getCurrent_mA();
    currentPowerStatus.batteryPower          = ina219Battery.getPower_mW();
    currentPowerStatus.solarShuntVoltage     = ina219Solar.getShuntVoltage_mV();
    currentPowerStatus.solarBusVoltage       = ina219Solar.getBusVoltage_V();
    currentPowerStatus.solarCurrent          = ina219Solar.getCurrent_mA();
    currentPowerStatus.solarPower            = ina219Solar.getPower_mW();            

    float values[sizeIn]  = {
                        currentPowerStatus.batteryShuntVoltage   ,
                        currentPowerStatus.batteryBusVoltage     ,
                        currentPowerStatus.batteryCurrent        ,
                        currentPowerStatus.batteryPower          ,  
                        currentPowerStatus.solarShuntVoltage     ,
                        currentPowerStatus.solarBusVoltage       ,
                        currentPowerStatus.solarCurrent          ,
                        currentPowerStatus.solarPower            ,                         
                        currentPowerStatus.internalBatteryVoltage,
                        };


      uint8_t sizeInBytes = sizeof(values);                    
      byte sendOut[sizeInBytes];
      memcpy(sendOut,&values,sizeof(values));
      sensorPrintFloats(sensorName,values,sizeIn);
      Serial.println(" ");   
      sensorPrintBytes(sensorName,sendOut,sizeInBytes);
      Serial.println(" ");   
      if (values[0] > 0){
          return loRaSendMints(sendOut,sizeInBytes, portIn);
          }else{
            return -100;
      }
    return 1;
    }
    else {
      return -200;
    }
  
  }


bool initializePA1010D(){
  bool gpsStatus = pa1010d.begin(0x10);  // The I2C address to use is 0x10
  if (gpsStatus) {
      pa1010d.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGAGSA);
      pa1010d.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ); // 10 Hz update rate
      pa1010d.sendCommand(PGCMD_NOANTENNA);
      delay(1000);
      // pa1010d.println(PMTK_Q_RELEASE);
  }
  return gpsStatus;
}

int readPA1010D(){
  // Serial.println("PA1010D reader");
  uint8_t sizeInDouble  = 2;
  uint8_t sizeInFloat   = 3;
  uint8_t sizeInUint16  = 1;
  uint8_t sizeInUint8   = 8;
  uint8_t portIn = 105;
  String sensorName = "PA1010D" ;
  
  unsigned long startTime = millis(); // Record the start time

// Define the duration in milliseconds for which you want the loop to run
  unsigned long duration = 30000; // 5 seconds

  while (millis() - startTime < duration) {
    char c = pa1010d.read();

    if (pa1010d.newNMEAreceived()) {
      Serial.println(pa1010d.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!pa1010d.parse(pa1010d.lastNMEA())) {// this also sets the newNMEAreceived() flag to false
      continue;
    }
      if (pa1010d.secondsSinceFix()<.10){

          String sensorName = "PA1010D" ;

              double valuesDouble[sizeInDouble]  = {
                            pa1010d.latitudeDegrees,
                            pa1010d.longitudeDegrees
              };// 42 bytes 

              float valuesFloat[sizeInFloat]  = {
                            pa1010d.altitude,
                            pa1010d.speed,
                            pa1010d.magvariation,           
              };// 42 bytes 

              uint16_t valuesUint16[sizeInUint16]  = {
                         pa1010d.year + 2000,
              };

              uint8_t valuesUint8[sizeInUint8]  = {
                              pa1010d.month,
                              pa1010d.day,
                              pa1010d.hour,
                              pa1010d.minute,
                              pa1010d.seconds,
                              pa1010d.satellites,  
                              pa1010d.fixquality,
                              pa1010d.fixquality_3d,
                             };  

            uint8_t sizeInBytesDouble = sizeof(valuesDouble);  
            uint8_t sizeInBytesFloat  = sizeof(valuesFloat);          
            uint8_t sizeInBytesUint16 = sizeof(valuesUint16);   
            uint8_t sizeInBytesUint8  = sizeof(valuesUint8);
            
            uint8_t sizeInBytes       =  sizeInBytesDouble + sizeInBytesFloat + sizeInBytesUint16+ sizeInBytesUint8;

            byte sendOutDouble[sizeInBytesDouble];
            byte sendOutFloat[sizeInBytesFloat];        
            byte sendOutUint16[sizeInBytesUint16];
            byte sendOutUint8[sizeInBytesUint8];
            
            byte sendOut[sizeInBytes];

            memcpy(sendOutDouble,&valuesDouble,sizeof(valuesDouble));
            memcpy(sendOutFloat,&valuesFloat,sizeof(valuesFloat));            
            memcpy(sendOutUint16,&valuesUint16,sizeof(valuesUint16));
            memcpy(sendOutUint8,&valuesUint8,sizeof(valuesUint8));

            memcpy(sendOut, &sendOutDouble, sizeof(valuesDouble));
            memcpy(sendOut + sizeInBytesDouble, &sendOutFloat, sizeof(valuesFloat));
            memcpy(sendOut + sizeInBytesDouble + sizeInBytesFloat, &sendOutUint16, sizeof(valuesUint16));            
            memcpy(sendOut + sizeInBytesDouble + sizeInBytesFloat + sizeInBytesUint16, &sendOutUint8 ,sizeof(valuesUint8));
            
            sensorPrintDoubles(sensorName,valuesDouble,sizeInDouble);
            sensorPrintFloats(sensorName,valuesFloat,sizeInFloat);
            sensorPrintUInt16s(sensorName,valuesUint16,sizeInUint16);
            sensorPrintUInt8s(sensorName,valuesUint8,sizeInUint8);

            sensorPrintBytes(sensorName,sendOut,sizeInBytes);
            
            delay(5000);
            return loRaSendMints(sendOut,sizeInBytes, portIn);
  
        // return 100;
        }
      }
    }


    return - 100;
    }
// uint8_t hour;          ///< GMT hours
        // uint8_t minute;        ///< GMT minutes
        // uint8_t seconds;       ///< GMT seconds
        // uint16_t milliseconds; ///< GMT milliseconds
        // uint8_t year;          ///< GMT year
        // uint8_t month;         ///< GMT month
        // uint8_t day;           ///< GMT day
        //  nmea_float_t latitude;  ///< Floating point latitude value in degrees/minutes
        //                           ///< as received from the GPS (DDMM.MMMM)
        //   nmea_float_t longitude; ///< Floating point longitude value in degrees/minutes
        //                           ///< as received from the GPS (DDDMM.MMMM)

          /** Fixed point latitude and longitude value with degrees stored in units of
            1/10000000 of a degree. See pull #13 for more details:
            https://github.com/adafruit/Adafruit-GPS-Library/pull/13 */
          // int32_t latitude_fixed;  ///< Fixed point latitude in decimal degrees.
          //                          ///< Divide by 10000000.0 to get a double.
          // int32_t longitude_fixed; ///< Fixed point longitude in decimal degrees
          //                          ///< Divide by 10000000.0 to get a double.

          // nmea_float_t latitudeDegrees;  ///< Latitude in decimal degrees
          // nmea_float_t longitudeDegrees; ///< Longitude in decimal degrees
          // nmea_float_t geoidheight;      ///< Diff between geoid height and WGS84 height
          // nmea_float_t altitude;         ///< Altitude in meters above MSL
          // nmea_float_t speed;            ///< Current speed over ground in knots
          // nmea_float_t angle;            ///< Course in degrees from true north
          // nmea_float_t magvariation; ///< Magnetic variation in degrees (vs. true north)
          // nmea_float_t HDOP; ///< Horizontal Dilution of Precision - relative accuracy
          //                    ///< of horizontal position
          // nmea_float_t VDOP; ///< Vertical Dilution of Precision - relative accuracy
          //                    ///< of vertical position
          // nmea_float_t PDOP;
          // uint8_t fixquality;    ///< Fix quality (0, 1, 2 = Invalid, GPS, DGPS)
          // uint8_t fixquality_3d; ///< 3D fix quality (1, 3, 3 = Nofix, 2D fix, 3D fix)
          // uint8_t satellites;





      //   Serial.println("New Fix Found");
      //   Serial.println(pa1010d.lastNMEA());
      //    Serial.print("Time: ");
      //   if (pa1010d.hour < 10) { Serial.print('0'); }
      //   Serial.print(pa1010d.hour, DEC); Serial.print(':');
      //   if (pa1010d.minute < 10) { Serial.print('0'); }
      //   Serial.print(pa1010d.minute, DEC); Serial.print(':');
      //   if (pa1010d.seconds < 10) { Serial.print('0'); }
      //   Serial.print(pa1010d.seconds, DEC); Serial.print('.');
      //   if (pa1010d.milliseconds < 10) {
      //     Serial.print("00");
      //     } else if (pa1010d.milliseconds > 9 && pa1010d.milliseconds < 100) {
      //       Serial.print("0");
      //     }
      // Serial.println(pa1010d.milliseconds);
      // Serial.print("Date: ");
      // Serial.print(pa1010d.day, DEC); Serial.print('/');
      // Serial.print(pa1010d.month, DEC); Serial.print("/20");
      // Serial.println(pa1010d.year, DEC);
      // Serial.print("Fix: "); Serial.print((int)pa1010d.fix);
      // Serial.print(" quality: "); Serial.println((int)pa1010d.fixquality);
      // Serial.print("Location: ");
      // Serial.print(pa1010d.latitude, 4); Serial.print(pa1010d.lat);
      // Serial.print(", ");
      // Serial.print(pa1010d.longitude, 4); Serial.println(pa1010d.lon);
      // Serial.print("Speed (knots): "); Serial.println(pa1010d.speed);
      // Serial.print("Angle: "); Serial.println(pa1010d.angle);
      // Serial.print("Altitude: "); Serial.println(pa1010d.altitude);
      // Serial.print("Satellites: "); Serial.println((int)pa1010d.satellites);
      // delay(1000);