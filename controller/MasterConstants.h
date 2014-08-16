//-- Jimmy Chion
//-- Blumen Lumen, controller arduino code
//-- IDEO, 2014

//-- pin definitons

//
// 		              	 Arduino pro mini pin out
//						 (~ indicates PWM possible)
//      	               ___________________
//          	   NC    - |TX            RAW| - 5V
//          	   NC    - |RX            GND| - GND
//          	   NC    - |RST           RST| - NC
//         		   GND   - |GND           VCC| - NC
//  		   OPEN_BTN -> |2              A3| <- WIND_SIG
//  		  CLOSE_BTN -> |3 ~            A2| <- CdS_IN
// 				LED_TOG -> |4              A1| <- AUDIO_SIG
//     		   MODE_TOG -> |5 ~            A0| <- ROTARY_SELECT
//       		LED_MAP	<- |6 ~            13| - SCK
//				LED_OUT <- |7              12| - MISO
//     				NC 	 - |8            ~ 11| - MOSI
//             		CE   - |9 ~          ~ 10| - CSN
//               	 	   -------------------

#define OPEN_BTN 		2
#define CLOSE_BTN		3
#define LED_TOGGLE		4
#define MODE_TOGGLE		5
#define	LED_MAP			6
#define LED_OUT			7
#define ROTARY_SELECT 	A0
#define AUDIO_SIG		A1
#define CdS_IN			A2
#define WIND_SIG		A3

#define DEBOUNCE_DELAY 100

//-- values for the 12pos rotary switch analog
#define  ROTARY_SELECT_1 	1023
#define  ROTARY_SELECT_2 	950
#define  ROTARY_SELECT_3 	854
#define  ROTARY_SELECT_4 	759
#define  ROTARY_SELECT_5 	664
#define  ROTARY_SELECT_6 	568
#define  ROTARY_SELECT_7 	474
#define  ROTARY_SELECT_8 	377
#define  ROTARY_SELECT_9 	282
#define  ROTARY_SELECT_10 	183
#define  ROTARY_SELECT_11 	90
#define  ROTARY_SELECT_12 	0

#define ANALOG_MARGIN 30

#define N_LEDS 10


// Anemomenter details:
// range from 0.4V (0 m/s wind) up to 2.0V (for 32.4m/s wind speed)
// 0.4V is 82
// 2.0V is 409
#define MIN_WIND		80
#define MAX_WIND		409
#define MIN_SPEED		0
#define MAX_SPEED		32.4
