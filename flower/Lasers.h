#pragma once

//-- stub for functions for laser animations


#include "Arduino.h"
#include "FlowerConstants.h"


#define LASER_DUTY_ON_TIME 60000
#define LASER_DUTY_OFF_TIME 3000

class Lasers {

public:
	Lasers();
	virtual ~Lasers();
	void init();
	void update(); //-- to be called every cycle in state machine
	void on(); //-- implements duty cycle 60s 2s off
	void off();
	void startPulsing(unsigned int _millisOn = 500, unsigned int _millisOff = 800);

	// void pattern()
	

private:
	void pulse();

	unsigned long onStartTime;
	unsigned long dutyPauseStartTime;
	bool lasersOn;
	bool dutyPausing;

	bool isPulsing;
	unsigned int millisOn;
	unsigned int millisOff;
};

