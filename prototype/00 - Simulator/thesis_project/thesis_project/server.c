//
//  server.c
//  thesis_project
//
//  Created by Alberto Sonnino on 31/05/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#include "server.h"


/*
 * KeyGen of MAC_GGM
 * NOTE: - sk should be a table of length (NUM_OF_MESSAGES + 1)
 *       - iparam should be a table of lenght (NUM_OF_MESSAGES)
 */
void MAC_GGM_keygen(scalar *sk, pt *iparam)
{
    // fill first sk
    sk[0] = random_at_most(GROUP_ORDER - 2) + 1;
    
    // fill sk and iparam
    for(int i=0; i<(NUM_OF_MESSAGES); i++) {
        // sk - between [1, GROUP_ORDER-1]
        sk[i+1] = random_at_most(GROUP_ORDER - 2) + 1;
        // iparam
        iparam[i] = point_mult(H, sk[i+1]);
    }
}

/*
 * MAC algorithm of MAC_GGM
 */
void MAC_GGM(scalar *sk, char *messages, pt *U_prime, scalar *hx)
{
    // compute function Hx(m)
    *hx = sk[0];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        *hx = (*hx + sk[i+1] * messages[i]) % GROUP_ORDER;
    }
    
    // compute U_prime
    *U_prime = point_mult(U, *hx);
}

/*
 * Verify of MAC_GGM
 */
int verify_GGM(pt P, pt P_prime, scalar hx)
{
    // parse (u,u')
    if(! (is_on_curve(P) && is_on_curve(P_prime))){
        return 0;
    }
    
    // verify u' = u^Hx(m)
    pt tmp = point_mult(P, hx);
    if(! ((P_prime.x == tmp.x) && (P_prime.y == tmp.y))){
        return 0;
    }
    
    // if all contitions hold
    return 1;
}



/*
 * Issue credentials
 */
void issue_credentials(scalar* sk, scalar x0_tilde, char *messages, scalar *r, scalar *c)
{
    // pick up random values w
    scalar w[NUM_OF_MESSAGES+2];
    for(int i=0; i<(NUM_OF_MESSAGES+2); i++) {
        // w - between [1, GROUP_ORDER-1]
        w[i] = random_at_most(GROUP_ORDER - 2) + 1;
    }
    
    // create witnesse for U'
    pt Wu_prime = point_mult(U, w[0]);
    pt tmp;
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        tmp = point_mult(U, (w[i+1]*messages[i])%GROUP_ORDER);
        Wu_prime = point_add(Wu_prime, tmp);
    }
    
    // create witness for Cx0
    pt Wcx0 = point_add(
        point_mult(G, w[0]),
        point_mult(H, w[NUM_OF_MESSAGES+1])
    );
    
    // create withnesses for iparams
    pt Wiparams[NUM_OF_MESSAGES];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        Wiparams[i] = point_mult(H, w[i+1]);
    }

    // create challenge
    pt point_list[NUM_OF_MESSAGES + 3];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[i] = Wiparams[i];
    }
    point_list[NUM_OF_MESSAGES]   = U;
    point_list[NUM_OF_MESSAGES+1] = Wu_prime;
    point_list[NUM_OF_MESSAGES+2] = Wcx0;
    *c = create_challenge(point_list, NUM_OF_MESSAGES + 3) % GROUP_ORDER;
    *c = (*c < 0) ? (*c + GROUP_ORDER) : *c;
    
    // create response
    for(int i=0; i<NUM_OF_MESSAGES+1; i++){
        r[i] = (w[i] - (*c) * sk[i]) % GROUP_ORDER;
        r[i] = (r[i]<0) ? r[i] + GROUP_ORDER : r[i];
    }
    r[NUM_OF_MESSAGES+1] = (w[NUM_OF_MESSAGES+1] - (*c) * x0_tilde) % GROUP_ORDER;
    r[NUM_OF_MESSAGES+1] = (r[NUM_OF_MESSAGES+1]<0) ? r[NUM_OF_MESSAGES+1] + GROUP_ORDER : r[NUM_OF_MESSAGES+1];
}




/*
 * Verify credential presentation
 */
int verify_credential_presentation(scalar* sk, pt* iparams, pt Ua, pt* Cm, pt Cua_prime, scalar* responses_m, scalar* responses_z, scalar response_r,scalar c)
{
    // compute V
    pt V = point_mult(Ua, sk[0]);
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        V = point_add(V, point_mult(Cm[i], sk[i+1]));
    }
    V = point_add(V, point_invert(Cua_prime));
    // reconstruct witnesses of Cm
    pt WCm[NUM_OF_MESSAGES];
    for(int i=0; i<(NUM_OF_MESSAGES); i++){
        WCm[i] = point_add(
            point_mult(Ua, responses_m[i]),
            point_mult(H, responses_z[i])
        );
        WCm[i] = point_add(WCm[i],point_mult(Cm[i], c));
    }

    // reconstruct witness of V
    pt WV = point_mult(G, response_r);
    for(int i=0; i<(NUM_OF_MESSAGES); i++){
        WV = point_add(WV, point_mult(iparams[i], responses_z[i]));
    }
    WV = point_add(WV, point_mult(V, c));
    
    // verify challenge
    pt point_list[3*NUM_OF_MESSAGES+2];
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[i] = iparams[i];
    }
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[NUM_OF_MESSAGES+i] = Cm[i];
    }
    point_list[2*NUM_OF_MESSAGES] = Cua_prime;
    for(int i=0; i<NUM_OF_MESSAGES; i++){
        point_list[2*NUM_OF_MESSAGES+1+i] = WCm[i];
    }
    point_list[3*NUM_OF_MESSAGES+1] = WV;
    
    scalar challenge = create_challenge(point_list, 3*NUM_OF_MESSAGES+2) % GROUP_ORDER;
    challenge = (challenge < 0) ? (challenge + GROUP_ORDER) : challenge;

    // return
    return (c == challenge);
}

