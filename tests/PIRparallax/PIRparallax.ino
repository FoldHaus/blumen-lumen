/*-------------------------------------
	test for PIR sensors for blumen lumen
	
	Written by: jimmy | jul 12 14
---------------------------------------*/

//------------
// #define DEBUG
//------------

//-- pin assignments
#define PIR_IN 4
#define LED_OUT 13


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(9600);
	#endif
	pinMode(PIR_IN, INPUT);
	pinMode(LED_OUT, OUTPUT);
	Serial.println("Ready");
}

//-----------------------------------------------
void loop() {
	if(digitalRead(PIR_IN) == HIGH) {
		Serial.println(1);
		digitalWrite(LED_OUT, HIGH);
	} else {
		Serial.println(0);
		digitalWrite(LED_OUT, LOW);
	}
}
