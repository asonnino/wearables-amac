/********************************************************************************************
 *
 * config.h
 * thesis_project
 * 
 * Author:  Sonnino Alberto
 * Version: 4.1
 *
 ********************************************************************************************/
#ifndef config_h
#define config_h


/********************************************************************************************
 * General
 ********************************************************************************************/
#define NaN            -1
#define SERVER_ADDR    {0x31, 0x4E, 0x6F, 0x64, 0x65, 0xB1}
#define WEARABLE_ADDR  {0x32, 0x4E, 0x6F, 0x64, 0x65, 0xB1}
#define BEACON_ADDR    {0x44, 0x4E, 0xAF, 0x64, 0x65, 0xC1}
#define EXT_ENT_ADDR   {0x9F, 0x12, 0xFA, 0xC4, 0x4D, 0x09}


/********************************************************************************************
 * Attributes
 * 
 * Note: If additional attributes are needed, code adaptations are required :
 *			- in the server methods send_credentials and check_credentials (where indicated)
 *			- in the wearable methods get_credentials and show_credentials (where indicated)
 *
 ********************************************************************************************/
#define ATTRIBUTES 			{0x10, 0x20} // update device's attributes here
#define NUM_OF_MESSAGES		2 			 // update number of attributes here



/********************************************************************************************
 * ECC config.
 ********************************************************************************************/
/* moved in ecc.h from version 2.7 */


/********************************************************************************************
 * ECC Points - prototyping purpose only
 ********************************************************************************************/
#define G 	(new Point(5,1))
#define H  	(new Point(9,16))


/********************************************************************************************
 * Location config. - prototyping purpose only
 ********************************************************************************************/
#define LOCATION_SIGN_PK     (new Point(10,6))
#define LOCATION_CODE_1  	 0x7F
#define LOCATION_CODE_2      0x3B
#define LOCATION_CODE_3      0x90


/********************************************************************************************
 * Ext. action config. - prototyping purpose only
 ********************************************************************************************/
#define ACTION_SIGN_PK       (new Point(13,10))
#define ACTION_CODE_1        0x1A
#define ACTION_CODE_2        0x57
#define ACTION_CODE_3        0x0C


/********************************************************************************************
 * Sync & Ack
 ********************************************************************************************/
#define SYNC_ISSUANCE        	0x10
#define CREDENTIALS_ACCEPTED	0x11
#define CREDENTIALS_REFUSED  	0x12
#define SYNC_SHOWING         	0x13
#define CREDENTIALS_VALID    	0x14
#define CREDENTIALS_INVALID  	0x15
#define SYNC_LOCATION        	0x16
#define SYNC_EXT_ENTITY        	0x17


/********************************************************************************************
 * nRF24 config.
 ********************************************************************************************/
/* moved in rf24_wrapper.h from version 2.7 */


#endif /* config_h */


