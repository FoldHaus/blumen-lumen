#include "NeopixelMap.h"

//-- handles the RGB LED map made up of Neopixels


Adafruit_NeoPixel birdseyeMap = Adafruit_NeoPixel(N_LEDS, LED_MAP, NEO_GRB + NEO_KHZ800);

NeopixelMap::NeopixelMap() {

}

NeopixelMap::~NeopixelMap() {

}

//-----------------------------------------------
void NeopixelMap::init() {
	pinMode(LED_MAP, OUTPUT);
	off();
}

//-----------------------------------------------
void  NeopixelMap::off() {
	setColorAll(0,0,0);
}

//-----------------------------------------------
void NeopixelMap::setFlowerRGB(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b) {
	birdseyeMap.setPixelColor(flowerNum, birdseyeMap.Color(r,g,b));
	birdseyeMap.show();
}

//-- breathe lights from 30-180, slow pulse, easing function
//-----------------------------------------------
void NeopixelMap::breathe(uint8_t flowerNum) {
	const uint8_t startIntensity = 30;
	const uint8_t endIntensity = 180;
	const uint8_t delayTime = 20;

	static bool isDelayDone = false;
	static uint8_t i = startIntensity;
	static unsigned long lastTime = millis();
	static uint8_t delta = 1;

	if ( millis()-lastTime > delayTime - 10 * getEasedDelayTime(i, startIntensity, endIntensity)) {
		isDelayDone = true;
		lastTime = millis();
	}
	
	if ( isDelayDone && i <= endIntensity && i >= startIntensity ) { //-- nonblocking for loop
		setFlowerRGB(flowerNum, i,0,i);
		isDelayDone = false;
		i += delta;
	} else if ( i < startIntensity ) {
		i = startIntensity+1;
		isDelayDone = false;
		delta *= -1;
	} else if ( i > endIntensity ) {
		i = endIntensity-1;
		isDelayDone = false;
		delta *= -1;
	}
}

//-- breathe lights from 30-180, slow pulse, easing function
//-----------------------------------------------
void NeopixelMap::breatheChecker() {
	const uint8_t startIntensity = 30;
	const uint8_t endIntensity = 120;
	const uint8_t delayTime = 20;

	static bool isDelayDone = false;
	static uint8_t i = startIntensity;
	static unsigned long lastTime = millis();
	static uint8_t delta = 1;

	if ( millis()-lastTime > delayTime - 10 * getEasedDelayTime(i, startIntensity, endIntensity)) {
		isDelayDone = true;
		lastTime = millis();
	}
	
	if ( isDelayDone && i <= endIntensity && i >= startIntensity ) { //-- nonblocking for loop
		for( uint8_t flowerNum=0; flowerNum < birdseyeMap.numPixels(); flowerNum++) {
			if (flowerNum % 2 == 0) {
				setFlowerRGB(flowerNum,0,i,i);
			} else {
				setFlowerRGB(flowerNum,endIntensity-i,0,endIntensity-i);
			}
		}
		isDelayDone = false;
		i += delta;
	} else if ( i < startIntensity ) {
		i = startIntensity+1;
		isDelayDone = false;
		delta *= -1;
	} else if ( i > endIntensity ) {
		i = endIntensity-1;
		isDelayDone = false;
		delta *= -1;
	}
}


//-- Easing function, modeled with cos[0,pi]
//-- returns the eased delay time between intervals
//-----------------------------------------------
float NeopixelMap::getEasedDelayTime(int i, int min_i, int max_i) {
	return sin((i - min_i) * 3.1415/2 / (max_i - min_i)); //-- t = map(i, min_i, max_i, 0, 3.1415) 
}

//-- set color for guard NeoPixel ring
//-----------------------------------------------
void NeopixelMap::setColorAll(uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i=0; i < birdseyeMap.numPixels(); i++) {
		birdseyeMap.setPixelColor(i, birdseyeMap.Color(r,g,b));
	}
	birdseyeMap.show(); //-- move this out of the loop to make instantaneous
}