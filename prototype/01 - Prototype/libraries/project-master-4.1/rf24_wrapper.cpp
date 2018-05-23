/********************************************************************************************
 *
 * rf24_wrapper.cpp
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#include "rf24_wrapper.h"


/********************************************************************************************
 *
 * Init
 *
 ********************************************************************************************/
RF24_wrapper::RF24_wrapper(byte RX_addr[6])
{
    // initialise rf24 transceiver
    rf24 = new RF24(CE_PIN, CSN_PIN);
    rf24->begin();
    rf24->openReadingPipe(1, RX_addr);
    rf24->setPALevel(POWER_LV);

    // save variable
    for (int i=0; i<6; i++) {this->RX_addr[i] = RX_addr[i];}
}


/********************************************************************************************
 *
 * Send
 *
 ********************************************************************************************/
bool RF24_wrapper::send(void* load, uint8_t length)
{
    // stop receiver mode
    rf24->stopListening();
    
    // send load
    if(! rf24->write(load, length)){
        return false;
    }

    // get ACK
    rf24->startListening();
    unsigned long started_waiting_at = GET_MICROS();
    bool timeout = false;
    while (! rf24->available()) { 
        if (GET_MICROS() - started_waiting_at > ACK_TIMEOUT) {
            timeout = true; break;
        }
    }
    if (timeout) {
        return false;
    }
    
    // check ACK
    byte ack;
    rf24->read(&ack, sizeof(byte));
    if (ack == ACK) {
        return true;
    }

    // transmission failled
    return false;
}


/********************************************************************************************
 *
 * Receive
 *
 ********************************************************************************************/
bool RF24_wrapper::receive(void* load, uint8_t length)
{
    // start receiver mode
    rf24->startListening(); 

    // check connection
    if (rf24->available()) {
        // get data
        while (rf24->available()) {
            rf24->read(load, length);
        }

        // send ACK
        rf24->stopListening();
        byte ack = ACK;
        rf24->write(&ack, sizeof(byte));

        // acknowldge success
        return true; 
    }
    else {
        return false;
    }
}


/********************************************************************************************
 *
 * Set TX addr
 *
 ********************************************************************************************/
void RF24_wrapper::set_TX(byte TX_addr[6])
{
    rf24->openWritingPipe(TX_addr);
}


/********************************************************************************************
 *
 * Send byte
 *
 ********************************************************************************************/
bool RF24_wrapper::send_byte(byte load)
{
    return this->send(&load, sizeof(byte));
}


/********************************************************************************************
 *
 * Get sync packet
 *
 ********************************************************************************************/
bool RF24_wrapper::get_sync(byte packet[SYNC_LEN])
{
    // start receiver mode
    this->rf24->startListening(); 

    // get sync packet
    if (this->rf24->available()) {
        // get data
        while (this->rf24->available()) {
            this->rf24->read(packet, SYNC_LEN);
        }
        return true;
    }
    else {return false;}
}


/********************************************************************************************
 *
 * Send sync packet
 *
 ********************************************************************************************/
bool RF24_wrapper::send_sync(byte packet)
{
    // stop receiver mode
    this->rf24->stopListening();

    // prepare packet
    byte sync[SYNC_LEN];
    sync[0] = packet; for (int i=0; i<6; i++) {sync[i+1] = this->RX_addr[i];}
    
    // send load
    if( this->rf24->write(&sync, SYNC_LEN) ) {return true;}
    else {return false;}
}


/********************************************************************************************
 *
 * Ping
 *
 ********************************************************************************************/
bool RF24_wrapper::ping()
{
    // ping
    if (! this->send_sync(PING) ){return false;}
    // get pong
    byte ack;
    unsigned long started_waiting_at = GET_MICROS();
    while (GET_MICROS() - started_waiting_at < RX_TIMEOUT) {
        if( this->receive(&ack, sizeof(byte)) ) {
            return (ack == PONG);
        }
    }
    // on error
    return false;
} 

/********************************************************************************************
 *
 * Ping
 *
 ********************************************************************************************/
void RF24_wrapper::pong()
{
    // pong
    this->send_byte(PONG);
} 


/********************************************************************************************
 *
 * Change power level
 *
 ********************************************************************************************/
void RF24_wrapper::change_power_level(byte power_level)
{
    this->rf24->setPALevel(power_level);
} 



/********************************************************************************************
 *
 * Print debugging details
 *
 ********************************************************************************************/
void RF24_wrapper::print_details()
{
	rf24->printDetails();
}