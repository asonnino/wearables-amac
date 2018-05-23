/********************************************************************************************
 *
 * Ext. Entity
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *      
 ********************************************************************************************/
 #include "rf24_wrapper.h"
 #include "config.h"
 #include "SHA256.h"
 #include "ecdsa_prototype.h"
 #include "random.h"


/********************************************************************************************
 * Defines
 ********************************************************************************************/
 #define DEBUG     
 
 #define RED_LED_PIN    4
 #define GREEN_LED_PIN  3
 #define BUZZER_PIN     5
 #define LASER_PIN      2
 #define PR_PIN         A4
 
 #define LED_DELAY      200
 #define OPEN_DELAY     3000
 #define PR_THRESHOLD   600

 #define ACTION_CODE    0x1A
 

/********************************************************************************************
 * Variables
 ********************************************************************************************/
 RF24_wrapper*  rf24_wrapper;   // RF wrapped object
 SHA256*        hasher;         // hasher
 byte           sync[SYNC_LEN]; // sync packet
 double         pr_value;       // photoresistor value
 boolean        is_armed;       // alarm statue
 boolean        was_activated;  // whether the alarm was activated
 
 struct signed_action {         // signature structure
  scalar r;
  scalar s;
  byte   c;
} signed_action;
 
 
/********************************************************************************************
 * 
 * Setup
 * 
 ********************************************************************************************/
 void setup() {

    // init console serial communication
    #ifdef DEBUG
       Serial.begin(9600); Serial.println(F(
         " \n"
         " +----------------------------------------------------------------------------+ \n" 
         " |                               EXT. ENTITY                                  | \n"
         " |                                                                            | \n"
         " | @author  Sonnino Alberto                                                   | \n"
         " | @version 4.1                                                               | \n"
         " +----------------------------------------------------------------------------+ \n"
       ));
       Serial.print(F(" >> initializing device................................"));
    #endif
    
    // set PIN mode
    pinMode(RED_LED_PIN, OUTPUT); pinMode(GREEN_LED_PIN, OUTPUT); 
    pinMode(BUZZER_PIN, OUTPUT); pinMode(LASER_PIN, OUTPUT); pinMode(PR_PIN, INPUT);     

    // init alarm system
    arm_system(true);

    // init RF object
    byte TX_addr[6] = SERVER_ADDR;  // TX address
    byte RX_addr[6] = EXT_ENT_ADDR; // RX address
    rf24_wrapper = new RF24_wrapper(RX_addr);
    rf24_wrapper->set_TX(TX_addr);
    
    #ifdef DEBUG
       Serial.println(F("DONE"));
    #endif

} // setup



/********************************************************************************************
 * 
 * Loop
 * 
 ********************************************************************************************/
 void loop() {

      // get photoresistor value
      pr_value = analogRead(PR_PIN);
      
      // intrusion detected
      if ( (pr_value < PR_THRESHOLD) && is_armed ) {
        
          // activate alarm
          activate_alarm();

          #ifdef DEBUG
             Serial.print(F("PR value: ")); Serial.println(pr_value);
          #endif
      }

      // get sync
      if (rf24_wrapper->get_sync(sync)) {
          // check sync
          if (sync[0] == SYNC_EXT_ENTITY) {
              // check addr
              byte received_addr[] = {
                  sync[1], sync[2], sync[3], sync[4], sync[5], sync[6]
              };
              // compare with server addr
              boolean is_server = true;
              byte server_addr[6] = SERVER_ADDR;
              for (int i=0; i<6; i++) {
                  if(received_addr[i] != server_addr[i]) {is_server = false; break;}
              }
              if (is_server) {
                  // send challenge
                  scalar challenge = get_random(1, GROUP_ORDER); // random
                  if (rf24_wrapper->send(&challenge, sizeof(scalar))) {

                      // verify signature
                      if (verify_sign(challenge)) {;

                          // disable system
                          digitalWrite(BUZZER_PIN, HIGH); delay(100); 
                          digitalWrite(BUZZER_PIN, LOW);
                          arm_system(false); delay(OPEN_DELAY); arm_system(true); 
                      }
                  }
              }
          }
      }

 } // end main loop



/********************************************************************************************
 * 
 * Arm system
 * 
 ********************************************************************************************/
 void arm_system(boolean stat) {
     // set init status
     is_armed = stat;
     pr_value  = 2 * PR_THRESHOLD;
     digitalWrite(RED_LED_PIN, stat);
     digitalWrite(GREEN_LED_PIN, (!stat ));
     digitalWrite(LASER_PIN, stat);  
     digitalWrite(BUZZER_PIN, LOW);
 }



 /********************************************************************************************
 * 
 * Arm system
 * 
 ********************************************************************************************/
 void activate_alarm() {
     // print on console
     Serial.println(F(" >> alarm activated !"));
     // place record
     was_activated = true;
     // activate buzzer
     digitalWrite(BUZZER_PIN, HIGH);
     // blink led and laser
     digitalWrite(RED_LED_PIN, LOW); digitalWrite(LASER_PIN, HIGH);
     delay(LED_DELAY);
     digitalWrite(RED_LED_PIN, HIGH); digitalWrite(LASER_PIN, LOW);
     delay(LED_DELAY);
     digitalWrite(RED_LED_PIN, LOW); digitalWrite(LASER_PIN, HIGH);
     delay(LED_DELAY);
     digitalWrite(RED_LED_PIN, HIGH); digitalWrite(LASER_PIN, LOW);
     delay(LED_DELAY);
   
 }



/********************************************************************************************
 * 
 * Arm system
 * 
 ********************************************************************************************/
 boolean verify_sign(scalar challenge)
 {
    boolean result = false;
    unsigned long started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        if (rf24_wrapper->receive(&signed_action, sizeof(signed_action))) {
          
            // check action code
            if (signed_action.c != ACTION_CODE) {return false;}

            // verify signature
            hasher = new SHA256();
            scalar z;
            z = ACTION_CODE; hasher->update(&z,SCALAR_BIT_LEN/8);
            z = challenge;   hasher->update(&z,SCALAR_BIT_LEN/8);
            hasher->finalize(&z, SCALAR_BIT_LEN/8);
            z = z % GROUP_ORDER;

            // verify signature
            result = ecdsa_verify(ACTION_SIGN_PK, z, signed_action.r, signed_action.s);

            // clean up
            hasher->clear(); delete hasher;
        }
    }

    // return
    return result;
 }

