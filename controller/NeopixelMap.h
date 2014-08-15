#pragma once

#include <Easing.h> //-- Tobias' old easing library
#include <Adafruit_NeoPixel.h> //-- Neopixel LEDs

#include "Arduino.h"
#include "MasterConstants.h"


class NeopixelMap {

public:
	NeopixelMap();
	virtual ~NeopixelMap();	
	void init();
	void off();
	void setFlowerRGB(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b);
	void breathe(uint8_t flowerNum);
	void breatheChecker();
	void setColorAll(uint8_t r, uint8_t g, uint8_t b);
	void comboAnimation();
	//-- Tobias --
	void droplets();

private:
	float getEasedDelayTime(int i, int min_i, int max_i);
};