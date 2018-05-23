//
//  client.h
//  thesis_project
//
//  Created by Alberto Sonnino on 31/05/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#ifndef client_h
#define client_h

#include <stdio.h>
#include "main.h"
#include "elliptic_curve.h"
#include "utilities.h"

/*
 * Verify credential issuance
 */
int verify_credentials(char* messages, pt U_prime, scalar c, scalar* r, pt Cx0, pt* iparams);

void credential_presentation(char* messages, pt U_prime, pt* iparams, pt* Ua, pt* Cm, pt* Cua_prime, scalar* responses_m, scalar* responses_z, scalar* response_r, scalar* c);

#endif /* client_h */
