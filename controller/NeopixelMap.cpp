#include "NeopixelMap.h"

//-- handles the RGB LED map made up of Neopixels

NeopixelMap::NeopixelMap() {

}

NeopixelMap::~NeopixelMap() {

}

void NeopixelMap::init() {
	pinMode(LED_MAP, OUTPUT);
}

void  NeopixelMap::off() {

}

void NeopixelMap::setNeopixel(uint8_t flowerNum, uint8_t r, uint8_t g, uint8_t b) {

}
