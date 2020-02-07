#ifndef UI_H
#define UI_H

#include "stm32f1xx_hal.h"
#include "dht11.h"
#include "nmea/nmea.h"
#include <stdbool.h>

#define		STATUS_BAR_WIDTH						240
#define		STATUS_BAR_HEIGHT						25
#define		STATUS_BAR_TITLE_OFFSET			4
#define		STATUS_BAR_TITLE						"Bus Automation 1.0"

#define		BOTTOM_NAV_WIDTH						240
#define		BOTTOM_NAV_HEIGHT						40
#define		BOTTOM_NAV_ITEM_SIZE				80
#define		BOTTOM_NAV_ITEM_OFFSET			4
#define		BOTTOM_NAV_ITEM_1						"ROOM"
#define		BOTTOM_NAV_ITEM_2						"SEATS"
#define		BOTTOM_NAV_ITEM_3						"GPS"

#define		BUZZ_X											50
#define		BUZZ_Y											50
#define		BUZZ_SIZE										20	

#define		TAB_MAX											3

// Temperature
#define		STRING_TEMP									"Temperature"
#define		STRING_TEMP_X								15
#define		STRING_TEMP_Y								70
#define 	VALUE_TEMP_X								200
#define 	VALUE_TEMP_Y								70

// Humidity
#define		STRING_HUMIDITY							"Humidity"
#define 	STRING_HUMIDITY_X						15
#define 	STRING_HUMIDITY_Y						110
#define 	VALUE_HUMIDITY_X						200
#define 	VALUE_HUMIDITY_Y						110

// Light
#define		STRING_LIGHT								"Light"
#define 	STRING_LIGHT_X							15
#define 	STRING_LIGHT_Y							150
#define 	VALUE_LIGHT_X								200
#define 	VALUE_LIGHT_Y								150

// RFID
#define		STRING_RFID									"RFID"
#define 	VALUE_RFID_X								70
#define 	VALUE_RFID_Y								220

// Seats
#define		SEATS_MAX										3
#define		SEAT_SIZE										50
#define		SEAT_1_X										40
#define		SEAT_1_Y										70
#define		SEAT_2_X										140
#define		SEAT_2_Y										70
#define		SEAT_3_X										40
#define		SEAT_3_Y										170

// Logout
#define		LOGOUT_MSG									"Please use RFID to login"
#define		LOGOUT_MSG_X								20
#define		LOGOUT_MSG_Y								50
#define		LOGIN_ERROR									"Invalid"
#define		LOGIN_ERROR_X								20
#define		LOGIN_ERROR_Y								100
#define		LOGIN_SUCCESS								"Success"
#define		LOGIN_SUCCESS_X							20
#define		LOGIN_SUCCESS_Y							100	


// GPS
#define		STRING_GPS_LATITUDE					"Latitude"
#define		STRING_GPS_LATITUDE_X				15	
#define		STRING_GPS_LATITUDE_Y				50
#define		VALUE_GPS_LATITUDE_X				110	
#define		VALUE_GPS_LATITUDE_Y				50

#define		STRING_GPS_LONGITUDE				"Longitude"
#define		STRING_GPS_LONGITUDE_X			15	
#define		STRING_GPS_LONGITUDE_Y			80
#define		VALUE_GPS_LONGITUDE_X				110	
#define		VALUE_GPS_LONGITUDE_Y				80

#define		STRING_GPS_ALTITUDE					"Altitude"
#define		STRING_GPS_ALTITUDE_X				15	
#define		STRING_GPS_ALTITUDE_Y				110
#define		VALUE_GPS_ALTITUDE_X				110
#define		VALUE_GPS_ALTITUDE_Y				110

#define		STRING_GPS_SPEED						"Speed"
#define		STRING_GPS_SPEED_X					15
#define		STRING_GPS_SPEED_Y					140
#define		VALUE_GPS_SPEED_X						110
#define		VALUE_GPS_SPEED_Y						140

#define		STRING_GPS_DIRECTION				"Direction"
#define		STRING_GPS_DIRECTION_X			15
#define		STRING_GPS_DIRECTION_Y			170
#define		VALUE_GPS_DIRECTION_X				110
#define		VALUE_GPS_DIRECTION_Y				170

#define		STRING_GPS_IN_USE						"In Use"
#define		STRING_GPS_IN_USE_X					15
#define		STRING_GPS_IN_USE_Y					200
#define		VALUE_GPS_IN_USE_X					110
#define		VALUE_GPS_IN_USE_Y					200

#define		STRING_GPS_VISIBLE					"Visible"
#define		STRING_GPS_VISIBLE_X				15
#define		STRING_GPS_VISIBLE_Y				230
#define		VALUE_GPS_VISIBLE_X					110
#define		VALUE_GPS_VISIBLE_Y					230

typedef	struct        
{			
	uint8_t id;			
	bool avail;

} Seat; 

typedef enum
{
	LOGIN,
	LOGOUT
} LogState;


void			UI_Init 										( void );


void			UI_Update										( void );

void			UI_Login										( bool valid );
void			UI_Logout										( void );
LogState	UI_LogState_Get							( void );				

Seat*			UI_Seat_Get									( uint8_t seatNo );
void			UI_Seat_Set									( uint8_t seatNo, bool avail );
uint8_t		UI_Seat_Remains							( void );

void			UI_Touch										( uint16_t usC , uint16_t usP );
void			UI_Room_TempHumi_Set				( DHT11_Data* data );
void 			UI_Room_Light_Set 					( uint32_t* data );
void			UI_GPS_Set									( double* deg_lat, double* deg_lon, nmeaINFO* info );
	
#endif
