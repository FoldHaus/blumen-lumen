#include "Motor.h"

Motor::Motor() {

}

Motor::~Motor() {

}

void Motor::init() {
	pinMode(MOT_IN_1, OUTPUT);
	pinMode(MOT_IN_2, OUTPUT);
}

void Motor::update() {

}