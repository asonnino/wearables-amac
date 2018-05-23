/********************************************************************************************
 *
 * random.cpp
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
#include "random.h"


/************************************************************************************
 * 
 * Init the rng
 * 
 ************************************************************************************/
void init_random()
{
	#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
		  // if analog input pin RNG_PIN is unconnected, random analog
		  // noise will cause the call to randomSeed() to generate
  		// different seed numbers each time the sketch runs.
  		// randomSeed() will then shuffle the random function.
  		randomSeed(analogRead(RNG_APIN));
  	#else
  		srand (time(NULL));
  	#endif

}

/************************************************************************************
 *
 * generate a random number
 *
 * NOTE: call init_random() once before using this method
 *
 ************************************************************************************/
long get_random(long min, long max)
{	
	#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
		return random(min, max);
	#else
		int r;
    	const unsigned int range   = 1 + max - min;
    	const unsigned int buckets = RAND_MAX / range;
    	const unsigned int limit   = buckets * range;

    	// create buckets, then test numbers randomly towards the buckets until 
     	// the desired range is met. 
    	do {
        	r = rand();
    	} while (r >= limit);

    	return min + (r / buckets);
	#endif
}