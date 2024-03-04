
#include "devicesMints.h"

//  IPS7100
void resetIPS7100(uint32_t secondsIn){
  Serial.println("Resetting IPS7100");
  for (uint16_t  cT = 1 ;cT<secondsIn ; cT++){
    Watchdog.reset();
    ips7100.update();
    delay(1000);
  } 
}

bool initializeIPS7100(){
    Watchdog.reset();
    delay(10);
    ips7100.update();
    if(ips7100.getPC01()> 0) {
      Serial.println("IPS7100 Initiated");
      delay(10);
      return true;
    }else{
      return false;   
    }
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
  Watchdog.reset();
  delay(10);

  bool status = bme280.begin();  
  // unsigned status = bme280.begin();  
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
  Watchdog.reset();
  delay(10);

  bool status = scd30.begin();  
  
  if (status) {
    
    delay(10);
    scd30.reset();
    
    delay(10);
    scd30.setMeasurementInterval(30);
    // Serial.println(scd30.setMeasurementInterval(30));
    delay(10);
    scd30.setAutoSelfCalibration(true);
    // Serial.println(scd30.setAutoSelfCalibration(true));
    delay(10);
    scd30.beginMeasuring();
    // Serial.println(scd30.beginMeasuring());

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
  // scd30.readMeasurement();
  // delay(10);
  unsigned long startTime = millis(); // Record the start time

  unsigned long duration = 3000; // 3 seconds

  while (millis() - startTime < duration)
  {
    if (scd30.dataAvailable())
    {
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
      if (values[0] >  0 )
      {
            return loRaSendMints(sendOut,sizeInBytes, portIn);
      }
    }
        delay(100);
  }
  Serial.println("SCD30 Data invalid"); 
  return -100;
}


// AS7265X ---------------------------------------
bool initializeAS7265X(){
  Watchdog.reset();
  delay(10);
  
  bool status = as7265x.begin();

  if (status) {
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
  as7265x.takeMeasurements();
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
                      as7265x.getCalibratedL(),
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

int readAS7265X1(){
  Serial.println("AS7265X1 reader");
  uint8_t sizeIn = 9;
  uint8_t portIn = 52;
  String sensorName = "AS7265X1" ;
  as7265x.takeMeasurements();
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



int readAS7265X2(){
  Serial.println("AS7265X2 reader");
  uint8_t sizeIn = 9;
  uint8_t portIn = 53;
  String sensorName = "AS7265X2" ;
  as7265x.takeMeasurements();
  float values[sizeIn]  = {
                      as7265x.getCalibratedI(),
                      as7265x.getCalibratedS(),
                      as7265x.getCalibratedJ(),
                      as7265x.getCalibratedT(),
                      as7265x.getCalibratedU(),
                      as7265x.getCalibratedV(),
                      as7265x.getCalibratedW(),
                      as7265x.getCalibratedK(),
                      as7265x.getCalibratedL(),
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

bool initializeRG15(){
  Watchdog.reset();
  delay(10);
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
  Serial.println("RG15 reader");
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
  Watchdog.reset();
  delay(10);

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
  uint8_t sizeIn = 9;
  uint8_t portIn = 103;
  String sensorName = "MLRPS001" ;
  
  Serial.print("Battery Voltage: ");
  currentPowerStatus.internalBatteryVoltage = (analogRead(A0) / 1023.0)*3.3*2;
  Serial.print(currentPowerStatus.internalBatteryVoltage);
  Serial.println(" V");
  Serial.println();


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
      return loRaSendMints(sendOut,sizeInBytes, portIn);

    return 1;
    }
    else {
      return -200;
    }
  
  }


bool initializePA1010D(){
  Watchdog.reset();
  delay(10);
  bool gpsStatus = pa1010d.begin(0x10);  // The I2C address to use is 0x10
  if (gpsStatus) {
      delay(10);
      pa1010d.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ); // 10 Hz update rate
      delay(10);
      pa1010d.sendCommand(PMTK_SET_BAUD_57600); // 10 Hz update rate
      delay(10);
      pa1010d.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
      delay(10);
      pa1010d.sendCommand("$PMTK320,0*26");
      delay(10);
      pa1010d.sendCommand("$PMTK225,0*2B");
      delay(10);
      pa1010d.sendCommand(PMTK_Q_RELEASE);
      delay(10);
      pa1010d.sendCommand(PMTK_ENABLE_SBAS);
      Serial.println("PA1010D initiated");
  }


  return gpsStatus;
}

int readPA1010D()
  {
  // Serial.println("PA1010D reader");
  uint8_t sizeInDouble  = 2;
  uint8_t sizeInFloat   = 3;
  uint8_t sizeInUint16  = 1;
  uint8_t sizeInUint8   = 8;
  uint8_t portIn = 105;
  String sensorName = "PA1010D" ;

  unsigned long startTime = millis(); // Record the start time

  unsigned long duration = 3000; // 3 seconds
  // pa1010d.sendCommand();
  while (millis() - startTime < duration) 
  {
    pa1010d.read();
    if (pa1010d.newNMEAreceived()) 
    {
      // Serial.println(pa1010d.lastNMEA());
      if (!pa1010d.parse(pa1010d.lastNMEA())) 
      {
        //  Serial.println("No Fix found"); 
        continue;
      }
      if (pa1010d.secondsSinceFix()<.10)
      {
        String sensorName = "PA1010D" ;
        double valuesDouble[sizeInDouble]  = {
                            pa1010d.latitudeDegrees,
                            pa1010d.longitudeDegrees
              };

        float valuesFloat[sizeInFloat]  = {
                            pa1010d.altitude,
                            pa1010d.speed,
                            pa1010d.magvariation,           
              };

        uint16_t valuesUint16[sizeInUint16]  = {
                        uint16_t(pa1010d.year) + 2000,
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
        return loRaSendMints(sendOut,sizeInBytes, portIn);
  
      }
    }
  }

    return - 100;
    }