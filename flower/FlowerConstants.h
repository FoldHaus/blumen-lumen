#pragma once

//-- Jimmy Chion
//-- Blumen Lumen, flower arduino code
//-- IDEO, 2014

//-- pin definitions

//
// 		              	 Arduino pro mini pin out
//						 (~ indicates PWM possible)
//      	               ___________________
//          	   NC   -  |TX            RAW| <- 5V
//          	   NC   -  |RX            GND| - GND
//          	   NC   -  |RST           RST| - NC
//         		   GND  -  |GND           VCC| - NC
//  				PIR	-> |2              A3| - NC
//  			BLUE    <~ |3 ~            A2| - NC
// 				LASER   <- |4              A1| - NC
//     			RED     <~ |5 ~            A0| - ULTRASONIC_SENSOR
//       		GREEN 	<~ |6 ~            13| <-> SCK
//			   MOT_IN_1 <- |7              12| <- MISO
//     		   MOT_IN_2 <- |8            ~ 11| -> MOSI
//             	CE	    <- |9 ~          ~ 10| -> SS
//               	 	   -------------------

#define PIR_SIG 	2
#define LED_B 		3
#define	LASER_SIG 	4
#define LED_R 		5
#define LED_G		6
#define MOT_IN_1	7
#define MOT_IN_2	8
#define ULTRASONIC 	A0
//-- pins 9-13 are defined by RF24 library

typedef enum { 
	ROLE_RECEIVER, 
	ROLE_TRANSMITTER
} ROLE_t;



// command protocol:
//--------------------------------
// START_BYTE 		0xAA
// RECEIVER_ID		0x10, 0x11,0x12... 0x1B, 0x1C (broadcast)
// MOTOR_MSG		0x20...0x2F
// LED_MSG			0x30...0x3F
// LASER_MSG		0x40...0x4F
// DATA_0			
// DATA_1			
// DATA_2		
// CHECKSUM			-	
// END_BYTE 		0xFF


// NULL_BYTE 		0x00



#define CMD_START_BYTE 		0xAA
#define CMD_MOTOR_OPEN		0x20
#define CMD_MOTOR_CLOSE		0x21
#define CMD_MOTOR_STOP		0x22
#define CMD_END_BYTE 		0xFF

#define CMD_BROADCAST 		0xAB

#define CMD_LENGTH 6 //-- includes checksum, everything
// 

























