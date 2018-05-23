/********************************************************************************************
 *
 * ecc_prototype.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 * NOTE: DUMP LIBRARY - PROTOTYPING PURPOSE ONLY !!
 *       --  NOT CRYPTOGRAPHICALLY SECURE --
 *
 *       This code is almost "copy-pasted" from the open-source libraries Oryx Embedded, NaCl 
 *       and WolfSSL. Some modifications had to be done in order to match the rest of the code   
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
 *          WolfSSL. 
 *          Embedded SSL Library. 
 *          https://www.wolfssl.com/wolfSSL/Home.html. 
 *          Visited on the 7th of Aug. 2016.
 *
 ********************************************************************************************/
#ifndef ecc_prototype_h
#define ecc_prototype_h


/********************************************************************************************
 * Includes
 ********************************************************************************************/
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
    #include "Arduino.h"
#elif defined(__arm__) // Raspberry Pi
 	#include <stdint.h>
 	#include <stdio.h>
#endif


/********************************************************************************************
 * Data type definition
 ********************************************************************************************/
typedef int32_t scalar;
#define SCALAR_BIT_LEN 32


/********************************************************************************************
 * Infinity point - Neutral element
 *
 * NOTE: (0,0) is not a point on the curve.
 ********************************************************************************************/
#define INF 0


/********************************************************************************************
 * Curve config.
 * 
 * EC of the form: y^2 = x^3 + a*x + b
 ********************************************************************************************/
#define GROUP_ORDER 19
#define PRIME       17
#define a_COEFF     2
#define b_COEFF     2



/********************************************************************************************
 * Class definition
 ********************************************************************************************/
class Point {

	public:
		/************************************************************************************
		 * init
		 *
		 * param: x - the point's X coordinate
		 *        y - the point's Y coordinate
		 ************************************************************************************/
		Point(
			scalar x, // input
			scalar y  // input
		);

		/************************************************************************************
		 * init
		 *
		 * param: 
		 ************************************************************************************/
		Point();

		/************************************************************************************
		 * init
		 *
		 * param: p - the point to clone
		 ************************************************************************************/
		Point(
			Point* p // input
		);

		/************************************************************************************
		 * destructor
		 ************************************************************************************/
		~Point();

		/************************************************************************************
		 * getX
		 *
		 * param: 
		 * return: get the point's X coordinate
		 ************************************************************************************/
		scalar getX();

		/************************************************************************************
		 * getY
		 *
		 * param: 
		 * return: get the point's Y coordinate
		 ************************************************************************************/
		scalar getY();

		/************************************************************************************
		 * set the point to intifity
		 *
		 * param:  
		 * return: 
		 ************************************************************************************/
		void zero();

		/************************************************************************************
		 * check whether the point is at infinity
		 *
		 * param:  
		 * return: whether the point is at infinity
		 ************************************************************************************/
		bool is_zero();

		/************************************************************************************
		 * check whether the point is on the curve
		 *
		 * param:  
		 * return: whether the point is on the curve
		 ************************************************************************************/
		bool is_on_curve();

		/************************************************************************************
		 * point addition
		 *
		 * param:  p - the point to add
		 * return: 
		 ************************************************************************************/
		void point_add(
			Point p // input
		);

		/************************************************************************************
		 * point doubling
		 *
		 * param:  
		 * return: 
		 ************************************************************************************/
		void point_double();

		/************************************************************************************
		 * point scalar multiplication
		 *
		 * param:  n - the scalar with whom to multiply the point
		 * return: 
		 ************************************************************************************/
		void point_mult(
			scalar n // input
		);

		/************************************************************************************
		 * point invert
		 *
		 * param:  
		 * return: 
		 ************************************************************************************/
		void point_invert();

		/************************************************************************************
		 * EEA implementation
		 * calculates a * (*x) + b * (*y) = gcd(a, b) = (*d)
		 *
		 * param:  a - first scalar
		 *         b - second scalar
		 *         x - first scalar's coefficient
		 *         y - second scalar's coefficient
		 * return: the greatest common divisor of a and b
		 ************************************************************************************/
		static scalar extended_euclid(
			scalar a,  // input
			scalar b,  // input
			scalar *x, // output
			scalar *y  // output
		);

		/************************************************************************************
		 * modular inverse
		 *
		 * param:  a - the scalar to invert
		 *         n - the modulus
		 * return: the inverted scalar
		 ************************************************************************************/
		static scalar mod_inverse(
			scalar a, // input
			scalar n  // input
		);

 		/************************************************************************************
		 * print point's coordinates
		 * NOTE: need to include "printf.h" and call "printf_begin();"
		 *
		 * param:
		 * return: 
		 ************************************************************************************/
		void print_point();


	private:
		scalar x; // point's X coordinate
		scalar y; // point's Y coordinate
};


#endif /* ecc_prototype_h */
