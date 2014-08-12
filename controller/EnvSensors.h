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

	int getWindSpeed(); //-- maybe uint16_t?
	int getLightValue();
	int getSoundAmplitude();
private:
	float mapAnemometer(float x, float in_min, float in_max, float out_min, float out_max);

};