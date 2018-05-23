//
//  client.c
//  thesis_project
//
//  Created by Alberto Sonnino on 31/05/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#include "client.h"


/*
 * Verify credential issuance
 */
int verify_credentials(char* messages, pt U_prime, scalar c, scalar* r, pt Cx0, pt* iparams)
{    
    // reconstruct witnesse of U'
    pt Wu_prime = point_mult(U, r[0]);
    pt tmp;
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        tmp = point_mult(U, (r[i+1]*messages[i])%GROUP_ORDER);
        Wu_prime = point_add(Wu_prime, tmp);
    }
    Wu_prime = point_add(Wu_prime, point_mult(U_prime, c));

    // reconstruct witness of Cx0
    pt Wcx0 = point_add(
        point_mult(G, r[0]),
        point_mult(H, r[NUM_OF_MESSAGES+1])
    );
    Wcx0 = point_add(Wcx0, point_mult(Cx0, c));

    // reconstruct withnesses of iparams
    pt Wiparams[NUM_OF_MESSAGES];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        Wiparams[i] = point_mult(H, r[i+1]);
        Wiparams[i] = point_add(Wiparams[i], point_mult(iparams[i], c));
    }

    // check challenge
    pt point_list[NUM_OF_MESSAGES + 3];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[i] = Wiparams[i];
    }
    point_list[NUM_OF_MESSAGES]   = U;
    point_list[NUM_OF_MESSAGES+1] = Wu_prime;
    point_list[NUM_OF_MESSAGES+2] = Wcx0;
    
    scalar challenge = create_challenge(point_list, NUM_OF_MESSAGES + 3) % GROUP_ORDER;
    challenge = (challenge < 0) ? (challenge + GROUP_ORDER) : challenge;
    return (c == challenge);
}


/*
 * Credential presentation
 */
void credential_presentation(char* messages, pt U_prime, pt* iparams, pt* Ua, pt* Cm, pt* Cua_prime, scalar* responses_m, scalar* responses_z, scalar* response_r,scalar* c)
{
    
    /*
     * Re-randomize
     */
    // pick up random value a
    scalar a = random_at_most(GROUP_ORDER - 2) + 1;
    
    // compute Ua and U'a
    *Ua = point_mult(U, a);
    pt Ua_prime = point_mult(U_prime, a);
    
    
    /*
     * Form committements
     */
    // pick up random value r
    scalar r = random_at_most(GROUP_ORDER - 2) + 1;
    
    // pick up random values z
    scalar z[NUM_OF_MESSAGES];
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        // w - between [1, GROUP_ORDER-1]
        z[i] = random_at_most(GROUP_ORDER - 2) + 1;
    }
    
    // compute Cm
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        Cm[i] = point_add(
            point_mult(*Ua, messages[i]),
            point_mult(H, z[i])
        );
    }
    
    // compute CU'
    *Cua_prime = point_add(Ua_prime, point_mult(G, r));
    
    
    
    /*
     * Create proof Pi
     */
    // pick up random w
    scalar wm[NUM_OF_MESSAGES];
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        // w - between [1, GROUP_ORDER-1]
        wm[i] = random_at_most(GROUP_ORDER - 2) + 1;
    }
    scalar wz[NUM_OF_MESSAGES];
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        // w - between [1, GROUP_ORDER-1]
        wz[i] = random_at_most(GROUP_ORDER - 2) + 1;
    }
    scalar wr = random_at_most(GROUP_ORDER - 2) + 1;
    
    // create witnesses for Cm
    pt WCm[NUM_OF_MESSAGES];
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        WCm[i] = point_add(
            point_mult(*Ua, wm[i]),
            point_mult(H, wz[i])
        );
    }
    
    
    // craete witness for V
    pt WV = point_mult(G, wr);
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        WV = point_add(WV, point_mult(iparams[i], wz[i]));
    }
    
    // create challenge
    pt point_list[3*NUM_OF_MESSAGES+2];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[i] = iparams[i];
    }
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[i+NUM_OF_MESSAGES] = Cm[i];
    }
    point_list[2*NUM_OF_MESSAGES] = *Cua_prime;
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[2*NUM_OF_MESSAGES+1+i] = WCm[i];
    }
    point_list[3*NUM_OF_MESSAGES+1] = WV;
    *c = create_challenge(point_list, (3*NUM_OF_MESSAGES+2)) % GROUP_ORDER;
    *c = (*c < 0) ? (*c + GROUP_ORDER) : *c;
    
    
    // compute responses
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        responses_m[i] = (wm[i] - (*c) * messages[i]) % GROUP_ORDER;
        responses_z[i] = (wz[i] - (*c) * z[i]) % GROUP_ORDER;
        
        responses_m[i] = responses_m[i]<0 ? responses_m[i]+GROUP_ORDER : responses_m[i];
        responses_z[i] = responses_z[i]<0 ? responses_z[i]+GROUP_ORDER : responses_z[i];
    }
    *response_r = (wr + (*c) * r) % GROUP_ORDER;
    *response_r = *response_r<0 ? *response_r+GROUP_ORDER : *response_r;
}



