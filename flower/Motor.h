#pragma once

//-- stub for running the 18" linear actuator in Blumen Lumen

#include "Arduino.h"
#include "FlowerConstants.h"


class Motor {

public:
	Motor();
	virtual ~Motor();
	void init();
	void update();
	// void openFlower();
	// void closeFlower();
	// void stop();
	// void setOpenRunTime(uint8_t time);
	// void setCloseRunTime(uint8_t time)

private:
};