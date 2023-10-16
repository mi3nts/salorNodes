
import serial
import datetime
import os
import csv
#import deepdish as dd
# from mintsXU4 import mintsLatest as mL
from mintsXU4 import mintsDefinitions as mD
# from mintsXU4 import mintsSensorReader as mSR
from getmac import get_mac_address
import time
import serial
import pynmea2
from collections import OrderedDict
import math
import base64
import json
import struct
import numpy as np
from datetime import timedelta

macAddress     = mD.macAddress
dataFolder     = mD.dataFolder
fPortIDs        = mD.fPortIDs

mqttOn         = mD.mqttOn
decoder        = json.JSONDecoder(object_pairs_hook=OrderedDict)

def loRaSummaryReceive(message,fPortIDs):
    nodeID = message.topic.split('/')[5]
    sensorPackage       =  decoder.decode(message.payload.decode("utf-8","ignore"))
    rxInfo              =  sensorPackage['rxInfo'][0]
    txInfo              =  sensorPackage['txInfo']
    loRaModulationInfo  =  txInfo['loRaModulationInfo']
    sensorID            = fPortIDs[getPortIndex(sensorPackage['fPort'],fPortIDs)]['sensor']
    dateTime            = datetime.datetime.fromisoformat(sensorPackage['publishedAt'][0:26])
    base16Data          = base64.b64decode(sensorPackage['data'].encode()).hex()
    gatewayID           = base64.b64decode(rxInfo['gatewayID']).hex()
    framePort           = sensorPackage['fPort']
    sensorDictionary =  OrderedDict([
            ("dateTime"        , str(dateTime)),
            ("nodeID"          , nodeID),
            ("sensorID"        , sensorID),
            ("gatewayID"       , gatewayID),
            ("rssi"            , rxInfo["rssi"]),
            ("loRaSNR"         , rxInfo["loRaSNR"]),
            ("channel"         , rxInfo["channel"] ),
            ("rfChain"         , rxInfo["rfChain"] ),
            ("frequency"       , txInfo["frequency"]),
            ("bandwidth"       , loRaModulationInfo["bandwidth"]),
            ("spreadingFactor" , loRaModulationInfo["spreadingFactor"] ),
            ("codeRate"        , loRaModulationInfo["codeRate"] ),
            ("dataRate"        , sensorPackage['dr']),
            ("frameCounters"   , sensorPackage['fCnt']),
            ("framePort"       , framePort),
            ("base64Data"      , sensorPackage['data']),
            ("base16Data"      , base16Data),
            ("devAddr"         , sensorPackage['devAddr']),
            ("deviceAddDecoded", base64.b64decode(sensorPackage['devAddr'].encode()).hex())
        ])
    return dateTime,gatewayID,nodeID,sensorID,framePort,base16Data;


def getPortIndex(portIDIn,fPortIDs):
    indexOut = 0
    for portID in fPortIDs:
        if (portIDIn == portID['portID']):
            return indexOut; 
        indexOut = indexOut +1
    return -1;


def encodeDecode(sensorID,sensorData,transmitReceive):
    # print("Encode Decode")
    if sensorID == "PMSalor":
        return sensingPMSalor(sensorData,transmitReceive);    
    if sensorID == "IPS7100CNR":
        return sensingIPS7100CNR(sensorData,transmitReceive);
    if sensorID == "IPS7100":
        return sensingIPS7100(sensorData,transmitReceive);
    if sensorID == "BME688CNR":
        return sensingBME688CNR(sensorData,transmitReceive); 
    if sensorID == "BME280V2":
        return sensingBME280V2(sensorData,transmitReceive); 
    if sensorID == "SCD30":
        return sensingSCD30(sensorData,transmitReceive);           
    if sensorID == "AS7265X":
        return sensingAS7265X(sensorData,transmitReceive);   
    if sensorID == "PM":
        return sensingPM(sensorData,transmitReceive);   
    if sensorID == "PMPoLo":
        return sensingPM(sensorData,transmitReceive); 
    if sensorID == "MacAD":
        return sensingMacAD(sensorData,transmitReceive);      
    if sensorID == "GPGGAPL":
        return sensingGPGGAPL(sensorData,transmitReceive);         
    if sensorID == "GPRMCPL":
        return sensingGPRMCPL(sensorData,transmitReceive);  
    if sensorID == "MBCLR001":
        return sensingMBCLR001(sensorData,transmitReceive);  
    if sensorID == "MBCLR002":
        return sensingMBCLR002(sensorData,transmitReceive);  
    if sensorID == "RG15":
        return sensingRG15(sensorData,transmitReceive);  

    return;   

def sensingBME280V2(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("BME280V2 Read")	
            if (len(dataIn)==5):
                strOut  = \
                    np.float32(dataIn[0]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[1]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[2]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[3]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[4]).tobytes().hex().zfill(8)
                return strOut;  
            else:
                print("Invalid data string read from the BME280")

                return None;

        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"     ,str(dateTime)),
                    ("temperature"  ,struct.unpack('<f',bytes.fromhex(dataIn[0:8]))[0]),
                    ("pressure"     ,struct.unpack('<f',bytes.fromhex(dataIn[8:16]))[0]),
                    ("humidity"     ,struct.unpack('<f',bytes.fromhex(dataIn[16:24]))[0]),
                    ("dewPoint"     ,struct.unpack('<f',bytes.fromhex(dataIn[24:32]))[0]),
                    ("altitude"     ,struct.unpack('<f',bytes.fromhex(dataIn[32:40]))[0]),        
            ])
            return sensorDictionary;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for SCD30")
        time.sleep(.5)
        return None


def sensingRG15(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("RG15 Read")	
            if (len(dataIn)==4):
                strOut  = \
                    np.float32(dataIn[0].replace(' ', "").replace('mm', "").replace('Acc', "")).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[1].replace(' ', "").replace('mm', "").replace('EventAcc', "")).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[2].replace(' ', "").replace('mm', "").replace('TotalAcc', "")).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[3].replace(' ', "").replace('mmph', "").replace('RInt', "")).tobytes().hex().zfill(8)      
                return strOut;  
            else:
                print("Invalid data string read from the RG15")
                return None;
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"           ,str(dateTime)),
                    ("accumulation"       ,struct.unpack('<f',bytes.fromhex(dataIn[0:8]))[0]),
                    ("eventAccumulation"  ,struct.unpack('<f',bytes.fromhex(dataIn[8:16]))[0]),
                    ("totalAccumulation"  ,struct.unpack('<f',bytes.fromhex(dataIn[16:24]))[0]),
                    ("rainPerInterval"    ,struct.unpack('<f',bytes.fromhex(dataIn[24:32]))[0]),
            ])
            return sensorDictionary;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for RG15")
        time.sleep(.5)
        return None
        
    # For transmitting data, transmitRecieve is True
def sensingMBCLR002(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("MBCLR002 Read")	
            strOut  = \
                np.ubyte(dataIn[0]).tobytes().hex().zfill(2)+ \
                np.uint16(dataIn[1]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[2]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[3]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[4]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[5]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[6]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[7]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[8]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[9]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[10]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[11]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[12]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[13]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[14]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[15]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[16]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[17]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[18]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[19]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[20]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[21]).tobytes().hex().zfill(8)+ \
                np.uint16(dataIn[22]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[23]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[24]).tobytes().hex().zfill(8)
            return strOut;
        else:
            dateTime = datetime.datetime.now() 
            lag0 = struct.unpack('<H',bytes.fromhex(dataIn[2:6]))[0]
            lag1 = struct.unpack('<H',bytes.fromhex(dataIn[18:22]))[0]
            lag2 = struct.unpack('<H',bytes.fromhex(dataIn[34:38]))[0]
            lag3 = struct.unpack('<H',bytes.fromhex(dataIn[50:54]))[0]    
            lag4 = struct.unpack('<H',bytes.fromhex(dataIn[66:70]))[0]    
            lag5 = struct.unpack('<H',bytes.fromhex(dataIn[82:86]))[0]    
            lag6 = struct.unpack('<H',bytes.fromhex(dataIn[98:102]))[0]    
            lag7 = struct.unpack('<H',bytes.fromhex(dataIn[114:118]))[0]            
            
            dateTime0 = dateTime - timedelta(seconds = lag0)
            dateTime1 = dateTime - timedelta(seconds = lag1)
            dateTime2 = dateTime - timedelta(seconds = lag2)
            dateTime3 = dateTime - timedelta(seconds = lag3)
            dateTime4 = dateTime - timedelta(seconds = lag4)
            dateTime5 = dateTime - timedelta(seconds = lag5)
            dateTime6 = dateTime - timedelta(seconds = lag6)
            dateTime7 = dateTime - timedelta(seconds = lag7)
            
            sensorDictionary =  OrderedDict([
                    ("dateTime"      ,str(dateTime)),
                    ("numOfCalls"    ,struct.unpack('<B',bytes.fromhex(dataIn[0:2]))[0]),
                    ("dateTime0"     ,str(dateTime0)),
                    ("lag0"          ,struct.unpack('<H',bytes.fromhex(dataIn[2:6]))[0]),
                    ("label0"        ,struct.unpack('<H',bytes.fromhex(dataIn[6:10]))[0]),
                    ("confidence0"   ,struct.unpack('<f',bytes.fromhex(dataIn[10:18]))[0]),
                    ("dateTime1"     ,str(dateTime1)),
                    ("lag1"          ,struct.unpack('<H',bytes.fromhex(dataIn[18:22]))[0]),
                    ("label1"        ,struct.unpack('<H',bytes.fromhex(dataIn[22:26]))[0]),
                    ("confidence1"   ,struct.unpack('<f',bytes.fromhex(dataIn[26:34]))[0]),
                    ("dateTime2"     ,str(dateTime2)),
                    ("lag2"          ,struct.unpack('<H',bytes.fromhex(dataIn[34:38]))[0]),
                    ("label2"        ,struct.unpack('<H',bytes.fromhex(dataIn[38:42]))[0]),
                    ("confidence2"   ,struct.unpack('<f',bytes.fromhex(dataIn[42:50]))[0]),
                    ("dateTime3"     ,str(dateTime3)),
                    ("lag3"          ,struct.unpack('<H',bytes.fromhex(dataIn[50:54]))[0]),
                    ("label3"        ,struct.unpack('<H',bytes.fromhex(dataIn[54:58]))[0]),
                    ("confidence3"   ,struct.unpack('<f',bytes.fromhex(dataIn[58:66]))[0]),
                    ("dateTime4"     ,str(dateTime4)),
                    ("lag4"          ,struct.unpack('<H',bytes.fromhex(dataIn[66:70]))[0]),
                    ("label4"        ,struct.unpack('<H',bytes.fromhex(dataIn[70:74]))[0]),
                    ("confidence4"   ,struct.unpack('<f',bytes.fromhex(dataIn[74:82]))[0]),
                    ("dateTime5"     ,str(dateTime5)),
                    ("lag5"          ,struct.unpack('<H',bytes.fromhex(dataIn[82:86]))[0]),
                    ("label5"        ,struct.unpack('<H',bytes.fromhex(dataIn[86:90]))[0]),
                    ("confidence5"   ,struct.unpack('<f',bytes.fromhex(dataIn[90:98]))[0]),
                    ("dateTime6"     ,str(dateTime6)),
                    ("lag6"          ,struct.unpack('<H',bytes.fromhex(dataIn[98:102]))[0]),
                    ("label6"        ,struct.unpack('<H',bytes.fromhex(dataIn[102:106]))[0]),
                    ("confidence6"   ,struct.unpack('<f',bytes.fromhex(dataIn[106:114]))[0]),
                    ("dateTime7"     ,str(dateTime7)),
                    ("lag7"          ,struct.unpack('<H',bytes.fromhex(dataIn[114:118]))[0]),
                    ("label7"        ,struct.unpack('<H',bytes.fromhex(dataIn[118:122]))[0]),
                    ("confidence7"   ,struct.unpack('<f',bytes.fromhex(dataIn[122:130]))[0]),
                ])
            return sensorDictionary;
    
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100")
        time.sleep(.5)
        return None     
    
def sensingMBCLR001(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("MBCLR001 Read")	
            strOut  = \
                np.ubyte(dataIn[0]).tobytes().hex().zfill(4)+ \
                np.uint16(dataIn[1]).tobytes().hex().zfill(4)+ \
                np.float32(dataIn[2]).tobytes().hex().zfill(8)
            return strOut;  
        else:
            dateTimePre = datetime.datetime.now() 
            lag = struct.unpack('<H',bytes.fromhex(dataIn[0:4]))[0]
            dateTime = dateTimePre - timedelta(seconds = lag)
            sensorDictionary =  OrderedDict([
                    ("dateTime"     ,str(dateTime)),
                    ("label"        ,struct.unpack('<H',bytes.fromhex(dataIn[4:8]))[0]),
                    ("confidence"   ,struct.unpack('<f',bytes.fromhex(dataIn[8:16]))[0]),
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for MBCLR001")
        time.sleep(.5)
        return None


def sensingPM(dataIn,transmitReceive):
    try:
        print("Reading Power Mode")	
        if (transmitReceive): 
            strOut  = \
                np.ubyte(dataIn[0]).tobytes().hex().zfill(2)
            return strOut;  
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"      ,str(dateTime)),
                    ("powerMode",struct.unpack('<B',bytes.fromhex(dataIn[0:8]))[0])
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for PM")
        time.sleep(.5)
        return None


def sensingPMPoLo(dataIn,transmitReceive):
    try:    
        print("Reading Power Mode")	
        if (transmitReceive): 
            strOut  = \
                np.ubyte(dataIn[0]).tobytes().hex().zfill(2)
            return strOut;  
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"      ,str(dateTime)),
                    ("powerMode",struct.unpack('<B',bytes.fromhex(dataIn[0:2]))[0])
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for PMPoLo")
        time.sleep(.5)
        return None
    
def sensingPMSalor(dataIn,transmitReceive):
    try:    
        print("Reading Power Mode")	
        if (transmitReceive): 
            strOut  = \
                np.ubyte(dataIn[0]).tobytes().hex().zfill(2)
            return strOut;  
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"      ,str(dateTime)),
                    ("powerMode",struct.unpack('<B',bytes.fromhex(dataIn[0:2]))[0])
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for PMPoLo")
        time.sleep(.5)
        return None


def sensingMacAD(dataIn,transmitReceive):
    try:
        print("Reading Mac Address")	
        if (transmitReceive): 
            strOut  = \
                dataIn[0].zfill(12)
            return strOut;  
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"      ,str(dateTime)),
                    ("macAddress" ,dataIn),
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for Mac Address")
        time.sleep(.5)
        return None
            
def getLatitudeCords(latitudeStr,latitudeDirection):
    latitude = float(latitudeStr)
    latitudeCord      =  math.floor(latitude/100) +(latitude - 100*(math.floor(latitude/100)))/60
    if(latitudeDirection=="S"):
        latitudeCord = -1*latitudeCord
    return latitudeCord

def getLongitudeCords(longitudeStr,longitudeDirection):
    longitude = float(longitudeStr)
    longitudeCord      =  math.floor(longitude/100) +(longitude - 100*(math.floor(longitude/100)))/60
    if(longitudeDirection=="W"):
        longitudeCord = -1*longitudeCord
    return longitudeCord      


def sensingGPGGAPL(dataIn,transmitReceive):
    
    try:
        if (transmitReceive): 
            dataIn = pynmea2.parse(dataIn)
            if (dataIn.gps_qual>0):
                timeStamp = str(dataIn.timestamp).split(":")
                print("GPGGAPL Read")	
                strOut  = \
                    np.ubyte(timeStamp[0]).tobytes().hex().zfill(2)+ \
                    np.ubyte(timeStamp[1]).tobytes().hex().zfill(2)+ \
                    np.ubyte(timeStamp[2]).tobytes().hex().zfill(2)+ \
                    np.double(getLatitudeCords(dataIn.lat,dataIn.lat_dir)).tobytes().hex().zfill(16)+ \
                    np.double(getLongitudeCords(dataIn.lon,dataIn.lon_dir)).tobytes().hex().zfill(16) + \
                    np.ubyte(dataIn.gps_qual).tobytes().hex().zfill(2)+ \
                    np.ubyte(dataIn.num_sats).tobytes().hex().zfill(2)+ \
                    np.float32(dataIn.horizontal_dil).tobytes().hex().zfill(8) +\
                    np.float32(dataIn.altitude).tobytes().hex().zfill(8) +\
                    np.float32(dataIn.geo_sep).tobytes().hex().zfill(8) ;
                return strOut;  
            else:
                print("GPGGAPL Data Not Read: No GPS Signal")	
                return None

        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"            ,str(dateTime)),
                    ("hour"                ,struct.unpack('<B',bytes.fromhex(dataIn[0:2]))[0]),
                    ("minute"              ,struct.unpack('<B',bytes.fromhex(dataIn[2:4]))[0]),
                    ("second"              ,struct.unpack('<B',bytes.fromhex(dataIn[4:6]))[0]),
                    ("latitudeCoordinate"  ,struct.unpack('<d',bytes.fromhex(dataIn[6:22]))[0]),
                    ("longitudeCoordinate" ,struct.unpack('<d',bytes.fromhex(dataIn[22:38]))[0]),
                    ("gpsQuality"          ,struct.unpack('<B',bytes.fromhex(dataIn[38:40]))[0]),
                    ("numberOfSatellites"  ,struct.unpack('<B',bytes.fromhex(dataIn[40:42]))[0]),
                    ("HorizontalDilution"  ,struct.unpack('<f',bytes.fromhex(dataIn[42:50]))[0]),
                    ("altitude"            ,struct.unpack('<f',bytes.fromhex(dataIn[50:58]))[0]),
                    ("undulation"          ,struct.unpack('<f',bytes.fromhex(dataIn[58:66]))[0]),
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for GPGGAPL")
        time.sleep(.5)
        return None

def sensingGPRMCPL(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            dataIn = pynmea2.parse(dataIn)
            if (dataIn.status=='A'):
                timeStamp = str(dataIn.timestamp).split(":")
                dateStamp = str(dataIn.datestamp).split("-")
                print("GPRMCPL Read")	
                strOut  = \
                    np.uint16(dateStamp[0]).tobytes().hex().zfill(4)+ \
                    np.ubyte(dateStamp[1]).tobytes().hex().zfill(2)+ \
                    np.ubyte(dateStamp[2]).tobytes().hex().zfill(2)+ \
                    np.ubyte(timeStamp[0]).tobytes().hex().zfill(2)+ \
                    np.ubyte(timeStamp[1]).tobytes().hex().zfill(2)+ \
                    np.ubyte(timeStamp[2]).tobytes().hex().zfill(2)+ \
                    np.double(getLatitudeCords(dataIn.lat,dataIn.lat_dir)).tobytes().hex().zfill(16)+ \
                    np.double(getLongitudeCords(dataIn.lon,dataIn.lon_dir)).tobytes().hex().zfill(16) + \
                    np.float32(dataIn.spd_over_grnd).tobytes().hex().zfill(8) ;
                return strOut;  
            else:
                print("GPRMCPL Data Not Read: No GPS Signal")	
                return None
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"            ,str(dateTime)),
                    ("year"                ,struct.unpack('<H',bytes.fromhex(dataIn[0:4]))[0]),
                    ("month"               ,struct.unpack('<B',bytes.fromhex(dataIn[4:6]))[0]),
                    ("day"                 ,struct.unpack('<B',bytes.fromhex(dataIn[6:8]))[0]),               
                    ("hour"                ,struct.unpack('<B',bytes.fromhex(dataIn[8:10]))[0]),
                    ("minute"              ,struct.unpack('<B',bytes.fromhex(dataIn[10:12]))[0]),
                    ("second"              ,struct.unpack('<B',bytes.fromhex(dataIn[12:14]))[0]),
                    ("latitudeCoordinate"  ,struct.unpack('<d',bytes.fromhex(dataIn[14:30]))[0]),
                    ("longitudeCoordinate" ,struct.unpack('<d',bytes.fromhex(dataIn[30:46]))[0]),
                    ("speedOverGround"     ,struct.unpack('<f',bytes.fromhex(dataIn[46:54]))[0]),
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for GPRMCPL")
        time.sleep(.5)
        return None

def sensingAS7265X(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("AS7265X Read")	
            if(len(dataIn)==18):
                strOut  = \
                    np.float32(dataIn[0]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[1]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[2]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[3]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[4]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[5]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[6]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[7]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[8]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[9]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[10]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[11]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[12]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[13]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[14]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[15]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[16]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[17]).tobytes().hex().zfill(8) ;
                return strOut;  
            else:
                print("Invalid data string read by the Canaaree")
                return None;
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"      ,str(dateTime)),
                    ("channelA410nm" ,struct.unpack('<f',bytes.fromhex(dataIn[0:8]))[0]),
                    ("channelB435nm" ,struct.unpack('<f',bytes.fromhex(dataIn[8:16]))[0]),
                    ("channelC460nm" ,struct.unpack('<f',bytes.fromhex(dataIn[16:24]))[0]),
                    ("channelD485nm" ,struct.unpack('<f',bytes.fromhex(dataIn[24:32]))[0]),                
                    ("channelE510nm" ,struct.unpack('<f',bytes.fromhex(dataIn[32:40]))[0]),
                    ("channelF535nm" ,struct.unpack('<f',bytes.fromhex(dataIn[40:48]))[0]),
                    ("channelG560nm" ,struct.unpack('<f',bytes.fromhex(dataIn[48:56]))[0]),
                    ("channelH585nm" ,struct.unpack('<f',bytes.fromhex(dataIn[56:64]))[0]),                
                    ("channelR610nm" ,struct.unpack('<f',bytes.fromhex(dataIn[64:72]))[0]),
                    ("channelI645nm" ,struct.unpack('<f',bytes.fromhex(dataIn[72:80]))[0]),
                    ("channelS680nm" ,struct.unpack('<f',bytes.fromhex(dataIn[80:88]))[0]),
                    ("channelJ705nm" ,struct.unpack('<f',bytes.fromhex(dataIn[88:96]))[0]),                
                    ("channelT730nm" ,struct.unpack('<f',bytes.fromhex(dataIn[96:104]))[0]),
                    ("channelU760nm" ,struct.unpack('<f',bytes.fromhex(dataIn[104:112]))[0]),
                    ("channelV810nm" ,struct.unpack('<f',bytes.fromhex(dataIn[112:120]))[0]),
                    ("channelW860nm" ,struct.unpack('<f',bytes.fromhex(dataIn[120:128]))[0]),                
                    ("channelK900nm" ,struct.unpack('<f',bytes.fromhex(dataIn[128:136]))[0]),
                    ("channelL940nm" ,struct.unpack('<f',bytes.fromhex(dataIn[136:144]))[0]),
            ])
            return sensorDictionary;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for AS7265X")
        time.sleep(.5)
        return None


def sensingSCD30(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("SCD30 Read")	
            if (len(dataIn)==3):
                strOut  = \
                    np.float32(dataIn[0]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[1]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[2]).tobytes().hex().zfill(8)
                return strOut;  
            else:
                print("Invalid data string read from the SCD30")
                return None;

        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"     ,str(dateTime)),
                    ("co2"          ,struct.unpack('<f',bytes.fromhex(dataIn[0:8]))[0]),
                    ("temperature"  ,struct.unpack('<f',bytes.fromhex(dataIn[8:16]))[0]),
                    ("humidity"     ,struct.unpack('<f',bytes.fromhex(dataIn[16:24]))[0]),
            ])
            return sensorDictionary;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for SCD30")
        time.sleep(.5)
        return None

def sensingBME688CNR(dataIn,transmitReceive):
    try:
        if (transmitReceive): 
            print("BME688CNR Read")	
            if (len(dataIn)== 44 or len(dataIn)== 43): 
                strOut  = \
                    np.float32(dataIn[29]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[31]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[33]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[35]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[37]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[39]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[41]).tobytes().hex().zfill(8)
                return strOut;  
            else:
                print("Invalid data string read from the Canaaree")
                return None;
        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime"    , str(dateTime)), 
                    ("temperature" ,struct.unpack('<f',bytes.fromhex(dataIn[0:8]))[0]),
                    ("humidity"    ,struct.unpack('<f',bytes.fromhex(dataIn[8:16]))[0]),
                    ("pressure"    ,struct.unpack('<f',bytes.fromhex(dataIn[16:24]))[0]),
                    ("vocAqi"      ,struct.unpack('<f',bytes.fromhex(dataIn[24:32]))[0]),
                    ("bvocEq"      ,struct.unpack('<f',bytes.fromhex(dataIn[32:40]))[0]),
                    ("gasEst"      ,struct.unpack('<f',bytes.fromhex(dataIn[40:48]))[0]), 
                    ("co2Eq"       ,struct.unpack('<f',bytes.fromhex(dataIn[48:56]))[0]),
            ])
            return sensorDictionary;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for BME688CNR")
        time.sleep(.5)
        return None
        
def sensingIPS7100(dataIn,transmitReceive):
    try:
        if (transmitReceive):  
            print("IPS7100 Read")	
            if (len(dataIn)== 44 or len(dataIn)== 43): 
                strOut  = \
                    np.uint32(dataIn[1]).tobytes().hex().zfill(8)+ \
                    np.uint32(dataIn[3]).tobytes().hex().zfill(8) + \
                    np.uint32(dataIn[5]).tobytes().hex().zfill(8)+ \
                    np.uint32(dataIn[7]).tobytes().hex().zfill(8) + \
                    np.uint32(dataIn[9]).tobytes().hex().zfill(8)+ \
                    np.uint32(dataIn[11]).tobytes().hex().zfill(8) + \
                    np.uint32(dataIn[13]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[15]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[17]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[19]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[21]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[23]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[25]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[27]).tobytes().hex().zfill(8)
                return strOut;  
            else:
                print("Invalid data string read from the Canaaree")
                return None

        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime" , str(dateTime)), 
                    ("pc0_1"  ,struct.unpack('<L',bytes.fromhex(dataIn[0:8]))[0]),
                    ("pc0_3"  ,struct.unpack('<L',bytes.fromhex(dataIn[8:16]))[0]),
                    ("pc0_5"  ,struct.unpack('<L',bytes.fromhex(dataIn[16:24]))[0]),
                    ("pc1_0"  ,struct.unpack('<L',bytes.fromhex(dataIn[24:32]))[0]),
                    ("pc2_5"  ,struct.unpack('<L',bytes.fromhex(dataIn[32:40]))[0]),
                    ("pc5_0"  ,struct.unpack('<L',bytes.fromhex(dataIn[40:48]))[0]), 
                    ("pc10_0" ,struct.unpack('<L',bytes.fromhex(dataIn[48:56]))[0]),
                    ("pm0_1"  ,struct.unpack('<f',bytes.fromhex(dataIn[56:64]))[0]), 
                    ("pm0_3"  ,struct.unpack('<f',bytes.fromhex(dataIn[64:72]))[0]),
                    ("pm0_5"  ,struct.unpack('<f',bytes.fromhex(dataIn[72:80]))[0]),
                    ("pm1_0"  ,struct.unpack('<f',bytes.fromhex(dataIn[80:88]))[0]),
                    ("pm2_5"  ,struct.unpack('<f',bytes.fromhex(dataIn[88:96]))[0]),
                    ("pm5_0"  ,struct.unpack('<f',bytes.fromhex(dataIn[96:104]))[0]), 
                    ("pm10_0" ,struct.unpack('<f',bytes.fromhex(dataIn[104:112]))[0])
            ])
            return sensorDictionary;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100")
        time.sleep(.5)
        return None




def sensingIPS7100CNR(dataIn,transmitReceive):
    try:
        if (transmitReceive):  
            print("IPS7100CNR Read")	
            if (len(dataIn)== 44 or len(dataIn)== 43): 
                strOut  = \
                    np.uint32(dataIn[1]).tobytes().hex().zfill(8)+ \
                    np.uint32(dataIn[3]).tobytes().hex().zfill(8) + \
                    np.uint32(dataIn[5]).tobytes().hex().zfill(8)+ \
                    np.uint32(dataIn[7]).tobytes().hex().zfill(8) + \
                    np.uint32(dataIn[9]).tobytes().hex().zfill(8)+ \
                    np.uint32(dataIn[11]).tobytes().hex().zfill(8) + \
                    np.uint32(dataIn[13]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[15]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[17]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[19]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[21]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[23]).tobytes().hex().zfill(8)+ \
                    np.float32(dataIn[25]).tobytes().hex().zfill(8) + \
                    np.float32(dataIn[27]).tobytes().hex().zfill(8)
                return strOut;  
            else:
                print("Invalid data string read from the Canaaree")
                return None

        else:
            dateTime = datetime.datetime.now()
            sensorDictionary =  OrderedDict([
                    ("dateTime" , str(dateTime)), 
                    ("pc0_1"  ,struct.unpack('<L',bytes.fromhex(dataIn[0:8]))[0]),
                    ("pc0_3"  ,struct.unpack('<L',bytes.fromhex(dataIn[8:16]))[0]),
                    ("pc0_5"  ,struct.unpack('<L',bytes.fromhex(dataIn[16:24]))[0]),
                    ("pc1_0"  ,struct.unpack('<L',bytes.fromhex(dataIn[24:32]))[0]),
                    ("pc2_5"  ,struct.unpack('<L',bytes.fromhex(dataIn[32:40]))[0]),
                    ("pc5_0"  ,struct.unpack('<L',bytes.fromhex(dataIn[40:48]))[0]), 
                    ("pc10_0" ,struct.unpack('<L',bytes.fromhex(dataIn[48:56]))[0]),
                    ("pm0_1"  ,struct.unpack('<f',bytes.fromhex(dataIn[56:64]))[0]), 
                    ("pm0_3"  ,struct.unpack('<f',bytes.fromhex(dataIn[64:72]))[0]),
                    ("pm0_5"  ,struct.unpack('<f',bytes.fromhex(dataIn[72:80]))[0]),
                    ("pm1_0"  ,struct.unpack('<f',bytes.fromhex(dataIn[80:88]))[0]),
                    ("pm2_5"  ,struct.unpack('<f',bytes.fromhex(dataIn[88:96]))[0]),
                    ("pm5_0"  ,struct.unpack('<f',bytes.fromhex(dataIn[96:104]))[0]), 
                    ("pm10_0" ,struct.unpack('<f',bytes.fromhex(dataIn[104:112]))[0])
            ])
            return sensorDictionary;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100CNR")
        time.sleep(.5)
        return None
    
def directoryCheck(outputPath):
    exists = os.path.isfile(outputPath)
    directoryIn = os.path.dirname(outputPath)
    if not os.path.exists(directoryIn):
        print("Creating a Directory @: " +directoryIn)
        os.makedirs(directoryIn)
    return exists

