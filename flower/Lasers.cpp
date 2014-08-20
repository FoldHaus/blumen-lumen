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
	isPulsing = false;
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

	if(isPulsing) {
		pulse();
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
	isPulsing = false;
}

void Lasers::startPulsing(unsigned int _millisOn, unsigned int _millisOff) {
	isPulsing = true;
	millisOn = _millisOn;
	millisOff = _millisOff;
}

void Lasers::pulse() {
	static bool inHighState = false;
	static unsigned long lastTime;

	if( inHighState && millis() - lastTime > millisOn) { //-- if on, and time has exceed how long it should be on
		off();
		inHighState = false; //-- switch to off
		lastTime = millis();
	} else if ( !inHighState && millis() - lastTime > millisOff ) { //-- if off, and has been off for _millisOff time
		isPulsing = true;
		on();
		inHighState = true; //-- switch to On
		lastTime = millis();
	}
}

void Lasers::randomize() {
	uint8_t newLaser = random(10); //0–9
	if(newLaser < 5) { // 50% of times
		Serial.println("rand: lasers ON");
		on();
	}
	else if (newLaser >= 5 && newLaser < 7) {
		Serial.println("rand: lasers to strobe");
		startPulsing( random(300, 1000), random(500, 2000));
	}
	else if (newLaser >= 7 && newLaser < 10) {
		Serial.println("rand: lasers OFF");
		off();
	}
}
