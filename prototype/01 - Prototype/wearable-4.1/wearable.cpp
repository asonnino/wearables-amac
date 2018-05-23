/********************************************************************************************
 *
 * wearable.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#include "wearable.h"
#ifdef NRF24_DEBUG
	#include <printf.h> // debug only
#endif

#ifdef MEM_CHECK
/********************************************************************************************
 *
 * Get free dynamic memory - debug only
 *
 ********************************************************************************************/
int freeRam() 
{
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif

/********************************************************************************************
 *
 * Init
 *
 ********************************************************************************************/
Wearable::Wearable(byte RX_addr[6])
{
    // initialise rf24 transceiver
    this->rf24_wrapper = new RF24_wrapper(RX_addr);

    // init variables
    this->U       = new Point();
    this->U_prime = new Point();
    for (int i=0; i<NUM_OF_MESSAGES; i++) {this->iparams[i] = new Point();}

    // create message's arrays
    byte tmp[NUM_OF_MESSAGES] = ATTRIBUTES; // workaround for AVR gcc bug #1378737
    for (int i=0; i<NUM_OF_MESSAGES; i++) {this->messages[i] = tmp[i];};

    // init random generator
    init_random();

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
void Wearable::set_TX(byte TX_addr[6])
{
    // wrapper
    this->rf24_wrapper->set_TX(TX_addr);
} 


/********************************************************************************************
 *
 * Send
 *
 ********************************************************************************************/
bool Wearable::send(void* load, uint8_t length)
{
    // wrapper
    return this->rf24_wrapper->send(load, length);
}


/********************************************************************************************
 *
 * Receive
 *
 ********************************************************************************************/
bool Wearable::receive(void* load, uint8_t length)
{
    // wrapper
    return this->rf24_wrapper->receive(load, length); 
}


/********************************************************************************************
 *
 * Get sync packet
 *
 ********************************************************************************************/
bool Wearable::syncronize(byte packet[SYNC_LEN])
{
    // get sync packet
    if (! this->rf24_wrapper->get_sync(packet)) {return false;}

    // set TX addr
    byte TX_addr[] = {packet[1], packet[2], packet[3], packet[4], packet[5], packet[6]};
    this->set_TX(TX_addr);

    // debug
    #ifdef SYNC_DEBUG
        Serial.print(F("\nWearable - new TX addr set to: 0x"));
        for (int i=0; i<6; i++) {Serial.print(packet[i+1],HEX);} Serial.println();
    #endif

    // return
    return true;
}


/********************************************************************************************
 *
 * Send sync packet
 *
 ********************************************************************************************/
bool Wearable::send_sync(byte packet)
{
    // wrapper
    return this->rf24_wrapper->send_sync(packet);
}


/********************************************************************************************
 *
 * Send byte
 *
 ********************************************************************************************/
bool Wearable::send_byte(byte load)
{
    // wrapper
    return this->rf24_wrapper->send_byte(load);
}


/********************************************************************************************
 *
 * Ping 
 *
 ********************************************************************************************/
bool Wearable::ping()
{
    // wrapper
    return this->rf24_wrapper->ping();
} 

/********************************************************************************************
 *
 * Pong 
 *
 ********************************************************************************************/
void Wearable::pong()
{
    // wrapper
    this->rf24_wrapper->pong();
} 


/********************************************************************************************
 *
 * Challenge beacon
 *
 * NOTE: This code is almost "copy-pasted" from the open-source library Oryx Embedded.
 *		 Some modifications had to be done in order to match the rest of the code and to fit
 *		 in the very limited CPU resources.
 *		 
 * 			Oryx Embedded. 
 * 			TCP/IP Solutions for Embedded Systems. 
 * 			http://www.oryx-embedded.com/\#\&panel1-1. 
 * 			Visited on the 7th of Aug. 2016.
 * 		 
 ********************************************************************************************/
bool Wearable::get_location(byte* location)
{
    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - get_location init: ")); 
        Serial.println(GET_MICROS());
    #endif

    // init structure
    struct signed_location {
        scalar r;
        scalar s;
        byte   c;
    } signed_location;

	// challenge beacon
    scalar challenge = get_random(1, GROUP_ORDER); // random
    if (! this->send(&challenge, sizeof(scalar))) {return false;}

    // get location
    unsigned long started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        if (this->receive(&signed_location, sizeof(signed_location))) {
            // save location
            (*location) = signed_location.c;

            // hash message
            SHA256* hasher = new SHA256();
            scalar z;
            z = (*location); hasher->update(&z,SCALAR_BIT_LEN/8);
            z = challenge;   hasher->update(&z,SCALAR_BIT_LEN/8);
            hasher->finalize(&z, SCALAR_BIT_LEN/8);
            z = z % GROUP_ORDER;

            // verify signature
            bool result = ecdsa_verify(LOCATION_SIGN_PK, z, signed_location.r, signed_location.s);

            // clean up
            hasher->clear(); delete hasher;

            //debug
            #ifdef POS_DEBUG
                Serial.print(F("\nWearable - received location code: 0x"));
                Serial.println(*location, HEX);
                Serial.print(F("\nWearable - received signature: (r = 0x")); 
                Serial.print(signed_location.r, HEX);
                Serial.print(F(", s = 0x")); Serial.print(signed_location.s, HEX); Serial.println(F(")"));
            #endif

            // debug
            #ifdef SPEED_TEST
                Serial.print(F("\n[SEEP TEST] - get_location final: ")); 
                Serial.println(GET_MICROS());
            #endif

            // return
            return result;
        }       
    }

    // timeout
    return false;
}



/********************************************************************************************
 *
 * Get credentials
 *
 * NOTE: Max 32 bytes per structure (see section 6 - "Implementation").
 * 		 The Code need to be extended if additional attributes have beed added :
 *				- define as many additional data structures 'data_Oxij' as needed
 *				- embbed additional while loops to treat each data packet
 *
 ********************************************************************************************/
bool Wearable::get_credentials()
{
    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - get_credentials init: ")); 
        Serial.println(GET_MICROS());
    #endif

    // time lapsed for timeouts
    unsigned long started_waiting_at;

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

    // treat data packet data_0x01
    started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        if( this->receive(&data_0x01, sizeof(data_0x01)) ) {
            // clean previous state
            delete this->U; delete this->U_prime; 
            for (int i=0; i<NUM_OF_MESSAGES; i++) {delete this->iparams[i];}

            // save credentials variables
            this->U       = new Point(data_0x01.U);
            this->U_prime = new Point(data_0x01.U_prime);
            for (int i=0; i<NUM_OF_MESSAGES; i++) {
                this->iparams[i] = new Point(&(data_0x01.iparams)[i]);
            }

            // treat data packet data_0x02
            started_waiting_at = GET_MICROS();
    		while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        		if( this->receive(&data_0x02, sizeof(data_0x02)) ) {            


    				/*
    				 *
    				 * <Add code here to treat additional data structures>
    				 *
    				 */


                    // verify credentials
                    bool result = this->verify_credentials( 
                            &(data_0x02.Cx0), data_0x02.c, data_0x02.r 
                    );

            		// debug
            		#ifdef SPEED_TEST
                		Serial.print(F("\n[SEEP TEST] - get_credentials final: ")); 
                		Serial.println(GET_MICROS());
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
 * Show credentials
 *
 * NOTE: Max 32 bytes per structure (see section 6 - "Implementation").
 * 		 The Code need to be extended if additional attributes have beed added :
 *				- define as many additional data structures 'data_Oxij' as needed
 *				- embbed additional while loops to treat each data packet
 *
 ********************************************************************************************/
bool Wearable::show_credentials()
{
    // debug
    #ifdef MEM_CHECK
        Serial.print(F("\n [MEM_CHECK - Credentials Presentation (intial free dyn mem) : "));
        Serial.print(freeRam()); Serial.println(F("]"));
    #endif

    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - show_credentials init: ")); 
        Serial.println(GET_MICROS());
    #endif

    // credential presentation
    Point* Ua; Point* Cm[NUM_OF_MESSAGES]; Point* CUa_prime; scalar c; scalar s[2*NUM_OF_MESSAGES+1];
    this->credential_presentation(&Ua, Cm, &CUa_prime, &c, s);


    // create data structures
    struct data_struct_0x01 {
        Point  Ua;
        Point  CUa_prime;
        Point  Cm[NUM_OF_MESSAGES];
    } data_0x01;
    struct data_struct_0x02 {
        scalar c;
        scalar s[2 * NUM_OF_MESSAGES + 1];
    } data_0x02;
    /* 
 	 *
 	 * <Add code here to define additional data structures> 
 	 *
 	 */

    // fill structures
    data_0x01.Ua        = *(Ua);
    data_0x01.CUa_prime = *(CUa_prime);
    for(int i=0; i<NUM_OF_MESSAGES; i++) {data_0x01.Cm[i] = *(Cm[i]);}
    data_0x02.c = c;
    for(int i=0; i<(2*NUM_OF_MESSAGES+1); i++) {data_0x02.s[i] = s[i];}
    /*
	 *
	 * <Add code here to fill additional data structures>
	 *
	 */

    // debug
    #ifdef SHOW_CRED_DEBUG
        Serial.print(F("\nClient sending: Ua    = ")); (data_0x01.Ua).print_point();  
        Serial.print(F("\nClient sending: CUa'  = ")); (data_0x01.CUa_prime).print_point();      
        Serial.print(F("\nClient sending: Cm[0] = ")); (data_0x01.Cm[0]).print_point();      
        Serial.print(F("\nClient sending: Cm[1] = ")); (data_0x01.Cm[1]).print_point();      
         
        Serial.print(F("\nClient sending: c     = ")); Serial.println(data_0x02.c);     
        Serial.print(F("\nClient sending: s[0]  = ")); Serial.println(data_0x02.s[0]); 
        Serial.print(F("\nClient sending: s[1]  = ")); Serial.println(data_0x02.s[1]);  
        Serial.print(F("\nClient sending: s[2]  = ")); Serial.println(data_0x02.s[2]);  
        Serial.print(F("\nClient sending: s[3]  = ")); Serial.println(data_0x02.s[3]);  
        Serial.print(F("\nClient sending: s[4]  = ")); Serial.println(data_0x02.s[4]);  
    #endif

    // send data packet data_0x01
    if (this->send(&data_0x01, sizeof(data_0x01))) {

        // delay
        unsigned long started_waiting_at = GET_MICROS();
        while (GET_MICROS() - started_waiting_at < TX_TIMEOUT);

        // send data packet data_0x02
        if (this->send(&data_0x02, sizeof(data_0x02))) {

        	/*
		 	 *
			 * <Add code here to send additional data structures>
			 *
			 */

            // free variables
            delete Ua; delete CUa_prime;
            for (int i=0; i<NUM_OF_MESSAGES; i++) {delete Cm[i];}

            // debug
            #ifdef MEM_CHECK
                Serial.print(F("\n [MEM_CHECK - Credentials Presentation (final  free dyn mem) : "));
                Serial.print(freeRam()); Serial.println(F("]"));
            #endif

            // get ACK
            byte ack;
            unsigned long started_waiting_at = GET_MICROS();
            while (GET_MICROS() - started_waiting_at < (2 * RX_TIMEOUT)) {
                if (this->receive(&ack, sizeof(byte))) {

                    // debug
                    #ifdef SPEED_TEST
                        Serial.print(F("\n[SEEP TEST] - show_credentials final: ")); 
                        Serial.println(GET_MICROS());
                    #endif

                    // return
                    return (ack == CREDENTIALS_VALID);
                }
            }
        }
    }

    // on error
    return false;
}


/********************************************************************************************
 *
 * Verify credential issuance
 *
 * NOTE: This code of the hash function is almost "copy-pasted" from the open-source library 
 *		 WolfSSL. Some modifications had to be done in order to match the rest of the code
 *		 and to fit in the very limited CPU resources.
 *		 
 * 			WolfSSL. 
 *			Embedded SSL Library. 
 *			https://www.wolfssl.com/wolfSSL/Home.html. 
 *			Visited on the 7th of Aug. 2016.
 * 		
 ********************************************************************************************/
bool Wearable::verify_credentials(Point* Cx0, scalar c, scalar r[])
{
    // debug
    #ifdef MEM_CHECK
        Serial.print(F("\n [MEM_CHECK - Verify Credentials (intial free dyn mem) : "));
        Serial.print(freeRam()); Serial.println(F("]"));
    #endif

    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - verify_credentials init: ")); 
        Serial.println(GET_MICROS());
    #endif

    // create tmp variable
    Point* tmp;

    // reconstruct witnesse of U'
    Point* WU_prime = new Point(this->U); WU_prime->point_mult(r[0]);
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        tmp = new Point(this->U);
        tmp->point_mult(r[i+1] * this->messages[i] % GROUP_ORDER);
        WU_prime->point_add(*tmp);
        delete tmp; // free heap
    }
    tmp = new Point(this->U_prime); tmp->point_mult(c);
    WU_prime->point_add(*tmp);
    delete tmp; // free heap

    // reconstruct witness of Cx0
    Point* WCx0 = G; WCx0->point_mult(r[0]);
    tmp = H; tmp->point_mult(r[NUM_OF_MESSAGES+1]);
    WCx0->point_add(*tmp);
    delete tmp; // free heap
    tmp = new Point(Cx0); tmp->point_mult(c);
    WCx0->point_add(*tmp);
    delete tmp; // free heap`

    // reconstruct withnesses of iparams
    Point* Wiparams[NUM_OF_MESSAGES];
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        Wiparams[i] = H; Wiparams[i]->point_mult(r[i+1]);
        tmp = new Point(this->iparams[i]); tmp->point_mult(c);
        Wiparams[i]->point_add(*tmp);
        delete tmp; // free heap
    }

    
    // reconstruct challenge
    SHA256* hasher = new SHA256();
    scalar hash;
     
    hash = Cx0->getX();  hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = Cx0->getY();  hasher->update(&hash, SCALAR_BIT_LEN/8); 
    for(int i=0; i<NUM_OF_MESSAGES; i++){
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

    // check challenge
    bool result = ( c == hash );
 

    // free variables
    hasher->clear(); delete hasher;
    delete WU_prime; delete WCx0; 
    for (int i=0; i<NUM_OF_MESSAGES; i++) {delete Wiparams[i];}


    // debug
    #ifdef MEM_CHECK
        Serial.print(F("\n [MEM_CHECK - Verify Credentials (final  free dyn mem) : "));
        Serial.print(freeRam()); Serial.println(F("]"));
    #endif

    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - verify_credentials final: ")); 
        Serial.println(GET_MICROS());
    #endif

    // return
    return result;
}


/********************************************************************************************
 *
 * Present credentials
 *
 * NOTE: This code of the hash function is almost "copy-pasted" from the open-source library 
 *		 WolfSSL. Some modifications had to be done in order to match the rest of the code
 *		 and to fit in the very limited CPU resources.
 *		 
 * 			WolfSSL. 
 *			Embedded SSL Library. 
 *			https://www.wolfssl.com/wolfSSL/Home.html. 
 *			Visited on the 7th of Aug. 2016.
 * 		
 ********************************************************************************************/
void Wearable::credential_presentation(Point** Ua, Point* Cm[], Point** CUa_prime, scalar* c, scalar s[])
{
    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - credential_presentation init: ")); 
        Serial.println(GET_MICROS());
    #endif

    // create tmp variable
    Point* tmp;

    // re-randomization
    scalar a = get_random(1, GROUP_ORDER); // random
    (*Ua) = new Point(this->U); (*Ua)->point_mult(a);
    
    // compute commitements Cmi and CUa'
    scalar r = get_random(1, GROUP_ORDER); // random
    scalar z[NUM_OF_MESSAGES];
    for (int i=0; i<(NUM_OF_MESSAGES); i++) {
    	z[i] = get_random(1, GROUP_ORDER); // random
    }
    
    for (int i=0; i<(NUM_OF_MESSAGES); i++) {
        Cm[i] = new Point(*Ua); Cm[i]->point_mult(this->messages[i]);
        tmp = H; tmp->point_mult(z[i]);
        Cm[i]->point_add(*tmp);
        delete tmp; // free heap
    }
    
    Point* Ua_prime = new Point(this->U_prime); Ua_prime->point_mult(a);
    (*CUa_prime) = G; 
    (*CUa_prime)->point_mult(r);
    (*CUa_prime)->point_add(*Ua_prime); 
    delete Ua_prime; // free heap

    // pick random values wmi, wzi, wr
    scalar wm[NUM_OF_MESSAGES];
    for (int i=0; i<(NUM_OF_MESSAGES); i++) {
    	wm[i] = get_random(1, GROUP_ORDER); // random
    }
    scalar wz[NUM_OF_MESSAGES];
    for (int i=0; i<(NUM_OF_MESSAGES); i++) {
    	wz[i] = get_random(1, GROUP_ORDER); // random
    }
    scalar wr = get_random(1, GROUP_ORDER); // random

    // create witnesses for Cmi
    Point* WCm[NUM_OF_MESSAGES];
    for (int i=0; i<NUM_OF_MESSAGES; i++){
        WCm[i] = new Point(*Ua); WCm[i]->point_mult(wm[i]);
        tmp = H; tmp->point_mult(wz[i]);
        WCm[i]->point_add(*tmp);
        delete tmp; // free heap
    }

    // create witnesses for V
    Point* WV = G; WV->point_mult(wr);
    for (int i=0; i<(NUM_OF_MESSAGES); i++) {
        tmp = new Point(this->iparams[i]); tmp->point_mult(wz[i]);
        WV->point_add(*tmp);
        delete tmp; // free heap
    }


    // create challenge
    SHA256* hasher = new SHA256();
    scalar hash;
     
    hash = (*CUa_prime)->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8);
    hash = (*CUa_prime)->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8);
    for (int i=0; i<NUM_OF_MESSAGES; i++) {
        hash = Cm[i]->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8);
        hash = Cm[i]->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8);

        hash = WCm[i]->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
        hash = WCm[i]->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    }
    hash = WV->getX(); hasher->update(&hash, SCALAR_BIT_LEN/8); 
    hash = WV->getY(); hasher->update(&hash, SCALAR_BIT_LEN/8); 

    hasher->finalize(&hash, SCALAR_BIT_LEN/8);
    hash = (hash < 0) ? (hash % GROUP_ORDER + GROUP_ORDER) : (hash % GROUP_ORDER);

    (*c) = hash;


    // debug
    #ifdef SHOW_CRED_DEBUG    
        Serial.print(F("\nClient computed: WCm[0] = ")); WCm[0]->print_point();      
        Serial.print(F("\nClient computed: WCm[1] = ")); WCm[1]->print_point();   
        Serial.print(F("\nClient computed: WV     = ")); WV->print_point();   
    #endif


    // free variables
    hasher->clear(); delete hasher; 
    for (int i=0; i<NUM_OF_MESSAGES; i++) {delete WCm[i];}
    delete WV;  

    
    // create responses
    for (int i=0; i<NUM_OF_MESSAGES; i++){
        s[i]                 = (wm[i] - (*c) * this->messages[i]) % GROUP_ORDER;
        s[NUM_OF_MESSAGES+i] = (wz[i] - (*c) * z[i])              % GROUP_ORDER;
        
        s[i]                 = ( s[i] < 0 ) ? s[i] + GROUP_ORDER : s[i];
        s[NUM_OF_MESSAGES+i] = ( s[NUM_OF_MESSAGES+i] < 0 ) ? 
                s[NUM_OF_MESSAGES+i] + GROUP_ORDER : s[NUM_OF_MESSAGES+i];
    }
    s[2*NUM_OF_MESSAGES] = (wr + (*c) * r) % GROUP_ORDER;
    s[2*NUM_OF_MESSAGES] = ( s[2*NUM_OF_MESSAGES] < 0 ) ? 
        s[2*NUM_OF_MESSAGES] + GROUP_ORDER : s[2*NUM_OF_MESSAGES];

    // debug
    #ifdef SPEED_TEST
        Serial.print(F("\n[SEEP TEST] - credential_presentation final: ")); 
        Serial.println(GET_MICROS());
    #endif
}