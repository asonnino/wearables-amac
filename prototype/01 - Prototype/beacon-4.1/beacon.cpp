/********************************************************************************************
 *
 * beacon.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 3.1
 *
 ********************************************************************************************/
#include "beacon.h"
#ifdef NRF24_DEBUG
	#include <printf.h> // debug only
#endif

#ifdef MEM_CHECK
/********************************************************************************************
 *
 * Get free dynamic memory - debug only
 *
 ********************************************************************************************/
int freeRam() 
{
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif


/********************************************************************************************
 *
 * Init
 *
 ********************************************************************************************/
Beacon::Beacon(byte RX_addr[6])
{
    // initialise rf24 transceiver
    this->rf24_wrapper = new RF24_wrapper(RX_addr);

    // init location code
    this->signed_location.c = LOCATION_CODE;

    // init random generator
    init_random();

    // debug
    #ifdef NRF24_DEBUG
    	printf_begin();
    	this->rf24_wrapper->print_details();
    #endif
}

/********************************************************************************************
 *
 * Set TX addr
 *
 ********************************************************************************************/
void Beacon::set_TX(byte TX_addr[6])
{
    // wrapper
    this->rf24_wrapper->set_TX(TX_addr);
} 


/********************************************************************************************
 *
 * Send
 *
 ********************************************************************************************/
bool Beacon::send(void* load, uint8_t length)
{
    // wrapper
    return this->rf24_wrapper->send(load, length);
}


/********************************************************************************************
 *
 * Receive
 *
 ********************************************************************************************/
bool Beacon::receive(void* load, uint8_t length)
{
    // wrapper
    return this->rf24_wrapper->receive(load, length); 
}


/********************************************************************************************
 *
 * Send sync packet
 *
 ********************************************************************************************/
bool Beacon::send_sync(byte packet)
{
    // wrapper
    return this->rf24_wrapper->send_sync(packet);
}


/********************************************************************************************
 *
 * Send byte
 *
 ********************************************************************************************/
bool Beacon::send_byte(byte load)
{
    // wrapper
    return this->rf24_wrapper->send_byte(load);
}


/********************************************************************************************
 *
 * Ping 
 *
 ********************************************************************************************/
bool Beacon::ping()
{
    // wrapper
    return this->rf24_wrapper->ping();
} 

/********************************************************************************************
 *
 * Pong 
 *
 ********************************************************************************************/
void Beacon::pong()
{
    // wrapper
    this->rf24_wrapper->pong();
} 


/************************************************************************************
 *
 * Broadcast Location
 * 
 ************************************************************************************/
void Beacon::broadcast_location(){

	// send sync packet
	this->send_sync(SYNC_LOCATION);

   	// debug
   	#ifdef BEACON_DEBUG
		Serial.print(F(" >> sent sync packet 0x")); Serial.println(SYNC_LOCATION,HEX);
	#endif

	// wait for challenge
    unsigned long started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
		if( this->receive(&(this->challenge), sizeof(scalar)) ) {

			// debug
            #ifdef BEACON_DEBUG
            	Serial.print(F(" >> received challenge "));
                Serial.print(F("0x")); Serial.println(this->challenge, HEX);
            #endif

            // create hash
    		SHA256* hasher = new SHA256();
            scalar z;
    		z = LOCATION_CODE;   hasher->update(&z,SCALAR_BIT_LEN/8);
    		z = this->challenge; hasher->update(&z,SCALAR_BIT_LEN/8);
    		hasher->finalize(&z, SCALAR_BIT_LEN/8);
    		z = z % GROUP_ORDER;

    		// clean up
    		hasher->clear(); delete hasher;
    
    		// create sigature    
    		ecdsa_sign(PRIVATE_KEY, z, &(this->signed_location.r), &(this->signed_location.s));

    		// debug
    		#ifdef BEACON_DEBUG
   				Serial.print(F("\t sending location.........................CODE 0x")); 
   				Serial.println(this->signed_location.c, HEX);
   				Serial.print(F("\t sending signature........................(0x")); 
   				Serial.print(this->signed_location.r, HEX);
       			Serial.print(F(",0x")); Serial.print(this->signed_location.s, HEX); Serial.println(F(")"));
    		#endif

    		// send signature
    		this->send(&(this->signed_location), sizeof(this->signed_location)); 

    		// pass wearable's credentials to server
            bool result = this->pass_credentials();

            // reset beacon's power level to the default value
            this->change_power_level(POWER_LV);

            // set wearable address
            byte TX_addr[6] = WEARABLE_ADDR; this->set_TX(TX_addr);

    		if (result) {this->send_byte(CREDENTIALS_VALID);}
    		else {this->send_byte(CREDENTIALS_INVALID);}

    		// debug
    		#ifdef BEACON_DEBUG
        		Serial.print(F(" >> new TX addr set to: 0x"));
        	for (int i=0; i<6; i++) {Serial.print(TX_addr[i],HEX);} Serial.println();
    		#endif
		}
	}
}


/************************************************************************************
 *
 * Get wearable credentials to pass them to the servers
 * 
 * NOTE: Max 32 bytes per structure (see section 6 - "Implementation").
 *       The Code need to be extended if additional attributes have beed added :
 *              - define as many additional data structures 'data_Oxij' as needed
 *              - embbed additional while loops to treat each data packet
 *
 ************************************************************************************/
bool Beacon::pass_credentials() {

    if (this->receive_wearable_credentials()) {

        // increase beacon's power
        this->change_power_level(RF24_PA_MAX);

    	if (this->send_credentials_to_server()) {

    		// get ACK
            byte ack;
    	    unsigned long started_waiting_at = GET_MICROS();
            while (GET_MICROS() - started_waiting_at < (RX_TIMEOUT)) {
                if (this->receive(&ack, sizeof(byte))) {

                    // return	
                    return (ack == CREDENTIALS_VALID);

                }
            }
    	}
    }

    // on error
    return false;
}


/************************************************************************************
 *
 * Get wearable credentials
 * 
 * NOTE: Max 32 bytes per structure (see section 6 - "Implementation").
 *       The Code need to be extended if additional attributes have beed added :
 *              - define as many additional data structures 'data_Oxij' as needed
 *              - embbed additional while loops to treat each data packet
 *
 ************************************************************************************/
bool Beacon::receive_wearable_credentials()
{
	// time lapsed for timeouts
    unsigned long started_waiting_at;

    // treat data packet data_0x01
    started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        if (this->receive(&(this->data_0x01), sizeof(this->data_0x01))) {

            // treat data packet data_0x02
            started_waiting_at = GET_MICROS();
            while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
                if (this->receive(&(this->data_0x02), sizeof(this->data_0x02))) {  


                    /*
                     *
                     * <Add code here to treat additional data structures>
                     *
                     */



                    // return result
                    return true;
                }
            }
        }
    }
    
    // on timeout
    return false;
}


/************************************************************************************
 *
 * Send credentials to server
 * 
 * NOTE: Max 32 bytes per structure (see section 6 - "Implementation").
 *       The Code need to be extended if additional attributes have beed added :
 *              - define as many additional data structures 'data_Oxij' as needed
 *              - embbed additional while loops to treat each data packet
 *
 ************************************************************************************/
bool Beacon::send_credentials_to_server(){

    // set server address
    byte TX_addr[6] = SERVER_ADDR; this->set_TX(TX_addr);

    // debug
    #ifdef BEACON_DEBUG
        Serial.print(F(" >> new TX addr set to: 0x"));
        for (int i=0; i<6; i++) {Serial.print(TX_addr[i],HEX);} Serial.println();
    #endif

	// send credentials
    if( this->send_sync(SYNC_SHOWING) ){


        // send data packet data_0x01
        if (this->send(&(this->data_0x01), sizeof(this->data_0x01))) {

            // delay
            unsigned long started_waiting_at = GET_MICROS();
            while (GET_MICROS() - started_waiting_at < TX_TIMEOUT);

            // send data packet data_0x02
            if (this->send(&(this->data_0x02), sizeof(this->data_0x02))) {

            	/*
			 	 *
				 * <Add code here to send additional data structures>
				 *
				 */

				return true;

            }
        }
    }
    // on error
    return false;
}


/********************************************************************************************
 *
 * Change power level
 *
 ********************************************************************************************/
void Beacon::change_power_level(byte power_level)
{
    // wrapper
    this->rf24_wrapper->change_power_level(power_level);
}
