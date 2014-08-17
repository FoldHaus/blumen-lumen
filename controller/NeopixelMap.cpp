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
	flowers[ flowerNum ].setColor(r,g,b);
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

//-- animates lights to the rhythm of the music
//-----------------------------------------------
// void NeopixelMap::toTheBeat(uint16_t musicBpm) {
// 	const uint16_t delayTime = 60 * 1000 / musicBpm ;
// 	static bool flowersOn = true;

// 	flowersOn = !flowersOn;

// 	for( uint8_t flowerNum=0; flowerNum < birdseyeMap.numPixels(); flowerNum++) {
// 		if ( flowersOn ) {
// 			uint8_t color = map( flowerNum, 0, birdseyeMap.numPixels(), 0, 255);
// 			setFlowerRGB( flowerNum, 0, 0, 10 );
// 		}
// 		else {
// 			setFlowerRGB( flowerNum, 0, 0, 0 );	
// 		}
// 	}

// 	Serial.print(delayTime);
// 	Serial.print(" with bpm: ");
// 	Serial.println(musicBpm);

// 	delay(delayTime);
// }

//-- animates lights in response to a beat (happening right now)
//-- take intensity (0-255) as an input
//-----------------------------------------------
void NeopixelMap::musicBeat(bool beatNow, uint8_t intensity) {
	if( beatNow ) Serial.println("Beat now");

	// static unsigned long beatsTimestamps[ARRAY_SIZE]; // array to store timestamps of function call
	// static uint8_t pointer = 0;
	// static bool timestampsArrayFull = false;
	// uint16_t currentBPM = 0;

	// // in case this function hasn't been called in a while (5 seconds) reset everything
	// if( timestampsArrayFull &&
	// 	( beatsTimestamps[pointer] - beatsTimestamps[ (pointer-1)%ARRAY_SIZE ] ) > 5000 ) {
	// 	pointer = 0;
	// 	timestampsArrayFull = false;
	// 	for( uint8_t beat = 0; beat < ARRAY_SIZE; beat++ ) {
	// 		beatsTimestamps[beat] = 0;
	// 	}	
	// }

	// // save timestamp and compute bpm
	// beatsTimestamps[ pointer % ARRAY_SIZE ] = millis();
	// pointer++;
	// if( pointer >= ARRAY_SIZE ){
	// 	timestampsArrayFull = true;
	// 	pointer = 0;
	// }
	// for( uint8_t beat = 0; beat < timestampsArrayFull ? ARRAY_SIZE : pointer; beat++ ) {
	// }

	const unsigned long changeIndicesTimeout = 5000;

	static unsigned long firstCall = 0;
	static unsigned long lastCall = 0;
	static uint8_t flowersToPulsate = 0;
	static bool shouldFlowerPulsate[ N_FLOWERS ];

	// function hasn't been called in a while (5 sec)
	// or hasn't been called at all
	unsigned long now = millis();
	if ( now - lastCall > 5000 || lastCall == 0 ) {  
		Serial.println("Init musicBeat function...");
		lastCall = now;
	}

	// need to refresh flowers to pulsate
	if ( now - firstCall > changeIndicesTimeout || firstCall == 0 ) {
		Serial.println("Refresh flowers... ");
		Serial.print("( ");
		Serial.print(now);
		Serial.print(" ");
		Serial.print(firstCall);
		Serial.println(" )");

		firstCall = now;

		// reset the flowers
		for( uint8_t i = 0; i < N_FLOWERS; i++ ) {
			shouldFlowerPulsate[i] = false;
		}

		// how many flowers to pulsate
		flowersToPulsate = map( intensity, 0, 255, 0, birdseyeMap.numPixels() );
		
		// random two indeces to pulsate
		for(uint8_t i = 0; i < flowersToPulsate; i++ ) {
			uint8_t indexToPulsate = random( 0, N_FLOWERS );
			while ( shouldFlowerPulsate[ indexToPulsate ] == true ) {
				indexToPulsate++;
				indexToPulsate = indexToPulsate % N_FLOWERS;
			}
			shouldFlowerPulsate[ indexToPulsate ] = true;
			Serial.print("To pulsate: ");
			Serial.println(indexToPulsate);
		}
	}

	// update flowers colors
	for(uint8_t flowerNum = 0; flowerNum < N_FLOWERS; flowerNum++ ) {
		uint8_t changeRed, changeGreen, changeBlue;

		if ( shouldFlowerPulsate[ flowerNum ] && beatNow ) {
			changeRed = random( 50, 255 );
			changeGreen = random( 50, 255 );
			changeBlue = random( 50, 255 );
		}
		else {
			changeRed = random( -3, 3 );
			changeGreen = random( -3, 3 );
			changeBlue = random( -3, 3 );
		}
		NeopixelColor currentColor = flowers[ flowerNum ].getColor();
			setFlowerRGB( flowerNum,
						  currentColor.getRed()+changeRed % 255,
						  currentColor.getGreen()+changeGreen % 255,
						  currentColor.getBlue()+changeBlue % 255 );

		Serial.print("Flower #");
		Serial.print(flowerNum);
		Serial.print(": ");
		Serial.print(changeRed);
		Serial.print(" ");
		Serial.print(changeGreen);
		Serial.print(" ");
		Serial.println(changeBlue);
	}

	lastCall = now;
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
					flowers[ (i+j)%endPixel ].setColor(0,255-j*50,0);
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
					birdseyeMap.setPixelColor( (i+j)%endPixel, birdseyeMap.Color( (i+j)%endPixel, (i+j)%endPixel, (i+j)%endPixel ) );
					flowers[ (i+j)%endPixel ].setColor( (i+j)%endPixel, (i+j)%endPixel, (i+j)%endPixel ); //TODO this may not be correct
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
		flowers[ i ].setColor(r,g,b);
	}
	birdseyeMap.show(); //-- move this out of the loop to make instantaneous
}