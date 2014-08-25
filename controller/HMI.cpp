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

//-- continually returns true if pushed, not just the first time.
//--------------------------------------
bool HMI::isOpenBtnPushed() {
	static unsigned long lastDebounceTime;
	static bool lastVal = HIGH;
	static bool isPushed = false; //-- commented out lines are if you want this to return a push only once
	bool currVal = digitalRead(OPEN_BTN);
    if( currVal == LOW && lastVal == HIGH ) {
    	isPushed = false;
    	lastDebounceTime = millis();
    }

    lastVal = currVal;
    
    if ( currVal == LOW && millis() - lastDebounceTime > DEBOUNCE_DELAY ) {//&& !isPushed ) {
    	isPushed = true;
    	return true;
    }
    return false;
}

//--------------------------------------
bool HMI::isCloseBtnPushed() {
	static unsigned long lastDebounceTime;
	static bool lastVal = HIGH;
    static bool isPushed = false; //-- commented out lines are if you want this to return a push only once
	bool currVal = digitalRead(CLOSE_BTN);
    if( currVal == LOW && lastVal == HIGH ) {
        isPushed = false;
    	lastDebounceTime = millis();
    }

    lastVal = currVal;
    
    if ( currVal == LOW && millis() - lastDebounceTime > DEBOUNCE_DELAY ) {//&& !isPushed ) {
    	return true;
    }
    return false;
}

//--------------------------------------
//-- if pushed, only returns true once, until released and pushed again.
bool HMI::isLEDTogglePushed() {
	static unsigned long lastDebounceTime;
	static bool lastVal = HIGH;
	static bool isPushed = false; //-- commented out lines are if you want this to return a push only once
	bool currVal = digitalRead(LED_TOGGLE);
    if( currVal == LOW && lastVal == HIGH ) {
    	isPushed = false;
    	lastDebounceTime = millis();
    }

    lastVal = currVal;
    
    if ( currVal == LOW && millis() - lastDebounceTime > DEBOUNCE_DELAY && !isPushed ) {
    	isPushed = true;
    	return true;
    }
    return false;
}

//-----------------------------------------------
//-- if pushed, only returns true once, until released and pushed again.
bool HMI::isModeTogglePushed() {
	static unsigned long lastDebounceTime;
	static bool lastVal = HIGH;
	static bool isPushed = false; //-- commented out lines are if you want this to return a push only once
	bool currVal = digitalRead(MODE_TOGGLE);
    if( currVal == LOW && lastVal == HIGH ) {
    	isPushed = false;
    	lastDebounceTime = millis();
    }

    lastVal = currVal;
    
    if ( currVal == LOW && millis() - lastDebounceTime > DEBOUNCE_DELAY && !isPushed ) {
    	isPushed = true;
    	return true;
    }
    return false;
}

//-----------------------------------------------
int HMI::getFlowerSelection() {
    int val = analogRead(ROTARY_SELECT);
    if ( val >= ROTARY_SELECT_1 - ANALOG_MARGIN ) return 1;
    if ( val >= ROTARY_SELECT_2 - ANALOG_MARGIN && val <= ROTARY_SELECT_2 + ANALOG_MARGIN) return 2;
    if ( val >= ROTARY_SELECT_3 - ANALOG_MARGIN && val <= ROTARY_SELECT_3 + ANALOG_MARGIN) return 3;
    if ( val >= ROTARY_SELECT_4 - ANALOG_MARGIN && val <= ROTARY_SELECT_4 + ANALOG_MARGIN) return 4;
    if ( val >= ROTARY_SELECT_5 - ANALOG_MARGIN && val <= ROTARY_SELECT_5 + ANALOG_MARGIN) return 5;
    if ( val >= ROTARY_SELECT_6 - ANALOG_MARGIN && val <= ROTARY_SELECT_6 + ANALOG_MARGIN) return 6;
    if ( val >= ROTARY_SELECT_7 - ANALOG_MARGIN && val <= ROTARY_SELECT_7 + ANALOG_MARGIN) return 7;
    if ( val >= ROTARY_SELECT_8 - ANALOG_MARGIN && val <= ROTARY_SELECT_8 + ANALOG_MARGIN) return 8;
    if ( val >= ROTARY_SELECT_9 - ANALOG_MARGIN && val <= ROTARY_SELECT_9 + ANALOG_MARGIN) return 9;
    if ( val >= ROTARY_SELECT_10 - ANALOG_MARGIN && val <= ROTARY_SELECT_10 + ANALOG_MARGIN) return 10;
    if ( val >= ROTARY_SELECT_11 - ANALOG_MARGIN && val <= ROTARY_SELECT_11 + ANALOG_MARGIN) return 11;
    if ( val <= ROTARY_SELECT_12 + ANALOG_MARGIN) return 12;
    return 0;

}