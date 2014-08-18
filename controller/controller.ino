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
#include "HMI.h"
#include "NeopixelMap.h"
#include "EnvSensors.h"


//-- pin assignments

HMI hmi;
NeopixelMap neopixelMap;
EnvSensors sensors;
Radio comm;


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

	sensors.init();
	comm.init();

	initUltrasonic();

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
	// neopixelMap.breathe(hmi.getFlowerSelection()-1);

	comm.update();


	// Update readings from sensors every X ms
	if( millis() - lastSensorsUpdate > SENSORS_UPDATE_PERIOD ) {
		requestUltrasonicState();

		lastSensorsUpdate = millis();
	}

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
		uint8_t arr[] = { CMD_REQUEST_ULTRASONIC };
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
			if( comm.getMessage()[0] == 0x00 ) { // no person detected
				ultrasonicState[ curUltrasonic ] = false;
			}
			else {
				ultrasonicState[ curUltrasonic ] = true;
			}
		}

	}
	
}
