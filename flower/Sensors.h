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
	// bool isSomeonePresent(); //-- uses a combination of PIR and ultrasound
	// void setUltrasoundThreshold();

private:
	// bool readPIR();
	// uint8_t readUltrasound();
};

