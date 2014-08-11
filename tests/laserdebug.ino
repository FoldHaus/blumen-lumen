/*-------------------------------------
	description of the program
	
	Written by: name | date
---------------------------------------*/

//-----------
//-----------------------------------------------
void setup() {
	// #ifdef DEBUG 
	// Serial.begin(9600);
	// #endif
	pinMode(3, OUTPUT);
}

//-----------------------------------------------
void loop() {
	digitalWrite(3, HIGH);
	delay(1000);
	digitalWrite(3, LOW);
	delay(1000);
}
