/*-------------------------------------
	Arduino code for the controller(master) and remote controllers
	
	Written by: Jimmy | 08/2014
---------------------------------------*/

//------------
#define DEBUG
//------------

#include <Adafruit_NeoPixel.h> //-- Neopixel LEDs
#include "HMI.h"
#include "NeopixelMap.h"
#include "EnvSensors.h"


// sound
//#define LOG_OUT 1 // use the log output function
#define LIN_OUT 1
//#define SCALE 64
#define FHT_N 256 // set to 256 point fht
#include <FHT.h> // include the library

//-- pin assignments

HMI hmi;
NeopixelMap neopixelMap;
EnvSensors sensors;

// just for testing animations
long int beginningOfTime;
int counter = 0;
int deltaTime = 1000;


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(115200); //57600);
	#endif

	hmi.init();
	neopixelMap.init();
	sensors.init();

	// just for tests
	beginningOfTime = millis();
	// Sound Detector
	TIMSK0 = 0; // turn off timer0 for lower jitter
	ADCSRA = 0xe5; // set the adc to free running mode
	ADMUX = 0x41; // use adc0
	DIDR0 = 0x01; // turn off the digital input for adc0

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

	
	/***** TODO uncomment

	//comm.update();

	// Update readings from sensors every X ms
	// if( millis() - lastSensorsUpdate > SENSORS_UPDATE_PERIOD ) {
	// 	requestUltrasonicState();

	// 	lastSensorsUpdate = millis();
	// }

	***/


	// audio 
	//Serial.println( sensors.getSoundAmplitude() );

	while(1) { // reduces jitter
		cli();  // UDRE interrupt slows this way down on arduino1.0
		for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
		  while(!(ADCSRA & 0x10)); // wait for adc to be ready
		  ADCSRA = 0xf5; // restart adc
		  byte m = ADCL; // fetch adc data
		  byte j = ADCH;
		  int k = (j << 8) | m; // form into an int
		  k -= 0x0200; // form into a signed int
		  k <<= 6; // form into a 16b signed int
		  fht_input[i] = k; // put real data into bins
		}
		fht_window(); // window the data for better frequency response
		fht_reorder(); // reorder the data before doing the fht
		fht_run(); // process the data in the fht
		//fht_mag_octave();
		fht_mag_lin();
		//fht_mag_log(); // take the output of the fht
		sei();
		Serial.write(255); // send a start byte
		Serial.write((uint8_t *)fht_lin_out, FHT_N/2); // send out the data //fht_log_out
		//Serial.println();
	}

	//neopixelMap.breathe(hmi.getFlowerSelection()-1);

	int intensity = 100;
	bool beatNow = false;
	if ( ( millis() - beginningOfTime ) / deltaTime > counter ) {
		counter++;
		beatNow = true;
	}
	neopixelMap.musicBeat( beatNow, intensity );

}
