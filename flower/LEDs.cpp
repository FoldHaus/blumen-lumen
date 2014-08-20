#include "LEDs.h"

LEDs::LEDs() {

}

LEDs::~LEDs() {

}

void LEDs::init() {
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
	isRainbowing = false;
}

void LEDs::update() {
	switch( animationMode ) {
		case ANIMATION_MANUAL:
		break;

		case ANIMATION_OFF:
			off();
		break;

		case ANIMATION_RAINBOW:
			doRainbow();
		break;

		case ANIMATION_DROPLET:
			//doDroplet(); //TODO enable
		break;
	}
}

void LEDs::setAnimationMode( ANIMATION_t mode ) {
	animationMode = mode;
}

void LEDs::setRGB(uint8_t r, uint8_t g, uint8_t b) {
	analogWrite(LED_R, r);
	analogWrite(LED_G, g);
	analogWrite(LED_B, b);
}

void LEDs::off() {
	setRGB(0, 0 , 0);
	isRainbowing = false;
}

void LEDs::startRainbow() {
	isRainbowing = true;
}

//-- Input a value 0 to 255 to get a color value.
//-- The colours are a transition r - g - b - back to r.
//-- needs ot be non-blocking
void LEDs::doRainbow() {
	// Constants to define the ranges.

	const int hueRedLow = 0;
	const int hueRedHigh = 255;
	const int hueBlue = 170;

	// The size of the angle of one sector (1/6 of a colour wheel), and of a complete
	// cycle of the colour wheel.

	const int angleMin = 0;
	const int angleSector = 60;
	const int angleMax = 360;

	const int brightMin = 0;
	const int brightMax = 255;

	const int saturation = 255;
	
	static unsigned int r;
	static unsigned int g;
	static unsigned int b;

	const uint8_t startPos = 0;
	const uint8_t endPos = 255;
	const uint8_t delayTime = 40;

	static bool isDelayDone = false;
	static uint8_t wheelPos = startPos;
	static unsigned long lastTime = millis();
	static uint8_t delta = 1;


	if ( millis()-lastTime > delayTime ){
		isDelayDone = true;
		lastTime = millis();
	}

	if ( isDelayDone && wheelPos <= endPos && wheelPos >= startPos ) { //-- nonblocking for loop
        int brightness = 255;
        int hue = map(wheelPos, angleMin, angleMax, hueRedLow, hueRedHigh);
        HSBToRGB(hue, saturation, brightness, &r, &g, &b);
        setRGB(r,g,b);

		wheelPos += delta;
		isDelayDone = false;

	} else if ( wheelPos < startPos ) {
		delta *= -1;
		wheelPos = startPos+1;
		isDelayDone = false;

	} else if ( wheelPos > endPos ) {
		delta *= -1;
		wheelPos = endPos-1;
		isDelayDone = false;

	}

}

//-------------------------------------------------

void LEDs::HSBToRGB(unsigned int inHue, unsigned int inSaturation, unsigned int inBrightness, unsigned int *oR, unsigned int *oG, unsigned int *oB ){
    if (inSaturation == 0) {
        // achromatic (grey)
        *oR = *oG = *oB = inBrightness;
    }
    else {
        unsigned int scaledHue = (inHue * 6);
        unsigned int sector = scaledHue >> 8; // sector 0 to 5 around the color wheel
        unsigned int offsetInSector = scaledHue - (sector << 8);	// position within the sector         
        unsigned int p = (inBrightness * ( 255 - inSaturation )) >> 8;
        unsigned int q = (inBrightness * ( 255 - ((inSaturation * offsetInSector) >> 8) )) >> 8;
        unsigned int t = (inBrightness * ( 255 - ((inSaturation * ( 255 - offsetInSector )) >> 8) )) >> 8;

        switch( sector ) {
        case 0:
            *oR = inBrightness;
            *oG = t;
            *oB = p;
            break;
        case 1:
            *oR = q;
            *oG = inBrightness;
            *oB = p;
            break;
        case 2:
            *oR = p;
            *oG = inBrightness;
            *oB = t;
            break;
        case 3:
            *oR = p;
            *oG = q;
            *oB = inBrightness;
            break;
        case 4:
            *oR = t;
            *oG = p;
            *oB = inBrightness;
            break;
        default:    // case 5:
            *oR = inBrightness;
            *oG = p;
            *oB = q;
            break;
        }
    }
}


