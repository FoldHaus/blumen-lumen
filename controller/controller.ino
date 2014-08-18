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

#include <Adafruit_NeoPixel.h> //-- Neopixel LEDs
#include <Easing.h>
#include "HMI.h"
#include "NeopixelMap.h"
#include "EnvSensors.h"


//-- pin assignments

HMI hmi;
NeopixelMap neopixelMap;
EnvSensors sensors;
Radio comm;

uint8_t selectedFlower = 0;

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
	neopixelMap.init();
	neopixelMap.setComm(&comm);

	sensors.init();
	comm.init();

	initUltrasonic();
	printKeyboardCommands();

}

//-----------------------------------------------
void loop() {
	// Serial.println(sensors.getWindSpeed());
	// Serial.prinxtln(sensors.getLightValue());
	// Serial.println(sensors.getSoundAmplitude());
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
	neopixelMap.droplets();

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
	Serial.println("open [o]");
	Serial.println("close [c]");
	Serial.println("stop [s]");

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
}

//-----------------------------------------------
void checkSerialInputs() {
	static int k = 0;
	uint8_t arr[5]; 
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


			default:
				break;
		}
	}
}

