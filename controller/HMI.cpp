#include "HMI.h"

//-- handles all the button presses and such of the controllers

HMI::HMI() {

}

HMI::~HMI() {

}

void HMI::init() {
	pinMode(OPEN_BTN, INPUT_PULLUP);
	pinMode(CLOSE_BTN, INPUT_PULLUP);
	pinMode(LED_TOGGLE, INPUT_PULLUP);
	pinMode(MODE_TOGGLE, INPUT_PULLUP);
	//-- flower selection is on analog
	pinMode(LED_OUT, OUTPUT);
}
bool HMI::isOpenBtnPushed() {

}

bool HMI::isCloseBtnPushed() {

}

bool HMI::isLEDToggleReleased() {

}

bool HMI::isModeToggleReleased() {

}

uint8_t HMI::getFlowerSelection() {
	
}