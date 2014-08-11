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

