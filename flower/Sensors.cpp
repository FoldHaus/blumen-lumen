#include "Sensors.h"

Sensors::Sensors() {

}

Sensors::~Sensors() {

}

void Sensors::init() {
	pinMode(PIR_SIG, INPUT);
	setUltrasoundThreshold(300);
}

void Sensors::update() {

}

bool Sensors::isSomeonePresent() {
	bool pirRead = false;
	bool ultrasonicRead = false;
	// if ( isNight ) {
		//-- use PIR too
		// pirRead = digitalRead( PIR_SIG );
	// } 

	if (pirRead || (analogRead( ULTRASONIC ) < ultrasonicThreshold) ) {
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
		return false;
	}
}

//-- TODO convert [0,1024] to distance in inches.
unsigned int getDistance() {
	return analogRead( ULTRASONIC );
}

