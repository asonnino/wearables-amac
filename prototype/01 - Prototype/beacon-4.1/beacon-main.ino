 /********************************************************************************************
 *
 * RF Beacon
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *    
 ********************************************************************************************/
 #include "beacon.h"



/********************************************************************************************
 * Defines
 ********************************************************************************************/
 #define LOOP_DELAY   500


/********************************************************************************************
 * Variables
 ********************************************************************************************/
 Beacon* beacon;


/********************************************************************************************
 * 
 * Setup
 * 
 ********************************************************************************************/
 void setup() {

   // init console serial communication
   Serial.begin(9600); Serial.println(F(
     " \n"
     " +----------------------------------------------------------------------------+ \n" 
     " |                                RF BEACON                                   | \n"
     " |                                                                            | \n"
     " | @author  Sonnino Alberto                                                   | \n"
     " | @version 4.1                                                               | \n"
     " +----------------------------------------------------------------------------+ \n"
    ));
    Serial.println(F(" >> initializing device"));
        
    // set RX addr
    Serial.print(F("\t setting device address..............0x"));
    byte RX_addr[6] = BEACON_ADDR; // RX address
    for (int i=0; i<6; i++) {Serial.print(RX_addr[i],HEX);} Serial.println();

    // instantiate device
    Serial.print(F("\t init beacon object............................"));
    beacon = new Beacon(RX_addr);
    Serial.println(F("DONE"));

    // set wearable addr
    Serial.print(F("\t setting server address..............0x"));
    byte TX_addr[6] = WEARABLE_ADDR; // TX address
    beacon->set_TX(TX_addr);
    for (int i=0; i<6; i++) {Serial.print(TX_addr[i],HEX);} Serial.println();

} // setup



/********************************************************************************************
 * 
 * Loop
 * 
 ********************************************************************************************/
 void loop() {

   // broadcast location
   Serial.print(F(" >> Broading for location code 0x")); Serial.println(LOCATION_CODE, HEX);
   beacon->broadcast_location();

   // delay
   delay(LOOP_DELAY);
   
 } // end of main loop


 
 


