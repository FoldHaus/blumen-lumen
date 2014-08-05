/*-------------------------------------
	Arduino code for flower for Blumen Lumen
	
	Written by: jimmy chion | 08/2014
---------------------------------------*/

//-- FLOWER UNIQUE ID
//--------------------
#define THIS_FLOWER_ID 0x10
// #define THIS_FLOWER_ID 0x20
// #define THIS_FLOWER_ID 0x30
// #define THIS_FLOWER_ID 0x40
// #define THIS_FLOWER_ID 0x50
// #define THIS_FLOWER_ID 0x60
// #define THIS_FLOWER_ID 0x70
// #define THIS_FLOWER_ID 0x80
// #define THIS_FLOWER_ID 0x90
// #define THIS_FLOWER_ID 0xA0
// #define THIS_FLOWER_ID 0xB0

//-- for nRF24L01
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#include "FlowerConstants.h"
#include "LEDs.h"
#include "Lasers.h"
#include "Radio.h"
#include "Motor.h"
#include "Sensors.h"

//------------
#define DEBUG
//------------

//-- this file should just hold the state machine for the flower

Motor motor;
LEDs leds;
Radio radio;
Lasers lasers;
Sensors sensors;

//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(9600);
	#endif

	motor.init();
	leds.init();
	radio.init(THIS_FLOWER_ID);
	lasers.init();
	sensors.init();

}

//-----------------------------------------------
void loop() {
	motor.update();
	leds.update();
	radio.update();
	lasers.update();
	sensors.update();

}

//-----------------------------------------------
