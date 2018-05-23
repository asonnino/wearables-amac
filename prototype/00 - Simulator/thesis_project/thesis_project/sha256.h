//
//  sha256.h
//  thesis_project
//
//  Created by Alberto Sonnino on 03/06/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//



/*
 * NOTE: CODE COMPES FROM:
 *  http://bradconte.com/sha256_c
 */
#ifndef sha256_h
#define sha256_h


#define uchar unsigned char // 8-bit byte
#define uint unsigned int // 32-bit word

// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef struct {
    uchar data[64];
    uint datalen;
    uint bitlen[2];
    uint state[8];
} SHA256_CTX;


void sha256_transform(SHA256_CTX *ctx, uchar data[]);
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, uchar data[], uint len);
void sha256_final(SHA256_CTX *ctx, uchar hash[]);



#endif /* sha256_h */
