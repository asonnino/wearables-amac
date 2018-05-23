/********************************************************************************************
 *
 * server.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#include "server.h"
#ifdef NRF24_DEBUG
	#include <printf.h> // debug only
#endif


/********************************************************************************************
 *
 * Init
 *
 ********************************************************************************************/
Server::Server(byte RX_addr[6])
{
    // initialise rf24 transceiver
    this->rf24_wrapper = new RF24_wrapper(RX_addr);

    // create message's arrays
    byte tmp[NUM_OF_MESSAGES] = ATTRIBUTES;
    for (int i=0; i<NUM_OF_MESSAGES; i++) {this->messages[i] = tmp[i];};

    // init RNG
    init_random();

    // run MAC_GGM keygen
    MAC_GGM_keygen();

    // run MAC_GGM
    scalar hx;
    MAC_GGM(&hx);

    // verify MAC_GGM
    // Sanity check skipped to spare resources

    // debug
    #ifdef NRF24_DEBUG
    	printf_begin();
    	this->rf24_wrapper->print_details();
    #endif
}


/********************************************************************************************
 *
 * Set TX addr
 *
 ********************************************************************************************/
void Server::set_TX(byte TX_addr[6])
{
    // wrapper
    this->rf24_wrapper->set_TX(TX_addr);
} 

/********************************************************************************************
 *
 * Send
 *
 ********************************************************************************************/
bool Server::send(void* load, uint8_t length)
{
    // wrapper
    return rf24_wrapper->send(load, length);
}


/********************************************************************************************
 *
 * Receive
 *
 ********************************************************************************************/
bool Server::receive(void* load, uint8_t length)
{
    // wrapper
    return rf24_wrapper->receive(load, length); 
}


/********************************************************************************************
 *
 * Get sync packet
 *
 ********************************************************************************************/
bool Server::syncronize(byte packet[SYNC_LEN])
{
    // get sync packet
    if (! this->rf24_wrapper->get_sync(packet) ) {return false;}

    // get received address
    byte TX_addr[] = {packet[1], packet[2], packet[3], packet[4], packet[5], packet[6]};
    this->set_TX(TX_addr);

    // debug
    #ifdef SYNC_DEBUG
        printf("\nServer - received sync packet %02x from: 0x", packet[0]);
        for (int i=0; i<6; i++) {printf("%02x", received_addr[i]);} printf("\n");
    #endif

    return true;
}


/********************************************************************************************
 *
 * Send sync packet
 *
 ********************************************************************************************/
bool Server::send_sync(byte packet)
{
    // wrapper
    return this->rf24_wrapper->send_sync(packet);
}


/********************************************************************************************
 *
 * Send byte
 *
 ********************************************************************************************/
bool Server::send_byte(byte load)
{
    // wrapper
    return this->rf24_wrapper->send_byte(load);
}

/********************************************************************************************
 *
 * Ping
 *
 ********************************************************************************************/
bool Server::ping()
{
    // wrapper
    return this->rf24_wrapper->ping();
} 


/********************************************************************************************
 *
 * Pong 
 *
 ********************************************************************************************/
void Server::pong()
{
    // wrapper
    this->rf24_wrapper->pong();
} 


/********************************************************************************************
 *
 * Send credentials
 *
 * NOTE: Max 32 bytes per structure (see section 3.2 - "Prototype's Implementation").
 *       The code need to be extended if additional attributes have beed added :
 *              - define as many additional data structures 'data_Oxij' as needed
 *              - embbed additional while loops to treat each data packet where indicated
 *
 ********************************************************************************************/
bool Server::send_credentials()
{
	// debug
    #ifdef SPEED_TEST
        printf("\n[SEEP TEST] - send_credentials init: %ld\n", GET_MICROS()); 
    #endif

    // create credentials
    Point* Cx0; scalar c; scalar r[NUM_OF_MESSAGES + 2];
    this->issue_credentials(&Cx0, &c, r);
    
    // send credentials
    if( this->send_sync(SYNC_ISSUANCE) ){

    	// create data structures
    	struct data_0x01 {
  			Point  U;
  			Point  U_prime;
  			Point  iparams[NUM_OF_MESSAGES];
		} data_0x01;
    	struct data_0x02 {
  			Point  Cx0;
  			scalar c;
  			scalar r[NUM_OF_MESSAGES + 2];
		} data_0x02;
        /* 
         *
         * <Add code here to define additional data structures> 
         *
         */


		// fill structures
		data_0x01.U       = *(this->U);
		data_0x01.U_prime = *(this->U_prime);
		for(int i=0; i<NUM_OF_MESSAGES; i++) {data_0x01.iparams[i] = *(this->iparams[i]);}
		data_0x02.Cx0 = *(Cx0);
		data_0x02.c   = c;
		for(int i=0; i<NUM_OF_MESSAGES+2; i++) {data_0x02.r[i] = r[i];}
        /*
         *
         * <Add code here to fill additional data structures>
         *
         */


		// debug
    	#ifdef ISSUE_CRED_DEBUG
			printf("\nServer sending: U    = "); (data_0x01.U).print_point(); 
            printf("\nServer sending: U'   = "); (data_0x01.U_prime).print_point();
            printf("\nServer sending: X[0] = "); (data_0x01.iparams[0]).print_point();
            printf("\nServer sending: X[1] = "); (data_0x01.iparams[1]).print_point();

            printf("\nServer sending: Cx0  = "); (proof_data.Cx0).print_point();
            printf("\nServer sending: c    = %d\n", data_0x02.c);
            printf("\nServer sending: r[0] = %d\n", data_0x02.r[0]);
            printf("\nServer sending: r[1] = %d\n", data_0x02.r[1]);
            printf("\nServer sending: r[2] = %d\n", data_0x02.r[2]);
            printf("\nServer sending: r[3] = %d\n", data_0x02.r[3]);
        #endif

		// send data packet data_0x01
		if ( this->send(&data_0x01, sizeof(data_0x01)) ) {

			// delay
			unsigned long started_waiting_at = GET_MICROS();
    		while (GET_MICROS() - started_waiting_at < TX_TIMEOUT);

    		// send data packet data_0x02
			if( this->send(&data_0x02, sizeof(data_0x02)) ) {


                /*
                 *
                 * <Add code here to send additional data structures>
                 *
                 */


                // free variables
                delete Cx0;

				// get ACK
				byte ack;
				unsigned long started_waiting_at = GET_MICROS();
    			while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        			if( this->receive(&ack, sizeof(byte)) ) {

        				// debug
    					#ifdef SPEED_TEST
        					printf("\n[SEEP TEST] - send_credentials final: %ld\n", GET_MICROS()); 
    					#endif

        				// return
        				return (ack == CREDENTIALS_ACCEPTED);
        			}
        		}
			}
		}
    }
    // on error
    return false;
}


/********************************************************************************************
 *
 * Check credentials
 *
 * NOTE: Max 32 bytes per structure (see section 3.2 - "Prototype's Implementation").
 *       The code need to be extended if additional attributes have beed added :
 *              - define as many additional data structures 'data_Oxij' as needed
 *              - embbed additional while loops to treat each data packet where indicated
 *
 ********************************************************************************************/
bool Server::check_credentials()
{
	// debug
	#ifdef SPEED_TEST
		printf("\n[SEEP TEST] - check_credentials init: %ld\n", GET_MICROS()); 
	#endif

    // time lapsed for timeouts
    unsigned long started_waiting_at;

	// create data structures
    struct data_struct_0x01 { 
        Point  Ua;
        Point  CUa_prime;
        Point  Cm[NUM_OF_MESSAGES];
    }data_0x01;
    struct data_struct_0x02 {
        scalar c;
        scalar s[2 * NUM_OF_MESSAGES + 1];
    }data_0x02;
    /* 
     *
     * <Add code here to define additional data structures> 
     *
     */


    // treat data packet data_0x01
    started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        if (this->receive(&data_0x01, sizeof(data_0x01))) {

            // treat data packet data_0x02
            started_waiting_at = GET_MICROS();
            while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
                if (this->receive(&data_0x02, sizeof(data_0x02))) {  


                    /*
                     *
                     * <Add code here to treat additional data structures>
                     *
                     */


                    // verify credentials
                    bool result = this->verify_credential_presentation(
                        data_0x01.Ua, data_0x01.CUa_prime, data_0x01.Cm, data_0x02.c, data_0x02.s
                    );

                    // debug
                    #ifdef SHOW_CRED_DEBUG
                        printf("\nServer receiving: Ua    = "); (data_0x01.Ua).print_point();  
                        printf("\nServer receiving: CUa'  = "); (data_0x01.CUa_prime).print_point(); 
                        printf("\nServer receiving: Cm[0] = "); (data_0x01.Cm[0]).print_point();    
                        printf("\nServer receiving: Cm[1] = "); (data_0x01.Cm[1]).print_point();   
                        printf("\nServer receiving: c    = %d\n", data_0x02.c);
                        printf("\nServer receiving: s[0] = %d\n", data_0x02.s[0]);
                        printf("\nServer receiving: s[1] = %d\n", data_0x02.s[1]);
                        printf("\nServer receiving: s[2] = %d\n", data_0x02.s[2]);
                        printf("\nServer receiving: s[3] = %d\n", data_0x02.s[3]);
                        printf("\nServer receiving: s[4] = %d\n", data_0x02.s[4]);
                    #endif

                    // debug
                    #ifdef SPEED_TEST
                        printf("\n[SEEP TEST] - check_credentials final: %ld\n", GET_MICROS()); 
                    #endif

                    // return result
                    return result;
                }
            }
        }
    }
    
    // on timeout
    return false;
}


/********************************************************************************************
 *
 * MAC_GGM KeyGen
 *
 ********************************************************************************************/
void Server::MAC_GGM_keygen()
{
    // fill first sk
    this->sk[0] = get_random(1, GROUP_ORDER); // random

    // fill other sk and iparams
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        // secret key
        this->sk[i+1] = get_random(1, GROUP_ORDER); // random
        // iparam
        this->iparams[i] = H; this->iparams[i]->point_mult(sk[i+1]);
    }
}


/********************************************************************************************
 *
 * MAC_GGM
 *
 ********************************************************************************************/
void Server::MAC_GGM(scalar* hx)
{
    // compute function Hx(m)
    *hx = this->sk[0];
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        *hx = (*hx + this->sk[i+1] * this->messages[i]) % GROUP_ORDER;
    }

    // generate a random U (different from O)
    this->U = G;
    scalar r;
    bool b = false;
    while (! b) {
        r = get_random(1, PRIME); // random
        this->U->point_mult(r);       
        b = (! this->U->is_zero()); 
    }
    
    // compute U_prime
    this->U_prime = new Point(this->U); U_prime->point_mult(*hx);
}


/********************************************************************************************
 *
 * MAC_GGM Verify
 *
 ********************************************************************************************/
bool Server::verify_GGM(Point  P, Point  P_prime, scalar hx)
{
    // parse (u,u')
    if (! (P.is_on_curve() && P_prime.is_on_curve()) ) {
        return false;
    }

    // verify u' = u^Hx(m)
    Point* tmp = new Point(&P); tmp->point_mult(hx);
    if (! ((P_prime.getX() == tmp->getX()) && (P_prime.getY() == tmp->getY()))) {
        delete tmp;
        return false;
    }

    // if all contitions hold
    delete tmp;
    return true;
}


/********************************************************************************************
 *
 * Issue credentials
 *
 ********************************************************************************************/
void Server::issue_credentials(Point** Cx0, scalar* c, scalar r[])
{
	// debug
	#ifdef SPEED_TEST
		printf("\n[SEEP TEST] - issue_credentials init: %ld\n", GET_MICROS()); 
	#endif

    // declare tmp variable
    Point* tmp;

    // pick random values wi
    scalar* w = new scalar[NUM_OF_MESSAGES + 2];
    for (int i=0; i<(NUM_OF_MESSAGES+2); i++) {
        w[i] = get_random(1, GROUP_ORDER); // random
    }

    // create witness for U'
    Point* WU_prime = new Point(this->U); WU_prime->point_mult(w[0]);
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        tmp = new Point(this->U); tmp->point_mult( (w[i+1] * this->messages[i]) % GROUP_ORDER );
        WU_prime->point_add(*tmp);
        delete tmp; // free heap
    }

    // create commitement for x0
    scalar x0_tilde = get_random(1, GROUP_ORDER); // random
    (*Cx0) = new Point(G); (*Cx0)->point_mult(this->sk[0]);
    tmp = H; tmp->point_mult(x0_tilde);
    (*Cx0)->point_add(*tmp);
    delete tmp; // free heap

    // create witness for Cx0
    Point* WCx0 = G; WCx0->point_mult(w[0]);
    tmp = H; tmp->point_mult(w[NUM_OF_MESSAGES + 1]);
    WCx0->point_add(*tmp);
    delete tmp; // free heap

    // create withnesses for iparams
    Point* Wiparams[NUM_OF_MESSAGES];
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        Wiparams[i] = H; Wiparams[i]->point_mult(w[i+1]);
    }

    // create challenge
    SHA256* hasher = new SHA256();
    scalar hash;
     
    hash = (*Cx0)->getX();  hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = (*Cx0)->getY();  hasher->update(&hash, SCALAR_BIT_LEN/8); 
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        hash = Wiparams[i]->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
        hash = Wiparams[i]->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    }
    hash = this->U->getX();  hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = this->U->getY();  hasher->update(&hash, SCALAR_BIT_LEN/8);
    hash = WU_prime->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = WU_prime->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = WCx0->getY();     hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = WCx0->getY();     hasher->update(&hash, SCALAR_BIT_LEN/8);

    hasher->finalize(&hash, SCALAR_BIT_LEN/8);
    hash = (hash < 0) ? (hash % GROUP_ORDER + GROUP_ORDER) : (hash % GROUP_ORDER);

    (*c) = hash;


    // free variables
    hasher->clear(); delete hasher;
    delete WU_prime; delete WCx0;
    for (int i=0; i<NUM_OF_MESSAGES; i++) {delete Wiparams[i];}


    // create responses
    for (int i=0; i<NUM_OF_MESSAGES+1; i++){
        r[i] = (w[i] - (*c) * this->sk[i]) % GROUP_ORDER;
        r[i] = (r[i]<0) ? r[i] + GROUP_ORDER : r[i];
    }
    r[NUM_OF_MESSAGES+1] = (w[NUM_OF_MESSAGES+1] - (*c) * x0_tilde) % GROUP_ORDER;
    r[NUM_OF_MESSAGES+1] = 
        (r[NUM_OF_MESSAGES+1]<0) ? r[NUM_OF_MESSAGES+1] + GROUP_ORDER : r[NUM_OF_MESSAGES+1];

    // debug
	#ifdef SPEED_TEST
		printf("\n[SEEP TEST] - issue_credentials final: %ld\n", GET_MICROS()); 
	#endif
}


/********************************************************************************************
 *
 * Verify credential's presentation
 *
 ********************************************************************************************/
bool Server::verify_credential_presentation(Point Ua, Point CUa_prime, Point Cm[], scalar c, scalar s[])
{
	// debug
	#ifdef SPEED_TEST
		printf("\n[SEEP TEST] - verify_credential_presentation init: %ld\n", GET_MICROS()); 
	#endif

	// create tmp variable
    Point* tmp;

	// compute V
	Point* V = new Point(&Ua); V->point_mult(this->sk[0]);
	for (int i=0; i<NUM_OF_MESSAGES; i++){
		tmp = new Point(&(Cm[i])); tmp->point_mult(this->sk[i+1]);
		V->point_add(*tmp);
		delete tmp; // free heap
    }
    tmp = new Point(&CUa_prime); tmp->point_invert();
    V->point_add(*tmp);
    delete tmp; // free heap

    // reconstruct witnesses of Cm
    Point* WCm[NUM_OF_MESSAGES];
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
    	WCm[i] = new Point(&Ua); WCm[i]->point_mult(s[i]); // responses for mi
    	tmp = H; tmp->point_mult(s[NUM_OF_MESSAGES+i]);   // resposnes for zi
    	WCm[i]->point_add(*tmp);
    	delete tmp; // free heap
    	tmp = new Point(&(Cm[i])); tmp->point_mult(c);
    	WCm[i]->point_add(tmp);
    	delete tmp; // free heap
    } 

    // reconstruct witness of V
    Point* WV = G; WV->point_mult(s[2*NUM_OF_MESSAGES]); // response for r
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
    	tmp = new Point(this->iparams[i]); tmp->point_mult(s[NUM_OF_MESSAGES+i]); // resposnes for zi
    	WV->point_add(*tmp);
    	delete tmp; // free heap
    }
    tmp = new Point(V); tmp->point_mult(c);
    WV->point_add(*tmp);
    delete tmp; // free heap


    // reconstruct challenge
    SHA256* hasher = new SHA256();
    scalar hash;
     
    hash = CUa_prime.getX(); hasher->update(&hash, SCALAR_BIT_LEN/8);
    hash = CUa_prime.getY(); hasher->update(&hash, SCALAR_BIT_LEN/8);
    for (int i=0; i<NUM_OF_MESSAGES; i++){
        hash = Cm[i].getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
        hash = Cm[i].getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 

        hash = WCm[i]->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
        hash = WCm[i]->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    }
    hash = WV->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = WV->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 

    hasher->finalize(&hash, SCALAR_BIT_LEN/8);
    hash = (hash < 0) ? (hash % GROUP_ORDER + GROUP_ORDER) : (hash % GROUP_ORDER);

    bool result = ( c == hash );


    // debug
	#ifdef SHOW_CRED_DEBUG
		printf("\nServer computed: WCm[0] = "); WCm[0]->print_point(); 
		printf("\nServer computed: WCm[1] = "); WCm[1]->print_point();   
		printf("\nServer computed: WV     = "); WV->print_point();     
    	printf("\nServer computed: hash   = %d\n", hash);
	#endif


    // free variables
    hasher->clear(); delete hasher;
    delete V; delete WV;
    for (int i=0; i<NUM_OF_MESSAGES; i++) {delete WCm[i];}

    // debug
	#ifdef SPEED_TEST
		printf("\n[SEEP TEST] - verify_credential_presentation final: %ld\n", GET_MICROS()); 
	#endif

    // return
	return result;
}


