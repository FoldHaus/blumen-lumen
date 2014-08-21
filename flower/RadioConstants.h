#pragma once

//-- Jimmy Chion
//-- Blumen Lumen, flower arduino code
//-- IDEO, 2014


typedef enum { 
	ROLE_RECEIVER, 
	ROLE_TRANSMITTER
} ROLE_t;



// command protocol:
//--------------------------------
// WAKEUP_BYTE (not seen) 0xAB
// 0: START_BYTE 		0xAA
// 1: CMD_TYPE			0x10 (motor command)
// 						0x11 (LED command)
//						0x12 (laser command)
//						0x13 (request for ultrasound)
//						0x14 (response to ultrasound request)
// 2: DATA_0			
// 3: DATA_1			
// 4: DATA_2		
// 5: DATA_3		
// 6: CHECKSUM			-	
// 7: END_BYTE 		0xEE


// NULL_BYTE 		0x00



#define CMD_WAKEUP_BYTE 		0xAB
#define CMD_START_BYTE 			0xAA

#define CMD_TYPE_MOTOR 			0x10 //-- Data1 should then be open, close, or stop
#define CMD_TYPE_LED_RGB 		0x11 //-- data1,2,3 should be r,g,b 
#define CMD_TYPE_LED 			0x12 //-- data 1 should be animation
#define CMD_TYPE_LASER 			0x13 //-- data1 should be on, off, or pulse, data 2 millisOn, data3 millisOff
#define CMD_TYPE_ULT_RQ			0x14 //-- no data
#define CMD_TYPE_REPLY 			0x15 //-- data1 should be response
#define CMD_SET_ULT_THRESH		0x16 //-- data1 should be threshold value
#define CMD_SET_MOTOR_OPEN_TIME	0x17 //-- data1,2,3,4 should be unsigned long time
#define CMD_SET_MOTOR_CLOSE_TIME 0x18 //-- data1,2,3,4 should be unsigned long time

//-- motor commands
#define CMD_MOTOR_OPEN			0x20
#define CMD_MOTOR_CLOSE			0x21
#define CMD_MOTOR_STOP			0x22

//-- LED animations
#define CMD_LED_OFF				0x30
#define CMD_LED_RAINBOW			0x31
#define CMD_LED_DROPLET			0x32
#define CMD_LED_SLOWFADE		0x33
#define CMD_LED_BEAT			0x34
#define CMD_LED_LSD				0x35


//-- LASER commands
#define CMD_LASER_ON			0x40
#define CMD_LASER_OFF			0x41
#define CMD_LASER_PULSE			0x42

//-- 
#define CMD_END_BYTE 			0xEE

#define CMD_LENGTH 				8 //-- includes checksum, everything
// 

#define CMD_TIMEOUT				800

#define EEPROM_ADDR_LOCATION 0


//-- 13 boards (0-12):
// 10 for blumen lumen
// 1 for camp
// 2 extra (not yet programmed with ID)

//-- ultrasound boards:
//-- 0,2,4

//-- 3 controllers (13-15):
// 1 master brain
// 2 remote controls

//-- master brain:
//-- 13
#define ID_MASTER 	13 // TODO make sure this is 13

//-- remote controls:
//-- 14, 15
#define ID_RC_A 	14
#define ID_RC_B 	15


#define N_ANIM		6 // not including ANIMATION_MANUAL

typedef enum { 
	ANIMATION_MANUAL,
	ANIMATION_OFF,
	ANIMATION_RAINBOW,
	ANIMATION_DROPLET,
	ANIMATION_SLOWFADE,
	ANIMATION_BEAT,
	ANIMATION_LSD
} ANIMATION_t;

#define DEFAULT_ANIM_DELAY 40
#define DEFAULT_ANIM_DURATION 300