#pragma once

#include <Easing.h> //-- Tobias' old easing library
#include <Adafruit_NeoPixel.h> //-- Neopixel LEDs

#include "Arduino.h"
#include "ControllerConstants.h"
#include "Radio.h"
#include "RadioConstants.h"


class Flowers {

Radio comm;
ANIMATION_t animationMode;
bool ultrasonicState[ N_ULTRASONIC ];
uint8_t ultrasonicIndexes[ N_ULTRASONIC ];


public:
	Flowers();
	virtual ~Flowers();	
	void init();
	void update();
	void off();
	void allOff();

	// commands
	void communicateWithFlower(uint8_t flowerNum);
	void turnLasersOn();
	void turnLasersOff();
	void lasersStrobe();
	void openFlower();
	void closeFlower();
	void stopMotor();
	void setRGB(uint8_t r, uint8_t g, uint8_t b);
	void setFlowerRGB(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b);

	// ultrasonic
	void updateUltrasonicState();
	void printUltrasonicState();

	// animations
	void breathe(uint8_t flowerNum);
	void breatheChecker();
	void setColorAll(uint8_t r, uint8_t g, uint8_t b);
	void comboAnimation();
	void startAnimationRainbow();
	void startAnimationDroplets();
	void startAnimationSlowFade();
	void startAnimationBeat();
	void startAnimationLSD();
	void doDroplets();
	void doRainbow();
	void doSlowFade();
	void doBeat();
	void doLSD();
	void doAnimationOnAllFlowers(ANIMATION_t anim);

private:
	float getEasedDelayTime(int i, int min_i, int max_i);
	void initUltrasonic();
};