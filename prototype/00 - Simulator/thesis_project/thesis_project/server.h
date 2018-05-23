//
//  server.h
//  thesis_project
//
//  Created by Alberto Sonnino on 31/05/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#ifndef server_h
#define server_h

#include <stdio.h>
#include "main.h"
#include "param.h"
#include "utilities.h"
#include "elliptic_curve.h"


/*
 * KeyGen of MAC_GGM
 * NOTE: - sk should be a table of length (NUM_OF_MESSAGES + 1)
 *       - iparam should be a table of lenght (NUM_OF_MESSAGES)
 */
void MAC_GGM_keygen(scalar *sk, pt *iparam);

/*
 * MAC algorithm of MAC_GGM
 */
void MAC_GGM(scalar *sk, char *messages, pt *u_prime, scalar *hx);

/*
 * Verify of MAC_GGM
 */
int verify_GGM(pt P, pt P_Prime, scalar hx);


/*
 * Issue credentials
 */
void issue_credentials(scalar* sk, scalar x0_tilde, char *messages, scalar *r, scalar *c);

/*
 * Verify credential presentation
 */
int verify_credential_presentation(scalar* sk, pt* iparams, pt Ua, pt* Cm, pt Cua_prime, scalar* responses_m, scalar* responses_z, scalar response_r,scalar c);

#endif /* server_h */
