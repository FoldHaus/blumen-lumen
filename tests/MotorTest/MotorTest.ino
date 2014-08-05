/*-------------------------------------
	motor test. Goes one direction for a second,
	the other direction for a second, and then 
	stops for a second.
	
	Written by: jimmy | 8/1/14
---------------------------------------*/

//------------
// #define DEBUG
//------------

//-- pin assignments
#define IN_1 7
#define IN_2 8
#define LED_PIN 13


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(9600);
	#endif
	pinMode(IN_1, OUTPUT);
	pinMode(IN_2, OUTPUT);
	
}

//-----------------------------------------------
void loop() {
	delay(1000);
	digitalWrite(IN_1, HIGH);
	digitalWrite(IN_2, LOW);
	digitalWrite(LED_PIN, HIGH);
	// delay(1000);
	// digitalWrite(IN_2, HIGH);
	// digitalWrite(IN_1, LOW);
	// digitalWrite(LED_PIN, LOW);
	// delay(1000);
	// digitalWrite(IN_2, LOW);
	// digitalWrite(IN_1, LOW);
	// digitalWrite(LED_PIN, LOW);
}
