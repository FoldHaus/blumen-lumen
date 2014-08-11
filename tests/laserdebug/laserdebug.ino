/*-------------------------------------
	description of the program
	
	Written by: name | date
---------------------------------------*/

//------------
#define DEBUG
//------------

//-- pin assignments
#define PIN 3


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(9600);
	#endif
	pinMode(PIN, OUTPUT);
}

//-----------------------------------------------
void loop() {
	digitalWrite(PIN, HIGH);
	delay(1000);
	digitalWrite(PIN, LOW);
	delay(1000);
}
