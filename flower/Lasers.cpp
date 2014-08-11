#include "Lasers.h"

//-- functions to control the LEDs. Takes care of the 60s on 2s off duty cycle.

Lasers::Lasers() {

}

Lasers::~Lasers() {

}

void Lasers::init() {
	pinMode(LASER_SIG, OUTPUT);
	off();
	onStartTime = millis();
	dutyPauseStartTime = 0;
}


void Lasers::update() {
	// -- if it's been on too long, pause for 3 s
	if( !dutyPausing && lasersOn && millis() - onStartTime > LASER_DUTY_ON_TIME ) {
		off();
		dutyPausing = true;
		dutyPauseStartTime = millis();
	}
	//-- and then return to on
	if ( dutyPausing && millis() - dutyPauseStartTime > LASER_DUTY_OFF_TIME ) {
		on();
		dutyPausing = false;
	} 
}

void Lasers::on() {
	digitalWrite(LASER_SIG, HIGH);
	lasersOn = true;
	onStartTime = millis();
}

void Lasers::off() {
	digitalWrite(LASER_SIG, LOW);
	lasersOn = false;
	dutyPausing = false;
}

// void Lasers::pulseAtBPM(unsigned int bpm) {
	
// }
