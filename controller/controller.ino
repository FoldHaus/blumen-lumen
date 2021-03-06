/*-------------------------------------
	Arduino code for the controller(master) and remote controllers
	
	Written by: Jimmy | 08/2014

	Build for Treasure Island Music Festival 2014
	- no lasers
	- no sound detection
	- no wind speed
	- each flower is reactive with ultrasonic, independent of master
	- simpler program/
	- remote control changes animation pattern, opens/closes,  
	- otherwise is independent and cycles through light animation when in night time state
---------------------------------------*/

//------------
#define DEBUG
//------------

<<<<<<< HEAD
=======

>>>>>>> treasure-island-14
//-- for nRF24L01
#include <SPI.h>
#include <EEPROM.h>
#include "RF24.h"
#include "Radio.h"
#include "ControllerConstants.h"

<<<<<<< HEAD
#include <Adafruit_NeoPixel.h> //-- Neopixel LEDs
=======
// #include <Adafruit_NeoPixel.h> 
>>>>>>> treasure-island-14
#include <Easing.h>
#include "HMI.h"
#include "Flowers.h"
#include "EnvSensors.h"


// sound
//#define LOG_OUT 1 // use the log output function
// #define LIN_OUT 1
//#define SCALE 64
// #define FHT_N 256 // set to 256 point fht
// #include <FHT.h> // include the library

//-- pin assignments

HMI hmi;
EnvSensors sensors;
<<<<<<< HEAD
Radio comm;

uint8_t selectedFlower = 0;
=======
Flowers flowers;

uint8_t selectedFlower = 0;

// just for testing animations
// long int beginningOfTime;
// int counter = 0;
// int deltaTime = 1000;

// controller variables
unsigned long lastSensorsUpdate;
unsigned long lastAnimationSwitch;

bool switchAnimAutomatically = true;
>>>>>>> treasure-island-14

// controller variables
unsigned long lastSensorsUpdate;
bool ultrasonicState[ N_ULTRASONIC ];
uint8_t ultrasonicIndexes[ N_ULTRASONIC ];
	

//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(57600);
	#endif
	hmi.init();
<<<<<<< HEAD
	neopixelMap.init();
	neopixelMap.setComm(&comm);

	sensors.init();
	comm.init();

	initUltrasonic();
	printKeyboardCommands();

=======
	flowers.init();
	sensors.init();

	printKeyboardCommands();

	lastAnimationSwitch = millis();

>>>>>>> treasure-island-14
}

//-----------------------------------------------
void loop() {
	static unsigned long lastPingTime = millis();

	//-- to change the EEPROM-saved ID
	// writeEEPROMAddress(); return;

	checkEnvSensors();
	checkSerialInputs(); //-- check for Serial Monitor input
	flowers.update();

	runStateMachine(NO_EVENT);

	// Serial.println(sensors.getSoundAmplitude());
<<<<<<< HEAD
	// hmi.getFlowerSelection());
	// delay(100);
	// neopixelMap.breatheChecker();
	// neopixelMap.comboAnimation();

	comm.update();
	checkSerialInputs(); //-- check for Serial Monitor input

	// Update readings from sensors every X ms
	// if( millis() - lastSensorsUpdate > SENSORS_UPDATE_PERIOD ) {
	// 	requestUltrasonicState();
	// 	lastSensorsUpdate = millis();
	// }

	// Lights
	// neopixelMap.breathe(hmi.getFlowerSelection()-1);
	// neopixelMap.droplets();

}


void initUltrasonic() {
	for( int i = 0; i < N_ULTRASONIC; i++ ) {
		ultrasonicState[ i ] = false;
		ultrasonicIndexes[ i ] = i*2; // 0, 2, 4 
	}
}


void requestUltrasonicState() {

	for( int curUltrasonic = 0; curUltrasonic < N_ULTRASONIC; curUltrasonic++ ) {
		// Query current ultrasonic
		comm.switchToPipeTx( ultrasonicIndexes[ curUltrasonic ] );

		// Send request
		uint8_t arr[] = { CMD_TYPE_ULT_RQ };
		comm.sendMessage(arr, 1);

		// Switch to listening
		comm.switchToPipeRx(1);
		// //-- wait for response
		unsigned long lastTime = millis();
		Serial.println("Listening for response...");
		while( (millis() - lastTime < ULTRASONIC_REQUEST_TIMEOUT) ) {
			comm.readBytes();
		}

		if( comm.isMsgReady() ) {
			if( comm.getMessage()[2] == 0x00 ) { // no person detected
				ultrasonicState[ curUltrasonic ] = false;
			}
			else {
				ultrasonicState[ curUltrasonic ] = true;
			}
		}

	}
	
}

void printKeyboardCommands() {
	Serial.println("Key commands");
	Serial.println("---------------");
	Serial.println("Switch to flower by typing in the number (0-9)");
	Serial.println("---------------");
	Serial.println("\n--Flower motor commands--");
	// Serial.println("toggle modes [m]")
=======
	// Serial.println(sensors.getLightValue());

	//-- possibly switch to a new animation
	// if ( switchAnimAutomatically && millis() - lastAnimationSwitch > ANIMATION_TIMEOUT ) {
	// 	Serial.print("Automatically switching to new animation #");

	// 	// set new light animation (random)
	// 	uint8_t newAnim = random(1, N_ANIM + 1);
	// 	Serial.print(newAnim);
	// 	flowers.doAnimationOnAllFlowers( (ANIMATION_t) newAnim );

	// 	lastAnimationSwitch = millis();
	// }

	if (millis() - lastPingTime > 60000) {
		flowers.sendPingsToAll();
		lastPingTime = millis();
	}

}




//-----------------------------------------------

void printKeyboardCommands() {
	Serial.println("CONTROLLER commands");
	Serial.println("---------------");

	Serial.println("Switch to flower by typing in the number (0-9)");
	Serial.println("---------------");

	Serial.println("\n--Flower motor commands--");
>>>>>>> treasure-island-14
	Serial.println("open [o]");
	Serial.println("close [c]");
	Serial.println("stop [s]");

<<<<<<< HEAD
	Serial.println("\n--Laser commands--");
	Serial.println("on [q]");
	Serial.println("off [w]");
	Serial.println("pulse [e]");

	Serial.println("\n--LED commands--");
	Serial.println("red [j]");
	Serial.println("green [k]");
	Serial.println("blue [l]");

	Serial.println("\nultrasound query [u]\n");



	// Serial.println("\n--Radio commands--");
	// Serial.println("receive mode [r]");
	// Serial.println("transmit mode [t]");
=======
	Serial.println("\n--LED RGB commands--");
	Serial.println("red [j]");
	Serial.println("green [k]");
	Serial.println("blue [l]");
	Serial.println("all red [J]");
	Serial.println("all green [K]");
	Serial.println("all blue [L]");


	Serial.println("\n--LED commands--");
	Serial.println("off [f]");
	Serial.println("all off [F]");
	Serial.println("rainbow [g]");
	Serial.println("droplets [h]");
	Serial.println("slow fade [v]");
	Serial.println("beat [b]");
	Serial.println("lsd [n]");


	Serial.println("\n--switch states, inject events--");
	// Serial.println("standby [e]");
	// Serial.println("state: day passive [t]");
	// Serial.println("state: day reactive [r]");
	// Serial.println("state: night passive [y]");
	// Serial.println("state: night reactive [w]")
	Serial.println("event: night time [<]");
	Serial.println("event: day time [>]");




	Serial.println("toggle anim auto switch [#]");


>>>>>>> treasure-island-14
}

//-----------------------------------------------
void checkSerialInputs() {
<<<<<<< HEAD
	static int k = 0;
	uint8_t arr[5]; 
=======
>>>>>>> treasure-island-14
	if ( Serial.available() ) {
		int key = Serial.read();

		switch(key) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
<<<<<<< HEAD
			case ':':
			case ';':
			case '<':
			case '=':
				comm.switchToPipeTx(key-'0');
				selectedFlower = key-'0';
				break;

			case 'q':
				Serial.println("> laser on");
				arr[0] = CMD_TYPE_LASER;
				arr[1] = CMD_LASER_ON;
				comm.sendMessage(arr, 2);
				// lasers.on();
				break;

			case 'w':
				Serial.println("> laser off");
				arr[0] = CMD_TYPE_LASER;
				arr[1] = CMD_LASER_OFF;
				comm.sendMessage(arr, 2);
				// lasers.off();
				break;

			case 'e':
				Serial.println("> laser strobe.");
				arr[0] = CMD_TYPE_LASER;
				arr[1] = CMD_LASER_PULSE;
				comm.sendMessage(arr, 2);
				// lasers.startPulsing(100, 60);
				break;

			case 'o':
				Serial.println("> opening flower.");
				arr[0] = CMD_TYPE_MOTOR;
				arr[1] = CMD_MOTOR_OPEN;
				comm.sendMessage(arr, 2);
				// motor.openFlower();
				break;
			case 'c':
				Serial.println("> closing flower.");
				arr[0] = CMD_TYPE_MOTOR;
				arr[1] = CMD_MOTOR_CLOSE;
				comm.sendMessage(arr, 2);
				// motor.closeFlower();
				break;
			case 's':
				Serial.println("> stopping.");
				arr[0] = CMD_TYPE_MOTOR;
				arr[1] = CMD_MOTOR_STOP;
				comm.sendMessage(arr, 2);
				// motor.stop();
				break;

			case 'j':
				Serial.println("red");
				arr[0] = CMD_TYPE_LED_RGB;
				arr[1] = 0xFF;
				arr[2] = 0x00;
				arr[3] = 0x00;
				comm.sendMessage(arr, 4);
				// leds.setRGB(255,0,0);
				break;

			case 'k':
				Serial.println("green");
				arr[0] = CMD_TYPE_LED_RGB;
				arr[1] = 0x00;
				arr[2] = 0xFF;
				arr[3] = 0x00;
				comm.sendMessage(arr, 4);
				// leds.setRGB(0,255,0);
				break;
			case 'l':	
				Serial.println("blue");
				arr[0] = CMD_TYPE_LED_RGB;
				arr[1] = 0x00;
				arr[2] = 0x00;
				arr[3] = 0xFF;
				comm.sendMessage(arr, 4);
				// leds.setRGB(0,0,255);
				break;

			case 'u':
				requestUltrasonicState();
				for (uint8_t i = 0; i < N_ULTRASONIC; i++ ) {
					Serial.print("\nultrasound state: ");
					Serial.println(ultrasonicState[i]);
				}
				break;


			case 't':
				k++;
				if ( k%2 == 0) {
					comm.switchToPipeTx(0);
					uint8_t arr[] = {0x10,0x21,0x31,0xFA};
					comm.sendMessage(arr, 4);
					
				} else if (k%2 == 1) {
					comm.switchToPipeTx(1);
					// for(int i = 0; i < 5; i++) {
					uint8_t arr[] = {0x10,0x21,0x30,0xFA};
					comm.sendMessage(arr, 4);
					// }
					comm.switchToPipeRx(1);
					// //-- wait for response
					unsigned long timeOut = 1000;
					unsigned long lastTime = millis();
					while( (millis() - lastTime < timeOut) ) {
						comm.readBytes();
						Serial.println("received response.");
					}
					// //-- switch back to sending
					// comm.switchToPipeTx(0);
				} 
				break;

			case 'r':
				// Serial.println("receive mode");
				// comm.switchToPipeRx();
				break;

=======
				Serial.print("Selected ");
				selectedFlower = key-'0';
				Serial.println(selectedFlower);
				flowers.communicateWithFlower(selectedFlower);
				break;


			case 'o':
				Serial.println("> opening flower.");
				flowers.openFlower();
				break;

			case 'O':
				Serial.println("> opening flower.");
				flowers.allOpen();
				break;

			case 'c':
				Serial.println("> closing flower.");
				flowers.closeFlower();
				break;

			case 'C':
				Serial.println("> closing flower.");
				flowers.allClose();
				break;
			case 's':
				Serial.println("> stopping.");
				flowers.stopMotor();
				break;
			case 'S':
				Serial.println("> stopping.");
				flowers.allStop();
				break;

			// case 'j':
			// 	Serial.println("red");
			// 	flowers.setRGB(255,0,0);
			// 	break;

			// case 'k':
			// 	Serial.println("green");
			// 	flowers.setRGB(0,255,0);
			// 	break;

			// case 'l':	
			// 	Serial.println("blue");
			// 	flowers.setRGB(0,0,255);
			// 	break;

			case 'J':
				Serial.println("all red");
				flowers.allLEDRGB(255,255,0);
				break;

			case 'K':
				Serial.println("all green");
				flowers.allLEDRGB(255,255,0);
				break;
			case 'L':	
				Serial.println("all blue");
				flowers.allLEDRGB(0,255,255);
				break;


			case 'f':
				Serial.println("leds off");
				flowers.off();
				break;

			case 'F':
				Serial.println("all leds off");
				flowers.allLEDOff();
				break;
			
			case 'g':
				Serial.println("leds rainbow");
				flowers.doAnimationOnAllFlowers(ANIMATION_RAINBOW);
				break;

			case 'h':
				Serial.println("leds droplets");
				flowers.doAnimationOnAllFlowers(ANIMATION_DROPLET);
				break;


			case 'v':
				Serial.println("leds slow fade");
				flowers.doAnimationOnAllFlowers(ANIMATION_SLOWFADE);
				break;
			
			case 'b':
				Serial.println("leds beat");
				flowers.doAnimationOnAllFlowers(ANIMATION_BEAT);
				break;

			case 'n':
				Serial.println("leds lsd");
				flowers.doAnimationOnAllFlowers(ANIMATION_LSD);
				break;

			// case '#':
			// 	switchAnimAutomatically = !switchAnimAutomatically;
			// 	Serial.println( switchAnimAutomatically ?
			// 			"switch anim automatically" : "do not switch anim automatically" );
			// 	break;
			// case 't':
			// 	Serial.println("state: day passive [t]");
			// 	break;
			// case 'y':
			// 	Serial.println("state: night passive [y]");
			// 	break;

			case '?':
				printKeyboardCommands();
				break;
>>>>>>> treasure-island-14

			default:
				break;
		}
	}
}

<<<<<<< HEAD
=======

void checkEnvSensors() {
	static unsigned long lastTimeDetected;
	static bool previouslyDay = false;
	static bool alreadyHappened = false;

	if (!alreadyHappened) {
		if(sensors.isDaytime() ) {
			previouslyDay = false;
		} else {
			previouslyDay = true;
		}
		alreadyHappened = true;
	}

	//-- check this not as often maybe
	if ( sensors.isDaytime() && !previouslyDay) {
		Serial.println("turned day");

		runStateMachine(DAY_TIME_DETECTED);
		previouslyDay = true;
	} else if ( !sensors.isDaytime() && previouslyDay) {
		Serial.println("turned night");
		runStateMachine(NIGHT_TIME_DETECTED);
		previouslyDay = false;
	}


}

//-----------------------------------------------
void runStateMachine(uint8_t event) {
	static uint8_t currState = DAY_PASSIVE;
	boolean makeTransition = false;
	uint8_t nextState = currState;


	//-- Different states: DAY_PASSIVE, DAY_REACTIVE, NIGHT_PASSIVE, NIGHT_REACTIVE, NIGHT_PARTY
	switch ( currState ) {



		//-- Day Passive does ambient animations until movement
		case DAY_PASSIVE:
			event = duringDayPassive(event);
			if ( event != NO_EVENT ) {
				switch ( event ){
				    case NIGHT_TIME_DETECTED:
				    makeTransition = true;
				    nextState = NIGHT_PASSIVE;
				    break;

				}
			}
		break; //-- </DAY_PASSIVE>


		//-- Night Passive does inverse ambient animations
		//-- except for the downward flowers
		case NIGHT_PASSIVE:
			event = duringNightPassive(event);
			if ( event != NO_EVENT ) {
				switch ( event ) {

					case DAY_TIME_DETECTED:
					makeTransition = true;
					nextState = DAY_PASSIVE;
					break;
				}
			}
		break; //-- </NIGHT_PASSIVE>


	}//-- end switch

  if ( makeTransition ) {
    runStateMachine(EV_EXIT); //-- Execute exit function for current state
    currState = nextState; 
    runStateMachine(EV_ENTRY); //-- Execute entry function for new state
  }

}


//-- all open, close one at a time.
//-----------------------------------------------
uint8_t duringDayPassive(uint8_t ev) {
	uint8_t randomFlower = 0;
	static uint8_t prevRandomFlower = randomFlower;
	static unsigned long lastTime;

	if ( ev == EV_ENTRY) {
		Serial.println("State: Day passive");
		for(uint8_t i = 0; i < 1; i++){
			flowers.allClose();
			flowers.allLEDOff();
		}
	} else if ( ev == EV_EXIT) {

	} else { //-- during function
		if ( millis() - lastTime > PASSIVE_OPEN_CLOSE_TIME) {
			Serial.print("new random flower opening.");
			randomFlower = random(0, N_FLOWERS);
			while( randomFlower == prevRandomFlower ) {
				randomFlower = random(0, N_FLOWERS);
			}
			Serial.println(randomFlower);
			flowers.communicateWithFlower(randomFlower); //-- send command to open that flower
			flowers.openFlower();
			flowers.communicateWithFlower(prevRandomFlower); //-- send command to close the last flower
			flowers.closeFlower();
			prevRandomFlower = randomFlower; //-- record flower
			lastTime = millis();
		}

	} 
	  return ev;
}



//-- all closed, and they open one at a time
//-----------------------------------------------
uint8_t duringNightPassive(uint8_t ev) {
	uint8_t randomFlower = 0;
	static uint8_t prevRandomFlower = randomFlower;
	static unsigned long lastMvtTime;
	static unsigned long lastLEDTime;
	static uint8_t ledIncrement = 0;

	if ( ev == EV_ENTRY) {
		for(uint8_t i = 0; i < 1; i++){
			flowers.allOpen();
			flowers.allRainbow();
		}
	} else if ( ev == EV_EXIT) {
	} else { //-- during function
		//-- once every 40s or so
		if ( millis() - lastMvtTime > PASSIVE_OPEN_CLOSE_TIME) {
			Serial.print("State: night passive. new random flower.");
			randomFlower = random(0, N_FLOWERS);
			while( randomFlower == prevRandomFlower ) {
				randomFlower = random(0,N_FLOWERS);
			}
			Serial.println(randomFlower);
			flowers.communicateWithFlower(randomFlower); //-- send command to close that flower
			flowers.closeFlower();
			if(randomFlower+1 < N_FLOWERS) { //-- close another flower;
				flowers.communicateWithFlower(randomFlower+1);	
				flowers.closeFlower();
			}
			flowers.communicateWithFlower(prevRandomFlower); //-- send command to open the last flower=sy
			flowers.openFlower();
			if(prevRandomFlower+1 < N_FLOWERS) { //-- open the second flower;
				flowers.communicateWithFlower(prevRandomFlower+1);	
				flowers.openFlower();
			}
			prevRandomFlower = randomFlower; //-- keep track of flower
			lastMvtTime = millis();
		}

	    //-- cycle through LED animations
		
		if ( millis() - lastLEDTime > LED_ROTATE_TIME) {
			ledIncrement++;
			switch ( ledIncrement % 6) {
				case 0:
				flowers.startAnimationRainbow();
				break;

				case 1:
				flowers.startAnimationDroplets();
				break;

				case 2:
				flowers.startAnimationSlowFade();
				break;

				case 3:
				flowers.startAnimationLSD();
				break;

				case 4:
				flowers.allClose();
				break;

				case 5:
				flowers.allOpen();
				break;
			}

			lastLEDTime = millis();
		}

	} 
	return ev;
}

//-----------------------------------------------
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
>>>>>>> treasure-island-14
