# salorNodes
Contains firmware for mints Solar powered Arduino based LORa nodes

## Further Additions

- On January 4th, the Chirpstack network experienced a security breach, leading to the non-recovery of V1 deployment sensors after the Chirpstack reinstatement. Consequently, the Lorawan radio fails to power cycle with the microcontroller, necessitating a forced reboot for the radio.
 - Incorporate confirmed data pings; if not confirmed, initiate a reboot for the node.

## Sensing Period
``` #define SENSING_PERIOD 9000; // mili seconds ```


## Resources
- Generic Library Used: https://github.com/arduino-libraries/MKRWAN
- AT Commands:
  https://github.com/RAKWireless/LPWAN-Tracker-Solution/blob/main/AT-Commands.md#atmask
  
- Power is set through +RFPOWER AT command 
- Datasheet for the LoRaWAN chip : https://www.mouser.com/catalog/specsheets/Murata_CMWX1ZZABZ-078.pdf
- Some context on the available functions: https://github.com/hardwario/lora-modem/blob/main/python/lora.py#L685
- AT commands used on mkrwan : https://github.com/arduino/mkrwan1300-fw
- Power Details from https://www.thethingsnetwork.org/forum/t/sx1276-transmission-power-managment/29720

1- RFO: from -4 dBm to 15 dBm (Pout=Pmax-(15-OutputPower) = 10.8 + 0.6 * 7 - (15 - 15) = 15) 7 is the max value for the MaxPower and 15 is the max value for the outputPower so that’s way the RFO can reach 15 dBm, but i don’t know why they said it’s only +14dBm? and set the PaSelect to 0 to use this mode and make sure the PA_Dac set to 0x04 (default value)

2- PA_Boost: from 2 dBm to 17 dBm (Pout=17-(15-OutputPower)) this is clear. set PaSelect bit to 1 to activate this mode. and make sure to set the PA_Dac to 0x04 (default value) (PA_DAC register = 0x84)

3- +20dBm: 20 dBm, to use this option we need to be in the PA_Boost mode (set PA_Boost bit to 1) and set the PA_DAC to 0x07 (PA_DAC register = 0x87).

About the RFO where i need to set 2 variable “MaxPower” and “Output Power”, i don’t know what is the right way to do that, i mean which one should i adjust to get the Tx power the user want to use. or should i set the “MaxPower” to 7 so the Pmax = 15 and from there i set the “Output Power” to get the Pout (which is the Tx power) the user wants, because it’s gonna be easy to go from 15 to 0 by step of 1 dBm. hope one of you have a better solution?!

Now the last thing, the OCP, which is require for the PA_Boost and +20 dBm option because both works in high power (PA_HP) and i don’t know how to set that register, i mean how to calculate Imax. i need help about this, and do i need to set this register for the RFO mode too?? if yes then how? (by the way, i know about the table of OCP in the datasheet and how to calculate the OCP from Imax, the problem that i don’t know how to get or figure about the Imax.

One last question, is the order to configure the PaSelect Variable and MaxPower and Output Power and PA_DAC register and OCP register matters, my real confusion is about PaSelect variable and Output Power variable and PA_DAC register configuration order.


- ST LoRaWAN AT Command set: https://www.st.com/resource/en/application_note/an4967-examples-of-at-commands-on-icubelrwan-stmicroelectronics.pdf

# Power Saving Stratergies 
- To measure the current get the voltage drop accross the shunt resistor of the INA219 using an osciloscope during a data send operation ( "R100±1%" to indicate that the resistor's actual resistance can deviate by no more than 1% from the nominal value of 100 ohms.)


## Things to do 

- Set all ADC into inputs
 ```
// Set digital pins to input  to save on current drain
 for (int i=0; i < 15; i++) 
 {
   pinMode(i, INPUT_PULLUP);
 }
```
- Usefule Examples
  - https://forum.arduino.cc/t/how-can-i-achieve-the-104-a-power-consumption/619331   

```
#include "ArduinoLowPower.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  LowPower.deepSleep(2000);
}
```
 - https://docs.arduino.cc/learn/electronics/low-power

```
// Low Power Library
#include "ArduinoLowPower.h"

// LoRa Library
#include <SPI.h>
#include <LoRa.h>

// LoRa Packet Content 
char* message = "Hello LoRa!";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // LoRa Setup
  Serial.println(F("LoRa Sender"));
  if (!LoRa.begin(868E6)) {
    Serial.println(F("Starting LoRa failed!"));
    while (1);
  } else {
    Serial.println(F("Starting LoRa Successful!"));
  }
}

void loop() {
  LoRa_Packet_Sender();
  GoToSleep();
}

// LoRa Task
void LoRa_Packet_Sender() {
  Serial.print(F("Sending packet: "));
  Serial.println(message);

  // send packet
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  // Putting LoRa Module to Sleep 
  Serial.println(F("LoRa Going in Sleep"));
  LoRa.sleep();
}

// Sleep Task 
void GoToSleep(){
  Serial.println(F("MKR WAN 1310 - Going in Sleep"));
  LowPower.deepSleep(20000);
}
```

- Almost exactly what we need
```
  /*
Low Power - Low Voltage Detection - SAMD21 Specific Configuration Example
*/

// Manual Power Management 
#include "ArduinoLowPower.h"

float voltValue, battery_volt, battery_percentage;
float minimal_voltage = 1800;
float battery_voltage_ref = 3.3;

void setup() {
  Serial.begin(57600);
  delay(100);

  // Low Power Indicator Set 
  pinMode(LED_BUILTIN, OUTPUT);

  // Default Analog Reference of 3.3V
  analogReference(AR_DEFAULT);

  // Setting up for resolution of 12-Bits
  analogReadResolution(12);
}

void loop() { 
  // Reading from the Battery Pin
  voltValue = analogRead(A0);

  // Calculate current voltage level
  battery_volt = ((voltValue*battery_voltage_ref)/4095)*1000;
  
  // Battery level expressed in percentage
  battery_percentage = 100*abs((battery_volt - minimal_voltage)/((battery_voltage_ref*1000) - minimal_voltage));

  Serial.print(F("Battery: "));
  Serial.print(battery_percentage);
  Serial.println(F(" % "));
  
  if (battery_volt <= minimal_voltage){
    //LED Notification for low voltage detection
    lowBatteryWarning();
  }

  delay(2000);
  
  // Going into Low Power for 20 seconds 
  LowPower.deepSleep(20000);
}

// Low battery indicator
void lowBatteryWarning(){
  digitalWrite(LED_BUILTIN, HIGH);  
  delay (1);             
  digitalWrite(LED_BUILTIN, LOW);    
  delay (999);             
}
  ```
 
Not sure if this appracj will allow watchdog timers
