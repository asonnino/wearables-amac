//
//  elliptic_curve.h
//  thesis_project
//
//  Created by Alberto Sonnino on 02/06/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#ifndef elliptic_curve_h
#define elliptic_curve_h

#include <stdio.h>
#include <math.h>
#include "main.h"
#include "param.h"
#include "utilities.h"


/*
 * Infinity point - neutral element
 * NOTE: (0,0) is never a point on the curve
 */
#define INF 0


/*
 * Init fuction - initialize a point
 */
pt init_pt(scalar x, scalar y);

/*
 * Create a point at infinity
 */
pt zero(void);

/*
 * Check if the point is at infinity
 */
int is_zero(pt p);

/*
 * Check if point is on curve
 * NOTE: consider EC of the form: y^2 = x^3 + a*x + b
 */
int is_on_curve(pt p);

/*
 * Point addition
 * NOTE: - only positive points are allowed
 *       - points have to be on the curve
 */
pt point_add(pt p1, pt p2);

/*
 * Point doubling
 * NOTE: - only positive points are allowed
 *       - points have to be on the curve
 */
pt point_double( pt p );

/*
 * Scalar multiplication
 * NOTE: - only positive points are allowed
 *       - points have to be on the curve
 */
pt point_mult (pt p, scalar n);

/*
 * Invert point
 */
pt point_invert(pt p);

#endif /* elliptic_curve_h */
