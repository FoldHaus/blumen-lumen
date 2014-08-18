#pragma once

//-- stub for communicating via nRF24L01+. Handles sending & receiving
//-- of messages, message protocol parsing, and buffering

#include "Arduino.h"
<<<<<<< Updated upstream:flower/Radio.h
#include "FlowerConstants.h"
=======
#include "RadioConstants.h"
#include "RF24.h"

>>>>>>> Stashed changes:controller/Radio.h

//-- for nRF24L01
// #include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// #include "printf.h"

class Radio {

public:
	Radio();
	virtual ~Radio();
	void init(uint8_t _addr);	
	void update();
	void receiveMode();
	void transmitMode();
	bool available();
	uint8_t readByte();
	void readBytes();
	bool sendByte(uint8_t _byt);
	void sendMessage(uint8_t data[], uint8_t dataLength);
	ROLE_t getRole();
	void testMode();
	uint8_t* getMessage();

	uint8_t commandMsg[CMD_LENGTH]; 
private:
	bool verifyChecksum(uint8_t arr[], uint8_t arrLength);
	uint8_t generateChecksum( uint8_t arr[], uint8_t arrLength );
	
	uint64_t pipes[2]; 	// Radio pipe addresses for the 2 nodes to communicate, one master, many slaves

	char* role_friendly_name[]; // The debug-friendly names of those roles
	ROLE_t currRole; // The role of the current running sketch

	uint8_t msgIndex;
	bool isMsgProcessed;
	bool isInMsg;

};