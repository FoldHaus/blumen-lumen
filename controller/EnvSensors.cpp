#include "EnvSensors.h"

//-- handles the anemometer, light sensor, and sound detector

EnvSensors::EnvSensors() {

}

EnvSensors::~EnvSensors() {

}

void EnvSensors::init() {
	//-- sound detector, CdS resistor and anemometer on analog

	// TODO set thresholds
}

bool EnvSensors::isTooWindy() {
	if ( getWindSpeed() > windThreshold ) {
		return true;
	} else {
		return false;
	}
}

bool EnvSensors::isDaytime() {
	if ( getLightValue() > lightThreshold ) {
		return true;
	} else {
		return false;
	}
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
	return mapAnemometer ( analogRead(WIND_SIG), MIN_WIND, MAX_WIND, MIN_SPEED, MAX_SPEED );
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
