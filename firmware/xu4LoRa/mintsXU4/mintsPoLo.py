# MQTT Client demo
# Continuously monitor two different MQTT topics for data,
# check if the received data matches two predefined 'commands'
import base64
from cgitb import strong
from datetime import datetime
from this import d
import time
from xmlrpc.client import DateTime 
import serial.tools.list_ports

from mintsXU4 import mintsDefinitions as mD
from mintsXU4 import mintsLoRaSensing as mLS

from collections import OrderedDict
import struct
import numpy as np

loRaE5MiniPorts     = mD.loRaE5MiniPorts
canareePorts        = mD.canareePorts
gpsPorts            = mD.gpsPorts
appKey              = mD.appKey
macAddress          = mD.macAddress
fPortIDs            = mD.fPortIDs
receiveTransmit     = True

def deriveSensorStats(sensorID):
    for port in fPortIDs:
        if(port['sensor'] == sensorID):
            return port;
    return port;

def getPort(portsIn,indexIn,baudRateIn):
    availabilty  = len(portsIn)>0
    serPort = []
    if(availabilty):
        serPort = openSerial(portsIn[indexIn],baudRateIn)
    return availabilty,serPort;

def getRG15Port(portsIn,indexIn,baudRateIn):
    availabilty  = False
    try: 
        serPort = openSerial(portsIn[indexIn],baudRateIn)
        
        print("Requesting data from the rain sensor")
        sendCommand(serPort,'R',1)

        print("Resetting rain sensor")
        sendCommand(serPort,'O',1)

        print("Requesting data from the rain sensor")
        second  = sendCommand(serPort,'R',1)

        print("Requesting High Res data from the rain sensor")
        highRes = sendCommand(serPort,'H',1)

        print("Requesting Metric units from the rain sensor")
        metric  = sendCommand(serPort,'M',1)

        print("Requesting Polling mode")
        metric  = sendCommand(serPort,'P',1)

        checkStr  = second[0] + highRes[0] + metric[0]

        print("Checking availabilty of the rain sensor")
        availabilty = "Acc" in checkStr
        
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(1)
        print("RG15 not found")
        time.sleep(.5)

    return availabilty,serPort;


def readingDeviceProperties(macAddress,loRaE5MiniPorts,canareePorts,gpsPorts):
    
    print("Mac Address: {0}".format(macAddress))
    print()
    print("LoRa E5 Mini Ports:")
    for dev in loRaE5MiniPorts:
        print("\t{0}".format(dev))
    
    print("Canaree Ports:")
    for dev in canareePorts:
        print("\t{0}".format(dev))
    
    print("GPS Ports:")
    for dev in gpsPorts:
        print("\t{0}".format(dev))

    return;

def loRaE5ReJoin(availE5Mini,serE5Mini):
    print()
    if (not availE5Mini):
        print("E5 Mini Not Connected")
        quit()
           
    joined = joinNetwork(10,serE5Mini,10)  
    time.sleep(5)
    return joined;
  
  
def loRaE5MiniJoin(availE5Mini,serE5Mini):
    print()
    if (not availE5Mini):
        print("E5 Mini Not Connected")
        quit()
         
    joined = False 
    # Read E5 Mini Credentials
    sendCommand(serE5Mini,'AT+RESET',2)
    sendCommand(serE5Mini,'AT+FDEFAULT',1)
    sendCommand(serE5Mini,'AT+VER',1)
    sendCommand(serE5Mini,'AT+FDEFAULT',1)
    sendCommand(serE5Mini,'AT+ID',1)
    sendCommand(serE5Mini,'AT+KEY=APPKEY, "'+appKey+'"',1)
    sendCommand(serE5Mini,'AT+MODE=LWOTAA',1)
    sendCommand(serE5Mini,'AT+DR=US915',1)
    sendCommand(serE5Mini,'AT+DR=dr2',1)
    sendCommand(serE5Mini,'AT+CH=NUM, 56-63',1)
    sendCommand(serE5Mini,'AT+POWER=20',1)

    # Check Join
    joined = joinNetwork(10,serE5Mini,10)

    if not joined:
        print("No network found, trying again in 60 seconds") 
        time.sleep(60)
        joined  = joinNetwork(10,serE5Mini,10)

    if not joined:
        print("No network found") 
        time.sleep(60)
        return joined;
    else:
        print()
        print("Network found")
        print()
 
    sensorID = "PMPoLo"    
    sendCommandHex(serE5Mini,sensorID,[254],deriveSensorStats(sensorID))
    
    sensorID = "MacAD"    
    sendCommandHex(serE5Mini,sensorID,[macAddress],deriveSensorStats(sensorID))

    return joined ;


def openSerial(portIn,baudRate):
    ser = serial.Serial(
            port= portIn,\
            baudrate=baudRate,\
            parity  =serial.PARITY_NONE,\
            stopbits=serial.STOPBITS_ONE,\
            bytesize=serial.EIGHTBITS,\
            timeout=0)

    print(" ")
    print("Connected to: " + ser.portstr)
    # print(" ")
    return ser;

def sendCommand2(serIn,commandStrIn,timeOutIn):
    serIn.write(str.encode(commandStrIn+ '\n\r'))
    line = []
    lines = []
    startTime = time.time()
    while (time.time()-startTime)<timeOutIn:
        for c in serIn.read():
            line.append(chr(c))
            if chr(c) == '\n':
                dataString = (''.join(line)).replace("\n","").replace("\r","")
                lines.append(dataString)
                # print(dataString)
                line = []
                break
    return serIn,lines;


def sendCommand(serIn,commandStrIn,timeOutIn):
    time.sleep(.5)
    serIn.write(str.encode(commandStrIn+ '\n\r'))
    line = []
    lines = []
    startTime = time.time()
    while (time.time()-startTime)<timeOutIn:
        for c in serIn.read():
            line.append(chr(c))
            if chr(c) == '\n':
                dataString = (''.join(line)).replace("\n","").replace("\r","")
                lines.append(dataString)
                print(dataString)
                line = []
                break
    return lines;

def readSerialLineStrAsIs(serIn,timeOutSensor,strExpected):
    line = []
    startTime = time.time()
    startFound = False
    while (time.time()-startTime)<timeOutSensor:   
        # try:
            for c in serIn.read():
                line.append(chr(c))
                # print((''.join(line)))

                if chr(c) == '\n':
                    if startFound == True:
                        dataString     = (''.join(line))
                        dataStringPost = dataString.replace('\r\n', '')
                        
                        if dataStringPost.find(strExpected) >0:
                            return dataStringPost;
                        else:
                            line = []
                    else:    
                        startFound = True
                        line = []

def readSerialLineStr(serIn,timeOutSensor,strExpected):
    line = []
    startTime = time.time()
    startFound = False
    while (time.time()-startTime)<timeOutSensor:   
        # try:
            for c in serIn.read():
                line.append(chr(c))
                # print((''.join(line)))

                if chr(c) == '\n':
                    if startFound == True:
                        dataString     = (''.join(line))
                        dataStringPost = dataString.replace('\r\n', '')
                        dataStringData =  dataStringPost.split(',')
                        
                        if dataStringPost.find(strExpected) >0:
                            return dataStringData;
                        else:
                            line = []
                    else:    
                        startFound = True
                        line = []

        # except:
        #     print("Incomplete String Read")
        #     line = []

def swapBytes(inputIn):
    return bytes([c for t in zip(inputIn[1::2], inputIn[::2]) for c in t])

def joinNetwork(numberOfTries,ser,timeOutIn):
    for currentTry in range(numberOfTries):
        print("Joining Network Trial: " + str(currentTry))
        lines = sendCommand(ser,'AT+JOIN',timeOutIn)
        
        for line in lines:
            if line == '+JOIN: Network joined':
                return True;

    return False;

def readSerialLine(serIn,timeOutSensor,sizeExpected,sizeExpectedCheck):
    line = []
    startTime = time.time()
    startFound = False
    while (time.time()-startTime)<timeOutSensor:   
        # try:
            for c in serIn.read():
                line.append(chr(c))
                if chr(c) == '\n':
                    if startFound == True:
                        dataString     = (''.join(line))
                        dataStringPost = dataString.replace('\r\n', '')
                        dataStringData = dataStringPost.split(',')
                        if sizeExpected == len(dataStringData):
                            print("Returning Data")
                            return dataStringData;
                        if sizeExpectedCheck == len(dataStringData):
                            print("Returning Data")
                            return dataStringData;                    
                        else:
                            line = []
                    else:    
                        startFound = True
                        line = []
    return;




def sendCommandHex(serPortE5,sensorID,sensorData,port):
    try:
        hexString = mLS.encodeDecode(sensorID,sensorData,receiveTransmit)
        print("HEX STRING: ")
        print(hexString)
        if hexString is not None:
            sendCommand(serPortE5,'AT+PORT='+ str(port['portID']),2) 
            sendCommand(serPortE5,'AT+MSGHEX='+str(hexString ),5)    
        else: 
            print("No data received for sensor " + sensorID)
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100CNR")
        time.sleep(.5)
        return;


def readSensorDataBirdSong(sensorData,sensorID,serPortE5):
    try:
        print("====================================")
        print("-----------" +sensorID+ "-----------" ) 
        print("Current Time (UTC): " +str(datetime.now()))
        print(sensorID + " Online") 
        port = deriveSensorStats(sensorID)  
        sendCommandHex(serPortE5,sensorID,sensorData,port)
        return;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100CNR")
        time.sleep(.5)
        return;

def readSensorDataGPS(online,serPort,sensorID,serPortE5):
    try:
        print("====================================")  
        print("-----------" +sensorID+ "-----------" ) 
        print("Current Time (UTC): " +str(datetime.now()))
        if online:
            print(sensorID + " Online") 
            port = deriveSensorStats(sensorID)
            if port['portID']==106:
                sensorData = readSerialLineStrAsIs(serPort,2,"GGA")
                print(sensorData)
                sendCommandHex(serPortE5,sensorID,sensorData,port)  
                return;          
            if port['portID']==107:
                sensorData = readSerialLineStrAsIs(serPort,2,"RMC")
                print(sensorData)
                # Add an if statement 
                sendCommandHex(serPortE5,sensorID,sensorData,port)
                return;
        else:
            print(sensorID + " Offline")       
            return;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100CNR")
        time.sleep(.5)
        return;

def readSensorData(online,serPort,sensorID,serPortE5):
    try:
        print("====================================")  
        print("-----------" +sensorID+ "-----------" ) 
        print("Current Time (UTC): " +str(datetime.now()))
        if online:
            print(sensorID + " Online") 
            port = deriveSensorStats(sensorID)
            if port['portID']<255:
                sensorData = readSerialLine(serPort,2,port['numOfParametors'],port['numOfParametorsCheck'])
                # print(sensorData)
                if (sensorData is not None): 
                	sendCommandHex(serPortE5,sensorID,sensorData,port)
                	return;
                else:
                    print(sensorID + " not read correctly")
                    return;
                  
        else:
            print(sensorID + " Offline")       
            return;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent")
        time.sleep(.5)
        return;


def readSensorDataRG15(online,serPort,sensorID,serPortE5):
    preRainData = "Acc  0.00 mm, EventAcc  0.00 mm, TotalAcc  0.00 mm, RInt  0.00 mmph"

    try:
        print("====================================")  
        print("-----------" +sensorID+ "-----------" ) 
        print("Current Time (UTC): " +str(datetime.now()))
        if online:
            print(sensorID + " Online") 
            port = deriveSensorStats(sensorID)
            # print(port)
            if port['portID']<255:
                sensorDataWhole = sendCommand(serPort,'R',1)
                sensorData      = sensorDataWhole[0].split(',')
                if port['numOfParametors'] == len(sensorData):
                    if (sensorDataWhole is not None) and str(sensorDataWhole[0]) != preRainData:
                        sendCommandHex(serPortE5,sensorID,sensorData,port)
                        return;
                    else:
                        print(sensorID + " not sent")
                        return;
                  
        else:
            print(sensorID + " Offline")       
            return;
    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent")
        time.sleep(.5)
        return;


def readSensorDataI2c(online,i2cObject,sensorID,serPortE5):
    try:
        print("====================================")  
        print("-----------" +sensorID+ "-----------" ) 
        print("Current Time (UTC): " +str(datetime.now()))    
        if online:
            print(sensorID + " Online")  
            port = deriveSensorStats(sensorID)
            if port['portID']<255:
                print("Reading I2C Data")
                sensorData  =  i2cObject.read()
                print(sensorData)
                sendCommandHex(serPortE5,sensorID,sensorData,port)  
                return;
        else:
            print(sensorID + " Offline")       
            return;

    except Exception as e:
        time.sleep(.5)
        print ("Error and type: %s - %s." % (e,type(e)))
        time.sleep(.5)
        print("Data Packet Not Sent for IPS7100CNR")
        time.sleep(.5)
        return;