/*-------------------------------------
	Arduino code for flower for Blumen Lumen
	
	Written by: jimmy chion | 08/2014
---------------------------------------*/

//-- for nRF24L01
#include <SPI.h>
#include <EEPROM.h>

// #include "nRF24L01.h"
#include "RF24.h"
// #include "printf.h"

#include <Easing.h>

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
	comm.init();
	lasers.init();
	sensors.init();
	

}

//-----------------------------------------------
void loop() {

	//-- update calls to each system
	motor.update();
	leds.update();
	comm.update();
	lasers.update();
	sensors.update();

	if ( comm.getRole() == ROLE_RECEIVER ) {
		checkIncomingMessages(); //-- check for RF messagee
	} 

}
//-----------------------------------------------
// void stateMachine() {
// }
//-----------------------------------------------


void checkIncomingMessages() {
//-----------------------------------------------
	comm.readBytes();
	if ( comm.isMsgReady() ){
		parseMessage();
	}
}

void parseMessage() {
//-----------------------------------------------
	//-- ignore bytes 0, N-1, and N-2 (Start byte, checksum, end byte)

	// -- byte 1: see if it's meant for me or a broadcast
	uint8_t data0 = comm.commandMsg[2];
	uint8_t data1 = comm.commandMsg[3];
	uint8_t data2 = comm.commandMsg[4];
	uint8_t data3 = comm.commandMsg[5];
	switch (comm.commandMsg[1]) {
		case CMD_TYPE_ULT_RQ:
			respondToUltrasoundRequest();
			break;
		case CMD_TYPE_MOTOR:
			if (data0 == CMD_MOTOR_OPEN) {
				motor.openFlower();
			} else if (data0 == CMD_MOTOR_CLOSE){
				motor.closeFlower();
			} else if (data0 == CMD_MOTOR_STOP ) {
				motor.stop();
			}
			break;

		case CMD_TYPE_LASER:
			if (data0 == CMD_LASER_ON) {
				lasers.on();
			} else if (data0 == CMD_LASER_OFF) {
				lasers.off();
			} else if (data0 == CMD_LASER_PULSE) {
				lasers.startPulsing(500, 800);//-- TODO use data 1-2
			}
			break;

		case CMD_TYPE_LED_RGB:
			leds.setRGB(data0, data1, data2);
			break;

		case CMD_TYPE_LED:
			switch(data0) {
				case CMD_LED_OFF:
					leds.setAnimationMode( ANIMATION_OFF );
					break;
				case CMD_LED_RAINBOW:
					leds.setAnimationMode( ANIMATION_RAINBOW );
					break;
				case CMD_LED_DROPLET:
					leds.setAnimationMode( ANIMATION_DROPLET );
					break;
			}
			break;

		case CMD_SET_ULT_THRESH:
			// sensors.setUltrasoundThreshold();
			break;
			
		case CMD_SET_MOTOR_CLOSE_TIME:
			// motor.setOpenRunTime();//-- TODO combine bytes data 0-3
			break;

		case CMD_SET_MOTOR_OPEN_TIME:
			// motor.setOpenRunTime();//-- TODO combine bytes data 0-3
			break;

		default:
			break;

	}


		// switch( comm.commandMsg[2] ) {
		// 	case CMD_MOTOR_OPEN:
		// 		motor.openFlower();
		// 		break;
		// 	case CMD_MOTOR_CLOSE:
		// 		motor.closeFlower();
		// 		break;
		// 	case CMD_MOTOR_STOP:
		// 		motor.stop();
		// 		break;
		// 	default:
		// 		break;
		// } 
    // } 	
}

void respondToUltrasoundRequest() {
	comm.switchToPipeTx(); //-- switch to sending
	uint8_t arr[2];
	arr[0] = CMD_TYPE_REPLY;
	sensors.readUltrasonic();
	if(sensors.getPresence()) {
		arr[1] = 1;
	} else {
		arr[2] = 0;
	}
	comm.sendMessage(arr, 1);
	comm.switchToPipeRx();
}

void writeEEPROMAddress() {
	if (Serial.available()){
		char c = Serial.read();
		EEPROM.write(EEPROM_ADDR_LOCATION, c-'0');

		// And we are done right now (no easy way to soft reset)
		Serial.print("\n\rManually reset address to: Press RESET to continue!");
		Serial.println(c);
		while(1) ;
		// }
	}
}



