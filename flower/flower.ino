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
// #include "nRF24L01.h"
// #include "RF24.h"
// #include "printf.h"

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
Radio comm;
Lasers lasers;
Sensors sensors;

//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(57600);
	#endif

	motor.init();
	leds.init();
	comm.init(THIS_FLOWER_ID);
	lasers.init();
	sensors.init();

	Serial.println("Key commands");
	Serial.println("\n--Flower motor commands--");
	// Serial.println("toggle modes [m]")
	Serial.println("open [o]");
	Serial.println("close [c]");
	Serial.println("stop [s]");

	Serial.println("\n--Laser commands--");
	Serial.println("toggle lasers [q]");

	Serial.println("\n--LED commands--");
	Serial.println("toggle modes[L]");

}

//-----------------------------------------------
void loop() {

	//-- update calls to each system
	motor.update();
	leds.update();
	comm.update();
	lasers.update();
	sensors.update();

	checkSerialInputs(); //-- check for Serial Monitor input
	// checkIncomingMessages(); //-- check for RF messages


}
//-----------------------------------------------
// void stateMachine() {
// }

//-----------------------------------------------
void checkSerialInputs() {
	static int i = 0;
	static uint8_t lasersOnOff = 0;
	if ( Serial.available() ) {
		int key = Serial.read();

		switch(key) {
			case 'q':
				lasersOnOff++;
				if(lasersOnOff % 3 == 1) {
					Serial.println("> laser on");
					lasers.on();
				} else if (lasersOnOff % 3 == 2) {
					Serial.println("> laser strobe.");
					lasers.startPulsing(100, 60);
				} else {
					Serial.println("> laser off");
					lasers.off();
				}
				break;

			case 'm':
				Serial.println("> toggle motor mode");
				motor.continuallyOpenClose();
				break;
			case 'o':
			case 'O':
				Serial.println("> opening flower.");
				motor.openFlower();
				break;
			case 'c':
			case 'C':
				Serial.println("> closing flower.");
				motor.closeFlower();
				break;
			case 's':
			case 'S':
				Serial.println("> stopping.");
				motor.stop();
				break;

			case 'l':
			case 'L':
				Serial.println("> LEDs on.");
				if (++i%5 == 1){
					leds.setRGB(0,255,0);
				} 
				else if (i%5 == 2){
					leds.setRGB(255,0,0);
				}
				else if (i%5 == 3){
					leds.setRGB(0,0,255);
				}else if (i%5 == 4){
					Serial.println("> LEDs off.");
					leds.off();
				}
				else {
					leds.startRainbow();
				} 
				break;

			default:
				break;
		}
	}
}

void checkIncomingMessages() {
	// if( comm.available() ) {
	// 	comm.getNextMessage();
	// }
}



