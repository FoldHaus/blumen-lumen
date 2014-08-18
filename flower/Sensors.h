#pragma once

//-- stub for functions for reading the motion sensors


#include "Arduino.h"
#include "FlowerConstants.h"



class Sensors {

public:
	Sensors();
	virtual ~Sensors();
	void init();
	void update();	
	unsigned int getDistance();
	void readUltrasonic();
	bool isSomeonePresent(); //-- uses a combination of PIR and ultrasound
	bool setUltrasoundThreshold( int _thresh );
	void setPresence(bool _b);
	bool getPresence();


private:
	uint8_t ultrasonicThreshold;
	bool presence;
};

