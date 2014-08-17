#pragma once

#include <Adafruit_NeoPixel.h> //-- Neopixel LEDs

#include "Arduino.h"
#include "MasterConstants.h"
#include "NeopixelColor.h"


class NeopixelMap {

	NeopixelColor flowers[N_FLOWERS];

public:
	NeopixelMap();
	virtual ~NeopixelMap();	
	void init();
	void off();
	void setFlowerRGB(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b);
	NeopixelColor getFlowerRGB(uint8_t flowerNum);
	void breathe(uint8_t flowerNum);
	void musicBeat(bool beatNow, uint8_t intensity);
	void breatheChecker();
	void setColorAll(uint8_t r, uint8_t g, uint8_t b);
	void comboAnimation();

private:
	float getEasedDelayTime(int i, int min_i, int max_i);
};