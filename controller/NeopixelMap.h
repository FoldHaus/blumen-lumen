#pragma once

#include "Arduino.h"
#include "MasterConstants.h"


class NeopixelMap {

public:
	NeopixelMap();
	virtual ~NeopixelMap();	
	void init();
	void off();
	void setNeopixel(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b);


private:

};