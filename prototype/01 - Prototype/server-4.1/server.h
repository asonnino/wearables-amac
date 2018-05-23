/********************************************************************************************
 *
 * server.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#ifndef server_h
#define server_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#include "header.h"


/********************************************************************************************
 * Class definition
 ********************************************************************************************/
class Server {

	public:
		/************************************************************************************
		 * init
		 *
		 * param: RX_addr - the RX address
		 ************************************************************************************/
		Server(
			byte RX_addr[6]  // input
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
		 * param:  load   - payload (a single byte)
		 * return: whether the transmission is successful
		 ************************************************************************************/
		bool send_byte(
			byte load // input
		);		

		/************************************************************************************
		 * test connection
		 *
		 * param:
		 * return: whether the target device is reachable
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
		 * send credentials
		 *
		 * param: 
		 * return: whether the client accepted the issued credentials
		 ************************************************************************************/
		bool send_credentials();

		/************************************************************************************
		 * check credentials
		 *
		 * param: 
		 * return: whether the shown credentials are valid
		 ************************************************************************************/
		bool check_credentials();


	private:
		/************************************************************************************
		 * MAC_GGM KeyGen
		 * generates sk and iparams - private variables
		 *
		 * param: 
		 * return:
		 ************************************************************************************/
		void MAC_GGM_keygen();

		/************************************************************************************
		 * MAC_GGM Sign
		 * generates U and U' - private variables
		 *
		 * param:  hx - MAC_GGM's Hx(m) function    
		 * return:
		 ************************************************************************************/
		void MAC_GGM(
			scalar* hx       // output
		);

		/************************************************************************************
		 * MAC_GGM Verify
		 *
		 * param:  P       - U value
		 *         P_prime - U' value
		 *         hx      - MAC_GGM's Hx(m) function
		 * return: whether the given P, P' and Hx(m) verify the MAC
		 ************************************************************************************/
		bool verify_GGM(
			Point  P,       // input
			Point  P_prime, // input 
			scalar hx       // input
		);

		/************************************************************************************
		 * issue credentials
		 *
		 * param:  Cx0 - commitement of x0
		 *		   c   - NIZK challenge   
		 *         r   - NIZK responses
		 * return: 
		 ************************************************************************************/
		void issue_credentials(
			Point** Cx0, // output
			scalar* c,   // output
			scalar  r[]  // output
		);

		/************************************************************************************
		 * verify credential's presentation
		 *
		 * param:  Ua        - re-randomazied U point
		 *		   CUa_prime - commitements of Ua'   
		 *         Cm        - commitements of messages 
		 *         c         - NIZK challenge   
		 *         r         - NIZK responses
		 * return: 
		 ************************************************************************************/
		bool verify_credential_presentation(
			Point  Ua,        // input
			Point  CUa_prime, // input 
			Point  Cm[],      // input
			scalar c,         // input
			scalar s[]        // input
		);



		/************************************************************************************
		 * variables 
		 ************************************************************************************/
		RF24_wrapper* rf24_wrapper; // RF wrapped object

		byte   messages[NUM_OF_MESSAGES]; // the credentials attributes
		scalar sk[NUM_OF_MESSAGES + 1];   // the scheme secret keys
		Point* U;                         // MAC_GGM U value
		Point* U_prime;                   // MAC_GGM U' value
		Point* iparams[NUM_OF_MESSAGES];  // the scheme iparams


};


#endif /* server_h */
