#pragma once

//-- stub for communicating via nRF24L01+. Handles sending & receiving
//-- of messages, message protocol parsing, and buffering

#include "Arduino.h"
#include "RadioConstants.h"
#include "RF24.h"


//-- for nRF24L01
// #include <SPI.h>
// #include "../RF24/nRF24L01.h"
// #include "printf.h"

class Radio {

public:
	Radio();
	virtual ~Radio();
	void init();
	void update();
	void switchToPipeRx(uint8_t flowerNum = ID_MASTER);
	void switchToPipeTx(uint8_t flowerNum = ID_MASTER);
	bool available();
	uint8_t readByte();
	void readBytes();
	bool sendByte(uint8_t _byt);
	void sendMessage(uint8_t data[], uint8_t dataLength);
	ROLE_t getRole();
	void testMode();
	uint8_t* getMessage();
	bool isMsgReady();

	uint8_t commandMsg[CMD_LENGTH]; 
private:
	bool verifyChecksum(uint8_t arr[], uint8_t arrLength);
	uint8_t generateChecksum( uint8_t arr[], uint8_t arrLength );
	
	uint64_t pipes[14]; 	// Radio pipe addresses for the 2 nodes to communicate, one master, many slaves

	uint8_t myID;


	ROLE_t currRole; // The role of the current running sketch

	bool hasBeenRead;
	unsigned long msgStartTimestamp;

};