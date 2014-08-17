#pragma once

#include "Arduino.h"
#include "MasterConstants.h"

class NeopixelColor {

uint8_t red;
uint8_t green;
uint8_t blue;


public:
	NeopixelColor();
	virtual ~NeopixelColor();	

	NeopixelColor getColor();
	uint8_t getRed();
	uint8_t getGreen();
	uint8_t getBlue();

	void setColor( uint8_t red, uint8_t green, uint8_t blue );
	void setRed( uint8_t red );
	void setGreen( uint8_t green );
	void setBlue( uint8_t blue );

};