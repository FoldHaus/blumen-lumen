#pragma once

#include "Arduino.h"
#include "MasterConstants.h"


class EnvSensors {

public:
	EnvSensors();
	virtual ~EnvSensors();
	void init();
	bool isTooWindy();
	bool isDaytime();
	bool isSoundLoud();

	void setWindThreshold(uint8_t _thresh);
	void setLightThreshold(uint8_t _thresh);
	void setSoundThreshold(uint8_t _thresh);

private:
	uint8_t getWindSpeed(); //-- maybe uint16_t?
	uint8_t getLightValue();
	int getSoundAmplitude();

};