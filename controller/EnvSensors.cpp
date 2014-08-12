#include "EnvSensors.h"

//-- handles the anemometer, light sensor, and sound detector

EnvSensors::EnvSensors() {

}

EnvSensors::~EnvSensors() {

}

void EnvSensors::init() {
	//-- sound detector, CdS resistor and anemometer on analog
}

bool EnvSensors::isTooWindy() {

}

bool EnvSensors::isDaytime() {

}

bool EnvSensors::isSoundLoud() {

}


void EnvSensors::setWindThreshold( uint8_t _thresh ) {

}

void EnvSensors::setLightThreshold( uint8_t _thresh ) {

}

void EnvSensors::setSoundThreshold( uint8_t _thresh ) {

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
