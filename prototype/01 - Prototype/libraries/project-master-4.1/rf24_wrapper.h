/********************************************************************************************
 *
 * rf24_wrapper.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#ifndef rf24_wrapper_h
#define rf24_wrapper_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
 	#include <SPI.h>
 	#include "RF24.h"
#elif defined(__arm__) // Raspberry Pi
 	#include <stdint.h>
 	#include <RF24/RF24.h>
#endif /* includes */


/********************************************************************************************
 * System entities redef.
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
 	#define GET_MICROS() micros()
#elif defined(__arm__) // Raspberry Pi
 	#define GET_MICROS() (millis() * 1000) // ms approximate value
 	typedef uint8_t byte;
#else
 	#define GET_MICROS() 0
#endif /* system entities redefinition */


/********************************************************************************************
 * Radio config.
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
	#define CE_PIN  7
	#define CSN_PIN 8
#elif defined(__arm__) // Raspberry Pi
 	#define CE_PIN  22
	#define CSN_PIN 0
#endif /* PINs */
#define POWER_LV    RF24_PA_MIN  // options: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define ACK         0x1          // ACK value
#define ACK_TIMEOUT 200000       // in [us] - keep at least 5x smaller than RX_TIMEOUT
#define RX_TIMEOUT  1000000      // in [us] 
#define TX_TIMEOUT  200000       // in [us] - keep at least 5x smaller than RX_TIMEOUT
#define PING        0x08         // PING packet
#define PONG        0x09         // PONG packet
#define SYNC_LEN    7            // in [B]


/********************************************************************************************
 * Class definition
 ********************************************************************************************/
class RF24_wrapper {

	public:
		/************************************************************************************
		 * init
		 *
		 * param: RX_addr - the TX address
		 ************************************************************************************/
		RF24_wrapper(
			byte RX_addr[6] // input
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
		 * set the TX address
		 *
		 * param:  TX_addr - the TX address
		 * return:
		 ************************************************************************************/
		void set_TX(
			byte TX_addr[6] // input
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
		 * receive sync packet
		 *
		 * param:  packet - the received sync packet
		 * return: whether a sync packet is receved
		 ***********************************************************************************/
		bool get_sync(
			byte packet[SYNC_LEN] // output
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
		 * change the transceiver's power level
		 *
		 * param:   power_level - the power level to set
		 * return:
		 ***********************************************************************************/
		void change_power_level(
			byte power_level // input
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
		 * print debugging information
		 * NOTE: 1. need to #include <printf.h>
		 *       2. call printf_begin(); at the program's begging 
		 *
		 * param: 
		 * return:
		 ************************************************************************************/
		void print_details();


	private:
		RF24* rf24;       // RF object
		byte  RX_addr[6]; // RX addr

};


#endif /* rf24_wrapper_h */


