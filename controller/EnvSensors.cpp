#include "EnvSensors.h"

//-- handles the anemometer, light sensor, and sound detector

EnvSensors::EnvSensors() {

}

EnvSensors::~EnvSensors() {

}

void EnvSensors::init() {
	//-- sound detector, CdS resistor and anemometer on analog

	// TODO set thresholds
	setWindThreshold(180);
	setLightThreshold(800);
}

bool EnvSensors::isTooWindy() {
	static unsigned long timeAtTransition = millis();
	static bool didTransition = false;

	if ( getWindSpeed() > windThreshold ) {
		if( didTransition && millis() - timeAtTransition > 10000 ) {
			didTransition = false;
			return true;
		}
		if( !didTransition ) {
			timeAtTransition = millis();
			didTransition = true;
		}
	} else {
		didTransition = false;
		return false;
	}
	return false;
}

bool EnvSensors::isDaytime() {
	static unsigned long timeAtTransition = millis();
	static bool didTransition = false;
	uint8_t val = getLightValue();
	if ( val > lightThreshold + 100 ) { //-- the less light the lower the value
		if( didTransition && millis() - timeAtTransition > 10000 ) {
			didTransition = false;
			Serial.println(val);
			return true;
		}
		if( !didTransition ) {
			timeAtTransition = millis();
			didTransition = true;
		}
	} else {
		didTransition = false;
		return false;
	}
	return false;
}

bool EnvSensors::isSoundLoud() {
	if ( getSoundAmplitude() > soundThreshold ) {
		return true;
	} else {
		return false;
	}
}


void EnvSensors::setWindThreshold( uint8_t _thresh ) {
	windThreshold = _thresh;
}

void EnvSensors::setLightThreshold( uint8_t _thresh ) {
	lightThreshold = _thresh;
}

void EnvSensors::setSoundThreshold( uint8_t _thresh ) {
	soundThreshold = _thresh;
}

int EnvSensors::getWindSpeed() {
	return analogRead(WIND_SIG);
}

int EnvSensors::getLightValue() {
	return analogRead( CdS_IN );
}

int EnvSensors::getSoundAmplitude() {
	return analogRead( AUDIO_SIG );
}

float EnvSensors::mapAnemometer(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
