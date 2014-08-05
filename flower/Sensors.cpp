#include "Sensors.h"

Sensors::Sensors() {

}

Sensors::~Sensors() {

}

void Sensors::init() {
	pinMode(PIR_SIG, INPUT);
	//-- ultrasound is on analog A0
}

void Sensors::update() {
	
}