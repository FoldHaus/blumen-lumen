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

