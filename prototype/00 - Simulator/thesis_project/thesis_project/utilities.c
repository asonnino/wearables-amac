//
//  utilities.c
//  thesis_project
//
//  Created by Alberto Sonnino on 02/06/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#include "utilities.h"


/*
 * Return a random long integer between 0 and max
 * NOTE: assumes 0 <= max <= RAND_MAX
 */
scalar random_at_most(scalar max)
{
    scalar
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (scalar) max + 1,
    num_rand = (scalar) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;
    
    // loop ensure randomness
    scalar x;
    do {
        x = random();
    }
    // This is carefully written not to overflow
    while (num_rand - defect <= (scalar)x);
    
    // Truncated division is intentional
    return x/bin_size;
}

/*
 * Implemetation of EEA
 */
scalar extended_euclid(scalar a, scalar b, scalar *x, scalar *y)
{
    scalar t, d;
    if (b == 0)	{
        *x = 1; *y = 0; return a;
    }
    d = extended_euclid(b, a % b, x, y);
    t = *x; *x = *y; *y = t - (a/b)*(*y);
    return d;
}

/*
 * Modular inverse
 */
scalar mod_inverse(scalar a, scalar n)
{
    scalar x, y;
    extended_euclid(a, n, &x, &y);
    return x;
}



/*
 * Convert int64_t into a char table
 */
void int64ToChar(unsigned char a[], int64_t n)
{
    memcpy(a, &n, 8);
}

/*
 * Convert a char table into a int64_t
 */
int64_t charTo64bitNum(unsigned char a[])
{
    int64_t n = 0;
    memcpy(&n, a, 8);
    return n;
}


/*
 * compute challenge by hashing U, Wu', Wcx0 and Wiparams
 */
scalar create_challenge(pt* point_list, int length)
{
    // init hash object
    SHA256_CTX ctx;
    sha256_init(&ctx);
    
    // update hash
    for(int i=0; i<length; i++)
    {
        // x component
        unsigned char x[SCALAR_BIT_LEN/8];
        int64ToChar(x, point_list[i].x);
        sha256_update(&ctx,x,SCALAR_BIT_LEN/8);
        // y component
        unsigned char y[SCALAR_BIT_LEN/8];
        int64ToChar(y, point_list[i].y);
        sha256_update(&ctx,y,SCALAR_BIT_LEN/8);
    }
    
    // finalize
    unsigned char hash[32];
    sha256_final(&ctx,hash);
    
    return charTo64bitNum(hash);
}
