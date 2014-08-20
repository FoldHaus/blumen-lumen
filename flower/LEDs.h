#pragma once

//-- stub for functions for LED animation
//-- design philosophy: any given message should carry enough information to
//-- complete a given animation

#include "Arduino.h"
#include "RadioConstants.h"
#include "FlowerConstants.h"
#include <Easing.h>


class LEDs {

ANIMATION_t animationMode;

public:
	LEDs();
	virtual ~LEDs();
	void init();
	void update();
	void setAnimationMode( ANIMATION_t mode );
	// void on(); //-- resets to last animation/color state
	void off();
	void setRGB(uint8_t r, uint8_t g, uint8_t b);
	// void setHSV(uint8_t h, uint8_t s, uint8_t v);
	// void pulseToMusic();
	// void setMusicThreshold();

	//-- animations (have to be non-blocking)
	void doRainbow();
	void startRainbow();
	void breathe();
	void doDroplet( int delay = DEFAULT_ANIM_DELAY, int duration = DEFAULT_ANIM_DURATION );
	

private:
	bool isRainbowing;
	bool doBeatNow;
	void HSBToRGB(unsigned int inHue, unsigned int inSaturation, unsigned int inBrightness, unsigned int *oR, unsigned int *oG, unsigned int *oB );


};