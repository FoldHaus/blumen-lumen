#include "Sensors.h"

Sensors::Sensors() {

}

Sensors::~Sensors() {

}

void Sensors::init() {
	pinMode(PIR_SIG, INPUT);
	setUltrasoundThreshold(410); //--104in
	setPresence(false);
}

void Sensors::update() {
}

void Sensors::readUltrasonic() {
	if ( analogRead( ULTRASONIC ) < ultrasonicThreshold ) {
		setPresence(true);
	} else {
		setPresence(false);
	}
}

bool Sensors::isSomeonePresent() {
	// bool pirRead = false;
	bool ultrasonicRead = false;
	// if ( isNight ) {
		//-- use PIR too
		// pirRead = digitalRead( PIR_SIG );
	// } 

	if ((analogRead( ULTRASONIC ) < ultrasonicThreshold) ) {
		return true;
	} else {
		return false;
	}
}

bool Sensors::setUltrasoundThreshold(int _thresh) {
	if( _thresh >= 0 && _thresh < 1024) {
		ultrasonicThreshold = _thresh;
		return true;
	} else {
		ultrasonicThreshold = 0;
		return false;
	}
}

//-- TODO convert [0,1024] to distance in inches.
unsigned int Sensors::getDistance() {
	return analogRead( ULTRASONIC );
}

void Sensors::setPresence(bool _b) {
	presence = _b;
}

bool Sensors::getPresence() {
	return presence;
}