/********************************************************************************************
 *
 * random.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 *
 * NOTE: DUMP LIBRARY - PROTOTYPING PURPOSE ONLY !!
 *       --  NOT CRYPTOGRAPHICALLY SECURE --
 *
 ********************************************************************************************/
#ifndef random_h
#define random_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
	#include "Arduino.h"
#else
 	#include <time.h>
 	#include <stdlib.h> // For random(), RAND_MAX
#endif /* includes */


/********************************************************************************************
 * Defines
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
	#define RNG_APIN   0 // should be an analog PIN
#endif /* defines */


/********************************************************************************************
 * Functions definition
 ********************************************************************************************/
#ifdef __cplusplus
 	extern "C" {
#endif

/************************************************************************************
 * init the rng
 *
 * param:
 ************************************************************************************/
void init_random();

/************************************************************************************
 * generate a random number
 * NOTE: call init_random() once before using this method
 *
 * param:  min - lower bound of the random value, inclusive
 *         max - upper bound of the random value, exclusive
 * return: a random number between min and max-1 (long)
 ************************************************************************************/
long get_random(
	long min, // input
	long max  // input
);

#ifdef __cplusplus
 	};
#endif


#endif /* random_h */

