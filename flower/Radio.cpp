#include "Radio.h"

RF24 radio(9,10);

Radio::Radio() {

}

Radio::~Radio() {

}

void Radio::init(uint8_t _addr) {
	pipes[0] = 0xF0F0F0F0E1LL;
	pipes[1] = 0xF0F0F0F0D2LL;
	role_friendly_name[0] = "receiever";
	role_friendly_name[1] = "transmitter";

	radio.begin();
	receiveMode(); //-- flower defaults to listening, controller to transmitting


	radio.setRetries(15,15); 	// optionally, increase the delay between retries & # of retries

	// radio.printDetails();

	msgIndex = 0;
	isMsgProcessed = true;
	isInMsg = false;

}

void Radio::receiveMode(){
	if(currRole != ROLE_RECEIVER) {
		currRole = ROLE_RECEIVER;
		radio.openWritingPipe(pipes[1]);
		radio.openReadingPipe(1,pipes[0]);
		radio.startListening();
	}
}


void Radio::transmitMode() {
	if(currRole != ROLE_TRANSMITTER) {
		currRole = ROLE_TRANSMITTER;
		radio.openWritingPipe(pipes[0]);
		radio.openReadingPipe(1,pipes[1]);
		radio.stopListening();
	}
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
	transmitMode();
	return radio.write( &_b, sizeof(uint8_t) );
}

ROLE_t Radio::getRole() {
	return currRole;
}

void Radio::sendMessage(uint8_t data[], uint8_t dataLength ) {
	sendByte(CMD_START_BYTE);
	for(uint8_t i = 0; i < dataLength; i++) {
		sendByte(data[i]);
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



void Radio::readBytes() {
//-------------------------------
	static uint8_t bufferArr[CMD_LENGTH]; //-- warning: multiple instances share this
	
	if( available() ) {
	 	uint8_t incomingByte = readByte();
	 	// Serial.println(incomingByte, HEX);
	 	if( incomingByte == CMD_START_BYTE  && !isInMsg ) { //-- start of message
	 		isInMsg = true;
	 		msgIndex = 0;
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
	    		#ifdef DEBUG
	    		Serial.println("------received message-------");
	    		for(int i = 0; i < CMD_LENGTH; ++i) {
			  		Serial.print(commandMsg[i], HEX);
			  		Serial.print(" ");
			  	}
			  	#endif
		    } else {
		    	//-- Throw error somewhere, i think
		    }

	 	}
	}
}




void Radio::update() {


}