#include "NeopixelColor.h"



NeopixelColor::NeopixelColor() {

}

NeopixelColor::~NeopixelColor() {

}

//-----------------------------------------------
NeopixelColor NeopixelColor::getColor() {
	return *this;
}

uint8_t NeopixelColor::getRed() {
	return red;
}

uint8_t NeopixelColor::getGreen(){
	return green;
}

uint8_t NeopixelColor::getBlue() {
	return blue;
}

void NeopixelColor::setColor( uint8_t red, uint8_t green, uint8_t blue ) {
	setRed(red);
	setGreen(green);
	setBlue(blue);
}

void NeopixelColor::setRed( uint8_t red ) {
	this->red = red;
}

void NeopixelColor::setGreen( uint8_t green ) {
	this->green = green;
}

void NeopixelColor::setBlue( uint8_t blue ) {
	this->blue = blue;
}
