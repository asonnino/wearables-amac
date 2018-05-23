//
//  param.h
//  thesis_project
//
//  Created by Alberto Sonnino on 02/06/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#ifndef param_h
#define param_h

#include "elliptic_curve.h"


/* 
 * Curve coefficients
 * NOTE: consider EC of the form: y^2 = x^3 + a*x + b
 */
#define a_COEFF  2
#define b_COEFF  2

/*
 * A points on the curve
 */
#define G init_pt(5,1)

/*
 * A points on the curve
 * NOTE: H = 5G
 */
#define H init_pt(9,16)

/*
 * A points on the curve
 * NOTE: U = 4G
 */
#define U init_pt(3,1)

/*
 * Order of the group
 */
#define GROUP_ORDER 19

/*
 * Prime modulus
 */
#define PRIME 17


/*
 * Number of messages
 */
#define NUM_OF_MESSAGES 2

/*
 * Messages
 */




#endif /* param_h */
