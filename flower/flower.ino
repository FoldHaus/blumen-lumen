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

unsigned long lastAnimationSwitch;
bool switchAnimAutomatically = true;

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
	
	printKeyboardCommands();

	lastAnimationSwitch = millis();

}

//-----------------------------------------------
void loop() {

	//-- update calls to each system
	motor.update();
	leds.update();
	comm.update();
	lasers.update();
	sensors.update();


	//-- check for inputs
	checkSerialInputs();

	//-- possibly switch to a new animation
	if ( switchAnimAutomatically &&
		millis() - lastAnimationSwitch > ANIMATION_TIMEOUT ) {
		Serial.print("Automatically switching to new animation #");

		// set new light animation (random)
		uint8_t newAnim = random(1, N_ANIM + 1);
		Serial.print(newAnim);
		leds.setAnimationMode( (ANIMATION_t) newAnim );

		// set new lasers behavior
		Serial.print(", ");
		lasers.randomize(); // TODO remove if lasers cmds come from controller?

		lastAnimationSwitch = millis();
	}

	if ( comm.getRole() == ROLE_RECEIVER ) {
		checkIncomingMessages(); //-- check for RF messagee
	} else {
		uint8_t arr[] = {0x10,0x20,0x30};
		comm.sendMessage(arr, 3);
	}

}
//-----------------------------------------------
// void stateMachine() {
// }


void checkIncomingMessages() {
//-----------------------------------------------
	comm.readBytes();
	if ( comm.isMsgReady() ){
		parseMessage();
	}
}

void parseMessage() {
	Serial.println("Parsing messages...");
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
			lastAnimationSwitch = millis();
			leds.setAnimationMode( ANIMATION_MANUAL );
			leds.setRGB(data0, data1, data2);
			break;

		case CMD_TYPE_LED:
			lastAnimationSwitch = millis();
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
				case CMD_LED_SLOWFADE:
					leds.setAnimationMode( ANIMATION_SLOWFADE );
					break;
				case CMD_LED_BEAT:
					leds.setAnimationMode( ANIMATION_BEAT );
					break;
				case CMD_LED_LSD:
					leds.setAnimationMode( ANIMATION_LSD );
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
		arr[1] = 0x01;
	} else {
		arr[1] = 0x02;
	}
	comm.sendMessage(arr, 2);
	comm.switchToPipeRx();
}

// void writeEEPROMAddress() {
// 	if (Serial.available()){
// 		char c = Serial.read();
// 		EEPROM.write(EEPROM_ADDR_LOCATION, c-'0');

// 		// And we are done right now (no easy way to soft reset)
// 		Serial.print("\n\rManually reset address to: Press RESET to continue!");
// 		Serial.println(c);
// 		while(1) ;
// 		// }
// 	}
// }


void printKeyboardCommands() {
	Serial.println("FLOWER commands");
	Serial.println("---------------");

	Serial.println("\n--Flower motor commands--");
	Serial.println("open [o]");
	Serial.println("close [c]");
	Serial.println("stop [s]");

	Serial.println("\n--Laser commands--");
	Serial.println("on [q]");
	Serial.println("off [w]");
	Serial.println("pulse [e]");

	Serial.println("\n--LED RGB commands--");
	Serial.println("red [j]");
	Serial.println("green [k]");
	Serial.println("blue [l]");

	Serial.println("\n--ANIMATION commands--");
	Serial.println("off [f]");
	Serial.println("rainbow [g]");
	Serial.println("droplets [h]");
	Serial.println("slow fade [v]");
	Serial.println("beat [b]");
	Serial.println("lsd [n]");

	Serial.println("toggle anim auto switch [#]");

}

//-----------------------------------------------
void checkSerialInputs() {
	lastAnimationSwitch = millis();

	static int k = 0;
	if ( Serial.available() ) {
		int key = Serial.read();

		switch(key) {

			// Lasers
			case 'q':
				Serial.println("> laser on");
				lasers.on();
				break;

			case 'w':
				Serial.println("> laser off");
				lasers.off();
				break;

			case 'e':
				Serial.println("> laser strobe.");
				lasers.startPulsing(500, 800);
				break;

			// Motor
			case 'o':
				Serial.println("> opening flower.");
				motor.openFlower();
				break;
			case 'c':
				Serial.println("> closing flower.");
				motor.closeFlower();
				break;
			case 's':
				Serial.println("> stopping.");
				motor.stop();
				break;

			// LEDs
			case 'j':
				Serial.println("red");
				leds.setAnimationMode( ANIMATION_MANUAL );
				leds.setRGB(255,0,0);
				break;

			case 'k':
				Serial.println("green");
				leds.setAnimationMode( ANIMATION_MANUAL );
				leds.setRGB(0,255,0);
				break;
			case 'l':	
				Serial.println("blue");
				leds.setAnimationMode( ANIMATION_MANUAL );
				leds.setRGB(0,0,255);
				break;

			// Animations
			case 'f':
				Serial.println("leds off");
				leds.setAnimationMode( ANIMATION_OFF );
				break;
			
			case 'g':
				Serial.println("leds rainbow");
				leds.setAnimationMode( ANIMATION_RAINBOW );
				break;

			case 'h':
				Serial.println("leds droplets");
				leds.setAnimationMode( ANIMATION_DROPLET );
				break;

			case 'v':
				Serial.println("leds slow fade");
				leds.setAnimationMode( ANIMATION_SLOWFADE );
				break;
			
			case 'b':
				Serial.println("leds beat");
				leds.setAnimationMode( ANIMATION_BEAT );
				break;

			case 'n':
				Serial.println("leds lsd");
				leds.setAnimationMode( ANIMATION_LSD );
				break;

			case '#':
				switchAnimAutomatically = !switchAnimAutomatically;
				Serial.println( switchAnimAutomatically ?
						"switch anim automatically" : "do not switch anim automatically" );
				break;

			// Help
			case '?':
				printKeyboardCommands();
				break;

			default:
				break;
		}
	}
}



