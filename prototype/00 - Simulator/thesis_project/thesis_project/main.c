//
//  main.c
//  thesis_project
//
//  Created by Alberto Sonnino on 31/05/16.
//  Copyright © 2016 Sonnino Alberto. All rights reserved.
//

#include <stdio.h>

#include "main.h"
#include "server.h"
#include "client.h"


/*
 * Define messages
 */
char messages[NUM_OF_MESSAGES] = {
    10, 20
};



/*
 * Print the first (num_of_elements) points of the group
 */
void print(int num_of_elements);
void print(int num_of_elements)
{
    // define elements to prints
    int elem_to_print =
        (num_of_elements < GROUP_ORDER) ? num_of_elements : GROUP_ORDER;
    
    // init variables
    pt p = zero();
    
    // print elements
    printf("PRINT %d POINTS OF THE GROUP\n", num_of_elements);
    for(int i=1; i<=elem_to_print; i++){
        p = point_add(G,p);
        printf("%2dG (%lld,%lld)\n", i,p.x,p.y);
    }
    printf("-------END PRINTING-------\n\n\n");

}


/*
 * Main function
 */
int main(int argc, const char * argv[])
{

    ///////////////////////////////////////////////////////////////////////////
    // Simulate credential issuance
    ///////////////////////////////////////////////////////////////////////////
    /*
     * For server
     */
    // MAC_GGM KeyGen
    scalar sk[NUM_OF_MESSAGES + 1];
    pt iparams[NUM_OF_MESSAGES];
    MAC_GGM_keygen(sk, iparams);
    
    for(int i=0; i<NUM_OF_MESSAGES+1; i++) {
        printf("sk[%d] = %lld", i, sk[i]);
    }
    
    // MAC
    scalar hx;
    pt U_prime;
    MAC_GGM(sk, messages, &U_prime, &hx);
    
    // MAC_GGM Verify
    printf("MAC GGM Verification: %d\n",verify_GGM(U, U_prime, hx));
    
    // issue credentials
    scalar responses[NUM_OF_MESSAGES+2];
    scalar c;
    scalar x0_tilde = random_at_most(GROUP_ORDER - 2) + 1;
    issue_credentials(sk, x0_tilde, messages, responses, &c);
    
    
    /*
     * Send to the client:
     *      U, U_prime, c, r, Cx0, iparams
     */
    
    
    /*
     * For client
     */
    // verify credentials
    pt Cx0 = point_add(
        point_mult(G, sk[0]),
        point_mult(H, x0_tilde)
    );
    int result = verify_credentials(messages, U_prime, c, responses, Cx0, iparams);
    printf("Credential Verification: %d\n", result);
    
    
    ///////////////////////////////////////////////////////////////////////////
    // Simulate credential show
    ///////////////////////////////////////////////////////////////////////////
    /*
     * For client
     */
    
    // show credentials
    pt Cm[NUM_OF_MESSAGES];
    pt Ua;
    pt Cua_prime;
    scalar responses_m[NUM_OF_MESSAGES], responses_z[NUM_OF_MESSAGES];
    scalar response_r;
    
    credential_presentation(messages, U_prime, iparams, &Ua, Cm, &Cua_prime, responses_m, responses_z, &response_r, &c);

    
    /*
     * Send data
     */
    
    
    /*
     * For server
     */
    result = verify_credential_presentation(sk, iparams, Ua, Cm, Cua_prime, responses_m, responses_z, response_r, c);
    
    printf("Credential Show Verification: %d\n", result);
    


    //
    // exit
    //
    return 0;
}

