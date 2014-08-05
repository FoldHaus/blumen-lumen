#pragma once

//-- stub for functions for LED animation
//-- design philosophy: any given message should carry enough information to
//-- complete a given animation

#include "Arduino.h"
#include "FlowerConstants.h"


class LEDs {

public:
	LEDs();
	virtual ~LEDs();
	void init();
	void update();
	// void on(); //-- resets to last animation/color state
	// void off();
	// void setRGB(uint8_t r, uint8_t g, uint8_t b);
	// void setHSV(uint8_t h, uint8_t s, uint8_t v);
	// void pulseToMusic();
	// void setMusicThreshold();

	//-- animations (have to be non-blocking)
	// void doRainbow();
	

private:

};