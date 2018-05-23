/********************************************************************************************
 *
 * ecdsa_prototype.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *       
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
#include "ecdsa_prototype.h"


/********************************************************************************************
 *
 * Sign
 *
 ********************************************************************************************/
void ecdsa_sign(scalar sk, scalar hash, scalar* r, scalar* s)
{
	// ECDSA sign
	while (true) {
        scalar k = get_random(1, GROUP_ORDER); // random
        Point* tmp = G; tmp->point_mult(k);
        if ( (tmp->getX() != INF) && (tmp->getX() != 0) ) {
            (*r) = tmp->getX();
            (*s) = 
                ( Point::mod_inverse(k,GROUP_ORDER) * (hash + (*r) * sk) ) % GROUP_ORDER;
            (*s) = ((*s) < 0 ) ? (*s) + GROUP_ORDER : (*s);
            delete tmp;
            break;
        }
        delete tmp;
    }
}

/********************************************************************************************
 *
 * Sign
 *
 ********************************************************************************************/
bool ecdsa_verify(Point pk, scalar hash, scalar r, scalar s)
{
	// ECDSA verification
	scalar w    = Point::mod_inverse(s, GROUP_ORDER);

    scalar u1   = (hash * w) % GROUP_ORDER; u1 = (u1<0) ? u1 + GROUP_ORDER : u1;
    scalar u2   = (r * w)    % GROUP_ORDER; u2 = (u2<0) ? u2 + GROUP_ORDER : u2;

    Point* tmp1 = G;              tmp1->point_mult(u1); 
    Point* tmp2 = new Point(&pk); tmp2->point_mult(u2);
    tmp2->point_add(tmp1);

    // result
    bool result = (r == tmp2->getX());

    // free variables
    delete tmp1; delete tmp2;

    // return
    return result;
}


