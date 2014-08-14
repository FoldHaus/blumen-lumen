#include "Radio.h"

RF24 radio(9,10);

Radio::Radio() {

}

Radio::~Radio() {

}

void Radio::init(uint8_t _addr) {
	// radio = new RF24(9,10);
	thisAddr = _addr;
	pipes[0] = 0xF0F0F0F0E1LL;
	pipes[1] = 0xF0F0F0F0D2LL;
	role_friendly_name[0] = "invalid";
	role_friendly_name[1] = "Ping out";
	role_friendly_name[2] = "Pong back";
	role = role_pong_back;

	radio.begin();

	// optionally, increase the delay between retries & # of retries
	radio.setRetries(15,15);

	//if ( role == role_ping_out ) {
		//radio.openWritingPipe(pipes[0]);
		radio.openReadingPipe(1,pipes[1]);
	// } else {
		//radio.openWritingPipe(pipes[1]);
		//radio.openReadingPipe(1,pipes[0]);
	// }
	radio.startListening();
	// radio.printDetails();
}

void Radio::update() {
	
}