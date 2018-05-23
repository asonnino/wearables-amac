//
//  utilities.h
//  thesis_project
//
//  Created by Alberto Sonnino on 02/06/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#include <stdio.h>
#include "main.h"
#include <stdlib.h> // For random(), RAND_MAX
#include <string.h> // For memcpy
#include "sha256.h"


/*
 * Return a random long integer between 0 and max
 * NOTE: assumes 0 <= max <= RAND_MAX
 */
scalar random_at_most(scalar max);


/*
 * Structure defining a point
 */
typedef struct {
    scalar x, y;
} pt;


/*
 * Implemetation of EEA
 */
scalar extended_euclid(scalar a, scalar b, scalar *x, scalar *y);

/*
 * Modular inverse
 */
scalar mod_inverse(scalar a, scalar n);


/*
 * Convert int64_t into a char table
 */
void int64ToChar(unsigned char a[], int64_t n);

/*
 * Convert a char table into a int64_t
 */
int64_t charTo64bitNum(unsigned char a[]);

/*
 * compute challenge by hasing the input points
 */
scalar create_challenge(pt* point_list, int length);


#endif /* utilities_h */
