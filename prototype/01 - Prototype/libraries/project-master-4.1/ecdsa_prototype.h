/********************************************************************************************
 *
 * ecdsa_prototype.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 * NOTE: DUMP LIBRARY - PROTOTYPING PURPOSE ONLY !!
 *       --  NOT CRYPTOGRAPHICALLY SECURE --
 *
 *       This code is almost "copy-pasted" from the open-source libraries Oryx Embedded and 
 *       NaCl. Some modifications had to be done in order to match the rest of the code   
 *       and to fit in the very limited CPU resources.
 *       
 *          Oryx Embedded. 
 *          TCP/IP Solutions for Embedded Systems. 
 *          http://www.oryx-embedded.com/\#\&panel1-1. 
 *          Visited on the 7th of Aug. 2016.
 *
 *          NaCl: Networking and Cryptography library. 
 *          https://nacl.cr.yp.to. Version 2016.03.15. 
 *          Visited on the 7th of Aug. 2016.
 *       
 ********************************************************************************************/
#ifndef ecdsa_prototype_h
#define ecdsa_prototype_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
    #include "Arduino.h"
#elif defined(__arm__) // Raspberry Pi
 	#include <stdint.h>
 	#include <stdio.h>
#endif
#include "config.h"
#include "ecc_prototype.h"
#include "random.h"


/********************************************************************************************
 * Functions definition
 ********************************************************************************************/
#ifdef __cplusplus
 	extern "C" {
#endif

/************************************************************************************
 * sign
 *
 * param:  sk   - the secret key
 *		   hash - the hash of the message to sign
 *		   s    - the DSA's signature parameter s
 *         r    - the DSA's signature parameter r
 * return: 
 ************************************************************************************/
void ecdsa_sign(
	scalar  sk,   // input
	scalar  hash, // input
	scalar* r,    // output
	scalar* s     // output
);

/************************************************************************************
 * verify
 *
 * param:  pk   - the public key
 *         hash - the hash of the message to sign
 *		   s    - the DSA's signature parameter s
 *         r    - the DSA's signature parameter r
 * return: whether the signature is valid
 ************************************************************************************/
bool ecdsa_verify(
	Point pk,    // input
	scalar hash, // input
	scalar r,    // input
	scalar s     // input
);


#ifdef __cplusplus
 	};
#endif


#endif /* ecdsa_prototype_h */
