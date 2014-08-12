/*-------------------------------------
	Arduino code for the controller(master) and remote controllers
	
	Written by: Jimmy | 08/2014
---------------------------------------*/

//------------
#define DEBUG
//------------

#include "HMI.h"
#include "NeopixelMap.h"
#include "EnvSensors.h"


//-- pin assignments

HMI hmi;
NeopixelMap neopixelMap;
EnvSensors sensors;


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(57600);
	#endif

	hmi.init();
	neopixelMap.init();
	sensors.init();
}

//-----------------------------------------------
void loop() {
	// Serial.println(sensors.getWindSpeed());
	// Serial.println(sensors.getLightValue());
	// Serial.println(sensors.getSoundAmplitude());
}
