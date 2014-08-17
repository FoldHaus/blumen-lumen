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
#include <EEPROM.h>

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

	Serial.println("\n--Radio commands--");
	Serial.println("receive mode [r]");
	Serial.println("transmit mode [t]");
	

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
	if ( comm.getRole() == ROLE_RECEIVER ) {
		checkIncomingMessages(); //-- check for RF messagee
	} else {
		uint8_t arr[] = {0x10,0x21,0x31};
		comm.sendMessage(arr, 3);
	}

}
//-----------------------------------------------
// void stateMachine() {
// }

//-----------------------------------------------
void checkSerialInputs() {
	static int i = 0;
	static int k = 0;
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
				if (++i%3 == 0){
					leds.setRGB(0,255,0);
				} 
				else if (i%3 == 1){
					leds.setRGB(255,0,0);
				}
				else if (i%3 == 2){
					leds.setRGB(0,0,255);
				}
				// else if (i%5 == 4){
				// 	Serial.println("> LEDs off.");
				// 	leds.off();
				// }
				// else {
				// 	// leds.startRainbow();
				// } 
				break;

			case 't':
			case 'T':
				k++;
				if ( k%2 == 0) {
					comm.switchToPipeTx(0);

					// //-- wait for response
					// unsigned long timeOut = 1000;
					// unsigned long lastTime = millis();
					// while( comm.available() || (millis() - lastTime < timeOut) ) {
					// 	if( comm.available() ) {
					// 		comm.readBytes();
					// 		Serial.println("received response.");
					// 	}
					// }
					// //-- switch back to sending
					// comm.switchToPipeTx(0);
				} else if (k%2 == 1) {
					comm.switchToPipeTx(1);
		
				} 
				break;

			case 'r':
			case 'R':
				// Serial.println("receive mode");
				// comm.switchToPipeRx();
				break;

			default:
				break;
		}
	}
}

void checkIncomingMessages() {
	comm.readBytes();
	if ( comm.isMsgReady() ){
		parseMessage();
	}
}


void parseMessage() {
	//-- ignore bytes 0, N-1, and N-2 (Start byte, checksum, end byte)

	// -- byte 1: see if it's meant for me or a broadcast
	if (comm.commandMsg[1] == 0x10 ) {
		comm.switchToPipeTx(); //-- switch to sending
		uint8_t arr[] = {0xBA, 0xFF};
		comm.sendMessage(arr, 2);
		comm.switchToPipeRx();


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
    } 	
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



