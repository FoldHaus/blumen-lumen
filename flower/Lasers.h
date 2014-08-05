#pragma once

//-- stub for functions for laser animations


#include "Arduino.h"
#include "FlowerConstants.h"



class Lasers {

public:
	Lasers();
	virtual ~Lasers();
	void init();
	void update(); //-- to be called every cycle in state machine
	// void on(); //-- implements duty cycle 60s 2s off
	// void off();
	// void pulseToMusic();
 	// void setMusicThreshold();

	// void pattern()
	

private:

};

