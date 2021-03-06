#include "Radio.h"
#include <EEPROM.h>

<<<<<<< HEAD:controller/Radio.cpp
//-- marco!
=======
>>>>>>> treasure-island-14:flower/Radio.cpp

RF24 radio(9,10);

Radio::Radio() {

}

Radio::~Radio() {

}

void Radio::init() {
	pipes[0] = 0xF0F0F0F0A0LL;
	pipes[1] = 0xF0F0F0F0A1LL;
	pipes[2] = 0xF0F0F0F0A2LL;
	pipes[3] = 0xF0F0F0F0A3LL;
	pipes[4] = 0xF0F0F0F0A4LL;
	pipes[5] = 0xF0F0F0F0A5LL;
	pipes[6] = 0xF0F0F0F0A6LL;
	pipes[7] = 0xF0F0F0F0A7LL;
	pipes[8] = 0xF0F0F0F0A8LL;
	pipes[9] = 0xF0F0F0F0A9LL;
	pipes[10] = 0xF0F0F0F0AALL;
	pipes[11] = 0xF0F0F0F0ABLL;
	pipes[12] = 0xF0F0F0F0ACLL;
	pipes[13] = 0xF0F0F0F0ADLL;
	role_friendly_name[0] = "receiever";
	role_friendly_name[1] = "transmitter";

	radio.begin();
	

	radio.setRetries(15,15); 	// optionally, increase the delay between retries & # of retries

	// radio.printDetails();

	msgIndex = 0;
	isMsgProcessed = true;
	isInMsg = false;
	hasBeenRead = false;

    uint8_t reading = EEPROM.read(EEPROM_ADDR_LOCATION);
    if (reading >= 0 && reading <= 15) {
    	myID = reading;
<<<<<<< HEAD:controller/Radio.cpp
    	Serial.print("ID read: ");
=======
    	Serial.print("ID of this flower: ");
>>>>>>> treasure-island-14:flower/Radio.cpp
    	Serial.println(myID);
    }

    if (myID == ID_MASTER ) {
    	switchToPipeTx(0);
    } else {
    	switchToPipeRx(myID);
    }

}

void Radio::switchToPipeRx(uint8_t flowerNum){
	// if(currRole != ROLE_RECEIVER) {
		if (myID != ID_MASTER) {
			flowerNum = myID;
		}
		currRole = ROLE_RECEIVER;
		radio.openReadingPipe(1,pipes[flowerNum]);
		radio.startListening();
	// }
}


void Radio::switchToPipeTx( uint8_t flowerNum) {
	// if(currRole != ROLE_TRANSMITTER) {
		if (myID != ID_MASTER) {
			flowerNum = myID;
		}
		currRole = ROLE_TRANSMITTER;
		radio.stopListening();
		radio.openWritingPipe(pipes[flowerNum]);
		Serial.print("switched sending to ");
		Serial.println(flowerNum);
	// }
}

bool Radio::available() {
	return radio.available();
}

uint8_t Radio::readByte() {
	uint8_t incoming;
	radio.read( &incoming, sizeof(uint8_t) );
	return incoming;
}

bool Radio::sendByte(uint8_t _b) {
	// transmitMode();
	return radio.write( &_b, sizeof(uint8_t) );
}

ROLE_t Radio::getRole() {
	return currRole;
}

void Radio::sendMessage(uint8_t data[], uint8_t dataLength ) {
	radio.powerUp();
	sendByte(CMD_WAKEUP_BYTE);
	sendByte(CMD_WAKEUP_BYTE);
	sendByte(CMD_START_BYTE);
	for(uint8_t i = 0; i < CMD_LENGTH-3; i++) { // 3 = start + checksum + end
		if( i < dataLength ) {
			sendByte(data[i]);
		}
		else {
			sendByte(0x00);
		}
	}
	sendByte(generateChecksum(data, dataLength+2) ^ CMD_START_BYTE);
	sendByte(CMD_END_BYTE);

}

//-- simple parity checksum
//-- computes the xor of all the bytes up until the checksum byte
uint8_t Radio::generateChecksum( uint8_t arr[], uint8_t arrLength ) {
//-------------------------------
 	uint8_t computedCheckSum = 0x00;
 	for (uint8_t i = 0; i < arrLength - 2; i++ ) {
 		computedCheckSum ^= arr[i];
 	}
 	return computedCheckSum;
}


//-- compares the computed checksum to the received checksum
bool Radio::verifyChecksum( uint8_t arr[], uint8_t arrLength ) {
//-------------------------------
	uint8_t checkSum = generateChecksum( arr, arrLength );
	if (checkSum == arr[ arrLength - 2 ] ) {
		return true;
	} else {
		return false;
	}
}

//-- at this point we know the command message is good
//-- we just need to parse and act on its contents
uint8_t* Radio::getMessage() {
//-------------------------------    
    return commandMsg;
}


bool Radio::isMsgReady() {
	bool valueAtTimeOfCall = hasBeenRead;
	hasBeenRead = false;
	return valueAtTimeOfCall;
}

void Radio::readBytes() {
//-------------------------------
	static uint8_t bufferArr[CMD_LENGTH]; //-- warning: multiple instances share this
	
	if( radio.available() ) {
	 	uint8_t incomingByte = readByte();
	 	// Serial.println(incomingByte, HEX);
	 	//if( incomingByte == CMD_START_BYTE && !isInMsg )
	 	if( incomingByte == CMD_START_BYTE ) { //-- start of message (no matter when received)
	 		isInMsg = true;
	 		msgIndex = 0;
	 		msgStartTimestamp = millis();
	 	} else if ( incomingByte == CMD_END_BYTE && isInMsg ) { //-- end of message
	 		isInMsg = false;
	 		bufferArr[msgIndex] = incomingByte;
	 		isMsgProcessed = false;
	 	}

	 	if ( isInMsg ) { //-- during message
	 		bufferArr[msgIndex] = incomingByte;
	 		msgIndex++;
	 	}

	 	if ( isMsgProcessed == false ) { //-- post-porocess
	 		isMsgProcessed = true;

		    if ( verifyChecksum(bufferArr, CMD_LENGTH) ) { //-- verify using checksum
	    		for(uint8_t i = 0; i < CMD_LENGTH; ++i) { //-- if all good, transfer to public commandMsg
	    			commandMsg[i] = bufferArr[i];
	    		}
		    	hasBeenRead = true;

	    		Serial.print("\n------received message: ");
	    		for(int i = 0; i < CMD_LENGTH; ++i) {
			  		Serial.print(commandMsg[i], HEX);
			  		Serial.print(" ");
			  	}
		    } else {
		    	//-- Throw error somewhere, i think
		    }

	 	}
	}
	else { // nothing has been received
		if( isInMsg && millis() - msgStartTimestamp > CMD_TIMEOUT ) { //timeout
			Serial.println("Communication Timeout");
			isInMsg = false;
	 		msgIndex = 0;
	 		isMsgProcessed = false;
		}
	}
}




void Radio::update() {


}


uint8_t Radio::getID() {
	return myID;
}