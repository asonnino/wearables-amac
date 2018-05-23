/********************************************************************************************
 *
 * wearable.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 3.1
 *
 ********************************************************************************************/
#ifndef wearable_h
#define wearable_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#include "header.h"


/********************************************************************************************
 * Class definition
 ********************************************************************************************/
class Wearable {

	public:
		/************************************************************************************
		 * init
		 *
		 * param: RX_addr - the TX address
		 ************************************************************************************/
		Wearable(
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
		 * return: whether a sync packet is receved
		 ***********************************************************************************/
		bool syncronize(
			byte packet[SYNC_LEN] // input
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
		 * get location
		 *
		 * param:  pos - the location code
		 * return: whether the location is valid
		 ***********************************************************************************/
		bool get_location(
			byte*  location // output
		);

		/************************************************************************************
		 * get the issued credentials
		 *
		 * param:
		 * return: whether the credentials have been accepted
		 ************************************************************************************/
		bool get_credentials();

		/************************************************************************************
		 * show credentials
		 *
		 * param: 
		 * return: whether the credentials have been succfully shown
		 ************************************************************************************/
		bool show_credentials();


	private:		
		/************************************************************************************
		 * verify the issued credentials
		 *
		 * param:  Cx0 - commitement of x0
		 *		   c   - NIZK challenge   
		 *         r   - NIZK responses
		 * return: whether the verification succeded
		 ************************************************************************************/
		bool verify_credentials(
			Point* Cx0, // input
			scalar c,   // input
			scalar r[]  // input
		);

		/************************************************************************************
		 * present credentials
		 *
		 * param:  Ua        - re-randomazied U point
		 *		   Cm        - commitements of messages  
		 *         CUa_prime - commitements of Ua'
		 *		   c         - NIZK challenge
		 *         s         - NIZK resposnes  
		 * return:
		 ************************************************************************************/
		void credential_presentation(
			Point**  Ua,        // output
			Point*   Cm[],      // output
			Point**  CUa_prime, // output
			scalar*  c,         // output
			scalar   s[]        // output
		);



		/************************************************************************************
		 * variables 
		 ************************************************************************************/
		RF24_wrapper* rf24_wrapper; // RF wrapped object

		byte   messages[NUM_OF_MESSAGES]; // the credentials attributes
		Point* U;                         // MAC_GGM U value
		Point* U_prime;                   // MAC_GGM U' value
		Point* iparams[NUM_OF_MESSAGES];  // the scheme iparams

};


#endif /* wearable_h */
