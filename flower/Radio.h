#pragma once

//-- stub for communicating via nRF24L01+. Handles sending & receiving
//-- of messages, message protocol parsing, and buffering

#include "Arduino.h"
#include "FlowerConstants.h"

//-- for nRF24L01
#include <SPI.h>
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
};