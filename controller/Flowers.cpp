#include "Flowers.h"

//===============================================
//-- LIGHTING ANIMATIONS for Blumen Lumen, 2014
//-- animations the NeoPixel Rings with 10 LEDs
//-- !These are non-blocking animations! 
//-- (i.e. they must be called repeatedly in a non-blocking loop)
//===============================================


//-- handles the RGB LED map made up of Neopixels



Adafruit_NeoPixel birdseyeMap = Adafruit_NeoPixel(N_LEDS, LED_MAP, NEO_GRB + NEO_KHZ800);


Flowers::Flowers() {
}

Flowers::~Flowers() {

}

//-----------------------------------------------
void Flowers::init() {
	pinMode(LED_MAP, OUTPUT);

	comm.init();
	initUltrasonic();

	//off();
	animationMode = ANIMATION_MANUAL;
}

void Flowers::initUltrasonic() {
	for( int i = 0; i < N_ULTRASONIC; i++ ) {
		ultrasonicState[ i ] = false;
		ultrasonicIndexes[ i ] = i*2; // 0, 2, 4 
	}
}

void Flowers::communicateWithFlower(uint8_t flowerNum) {
	comm.switchToPipeTx(flowerNum);
}

void Flowers::turnLasersOn() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_LASER;
	arr[1] = CMD_LASER_ON;
	comm.sendMessage(arr, 2);
}

void Flowers::turnLasersOff() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_LASER;
	arr[1] = CMD_LASER_OFF;
	comm.sendMessage(arr, 2);
}

void Flowers::lasersStrobe() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_LASER;
	arr[1] = CMD_LASER_PULSE;
	comm.sendMessage(arr, 2);
}

void Flowers::openFlower() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_MOTOR;
	arr[1] = CMD_MOTOR_OPEN;
	comm.sendMessage(arr, 2);
}

void Flowers::closeFlower() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_MOTOR;
	arr[1] = CMD_MOTOR_CLOSE;
	comm.sendMessage(arr, 2);
}

void Flowers::stopMotor() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_MOTOR;
	arr[1] = CMD_MOTOR_STOP;
	comm.sendMessage(arr, 2);
}

void Flowers::setRGB(uint8_t r, uint8_t g, uint8_t b) {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_LED_RGB;
	arr[1] = r;
	arr[2] = g;
	arr[3] = b;
	comm.sendMessage(arr, 4);
}

void Flowers::startAnimationDroplets() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_LED;
	arr[1] = CMD_LED_DROPLET;
	comm.sendMessage(arr, 2);
}

void Flowers::startAnimationRainbow() {
	uint8_t arr[5];
	arr[0] = CMD_TYPE_LED;
	arr[1] = CMD_LED_RAINBOW;
	comm.sendMessage(arr, 2);
}

void Flowers::update() {
	switch( animationMode ) {
		case ANIMATION_OFF:
			off();
		break;

		case ANIMATION_RAINBOW:
			doRainbow();
		break;

		case ANIMATION_DROPLET:
			doDroplets();
		break;
	}
}

void Flowers::updateUltrasonicState() {

	for( int curUltrasonic = 0; curUltrasonic < N_ULTRASONIC; curUltrasonic++ ) {
		// Query current ultrasonic
		comm.switchToPipeTx( ultrasonicIndexes[ curUltrasonic ] );

		// Send request
		uint8_t arr[] = { CMD_TYPE_ULT_RQ };
		comm.sendMessage(arr, 1);

		// Switch to listening
		comm.switchToPipeRx(1);
		// //-- wait for response
		unsigned long lastTime = millis();
		Serial.println("Listening for response...");
		while( (millis() - lastTime < ULTRASONIC_REQUEST_TIMEOUT) ) {
			comm.readBytes();
		}

		if( comm.isMsgReady() ) {
			if( comm.getMessage()[2] == 0x00 ) { // no person detected
				ultrasonicState[ curUltrasonic ] = false;
			}
			else {
				ultrasonicState[ curUltrasonic ] = true;
			}
		}

	}	
}

void Flowers::printUltrasonicState() {
	for (uint8_t i = 0; i < N_ULTRASONIC; i++ ) {
		Serial.print("\nultrasound state: ");
		Serial.println(ultrasonicState[i]);
	}
}

//-----------------------------------------------
void  Flowers::off() {
	setColorAll(0,0,0);
	Serial.println("Turning OFF mode all flowers");

	for( uint8_t flowerNum=0; flowerNum < N_FLOWERS; flowerNum++) {

		// Query current flower
		communicateWithFlower( flowerNum );

		// Send request
		uint8_t arr[] = { CMD_TYPE_LED, CMD_LED_OFF };
		comm.sendMessage(arr, 2);

	}
}

//-----------------------------------------------
void Flowers::setFlowerRGB(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b) {
	birdseyeMap.setPixelColor(flowerNum, birdseyeMap.Color(r,g,b));
	birdseyeMap.show();
}

//-- breathe lights from 30-180, slow pulse, easing function
//-----------------------------------------------
void Flowers::breathe(uint8_t flowerNum) {
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
void Flowers::breatheChecker() {
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
void Flowers::comboAnimation() {

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


//-- Start at high intensity then fade out slowly
//-- Make it seem somewhat random which flower comes on when
//-- Try different configs, swapping start and end, other easing algo's etc.
//-----------------------------------------------
void Flowers::doDroplets() {
	static unsigned long startTime = millis();
	static bool hasBeenActivated[] = { false, false, false, false, false, false, false, false, false, false };
	const int startTimes[] = {0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800}; //offset for individual timelines


	for( uint8_t flowerNum=0; flowerNum < birdseyeMap.numPixels(); flowerNum++) {
		if( millis() - startTime > startTimes[ flowerNum ] && !hasBeenActivated[ flowerNum ] ) {
			Serial.print("Enabling DROPLET mode on flower ");
			Serial.println(flowerNum);

			// Query current flower
			comm.switchToPipeTx( 0 );

			// Send request
			uint8_t arr[] = { CMD_TYPE_LED, CMD_LED_DROPLET };
			comm.sendMessage(arr, 2);
		}
	}
	
}


void Flowers::doRainbow() {
	static unsigned long startTime = millis();
	static bool hasBeenActivated[] = { false, false, false, false, false, false, false, false, false, false };
	const uint8_t startTimes[] = {0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800}; //offset for individual timelines


	for( uint8_t flowerNum=0; flowerNum < birdseyeMap.numPixels(); flowerNum++) {
		if( millis() - startTime > startTimes[ flowerNum ] && !hasBeenActivated[ flowerNum ] ) {
			Serial.print("Enabling RAINBOW mode on flower ");
			Serial.println(flowerNum);

			// Query current flower
			comm.switchToPipeTx( flowerNum );

			// Send request
			uint8_t arr[] = { CMD_TYPE_LED, CMD_LED_RAINBOW };
			comm.sendMessage(arr, 2);
		}
	}
}



//-- Easing function, modeled with cos[0,pi]
//-- returns the eased delay time between intervals
//-----------------------------------------------
float Flowers::getEasedDelayTime(int i, int min_i, int max_i) {
	return sin((i - min_i) * 3.1415/2 / (max_i - min_i)); //-- essentially t = map(i, min_i, max_i, 0, 3.1415) 
}

//-- set color for guard NeoPixel ring
//-----------------------------------------------
void Flowers::setColorAll(uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i=0; i < birdseyeMap.numPixels(); i++) {
		birdseyeMap.setPixelColor(i, birdseyeMap.Color(r,g,b));
	}
	birdseyeMap.show(); //-- move this out of the loop to make instantaneous
}
