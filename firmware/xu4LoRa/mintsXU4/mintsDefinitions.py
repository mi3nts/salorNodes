
# from turtle import st
from getmac import get_mac_address
import serial.tools.list_ports
import yaml

def findPorts(strIn1,strIn2):
    ports = list(serial.tools.list_ports.comports())
    allPorts = []
    for p in ports:
        currentPortStr1 = str(p[1])
        currentPortStr2 = str(p[2])
        if(currentPortStr1.find(strIn1)>=0 and currentPortStr2.find(strIn2)>=0 ):
            allPorts.append(str(p[0]).split(" ")[0])
    return allPorts

def findPortsGPS(strIn1):
    ports = list(serial.tools.list_ports.comports())
    allPorts = []
    for p in ports:
        currentPortStr1 = str(p[1])
        if(currentPortStr1.find(strIn1)>=0):
            allPorts.append(str(p[0]).split(" ")[0])
    return allPorts
  
  
  
def findMacAddress():
    macAddress= get_mac_address(interface="eth0")
    if (macAddress!= None):
        return macAddress.replace(":","")

    macAddress= get_mac_address(interface="docker0")
    if (macAddress!= None):
        return macAddress.replace(":","")

    macAddress= get_mac_address(interface="enp1s0")
    if (macAddress!= None):
        return macAddress.replace(":","")

    macAddress= get_mac_address(interface="enp31s0")
    if (macAddress!= None):
        return macAddress.replace(":","")

    macAddress= get_mac_address(interface="wlan0")
    if (macAddress!= None):
        return macAddress.replace(":","")

    return "xxxxxxxx"



macAddress               = findMacAddress()

mintsDefinitions         = yaml.load(open('mintsXU4/credentials/mintsDefinitions.yaml'),Loader=yaml.FullLoader)
credentials              = yaml.load(open('mintsXU4/credentials/credentials.yaml'),Loader=yaml.FullLoader)
loRaCredentials          = yaml.load(open('mintsXU4/credentials/loRacredentials.yaml'),Loader=yaml.FullLoader)
fPortIDs                 = yaml.load(open('mintsXU4/credentials/portIDs.yml'),Loader=yaml.FullLoader)['portIDs']
nodeIDs                  = yaml.load(open('mintsXU4/credentials/nodeIDs.yaml'),Loader=yaml.FullLoader)

keys                     = yaml.load(open('mintsXU4/credentials/keys.yaml'),Loader=yaml.FullLoader)

dataFolder                = mintsDefinitions['dataFolder']
dataFolderTmp             = mintsDefinitions['dataFolderTmp']
dataFolderJson            = mintsDefinitions['dataFolderJson']

mqttPortLoRa              = loRaCredentials['port']
mqttBrokerLoRa            = loRaCredentials['broker']

mqttOn                    = True

nodeIDs                  = nodeIDs['nodeIDs']

keys                     = yaml.load(open('mintsXU4/credentials/keys.yaml'),Loader=yaml.FullLoader)

appKey                   = keys['appKey']


# mqttBroker                = mintsDefinitions['broker']
tlsCert                   = mintsDefinitions['tlsCert']

loRaE5MiniPorts          = findPorts("CP2102N USB to UART Bridge Controller","PID=10C4:EA60")
canareePorts             = findPorts("Canaree PM","PID=10C4:EA60")
gpsPorts                 = findPortsGPS("u-blox")

    
rainPorts                = ['/dev/ttyS1'] # Direct connected to the gpio port - May not be available on all polo nodes


 


     