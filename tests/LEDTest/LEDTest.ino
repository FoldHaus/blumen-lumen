/*-------------------------------------
	LED test. pwm 50% on each of the RGB channels
	
	Written by: jimmy | 8/1/14
---------------------------------------*/

//------------
// #define DEBUG
//------------

//-- pin assignments
#define RED_PWM 3
#define BLUE_PWM 6
#define GREEN_PWM 5


//-----------------------------------------------
void setup() {
	#ifdef DEBUG 
	Serial.begin(9600);
	#endif
	pinMode(RED_PWM, OUTPUT);
	pinMode(BLUE_PWM, OUTPUT);
	pinMode(GREEN_PWM, OUTPUT);
}

//-----------------------------------------------
void loop() {
	for (int i = 0; i < 255; i++) {
		analogWrite(RED_PWM, (i+50)%255);
		analogWrite(GREEN_PWM, i);
		analogWrite(BLUE_PWM, (i+100)%255);
		delay(20);
	}
	for (int i = 255; i > 0; i--) {
		analogWrite(RED_PWM, (i+30)%255);
		analogWrite(GREEN_PWM, (i+70)%255);
		analogWrite(BLUE_PWM, i);
		delay(20);
	}
}
