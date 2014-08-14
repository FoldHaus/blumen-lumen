#pragma once

//-- stub for communicating via nRF24L01+. Handles sending & receiving
//-- of messages, message protocol parsing, and buffering

#include "Arduino.h"
#include "FlowerConstants.h"

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
	// readBuffer()

private:
	uint8_t thisAddr;

	// Radio pipe addresses for the 2 nodes to communicate.
	uint64_t pipes[2];


	//
	// Role management
	//
	// Set up role.  This sketch uses the same software for all the nodes
	// in this system.  Doing so greatly simplifies testing.  
	//

	// The various roles supported by this sketch
	typedef enum { 
		role_ping_out = 1, 
		role_pong_back 
	} role_e;

	char* role_friendly_name[]; // The debug-friendly names of those roles
	role_e role; // The role of the current running sketch


};