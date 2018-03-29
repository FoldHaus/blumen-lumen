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
	doBeatNow = false;
}

void LEDs::update() {
	switch( animationMode ) {
		// case ANIMATION_MANUAL:
		// break;

		// case ANIMATION_OFF:
		// 	off();
		// break;

		case ANIMATION_RAINBOW:
			doRainbow();
		break;

		// case ANIMATION_DROPLET:
		// 	doDroplet();
		// break;

		// case ANIMATION_SLOWFADE:
		// 	doSlowFade();
		// break;

		// case ANIMATION_BEAT:
		// 	doBeat();
		// break;

		// case ANIMATION_LSD:
		// 	doLSD();
		// break;

		default:
			doRainbow();
		break;
	}
}

void LEDs::setAnimationMode( ANIMATION_t mode ) {
	animationMode = mode;

	if( animationMode == ANIMATION_BEAT ) {
		doBeatNow = true; // resets animation
	}
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

void LEDs::doDroplet() {
	const unsigned long duration = 100;
	const unsigned long delayTime = 40;
	static uint8_t startIntensity = 0;
	static uint8_t endIntensity = random(170,250);

	static unsigned long lastTime = millis();
	static int currentTime = 0; //timeline position
	
	if(millis() - lastTime > delayTime) {
		//fade
		uint8_t r = Easing::easeInBounce(currentTime, endIntensity, (startIntensity-endIntensity), duration);
		uint8_t g = Easing::easeInOutBounce(currentTime, endIntensity, (startIntensity-endIntensity), duration);
		uint8_t b = Easing::easeOutBounce(currentTime, endIntensity, (startIntensity-endIntensity), duration);
		setRGB(r, g, b);

		//reset individual timeline
		if (currentTime++ > duration){
			currentTime = 0;
			endIntensity = random(170,250);
		}

		lastTime = millis();
	}

}

void LEDs::doSlowFade() {
	doAnimation(350, 1000);
}

void LEDs::doBeat() {
	const unsigned long duration = 500;

	static uint8_t startIntensity_r = 220 + random(0, 35);
	static uint8_t startIntensity_g = 220 + random(0, 35);
	static uint8_t startIntensity_b = 220 + random(0, 35);
	static uint8_t endIntensity = 0;

	static uint16_t currentTime = 0; //timeline position

	if (currentTime > duration && doBeatNow) {
		doBeatNow = false;
		currentTime = 0;
		startIntensity_r = 220 + random(0, 35);
		startIntensity_g = 220 + random(0, 35);
		startIntensity_b = 220 + random(0, 35);
		endIntensity = 0;
		return;
	}
	else if (currentTime > duration && !doBeatNow) {
		return;
	}

	uint8_t r = Easing::easeInExpo(currentTime, endIntensity, (startIntensity_r-endIntensity), duration);
	uint8_t g = Easing::easeOutExpo(currentTime, endIntensity, (startIntensity_g-endIntensity), duration);
	uint8_t b = Easing::easeInOutExpo(currentTime, endIntensity, (startIntensity_b-endIntensity), duration);
	setRGB(r, g, b);
		
	currentTime++;

}


void LEDs::doLSD() {
	doAnimation(10, 50);
}

void LEDs::doAnimation(int minDuration, int maxDuration) {
	static uint8_t startIntensity_r = random(0, 100);
	static uint8_t startIntensity_g = random(0, 100);
	static uint8_t startIntensity_b = random(0, 100);
	static uint8_t endIntensity_r = 200 + random(-55, 55);
	static uint8_t endIntensity_g = 200 + random(-55, 55);
	static uint8_t endIntensity_b = 200 + random(-55, 55);

	static uint16_t duration = random(minDuration, maxDuration);
	static bool invert = false;

	const unsigned long delayTime = 40;

	static unsigned long lastTime = millis();
	static int currentTime = 0; //timeline position
	
	if(millis() - lastTime > delayTime) {
		uint8_t r, g, b;

		if(invert) {
			r = Easing::easeInExpo(currentTime, startIntensity_r, (endIntensity_r-startIntensity_r), duration);
			g = Easing::easeOutExpo(currentTime, startIntensity_g, (endIntensity_g-startIntensity_g), duration);
			b = Easing::easeInOutExpo(currentTime, startIntensity_b, (endIntensity_b-startIntensity_b), duration);
		}
		else {
			r = Easing::easeInExpo(currentTime, endIntensity_r, (startIntensity_r-endIntensity_r), duration);
			g = Easing::easeOutExpo(currentTime, endIntensity_g, (startIntensity_g-endIntensity_g), duration);
			b = Easing::easeInOutExpo(currentTime, endIntensity_b, (startIntensity_b-endIntensity_b), duration);
		}

		setRGB(r, g, b);
			
		//reset individual timeline
		if (currentTime++ > duration){
			currentTime = 0;
			duration = random(minDuration, maxDuration);

			if(invert) {
				startIntensity_r = endIntensity_r;
				startIntensity_g = endIntensity_g;
				startIntensity_b = endIntensity_b;

				endIntensity_r =  200 + random(-55, 55);
				endIntensity_g =  200 + random(-55, 55);
				endIntensity_b =  200 + random(-55, 55);
			}
			else {
				endIntensity_r = startIntensity_r;
				endIntensity_g = startIntensity_g;
				endIntensity_b = startIntensity_b;

				startIntensity_r = random(0, 100);
				startIntensity_g = random(0, 100);
				startIntensity_b = random(0, 100);
			}

			invert = !invert;
		}

		lastTime = millis();
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


