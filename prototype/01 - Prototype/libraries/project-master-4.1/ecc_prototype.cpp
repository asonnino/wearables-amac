/********************************************************************************************
 *
 * ecc_prototype.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *       
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
#include "ecc_prototype.h"


/********************************************************************************************
 *
 * Init
 *
 ********************************************************************************************/
Point::Point(scalar x, scalar y) 
{
    this->x = x;
    this->y = y;
}

Point::Point() 
{
    this->x = INF;
    this->y = INF;
}

Point::Point(Point* p) 
{
    this->x = p->x;
    this->y = p->y;
}


/********************************************************************************************
 *
 * Destructor
 *
 ********************************************************************************************/
Point::~Point()
{
    //empty
}


/********************************************************************************************
 *
 * Get the point's coordinates
 *
 ********************************************************************************************/
scalar Point::getX() 
{
    return this->x;
}

scalar Point::getY() 
{
    return this->y;
}


/********************************************************************************************
 *
 * Set to infinity
 *
 ********************************************************************************************/
void Point::zero() {
    this->x = INF;
    this->y = INF;
}


/********************************************************************************************
 *
 * Check if point is at ininity
 *
 ********************************************************************************************/
bool Point::is_zero()
{
    if (x == INF && y == INF) {
        return true;
    }
    else{
        return false;
    }
}


/********************************************************************************************
 *
 * Check whether the point is on the curve
 *
 ********************************************************************************************/
bool Point::is_on_curve()
{
    if ((y * y) % PRIME == (x * x * x + a_COEFF * x + b_COEFF) % PRIME) {
        return true;
    }
    else{
        return false;
    }
}


/********************************************************************************************
 *
 * Point addition
 *
 * NOTE: Safety checks have been removed to fit the very limited Arduino CPU resources. 
 *       Therefore:
 *          - only "positive" point's coordinates are allowed
 *          - points have to be on the curve
 *
 ********************************************************************************************/
void Point::point_add(Point p)
{
    // sanity checks
    if (this->is_zero()) {
        this->x = p.x; this->y = p.y; return;
    }
    if (p.is_zero()){ 
        return; 
    }
    if (this->x == p.x && this->y == p.y) { 
        this->point_double(); return;
    }
    if (this->x == p.x) {
        this->zero(); return;
    }
    
    // temp variable declaration
    Point r(INF, INF);
    
    // compute slope
    scalar den = (p.x - this->x) % PRIME; den = den < 0 ? (den + PRIME) : den;
    scalar num = (p.y - this->y) % PRIME; num = num < 0 ? (num + PRIME) : num;
    scalar s   = (num * mod_inverse(den, PRIME)) % PRIME;

    // compute points coordinates
    r.x = (s * s - this->x - p.x) % PRIME;
    r.y = (s * (this->x - r.x) - this->y) % PRIME;
    
    // ensure positive outputs
    r.x = (r.x < 0) ? r.x + PRIME : r.x; 
    r.y = (r.y < 0) ? r.y + PRIME : r.y;
    
    // set result
    this->x = r.x; this->y = r.y;
}


/********************************************************************************************
 *
 * Point doubling
 *
 * NOTE: Safety checks have been removed to fit the very limited Arduino CPU resources. 
 *       Therefore:
 *          - only "positive" point's coordinates are allowed
 *          - points have to be on the curve
 *
 ********************************************************************************************/
void Point::point_double()
{
    // sanity check
    if (this->is_zero()) {
        return;
    }
    
    // declare temp variables
    Point r(INF, INF);
    
    // compute slope
    scalar s = ((3 * this->x * this->x + a_COEFF) * mod_inverse(2 * this->y, PRIME)) %  PRIME;

    // compute points coordinates
    r.x = (s * s - 2 * this->x) % PRIME;
    r.y = (s * (this->x - r.x) - this->y) % PRIME;
    
    // ensure p_ositive outputs
    r.x = (r.x < 0) ? r.x + PRIME : r.x;
    r.y = (r.y < 0) ? r.y + PRIME : r.y;
    
    // set result
    this->x = r.x; this->y = r.y;
}

/********************************************************************************************
 *
 * Point scalar multiplication
 *
 * NOTE: To save performances, this function is NOT time constant - prototyping prupose only.
 *       Safety checks have been removed to fit the very limited Arduino CPU resources. 
 *       Therefore:
 *          - only "positive" point's coordinates are allowed
 *          - points have to be on the curve
 *       
 ********************************************************************************************/
void Point::point_mult(scalar n)
{
    // initialize
    Point r(INF, INF);
    
    // multiply
    for (scalar i = 1; i <= n; i <<= 1) {
        if (i & n) {
            r.point_add(*this);
        }
        point_double();
    }
    
    // return result
    this->x = r.x; this->y = r.y;
}


/********************************************************************************************
 *
 * Point invert
 *
 ********************************************************************************************/
void Point::point_invert()
{
    this->y = (this->y == 0) ? 0 : (-this->y + PRIME); // consider point Origin 
}


/********************************************************************************************
 *
 * EEA implementation
 *
 ********************************************************************************************/
scalar Point::extended_euclid(scalar a, scalar b, scalar *x, scalar *y)
{
    scalar t, d;
    if (b == 0) {
        *x = 1; *y = 0; return a;
    }
    d = extended_euclid(b, a % b, x, y);
    t = *x; *x = *y; *y = t - (a/b)*(*y);
    return d;
}


/********************************************************************************************
 *
 * Modular inverse
 *
 ********************************************************************************************/
scalar Point::mod_inverse(scalar a, scalar n)
{
    scalar x, y;
    extended_euclid(a, n, &x, &y);
    return x;
}


/********************************************************************************************
 *
 * Print point's coordinates
 *
 * NOTE: need to include "printf.h" and call "printf_begin();"
 ********************************************************************************************/
void Point::print_point()
{
    #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Arduino UNO and older boards
        Serial.print("("); Serial.print(x); Serial.print(","); Serial.print(y); Serial.println(")");
    #else
        printf("(%d,%d)\n", x,y);
    #endif 
}



