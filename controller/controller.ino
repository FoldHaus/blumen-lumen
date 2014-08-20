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

#include <Adafruit_NeoPixel.h> 
#include <Easing.h>
#include "HMI.h"
#include "Flowers.h"
#include "EnvSensors.h"


// sound
//#define LOG_OUT 1 // use the log output function
#define LIN_OUT 1
//#define SCALE 64
#define FHT_N 256 // set to 256 point fht
#include <FHT.h> // include the library

//-- pin assignments

HMI hmi;
EnvSensors sensors;
Flowers flowers;

uint8_t selectedFlower = 0;

// just for testing animations
long int beginningOfTime;
int counter = 0;
int deltaTime = 1000;

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

	// writeEEPROMAddress();
	// return;

	// Serial.println(sensors.getWindSpeed());
	// Serial.prinxtln(sensors.getLightValue());
	// Serial.println(sensors.getSoundAmplitude());
	// hmi.getFlowerSelection());
	// delay(100);
	// Flowers.breatheChecker();
	// Flowers.comboAnimation();
	// Flowers.breathe(hmi.getFlowerSelection()-1);



	checkSerialInputs(); //-- check for Serial Monitor input
	flowers.update();

	// TODO - Jimmy add state machine

	/*** TODO UNCOMMENT ****/
	// Update readings from sensors every X ms
	// if( millis() - lastSensorsUpdate > SENSORS_UPDATE_PERIOD ) {
	// 	requestUltrasonicState();
	// 	flowers.updateUltrasonicState();
	// 	lastSensorsUpdate = millis();
	// }

}




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

	Serial.println("\n--LED commands--");
	Serial.println("off [f]");
	Serial.println("rainbow [g]");
	Serial.println("droplets [h]");
	Serial.println("slow fade [v]");
	Serial.println("beat [b]");
	Serial.println("lsd [n]");

	Serial.println("toggle anim auto switch [#]");


	Serial.println("\nultrasound query [u]\n");

}

//-----------------------------------------------
void checkSerialInputs() {
	static int k = 0;
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
			case 'c':
				Serial.println("> closing flower.");
				flowers.closeFlower();
				break;
			case 's':
				Serial.println("> stopping.");
				flowers.stopMotor();
				break;

			case 'j':
				Serial.println("red");
				flowers.setRGB(255,0,0);
				break;

			case 'k':
				Serial.println("green");
				flowers.setRGB(0,255,0);
				break;
			case 'l':	
				Serial.println("blue");
				flowers.setRGB(0,0,255);
				break;

			case 'u':
				flowers.updateUltrasonicState();
				flowers.printUltrasonicState();
				break;

			case 'f':
				Serial.println("leds off");
				flowers.off();
				break;
			
			case 'g':
				Serial.println("leds rainbow");
				flowers.startAnimationRainbow();
				break;

			case 'h':
				Serial.println("leds droplets");
				flowers.startAnimationDroplets();
				break;

			case 'v': //TODO enable
				Serial.println("leds slow fade");
				//flowers.startAnimationSlowFade();
				break;
			
			case 'b':
				Serial.println("leds beat");
				//flowers.startAnimationBeat();
				break;

			case 'n':
				Serial.println("leds lsd");
				//flowers.startAnimationLSD();
				break;

			case '#':
				switchAnimAutomatically = !switchAnimAutomatically;
				Serial.println( switchAnimAutomatically ?
						"switch anim automatically" : "do not switch anim automatically" );
				break;

			case '?':
				printKeyboardCommands();
				break;

			default:
				break;
		}
	}
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
