 /********************************************************************************************
 *
 * Arduino project
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
 #include <wearable.h>


/********************************************************************************************
 * Variables
 ********************************************************************************************/ 
 Wearable* wearable;       // wearable object
 byte      sync[SYNC_LEN]; // sync packet



/********************************************************************************************
 * 
 * Setup
 * 
 ********************************************************************************************/
 void setup() {

    // init console serial communication
    Serial.begin(9600); Serial.println(F(
      " \n"
      " +----------------------------------------------------------------------------+ \n" 
      " |                                 WEARABLE                                   | \n"
      " |                                                                            | \n"
      " | @author  Sonnino Alberto                                                   | \n"
      " | @version 4.1                                                               | \n"
      " +----------------------------------------------------------------------------+ \n"
     ));
    Serial.println(F(" >> initializing device"));

    // set RX addr
    Serial.print(F("\t setting device address..............0x"));
    byte RX_addr[6] = WEARABLE_ADDR; // RX address
    for (int i=0; i<6; i++) {Serial.print(RX_addr[i],HEX);} Serial.println();

    // instantiate device
    Serial.print(F("\t init wearable object.........................."));
    wearable = new Wearable(RX_addr);
    Serial.println(F("DONE"));

    // set server addr
    Serial.print(F("\t setting server address..............0x"));
    byte TX_addr[6] = SERVER_ADDR; // TX address
    wearable->set_TX(TX_addr);
    for (int i=0; i<6; i++) {Serial.print(TX_addr[i],HEX);} Serial.println();

    // test server connection
    Serial.print(F("\t testing server connection..............")); 
    if (wearable->ping()) {Serial.println(F(".........OK"));}
    else {Serial.println(F("UNREACHABLE"));}
        
 } // setup



/********************************************************************************************
 * 
 * Loop
 * 
 ********************************************************************************************/
 void loop() {
    
    // test reception
    if (wearable->syncronize(sync)) {
        Serial.print(F(" >> received sync packet 0x")); Serial.println(sync[0],HEX);
       
        // determine action to perform
        switch (sync[0]) {
            case PING: // ping
                Serial.println(F("\t ping received.................................STOP"));
                wearable->pong();
                break;

            case SYNC_LOCATION: // getting location
                 Serial.print(F("\t new location received..................."));
                 byte location; 
                 if (wearable->get_location(&location)) {
                     Serial.print(F("CODE: 0x")); Serial.println(location,HEX);
                     // perform action according to location code
                     perform_action(location);
                 }
                 else{
                     Serial.println(F("...REFUSED"));
                 }
                 break;
                 
            case SYNC_ISSUANCE: // credential issuance
                 Serial.print(F("\t new credentials received.................."));
                 if (wearable->get_credentials()) {
                     wearable->send_byte(CREDENTIALS_ACCEPTED);
                     Serial.println(F("ACCEPTED"));

                     // test received credentials
                     Serial.print(F("\t testing new credentials.................."));
                     if (wearable->show_credentials()) {
                             Serial.println(F("SUCCEEDED"));                  
                     }
                     else {
                             Serial.println(F("...FAILED"));    
                     }
                 }
                 else {
                     wearable->send_byte(CREDENTIALS_REFUSED);
                     Serial.println(F(".REFUSED"));
                 }
                 break;

             default:
                 Serial.println(F("\t unknown command.............................IGNORE"));
        }        
    }
 
 } // end of main loop


/********************************************************************************************
 * 
 * Perform action according to location code
 * 
 ********************************************************************************************/
 void perform_action(byte location)
 {     
     // analyse location code     
     switch (location) {

         // identify location code
         case 0x7F:
              // show credentials
              Serial.print(F("\t showing credentials......................"));
              if (wearable->show_credentials()){
                  Serial.println(F("SUCCEEDED"));                  
              }
              else {
                  Serial.println(F("...FAILED"));    
              }
             break;
         /* 
          *
          * <Add code here to treat other location codes> 
          *
          */


         // default
         default:
             Serial.println(F("\t unknown command.............................IGNORE"));                 
     }
     
 } // end perform_action
 

