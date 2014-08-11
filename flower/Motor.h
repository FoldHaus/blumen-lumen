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
	void openFlower();
	void closeFlower();
	void stop();
	void setOpenRunTime(unsigned long time);
	void setCloseRunTime(unsigned long time);
	bool isFlowerMoving();
	void continuallyOpenClose(); //-- not yet correctly implemented as non-blocking, single-call

private:
	unsigned long timeToOpen;
	unsigned long timeToClose;
	unsigned long timeToPause;
	unsigned long lastTime;
	bool isOpening;
	bool isClosing;
	bool isPausing; //-- transition between opening and closing a flower instantaneously

};