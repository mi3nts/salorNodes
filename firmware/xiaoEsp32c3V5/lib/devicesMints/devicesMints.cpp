// #include "devicesMints.h"

// // bool initializeBLEMints(){
// //   // Configure BLE
// //   // Setup the BLE LED to be enabled on CONNECT
// //   // Note: This is actually the default behaviour, but provided
// //   // here in case you want to control this LED manually via PIN 19
// //   Bluefruit.autoConnLed(true);

// //   // Config the peripheral connection with maximum bandwidth 
// //   Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

// //   Bluefruit.begin();
// //   Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
// //   Bluefruit.setName("MINTSWearable001");
// //   Bluefruit.Periph.setConnectCallback(connect_callback);
// //   Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

// //   // To be consistent OTA DFU should be added first if it exists
// //   bledfu.begin();

// //   // Configure and Start Device Information Service
// //   bledis.setManufacturer("MINTS");
// //   bledis.setModel("Feather52");
// //   bledis.begin();

// //   // Configure and Start BLE Uart Service
// //   bleuart.begin();

// //   // Start BLE Battery Service
// //   blebas.begin();
// //   blebas.write(100);

// //   // Set up and start advertising
// //   startAdv();

// // return true;
// // }



// void startAdv(void)
// {

// // Advertising Packet
//   // Advertising packet
//   Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
//   Bluefruit.Advertising.addTxPower();

//   // Include bleuart 128-bit uuid
//   Bluefruit.Advertising.addService(bleuart);

//   // Secondary Scan Response packet (optional)
//   // Since there is no room for 'Name' in Advertising packet
//   Bluefruit.ScanResponse.addName();
  
//   /* Start Advertising
//    * - Enable auto advertising if disconnected
//    * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
//    * - Timeout for fast mode is 30 seconds
//    * - Start(timeout) with timeout = 0 will advertise forever (until connected)
//    * 
//    * For recommended advertising interval
//    * https://developer.apple.com/library/content/qa/qa1931/_index.html   
//    */
//   Bluefruit.Advertising.restartOnDisconnect(true);
//   Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
//   Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
//   Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
// }
// // callback invoked when central connects
// void connect_callback(uint16_t conn_handle)
// {
//   // Get the reference to current connection
//   BLEConnection* connection = Bluefruit.Connection(conn_handle);

//   char central_name[32] = { 0 };
//   connection->getPeerName(central_name, sizeof(central_name));

//   Serial.print("Connected to ");
//   Serial.println(central_name);
// }

// void disconnect_callback(uint16_t conn_handle, uint8_t reason)
// {
//   (void) conn_handle;
//   (void) reason;

//   Serial.println();
//   Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
// }




