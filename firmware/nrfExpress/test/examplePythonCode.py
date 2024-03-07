# from network import Bluetooth

# bluetooth = Bluetooth()
# bluetooth.start_scan(-1)    # start scanning with no timeout

# while True:
#     print(bluetooth.get_adv())


import time

# Protocol defined here:
#     https://github.com/zh2x/BCI_Protocol
# Thanks as well to:
#     https://github.com/ehborisov/BerryMed-Pulse-Oximeter-tool
#     https://github.com/ScheindorfHyenetics/berrymedBluetoothOxymeter
#
# The sensor updates the readings at 100Hz.
import datetime
import _bleio
import adafruit_ble
from adafruit_ble.advertising.standard import Advertisement
from adafruit_ble.services.standard.device_info import DeviceInfoService
from adafruit_ble.services.nordic import UARTService 
uart = UARTService()
uart_connection = None


# from adafruit_ble_berrymed_pulse_oximeter import BerryMedPulseOximeterService

# CircuitPython <6 uses its own ConnectionError type. So, is it if available. Otherwise,
# the built in ConnectionError is used.
connection_error = ConnectionError
if hasattr(_bleio, "ConnectionError"):
    connection_error = _bleio.ConnectionError

# PyLint can't find BLERadio for some reason so special case it here.
ble = adafruit_ble.BLERadio()  # pylint: disable=no-member

wrist_band_connectio = None

while True:
    print("Scanning...")
    for adv in ble.start_scan(Advertisement, timeout=5):
        name = adv.complete_name
        print(adv)
        print(name)
        if not name:
            continue
        if name.strip("\x00") == "MINTS":
            wrist_band_connection = ble.connect(adv)
            print(wrist_band_connection)
            print("Connected")
            break
    # Stop scanning whether or not we are connected.
    ble.stop_scan()
    print("Stopped scan")
    # time.sleep(5)
    # print(wrist_band_connection)
    # print(wrist_band_connection.connected)
    # time.sleep(5)
    try:
        if wrist_band_connection and wrist_band_connection.connected:
            print("Fetch connection")
            print(wrist_band_connection)
            uart_service = wrist_band_connection[UARTService]
            while wrist_band_connection.connected:    
                one_byte = uart_service.read(5)
                if one_byte:
                    print(datetime.datetime.now())
                    print(one_byte)
                    print()
                # print(uart_service.readline().decode("utf-8"))
        #             print(dis)
    #             print("Device Information Services")
    #             time.sleep(10)
    #             try:
    #                 manufacturer = dis.manufacturer
    #                 print(manufacturer)
    #             except AttributeError:
    #                 manufacturer = "(Manufacturer Not specified)"
    #             try:
    #                 model_number = dis.model_number
    #                 print(model_number)
    #             except AttributeError:
    #                 model_number = "(Model number not specified)"
    #             print("Device:", manufacturer, model_number)
    #         else:
    #             print("No device information")
    #         # pulse_ox_service = pulse_ox_connection[BerryMedPulseOximeterService]

    #         # while pulse_ox_connection.connected:
    #         #     print(pulse_ox_service.values)
    except connection_error:
        try:
            pulse_ox_connection.disconnect()
        except connection_error:
            pass
        pulse_ox_connection = None