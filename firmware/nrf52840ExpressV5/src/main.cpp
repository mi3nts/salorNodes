

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial

void setup() 
{
  Serial.begin(9600);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 Firmware Info Example");
  Serial.println("---------------------------------\n");
}


void loop() 
{
  // dbgPrintVersion();
  Serial.println("Toggling Light");
  Serial.println("---------------------------------\n");
  delay(1000);                       // wait for a second
}