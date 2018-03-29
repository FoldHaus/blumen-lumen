#pragma once
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
#define N_FLOWERS 10


/*** SENSORS ***/
<<<<<<< HEAD:controller/MasterConstants.h
#define SENSORS_UPDATE_PERIOD 3000
=======
#define SENSORS_UPDATE_PERIOD 10000
>>>>>>> treasure-island-14:repeater/ControllerConstants.h

// Ultrasonic distance sensor
#define ULTRASONIC_REQUEST_TIMEOUT 500
#define N_ULTRASONIC 3

// Sound detector
// Output voltage range: 0.01V to 2.69V
#define MIN_BPM 100
#define MAX_BPM 250

// Anemomenter
// range from 0.4V (0 m/s wind) up to 2.0V (for 32.4m/s wind speed)
// 0.4V is 82
// 2.0V is 409
#define MIN_WIND		80
#define MAX_WIND		409
#define MIN_SPEED		0
#define MAX_SPEED		32.4

#define WIND_SPEED_CALM_TIME 10000

#define CONTROLLER_ANIM_TIMEOUT 30000 //TODO is this enough/too much?

#define PASSIVE_OPEN_CLOSE_TIME 60000 //-- 40s

#define NO_MOTION_FOR_A_WHILE_TIME 720000 //-- six minutes

typedef enum {
	EV_ENTRY,
	EV_EXIT,
	WIND_SPEED_EXCESSIVE,
	WIND_SPEED_NOMINAL,
	NIGHT_TIME_DETECTED,
	DAY_TIME_DETECTED,
	MOTION_DETECTED,
	NO_MOTION_FOR_A_WHILE,
	NO_EVENT
} EVENT_t;

typedef enum {
	STANDBY,
	DAY_PASSIVE,
	DAY_REACTIVE,
	NIGHT_REACTIVE,
	NIGHT_PASSIVE,
	NIGHT_PARTY
} STATE_t;
