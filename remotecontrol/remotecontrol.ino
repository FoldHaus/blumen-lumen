/*-------------------------------------
	Arduino code for the controller(master) and remote controllers
	
	Written by: Jimmy | 08/2014
---------------------------------------*/

//------------
#define DEBUG
//------------


//-- for nRF24L01
#include <SPI.h>
#include <EEPROM.h>
#include "RF24.h"
#include "Radio.h"
#include "ControllerConstants.h"

// #include <Adafruit_NeoPixel.h> 
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


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(57600);
	#endif
	hmi.init();
	flowers.init();
	sensors.init();

	printKeyboardCommands();

	lastAnimationSwitch = millis();
}

//-----------------------------------------------
void loop() {
	static unsigned long lastPingTime = millis();

	//-- to change the EEPROM-saved ID
	// writeEEPROMAddress(); return;

	// checkEnvSensors();
	checkSerialInputs(); //-- check for Serial Monitor input
	checkHMIInputs();
	flowers.update();

	runStateMachine(NO_EVENT);

	//-- Update readings from sensors every 5s
	// if( millis() - lastSensorsUpdate > SENSORS_UPDATE_PERIOD ) {
	// 	flowers.updateUltrasonicState();
	// 	flowers.printUltrasonicState();
	// 	lastSensorsUpdate = millis();
	// }

	// Serial.println(sensors.getSoundAmplitude());

	//-- possibly switch to a new animation
	// if ( switchAnimAutomatically && millis() - lastAnimationSwitch > ANIMATION_TIMEOUT ) {
	// 	Serial.print("Automatically switching to new animation #");

	// 	// set new light animation (random)
	// 	uint8_t newAnim = random(1, N_ANIM + 1);
	// 	Serial.print(newAnim);
	// 	leds.setAnimationMode( (ANIMATION_t) newAnim );

	// 	// set new lasers behavior
	// 	Serial.print(", ");
	// 	lasers.randomize(); // TODO remove if lasers cmds come from controller?

	// 	lastAnimationSwitch = millis();
	// }

	// if (millis() - lastPingTime > 60000) {
	// 	flowers.sendPingsToAll();
	// 	lastPingTime = millis();
	// }

}


//-----------------------------------------------
void checkHMIInputs() {
	static uint8_t ledIncrement = 0;
	static uint8_t modeIncrement = 0;
	static uint8_t flowerSelection = 0;
	static bool isAll = false;

	if(hmi.getFlowerSelection() != flowerSelection) {
		flowerSelection = hmi.getFlowerSelection();
		flowers.communicateWithFlower(flowerSelection);
		if(flowerSelection > 10) {
			isAll = true;
		} else {
			isAll = false;
		}
		Serial.print("flower select: ");
		Serial.println(flowerSelection);

	}

	if ( hmi.isOpenBtnPushed() ) {
		Serial.println("open");
		flowers.openFlower();
		if (isAll ) flowers.allOpen();
	} else if ( hmi.isCloseBtnPushed() ) {
		Serial.println("close");
		flowers.closeFlower();
		if (isAll ) flowers.allClose();
	} else if ( hmi.isLEDTogglePushed() ) {
		Serial.println("LED");
		ledIncrement++;
		switch (ledIncrement % 9) {
			case 0:
			flowers.setRGB(255,255,0);
			if (isAll) flowers.allLEDRGB(255,255,0);
			break;

			case 1:
			flowers.setRGB(255,0,255);
			if (isAll) flowers.allLEDRGB(255,0,255);
			break;

			case 2:
			flowers.setRGB(0,255,255);
			if (isAll) flowers.allLEDRGB(255,255,0);
			break;

			case 3:
			flowers.off();
			if (isAll) flowers.allLEDOff();
			break;

			case 4:
			flowers.startAnimationRainbow();
			break;

			case 5:
			flowers.startAnimationDroplets();
			break;

			case 6:
			flowers.startAnimationSlowFade();
			break;

			case 7:
			flowers.startAnimationBeat();
			break;

			case 8:
			flowers.startAnimationLSD();
			break;
		}

	} else if ( hmi.isModeTogglePushed() ) {
		
		Serial.println("lasers");

		//-- cycle through the states
		modeIncrement++;
		switch ( modeIncrement % 3 ) {
			case 0:
			flowers.turnLasersOn();
			if (isAll) flowers.allLasersOn();
			break;
			case 1:
			flowers.lasersStrobe();
			break;
			case 2:
			flowers.turnLasersOff();
			if (isAll) flowers.allLasersOff();
			break;
		}
	}
}

//-----------------------------------------------
bool isAnyUltrasoundActivated() {
	for ( uint8_t i = 0; i < N_ULTRASONIC; i++ ) {
		if ( flowers.getUltrasonicState()[i] ) {
			return true;
		}
	}
	return false;
}


//-----------------------------------------------

void printKeyboardCommands() {
	Serial.println("CONTROLLER commands");
	Serial.println("---------------");

	Serial.println("Switch to flower by typing in the number (0-9)");
	Serial.println("---------------");

	Serial.println("\n--Flower motor commands--");
	Serial.println("open [o]");
	Serial.println("close [c]");
	Serial.println("stop [s]");

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

	Serial.println("\n--laser commands--");
	Serial.println("on [q]");
	Serial.println("off [w]");
	Serial.println("strobe [e]");

	Serial.println("\n--switch states, inject events--");
	// Serial.println("standby [e]");
	// Serial.println("state: day passive [t]");
	// Serial.println("state: day reactive [r]");
	// Serial.println("state: night passive [y]");
	// Serial.println("state: night reactive [w]");
	Serial.println("event: excessive wind [!]");
	Serial.println("event: normal wind [@]");
	Serial.println("event: night time [<]");
	Serial.println("event: day time [>]");
	Serial.println("event: motion detected [,]");
	Serial.println("event: no motion in a while [.]");



	Serial.println("toggle anim auto switch [#]");


	Serial.println("\nultrasound query [u]\n");

}

//-----------------------------------------------
void checkSerialInputs() {
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
				Serial.print("Selected ");
				selectedFlower = key-'0';
				Serial.println(selectedFlower);
				flowers.communicateWithFlower(selectedFlower);
				break;

			case 'q':
				Serial.println("> laser on");
				flowers.turnLasersOn();
				// lasers.on();
				break;

			case 'w':
				Serial.println("> laser off");
				flowers.turnLasersOff();
				break;

			case 'e':
				Serial.println("> laser strobe.");
				flowers.lasersStrobe();
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

			case 'u':
				flowers.updateUltrasonicState();
				flowers.printUltrasonicState();
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

			// case 'z':
			// 	Serial.println("state: standby [e]");
			// 	break;
			// case 't':
			// 	Serial.println("state: day passive [t]");
			// 	break;
			// case 'r':
			// 	Serial.println("state: day reactive [r]");
			// 	break;
			// case 'y':
			// 	Serial.println("state: night passive [y]");
			// 	break;
			// case 'w':
			// 	Serial.println("state: night reactive [w]");
			// 	break;
			// case '!':
			// 	Serial.println("event: excessive wind");
			// 	runStateMachine(WIND_SPEED_EXCESSIVE);
			// 	break;
			// case '@':
			// 	runStateMachine(WIND_SPEED_NOMINAL);
			// 	Serial.println("event: normal wind");
			// 	break;
			// case '<':
			// 	runStateMachine(NIGHT_TIME_DETECTED);
			// 	Serial.println("event: night time");
			// 	break;
			// case '>':
			// 	runStateMachine(DAY_TIME_DETECTED);
			// 	Serial.println("event: day time");
			// 	break;
			// case ',':
			// 	runStateMachine(MOTION_DETECTED);
			// 	Serial.println("event: motion detected");
			// 	break;
			// case '.':
			// 	runStateMachine(NO_MOTION_FOR_A_WHILE);
			// 	Serial.println("event: no motion in a while");
			// 	break;

			case '?':
				printKeyboardCommands();
				break;

			default:
				break;
		}
	}
}


void checkEnvSensors() {
	static unsigned long lastTimeDetected;
	static bool previouslyDay = false;

	if ( sensors.isTooWindy() ) {
		Serial.println("wind speed excessive");
		runStateMachine(WIND_SPEED_EXCESSIVE);
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

	// if ( isAnyUltrasoundActivated() ) {
	// 	runStateMachine(MOTION_DETECTED);
	// 	lastTimeDetected = millis();
	// } else {
	// 	if ( millis() - lastTimeDetected > NO_MOTION_FOR_A_WHILE_TIME) { //
	// 		runStateMachine(NO_MOTION_FOR_A_WHILE);
	// 	}
	// }


}

//-----------------------------------------------
void runStateMachine(uint8_t event) {
	static uint8_t currState = DAY_PASSIVE;
	boolean makeTransition = false;
	uint8_t nextState = currState;

	//-- above all, if wind is nuts, go to standby
	if ( event == WIND_SPEED_EXCESSIVE ) {
		makeTransition = true;
		nextState = STANDBY;
	}

	//-- Different states: DAY_PASSIVE, DAY_REACTIVE, NIGHT_PASSIVE, NIGHT_REACTIVE, NIGHT_PARTY
	switch ( currState ) {

		//-- Standby until wind is not that strong anymore
		case STANDBY:
			event = duringStandby(event);
			if ( event != NO_EVENT ) {
				switch ( event ) {
					case WIND_SPEED_NOMINAL:
					makeTransition = true;
					nextState = DAY_PASSIVE;
					break;
				}
			}
		break;

		//-- Day Passive does ambient animations until movement
		case DAY_PASSIVE:
			event = duringDayPassive(event);
			if ( event != NO_EVENT ) {
				switch ( event ){
				    case NIGHT_TIME_DETECTED:
				    makeTransition = true;
				    nextState = NIGHT_PASSIVE;
				    break;

					case MOTION_DETECTED:
					makeTransition = true;
					nextState = DAY_REACTIVE;
					break;
				}
			}
		break; //-- </DAY_PASSIVE>

		//-- Day Reactive opens up and is more alive than passive
		case DAY_REACTIVE:
			event = duringDayReactive(event);
			if ( event != NO_EVENT ) {
				switch ( event ) {

					case NO_MOTION_FOR_A_WHILE:
					makeTransition = true;
					nextState = DAY_PASSIVE;
					break;

					case NIGHT_TIME_DETECTED:
					makeTransition = true;
					nextState = NIGHT_PASSIVE;
					break;
				}
			}
		break; //-- </DAY_REACTIVE>

		//-- Night Passive does inverse ambient animations
		//-- except for the downward flowers
		case NIGHT_PASSIVE:
			event = duringNightPassive(event);
			if ( event != NO_EVENT ) {
				switch ( event ) {
					case MOTION_DETECTED:
					makeTransition = true;
					nextState = NIGHT_REACTIVE;
					break;

					case DAY_TIME_DETECTED:
					makeTransition = true;
					nextState = DAY_PASSIVE;
					break;
				}
			}
		break; //-- </NIGHT_PASSIVE>

		//-- Night Reactive reacts to people's movements
		case NIGHT_REACTIVE:
			event = duringNightReactive(event);
			if ( event != NO_EVENT ) {
				switch ( event ) {
					case DAY_TIME_DETECTED:
					makeTransition = true;
					nextState = DAY_PASSIVE;
					break;

					case NO_MOTION_FOR_A_WHILE:
					makeTransition = true;
					nextState = NIGHT_PASSIVE;
					break;
				}
			}
		break; //-- </NIGHT_REACTIVE

		//-- Night Party reacts to music
		case NIGHT_PARTY:
			event = duringNightParty(event);
			if ( event != NO_EVENT ) {
				switch ( event ) {
					case DAY_TIME_DETECTED:
					makeTransition = true;
					nextState = DAY_PASSIVE;
					break;

					//-- what else breaks out of party mode
				}
			}
		break; //-- </NIGHT_PARTY>
	}//-- end switch

  if ( makeTransition ) {
    runStateMachine(EV_EXIT); //-- Execute exit function for current state
    currState = nextState; 
    runStateMachine(EV_ENTRY); //-- Execute entry function for new state
  }

}

//-----------------------------------------------
uint8_t duringStandby(uint8_t ev) {
	static bool firstTimeNotWindy = true;
	static unsigned long startTime;

	if ( ev == EV_ENTRY) {
		Serial.println("Standby state");
		for(uint8_t i = 0; i < 100; i++ ) { //-- make sure it closes;
			flowers.allClose(); //-- close flowers
		}
		flowers.allLEDOff(); //-- turn off all lights
	} else if ( ev == EV_EXIT) {
		//-- keep flowers closed
	} else { //-- during function
	    //-- monitor the wind speed
	    if ( !sensors.isTooWindy() ) {
	    	if( firstTimeNotWindy ) {
		    	startTime = millis();
		    	firstTimeNotWindy = false;
		    } else if ( millis() - startTime > WIND_SPEED_CALM_TIME ) {
		    	Serial.println("Wind speed nominal");
		    	ev = WIND_SPEED_NOMINAL;
		    }
	    } else {
	    	ev = WIND_SPEED_EXCESSIVE;
	    }

	} 
	return ev;
}

//-- all open, close one at a time.
//-----------------------------------------------
uint8_t duringDayPassive(uint8_t ev) {
	uint8_t randomFlower = 0;
	static uint8_t prevRandomFlower = randomFlower;
	static unsigned long lastTime;

	if ( ev == EV_ENTRY) {
		Serial.println("State: Day passive");
		for(uint8_t i = 0; i < 10; i++){
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

//-----------------------------------------------
uint8_t duringDayReactive(uint8_t ev) {
	if ( ev == EV_ENTRY) {

	} else if ( ev == EV_EXIT) {

	} else { //-- during function
		//-- check the ultrasound
		for ( uint8_t i = 0; i < N_ULTRASONIC; i++ ) {
			if ( flowers.getUltrasonicState()[i] ) {
				flowers.communicateWithFlower(i);
				flowers.openFlower();
				//-- after some delay, open the asociated flowers
				// if (millis() - lastTime > ASSOCIATED_FLOWER_DELAY ) {

				// 
				//-- somehow open the three asociated flowers
				ev = DAY_REACTIVE;
			}

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
		for(uint8_t i = 0; i < 10; i++){
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
			flowers.communicateWithFlower(prevRandomFlower); //-- send command to open the last flower=sy
			flowers.openFlower();
			prevRandomFlower = randomFlower; //-- keep track of flower
			lastMvtTime = millis();
		}

	    //-- cycle through LED animations
		
		if ( millis() - lastLEDTime > PASSIVE_OPEN_CLOSE_TIME) {
			ledIncrement++;
			switch ( ledIncrement % 4) {
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
			}

			lastLEDTime = millis();
		}

	} 
	return ev;
}


//-----------------------------------------------
uint8_t duringNightReactive(uint8_t ev) {
	if ( ev == EV_ENTRY) {

	} else if ( ev == EV_EXIT) {

	} else { //-- during function
	    
	} 
	  return ev;
}


//-----------------------------------------------
uint8_t duringNightParty(uint8_t ev) {
	if ( ev == EV_ENTRY) {

	} else if ( ev == EV_EXIT) {

	} else { //-- during function
	    
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
