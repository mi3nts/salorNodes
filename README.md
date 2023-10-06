# salorNodes
Contains firmware for mints Solar powered Arduino based LORa nodes

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
