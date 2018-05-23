/********************************************************************************************
 *
 * main.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#include "server.h"


/********************************************************************************************
 * Declarations
 ********************************************************************************************/
#define PRIVATE_KEY    0x0B  // prototyping only


/********************************************************************************************
 * Declarations
 ********************************************************************************************/
Server* server;  // server object
SHA256* hasher;  // hasher object
void contact_entity(byte action);
struct signed_action {
  scalar r;
  scalar s;
  byte   c;
} signed_action;



/********************************************************************************************
 * Main method
 ********************************************************************************************/
int main()
{
	printf(
		" \n"
      	" +----------------------------------------------------------------------------+ \n" 
      	" |                                  SERVER                                    | \n"
      	" |                                                                            | \n"
      	" | @author  Sonnino Alberto                                                   | \n"
      	" | @version 4.1                                                               | \n"
      	" +----------------------------------------------------------------------------+ \n"
      	"\n"
    );

  printf(" >> initializing device\n");

  // init sync packet
	byte sync[SYNC_LEN]; // sync packet

	// set RX addr
	printf("\t setting device address..............0x");
	byte RX_addr[6] = SERVER_ADDR;   // RX address
	for (int i=0; i<6; i++) {printf("%02x", RX_addr[i]);} printf("\n");

 	// init server object
	printf("\t init server object............................");
	server = new Server(RX_addr); 
	printf("DONE\n");

	// set wearable addr
	printf("\t setting wearable address............0x");
	byte TX_addr[6] = WEARABLE_ADDR; // TX address
	for (int i=0; i<6; i++) {printf("%02x", TX_addr[i]);} printf("\n");
	server->set_TX(TX_addr);
	
	// credential issuance
  // 
  // NOTE: This should be triggered by the web interface
  //
	printf(" >> credentials issuance...........................");
	if ( server->send_credentials() ) {
		printf("ACCEPTED\n");
	}
	else {
		printf(".REFUSED\n");
	}



	/************************************************************************************
	 * Main loop                                                                        
	 ************************************************************************************/
	while(true){

    	byte ack  = NaN;
    	if( server->syncronize(sync) ) {
        	printf(" >> received sync packet 0x%02x\n", sync[0]);
      
        	// determine action to perform
        	switch (sync[0]) {
        		case PING: // ping
        			printf("\t ping received.................................STOP\n");
        			server->pong();
        			break;

            	case SYNC_SHOWING: // credential prensentation
                	printf("\t showing credentials.......................");
                	if ( server->check_credentials() ) {
                    	server->send_byte(CREDENTIALS_VALID);
                    	printf("ACCEPTED\n");
                      printf("\t contacting external entity I..................");
                      contact_entity(ACTION_CODE_1); // request for action 1
                      printf("DONE\n");               
                 	}
                 	else {
                    	server->send_byte(CREDENTIALS_INVALID);
                    	printf(".REFUSED\n");
                 	}
                 	break;

                default:
                	printf("\t unknown command...............................STOP\n");
        	}        	
    	}

	} // end of main loop



  // unreachable statement
	return 0; 
}



/********************************************************************************************
 * Contact ext. entity
 ********************************************************************************************/
void contact_entity(byte action)
{
  // set ext. entity addr
  byte ext_ent_addr[6] = EXT_ENT_ADDR; server->set_TX(ext_ent_addr);

  // send sync
  server->send_sync(SYNC_EXT_ENTITY);

  // wait for challenge
  scalar challenge;
  unsigned long started_waiting_at = GET_MICROS();
  while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
    if( server->receive(&challenge, sizeof(scalar)) ) {

      // create hash
      hasher = new SHA256();
      scalar z = action; hasher->update(&z,SCALAR_BIT_LEN/8);
      z = challenge;     hasher->update(&z,SCALAR_BIT_LEN/8);
      hasher->finalize(&z, SCALAR_BIT_LEN/8);
      z = z % GROUP_ORDER;

      // clean up
      hasher->clear(); delete hasher;
    
      // create sigature    
      ecdsa_sign(PRIVATE_KEY, z, &(signed_action.r), &(signed_action.s));

      // send signature
      server->send(&signed_action, sizeof(signed_action)); 
    }
  }
}
