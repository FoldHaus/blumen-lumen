#include "Motor.h"

Motor::Motor() {

}

Motor::~Motor() {

}

void Motor::init() {
	pinMode(MOT_IN_1, OUTPUT);
	pinMode(MOT_IN_2, OUTPUT);
	stop();

	timeToOpen = 33500;
	timeToClose = 33500;
	timeToPause = 300;
	isOpening = false;
	isClosing = false;
	isPausing = false;
	lastTime = millis();
}

void Motor::update() {

	if( !isPausing && isOpening ) {
		digitalWrite( MOT_IN_1, LOW );
		digitalWrite( MOT_IN_2, HIGH );
	} else if ( !isPausing && isClosing ) {
		digitalWrite( MOT_IN_1, HIGH );
		digitalWrite( MOT_IN_2, LOW );
	}

	if (isOpening && millis() - lastTime > timeToOpen ) { //-- open for timeToOpen length of time
		stop();
		isOpening = false;
	} else if ( isClosing && millis() - lastTime > timeToClose ) {
		stop();
		isClosing = false;
	}  else if ( isPausing && millis() - lastTime > timeToPause ) {
		isPausing = false;
	}
}

void Motor::openFlower() {
	if (isClosing) { //-- if in the middle of going opposite direction, give it some pause
		stop();
		isPausing = true;
		isClosing = false;
	}
	isOpening = true;
	lastTime = millis();
}

void Motor::closeFlower() {
	if (isOpening) { //-- if in the middle of going opposite direction, give it some pause
		stop();
		isPausing = true;
		isOpening = false;
	}
	isClosing = true;
	lastTime = millis();
}

void Motor::stop() {
	isOpening = false;
	isClosing = false;
	isPausing = false;
	digitalWrite( MOT_IN_2, LOW );
	digitalWrite( MOT_IN_1, LOW );
}

void Motor::setOpenRunTime( unsigned long _t ) {
	timeToOpen = _t;
}

void Motor::setCloseRunTime( unsigned long _t) {
	timeToClose = _t;
}

bool Motor::isFlowerMoving( ){
	return (isClosing || isOpening || isPausing);
}

void Motor::continuallyOpenClose() {
	// //-- auto open and close with a duty cycle of 50%
	// static uint8_t openOrClosing = 0;
	// if( !motor.isFlowerMoving() ) {
	// 	if (openOrClosing++ % 2 == 0) {
	// 		motor.openFlower();
	// 	} else {
	// 		motor.closeFlower();
	// 	}
	// }
}
