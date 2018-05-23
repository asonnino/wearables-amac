/********************************************************************************************
 *
 * beacon.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#ifndef beacon_h
#define beacon_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#include "header.h"


/********************************************************************************************
 * Defines
 ********************************************************************************************/
#define LOCATION_CODE  0x7F
#define PRIVATE_KEY    0x03   // prototyping only


/********************************************************************************************
 * Class definition
 ********************************************************************************************/
class Beacon {

	public:
		/************************************************************************************
		 * init
		 *
		 * param: RX_addr - the TX address
		 ************************************************************************************/
		Beacon(
			byte RX_addr[6] // input
		);

		/************************************************************************************
		 * set the TX address
		 *
		 * param:  TX_addr - the TX address
		 * return:
		 ************************************************************************************/
		void set_TX(
			byte TX_addr[6] // input
		);

		/************************************************************************************
		 * send data
		 *
		 * param:  load   - payload
		 *         length - the payload's length 
		 * return: whether the transmission is successful
		 ************************************************************************************/
		bool send(
			void*   load,  // input
			uint8_t length // input
		);

		/************************************************************************************
		 * receive data
		 *
		 * param:  load   - payload
		 *         length - the payload's length 
		 * return: whether the reception is successful
		 ************************************************************************************/
		bool receive(
			void*   load,  // input
			uint8_t length // input
		);

		/************************************************************************************
		 * receive sync packet
		 *
		 * param:  packet - the sync packet
		 * return: whether a sync packet is sent
		 ***********************************************************************************/
		bool send_sync(
			byte packet // input
		);

		/************************************************************************************
		 * send a byte
		 *
		 * param:  load - payload (a single byte)
		 * return: whether the transmission is successful
		 ************************************************************************************/
		bool send_byte(
			byte load // input
		);

		/************************************************************************************
		 * test connection
		 *
		 * param:
		 * return: whether the targeted device is reachable
		 ***********************************************************************************/
		bool ping();

		/************************************************************************************
		 * answer test connection
		 *
		 * param:
		 * return: 
		 ***********************************************************************************/
		void pong();

		/************************************************************************************
		 * broadcast (low range) the beacon's location code area
		 *
		 * param:
		 * return:
		 ************************************************************************************/
		void broadcast_location();


	private:		
		/************************************************************************************
		 * get the wearable's credentials and send them to the server
		 *
		 * param:
		 * return: whether the credentials have been succefully accepted by the server
		 ************************************************************************************/
		bool pass_credentials();

		/************************************************************************************
		 * verify the issued credentials
		 *
		 * param: 
		 * return: 
		 ************************************************************************************/
		bool receive_wearable_credentials();

		/************************************************************************************
		 * present credentials
		 *
		 * param: 
		 * return: whether the credentials have been succefully accepted by the server
		 ************************************************************************************/
		bool send_credentials_to_server();

		/************************************************************************************
		 * answer test connection
		 *
		 * param:  power_level - the power level to set (see rf24_wrapper.h for options)
		 * return: 
		 ***********************************************************************************/
		void change_power_level(
			byte change_power_level // input
		);



		/************************************************************************************
		 * variables 
		 ************************************************************************************/
		RF24_wrapper* rf24_wrapper; // RF wrapped object

		scalar challenge;         // wearable location challenge
	 	struct signed_location {  // location signature's struct
    		scalar r;
    		scalar s;
    		byte   c;
 		} signed_location;

 		// data structures for credentials
 		// 
 		// NOTE: Max 32 bytes per structure (see section 6 - "Implementation").
        // The Code need to be extended if additional attributes have beed added :
        //      - define as many additional data structures 'data_Oxij' as needed
        //      - embbed additional while loops to treat each data packet
 		//
		struct data_struct_0x01 { 
        	Point  Ua;
        	Point  CUa_prime;
        	Point  Cm[NUM_OF_MESSAGES];
    	}data_0x01;
    	struct data_struct_0x02 {
     	   scalar c;
     	   scalar s[2 * NUM_OF_MESSAGES + 1];
    	}data_0x02;
    	/* 
    	 *
    	 * <Add code here to define additional data structures> 
    	 *
    	 */
};


#endif /* beacon_h */
