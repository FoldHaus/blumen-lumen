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

uint8_t EnvSensors::getWindSpeed() {

}

uint8_t EnvSensors::getLightValue() {

}

int EnvSensors::getSoundAmplitude() {


}
