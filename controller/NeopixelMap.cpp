#include "NeopixelMap.h"

//===============================================
//-- LIGHTING ANIMATIONS for Blumen Lumen, 2014
//-- animations the NeoPixel Rings with 10 LEDs
//-- !These are non-blocking animations! 
//-- (i.e. they must be called repeatedly in a non-blocking loop)
//===============================================


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

//-- breathe alternating lights from 30-180, slow pulse, easing function
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

//-- spinning slowly, spinning quickly, and eventually breath in
//-----------------------------------------------
void NeopixelMap::comboAnimation() {

	const uint8_t cycles = 10;
	const uint8_t startPixel = 0;
	const uint8_t endPixel = N_LEDS;
	const uint8_t nLEDsOn = 4;

	static unsigned int delayTime = 45;
	static bool isDelayDone = false;
	static uint8_t i = startPixel;
	static unsigned long lastTime = millis();
	static uint8_t stage = 0;

	if ( millis()-lastTime > delayTime ) {
		isDelayDone = true;
		lastTime = millis();
	}
	
	if ( isDelayDone ) {
		if(stage == 0) { 	//-- spin slowly
			if(i < endPixel*cycles) {
				//-- pick a random pixel and change the intensity randomly
				if (i < endPixel*cycles) setColorAll(0,0,0);
				for(uint8_t j = 0; j < nLEDsOn; j++ ) {
					birdseyeMap.setPixelColor((i+j)%endPixel, birdseyeMap.Color(0,255-j*50,0));
				}
				birdseyeMap.show();
				i++;	
				isDelayDone = false;
			} else if (i >= endPixel*cycles) {
				delayTime = 15;
				stage = 1;
				i = startPixel;
			}
		}

		else if (stage == 1) { 	//-- spin faster
			if (i < endPixel*cycles) { //-- nonblocking for loop
				if (i < endPixel*(cycles-1)) setColorAll(0,0,0);
				for(uint8_t j = 0; j < nLEDsOn; j++ ) {
					birdseyeMap.setPixelColor((i+j)%endPixel, birdseyeMap.Color(0,255-j*50,0));
				}
				isDelayDone = false;
				i++;
				birdseyeMap.show();
			} else if ( i >= endPixel*cycles ) {
				isDelayDone = false;
				i = startPixel;
				delayTime = 1000;
				stage = 2;
			}
		}
		// else if (stage == 2) { //-- i don't know what this does
		// 	if ( i <= endPixel ) {
		// 		birdseyeMap.setPixelColor(i, birdseyeMap.Color(255,0,0));
		// 		isDelayDone = false;
		// 		i++;
		// 		birdseyeMap.show();
		// 	}
		// }
	}

}

//-----------------------------------------------
//-- Functions added by Tobias
//-- Do not trust
//-----------------------------------------------


//-- Start at high intensity then fade out slowly
//-- Make it seem somewhat random which flower comes on when
//-- Try different configs, swapping start and end, other easing algo's etc.
//-- Note: Requries this Easing library: https://github.com/tobiastoft/ArduinoEasing
//-----------------------------------------------
void NeopixelMap::droplets() {
	const uint8_t startIntensity = 0;
	const uint8_t endIntensity = 255;
	const uint8_t delayTime = 20;
	const uint8_t duration = 300;
	const uint8_t startTimes[] = {0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800}; //offset for individual timelines

	static uint8_t i = 0; //global timeline/playhead
	static uint8_t currentTimes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //individual timeline positions
	static unsigned long lastTime = millis();
	
	if (millis()-lastTime > delayTime){
		for( uint8_t flowerNum=0; flowerNum < birdseyeMap.numPixels(); flowerNum++) {

			//increment individual timelines
			if (i > startTimes[flowerNum]){
				currentTimes[flowerNum]++;

				//fade
				uint8_t r = Easing::easeInBounce(currentTimes[flowerNum], endIntensity, (startIntensity-endIntensity), duration);
				uint8_t g = Easing::easeInOutBounce(currentTimes[flowerNum], endIntensity, (startIntensity-endIntensity), duration);
				uint8_t b = Easing::easeOutBounce(currentTimes[flowerNum], endIntensity, (startIntensity-endIntensity), duration);
				setFlowerRGB(flowerNum, r, g, b);
			}
			
			//reset individual timeline
			if (currentTimes[flowerNum] > duration){
				currentTimes[flowerNum] = 0;
			}
		}

		i++;
		lastTime = millis();
	}
}


//------------- End Tobias edit -----------------


//-- Easing function, modeled with cos[0,pi]
//-- returns the eased delay time between intervals
//-----------------------------------------------
float NeopixelMap::getEasedDelayTime(int i, int min_i, int max_i) {
	return sin((i - min_i) * 3.1415/2 / (max_i - min_i)); //-- essentially t = map(i, min_i, max_i, 0, 3.1415) 
}

//-- set color for guard NeoPixel ring
//-----------------------------------------------
void NeopixelMap::setColorAll(uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i=0; i < birdseyeMap.numPixels(); i++) {
		birdseyeMap.setPixelColor(i, birdseyeMap.Color(r,g,b));
	}
	birdseyeMap.show(); //-- move this out of the loop to make instantaneous
}