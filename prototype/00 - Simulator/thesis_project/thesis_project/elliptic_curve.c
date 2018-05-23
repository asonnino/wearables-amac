//
//  elliptic_curve.c
//  thesis_project
//
//  Created by Alberto Sonnino on 02/06/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#include "elliptic_curve.h"

/*
 * Init fuction - initialize a point
 */
pt init_pt(scalar x, scalar y)
{
    pt p;
    p.x = x;
    p.y = y;
    return p;
}

/*
 * Create a point at infinity
 */
pt zero(void)
{
    return (pt){INF, INF};
}

/*
 * Check if the point is at infinity
 */
int is_zero(pt p)
{
    if(p.x == INF && p.y == INF){return 1;}
    else{return 0;}
}


/*
 * Check if point is on curve
 * NOTE: consider EC of the form: y^2 = x^3 + a*x + b
 */
int is_on_curve(pt p)
{
    if(
        (p.y * p.y) % PRIME == (p.x * p.x * p.x + a_COEFF * p.x + b_COEFF) % PRIME
    ){return 1;}
    else{return 0;}
}

/*
 * Point addition
 * NOTE: - only positive points are allowed
 *       - points have to be on the curve
 */
pt point_add(pt p1, pt p2)
{
    // sanity checks
    if (is_zero(p1)){return p2;}
    if (is_zero(p2)){return p1;}
    if (p1.x == p2.x && p1.y == p2.y){return point_double(p1);}
    if (p1.x == p2.x){return init_pt(INF, INF);}
    
    // variable declaration
    pt r;
    
    // compute slope
    scalar den = (p2.x - p1.x) % PRIME;
    den = den < 0 ? (den + PRIME) : den;
    scalar num = (p2.y - p1.y) % PRIME;
    num = num < 0 ? (num + PRIME) : num;
    scalar s = (num * mod_inverse(den, PRIME)) % PRIME;
    
    // compute points coordinates
    r.x = (s * s - p1.x - p2.x) % PRIME;
    r.y = (s * (p1.x - r.x) - p1.y) % PRIME;
    
    // ensure positive outputs
    r.x = (r.x < 0) ? r.x + PRIME : r.x;
    r.y = (r.y < 0) ? r.y + PRIME : r.y;
    
    // return result
    return r;
}

/*
 * Point doubling
 * NOTE: - only positive points are allowed
 *       - points have to be on the curve
 */
pt point_double(pt p)
{
    // sanity check
    if (is_zero(p)){return p;}
    
    // declare variables
    pt r;
    
    // compute slope
    scalar s = ((3 * p.x * p.x + a_COEFF) * mod_inverse(2 * p.y, PRIME)) %  PRIME;

    // compute points coordinates
    r.x = (s * s - 2 * p.x) % PRIME;
    r.y = (s * (p.x - r.x) - p.y) % PRIME;
    
    // ensure positive outputs
    r.x = (r.x < 0) ? r.x + PRIME : r.x;
    r.y = (r.y < 0) ? r.y + PRIME : r.y;
    
    // return result
    return r;
}

/*
 * Scalar multiplication
 * NOTE: - only positive points are allowed
 *       - points have to be on the curve
 */
pt point_mult (pt p, scalar n)
{
    // initialize
    pt r = zero();
    
    // multiply
    for (scalar i = 1; i <= n; i <<= 1) {
        if (i & n) {
            r = point_add(r, p);
        }
        p = point_double(p);
    }
    
    // return result
    return r;
}

/*
 * Invert point
 */
pt point_invert(pt p)
{
    return init_pt(p.x, -p.y + PRIME);
}