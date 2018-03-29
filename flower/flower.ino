/*-------------------------------------
	Arduino code for flower for Blumen Lumen
	
	Written by: jimmy chion | 08/2014

last updated for omaha install

---------------------------------------*/

//-- for nRF24L01
#include <SPI.h>
#include <EEPROM.h>

// #include "nRF24L01.h"
// #include "RF24.h"
#include "Easing.h"
#include "FlowerConstants.h"
#include "LEDs.h"
// #include "Radio.h"
#include "Motor.h"
#include "Sensors.h"


//------------
#define DEBUG
//------------

//-- this file should just hold the state machine for the flower

Motor motor;
LEDs leds;
// Radio comm;
Sensors sensors;

unsigned long lastAnimationSwitch;
unsigned long lastMoveTime;
bool switchAnimAutomatically = true;

// unsigned long lastTimeMsgReceived;
unsigned long timeSinceLastTrigger;
int lastSensorSensitivity;

uint8_t thisID;
//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(9600);
	#endif
	motor.init();
	leds.init();
	// comm.init();
	sensors.init();
	randomSeed(analogRead(2));

	printKeyboardCommands();

	lastAnimationSwitch = millis();
	timeSinceLastTrigger = millis();
	lastMoveTime = millis();

	sensors.setUltrasoundThreshold(220);
	// lastTimeMsgReceived = millis();
	// lastSensorSensitivity = analogRead(A2);

  thisID = EEPROM.read(EEPROM_ADDR_LOCATION);
  Serial.print("Blumen ID: ");
  Serial.println(thisID);
}

//-----------------------------------------------
void loop() {
	// writeEEPROMAddress();

	//-- update calls to each system
	motor.update();
	leds.update();
	// comm.update();
	sensors.update();

	//-- this is the special knob for PA flower to adjust sensitivity
	// if (analogRead(A2) != lastSensorSensitivity) {
	// 	Serial.println(analogRead(A2));
	// 	lastSensorSensitivity = analogRead(A2);
	// 	sensors.setUltrasoundThreshold(lastSensorSensitivity);	
	// }


	//-- check for inputs
	checkSerialInputs();

	// if ( comm.getRole() == ROLE_RECEIVER ) {
	// 	checkIncomingMessages(); //-- check for RF messagee
	// } 


	//-- if the ultrasound thing detects somebody, react by opening or closing
	// static unsigned long counter = 0 ;
	// uint8_t thisID = comm.getID();



	if ( thisID == 0 || thisID == 2 || thisID == 4){ //-- the IDs on those with ultrasound sensors
		if ( !motor.isFlowerMoving() &&  sensors.isSomeonePresent()) {
			if ( millis() - timeSinceLastTrigger > TIME_BETWEEN_USOUNDTRIGGER ) {
				Serial.println("motion detected");
				if ( motor.getApproxState() == MOTOR_STATE_OPEN ) {
					motor.closeFlower();
				} else {
					motor.openFlower();
				}
				timeSinceLastTrigger = millis();
			}
		}
	}

	leds.setAnimationMode(0);

	//-- cycle through lighting animations
	// static uint8_t anim = 0;
	// if ( millis() - lastAnimationSwitch > TIME_BETWEEN_ANIMSWITCH ) {
	// 	Serial.print("Animation switch");
	// 	anim = anim % N_ANIM;
	// 	Serial.println(anim);
	// 	leds.setAnimationMode( (ANIMATION_t) anim );
	// 	lastAnimationSwitch = millis();
	// }

	//-- randomly open and close
	static long randomMoveTimeInterval = random(7200000,10000000);
	if ( thisID != 0 && thisID != 2 && thisID != 4 ) {
		if ( millis() - lastMoveTime > randomMoveTimeInterval ) {
			randomMoveTimeInterval = random(7200000,10000000);
			Serial.print("random time interval to open/close: ");
			Serial.println(randomMoveTimeInterval);
			if (motor.getApproxState() == MOTOR_STATE_OPEN ) {
				motor.closeFlower();
			} else {
				motor.openFlower();
			}
			lastMoveTime = millis();
		}
 	}


}

// void checkIncomingMessages() {
// //-----------------------------------------------
// 	comm.readBytes();
// 	if ( comm.isMsgReady() ){
// 		parseMessage();
// 		// lastTimeMsgReceived = millis();
// 	}
// }

// void parseMessage() {
// 	Serial.println("Parsing messages...");
// //-----------------------------------------------
// 	//-- ignore bytes 0, N-1, and N-2 (Start byte, checksum, end byte)

// 	// -- byte 1: see if it's meant for me or a broadcast
// 	uint8_t data0 = comm.commandMsg[2];
// 	uint8_t data1 = comm.commandMsg[3];
// 	uint8_t data2 = comm.commandMsg[4];
// 	uint8_t data3 = comm.commandMsg[5];
// 	switch (comm.commandMsg[1]) {
// 		case CMD_TYPE_MOTOR:
// 			if (data0 == CMD_MOTOR_OPEN) {
// 				motor.openFlower();
// 			} else if (data0 == CMD_MOTOR_CLOSE){
// 				motor.closeFlower();
// 			} else if (data0 == CMD_MOTOR_STOP ) {
// 				motor.stop();
// 			}
// 			break;


// 		case CMD_TYPE_LED_RGB:
// 			lastAnimationSwitch = millis();
// 			leds.setAnimationMode( ANIMATION_MANUAL );
// 			leds.setRGB(data0, data1, data2);
// 			break;

// 		case CMD_TYPE_LED:
// 			lastAnimationSwitch = millis();
// 			switch(data0) {
// 				case CMD_LED_OFF:
// 					leds.setAnimationMode( ANIMATION_OFF );
// 					break;
// 				case CMD_LED_RAINBOW:
// 					leds.setAnimationMode( ANIMATION_RAINBOW );
// 					break;
// 				case CMD_LED_DROPLET:
// 					leds.setAnimationMode( ANIMATION_DROPLET );
// 					break;
// 				case CMD_LED_SLOWFADE:
// 					leds.setAnimationMode( ANIMATION_SLOWFADE );
// 					break;
// 				case CMD_LED_BEAT:
// 					leds.setAnimationMode( ANIMATION_BEAT );
// 					break;
// 				case CMD_LED_LSD:
// 					leds.setAnimationMode( ANIMATION_LSD );
// 					break;
// 			}
// 			break;

// 		// case CMD_SET_ULT_THRESH:
// 		// 		sensors.setUltrasoundThreshold(data1); //-- TODO test
// 		// 	break;
			
// 		// case CMD_SET_MOTOR_CLOSE_TIME:
// 		// 	motor.setOpenRunTime( data0 << 24 | data1 << 16 | data2 << 8 | data3 );//-- TODO test
// 		// 	break;

// 		// case CMD_SET_MOTOR_OPEN_TIME:
// 		// 	motor.setOpenRunTime( data0 << 24 | data1 << 16 | data2 << 8 | data3 ) ;//-- TODO test
// 		// 	break;

// 		default:
// 			break;

// 	}
// }

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


void printKeyboardCommands() {
	// Serial.println("FLOWER commands");
	// Serial.println("---------------");

	Serial.println("\n--Flower motor commands--");
	Serial.println("open [o]");
	Serial.println("close [c]");
	Serial.println("stop [s]");

	// Serial.println("\n--LED RGB commands--");
	// Serial.println("red [j]");
	// Serial.println("green [k]");
	// Serial.println("blue [l]");

	// Serial.println("\n--ANIMATION commands--");
	// Serial.println("off [f]");
	// Serial.println("rainbow [g]");
	// Serial.println("droplets [h]");
	// Serial.println("slow fade [v]");
	// Serial.println("beat [b]");
	// Serial.println("lsd [n]");

	// Serial.println("toggle anim auto switch [#]");

}

//-----------------------------------------------
void checkSerialInputs() {

	static int k = 0;
	if ( Serial.available() ) {
		int key = Serial.read();

		switch(key) {

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



