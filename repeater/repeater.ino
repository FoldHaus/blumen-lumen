/*-------------------------------------
	Arduino code for the repeater for the thing

	Written by: Jimmy | 08/2014
---------------------------------------*/

//------------
#define DEBUG
//------------


//-- for nRF24L01
#include <SPI.h>
#include <EEPROM.h>
#include "RF24.h"
#include "Radio.h"
#include "ControllerConstants.h"

Radio comm;

uint8_t *msg;

//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(57600);
	#endif

	comm.init();

}

//-----------------------------------------------
void loop() {

	//-- to change the EEPROM-saved ID
	// writeEEPROMAddress(); return;


	comm.readBytes();

	if ( comm.isMsgReady() ) {
		msg = comm.getMessage(); //-- store message
		// delay(100); //-- phase it from the master
		for (uint8_t i = 0; i < 10; i++ ) {
			comm.switchToPipeTx(i);
			comm.sendMessage(msg, CMD_LENGTH);	
		}
		comm.switchToPipeRx(13); //-- switch back to receiving
	}
	
}




//-----------------------------------------------
void writeEEPROMAddress() {
	if (Serial.available()){
		char c = Serial.read();
		EEPROM.write(EEPROM_ADDR_LOCATION, c-'0');

		// And we are done right now (no easy way to soft reset)
		Serial.print("\n\rManually reset address to: Press RESET to continue!");
		Serial.println(c);
		while(1) ;
		// }
	}
}
