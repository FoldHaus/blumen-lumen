#include "LEDs.h"

LEDs::LEDs() {

}

LEDs::~LEDs() {

}

void LEDs::init() {
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
}

void LEDs::update() {
	
}