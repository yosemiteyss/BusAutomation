#include "ui.h"
#include "lcd.h"
#include <stdbool.h>

uint8_t 		tab_current;
LogState		logState = LOGOUT;

Seat				seats[SEATS_MAX];

// Init
void 			UI_Init_Login 					( void );
void 			UI_Init_Logout 					( void );
void 			UI_Init_StatusBar				( void );
void 			UI_Init_NavigationBar		( void );
void			UI_Init_Seats						( void );
void			UI_Clear								( void );

// Tab
void			UI_Tab_Set							( uint8_t tabNo );
uint8_t		UI_Tab_Touched					( uint16_t usC , uint16_t usP );
void			UI_Tab_Draw							( void );

void 			UI_Seats_Update 				( void );



/*-----------------------------------------Init------------------------------------------------*/
void UI_Init ( void )
{
	if (logState == LOGIN)
		UI_Init_Login();
	
	else
		UI_Init_Logout();
}

void UI_Init_Login (void)
{
	UI_Init_StatusBar();
	UI_Init_NavigationBar();
	UI_Init_Seats();
	
	// Default tab
	UI_Tab_Set(0);
}

void UI_Init_Logout (void)
{
	LCD_DrawString(LOGOUT_MSG_X, LOGOUT_MSG_Y, LOGOUT_MSG, BLACK, BACKGROUND);
}

void UI_Init_StatusBar (void)
{
	LCD_FillColor(STATUS_BAR_WIDTH * STATUS_BAR_HEIGHT, BLACK);
	LCD_DrawString(STATUS_BAR_TITLE_OFFSET, STATUS_BAR_TITLE_OFFSET, STATUS_BAR_TITLE, WHITE, BLACK);
}

void UI_Init_NavigationBar (void)
{
	switch (tab_current)
	{
		case 0:	
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, BLACK);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_2, BLACK, BACKGROUND);
		
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, BLACK);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_3, BLACK, BACKGROUND);
			
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, RED);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_1, RED, BACKGROUND);
		
			break;
		
		case 1:	
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, BLACK);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_1, BLACK, BACKGROUND);
		
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, BLACK);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_3, BLACK, BACKGROUND);	
			
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, RED);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_2, RED, BACKGROUND);
		
			break;
		
		case 2:
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, BLACK);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_1, BLACK, BACKGROUND);
		
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, BLACK);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_2, BLACK, BACKGROUND);
			
			LCD_DrawRectangle(LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT, BOTTOM_NAV_ITEM_SIZE, BOTTOM_NAV_HEIGHT, RED);
			LCD_DrawString(LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE + BOTTOM_NAV_ITEM_OFFSET, LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT + BOTTOM_NAV_ITEM_OFFSET, BOTTOM_NAV_ITEM_3, RED, BACKGROUND);
		
			break;
		
		default:
			break;
	}
}


void UI_Init_Seats ( void )
{
	uint8_t i;
	
	for (i = 0; i < SEATS_MAX; ++i)
	{
		seats[i].id = i + 1;
		seats[i].avail = true;
	}
}

void UI_Clear ( void )
{
	LCD_Clear(LCD_DispWindow_Start_COLUMN, LCD_DispWindow_Start_PAGE + STATUS_BAR_HEIGHT, LCD_DispWindow_COLUMN, LCD_DispWindow_PAGE - STATUS_BAR_HEIGHT - BOTTOM_NAV_HEIGHT, BACKGROUND);
}



/*-----------------------------------------Tabs-----------------------------------------------*/
void UI_Tab_Set (uint8_t tabNo)
{
	tab_current = tabNo;
	
	UI_Init_NavigationBar();
	UI_Tab_Draw();
}

uint8_t UI_Tab_Touched ( uint16_t usC , uint16_t usP )
{
	if ( (usC < BOTTOM_NAV_ITEM_SIZE) && 
		   (usP >= (LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT) && usP < LCD_DispWindow_PAGE) )
		return 0;
	
	else if ( (usC >= (LCD_DispWindow_Start_COLUMN + 1 * BOTTOM_NAV_ITEM_SIZE) && usC < (LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE)) && 
		   (usP >= (LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT) && usP < LCD_DispWindow_PAGE) )
		return 1;
	
	else if ( (usC >= (LCD_DispWindow_Start_COLUMN + 2 * BOTTOM_NAV_ITEM_SIZE) && usC < (LCD_DispWindow_Start_COLUMN + 3 * BOTTOM_NAV_ITEM_SIZE)) && 
		   (usP >= (LCD_DispWindow_PAGE - BOTTOM_NAV_HEIGHT) && usP < LCD_DispWindow_PAGE) )
		return 2;
	
	return TAB_MAX;
}

void UI_Tab_Draw ( void )
{
	switch (tab_current)
	{
		case 0:
			UI_Clear();
			LCD_DrawString(STRING_TEMP_X, STRING_TEMP_Y, STRING_TEMP, BLACK, BACKGROUND);
			LCD_DrawString(STRING_HUMIDITY_X, STRING_HUMIDITY_Y, STRING_HUMIDITY, BLACK, BACKGROUND);
			LCD_DrawString(STRING_LIGHT_X, STRING_LIGHT_Y, STRING_LIGHT, BLACK, BACKGROUND);
			break;
		
		case 1:
			UI_Clear();
			UI_Seats_Update();
			break;
		
		case 2:
			UI_Clear();
			LCD_DrawString(STRING_GPS_LATITUDE_X, STRING_GPS_LATITUDE_Y, STRING_GPS_LATITUDE, BLACK, BACKGROUND);
			LCD_DrawString(STRING_GPS_LONGITUDE_X, STRING_GPS_LONGITUDE_Y, STRING_GPS_LONGITUDE, BLACK, BACKGROUND);
			LCD_DrawString(STRING_GPS_ALTITUDE_X, STRING_GPS_ALTITUDE_Y, STRING_GPS_ALTITUDE, BLACK, BACKGROUND);
			LCD_DrawString(STRING_GPS_SPEED_X, STRING_GPS_SPEED_Y, STRING_GPS_SPEED, BLACK, BACKGROUND);
			LCD_DrawString(STRING_GPS_DIRECTION_X, STRING_GPS_DIRECTION_Y, STRING_GPS_DIRECTION, BLACK, BACKGROUND);
			LCD_DrawString(STRING_GPS_IN_USE_X, STRING_GPS_IN_USE_Y, STRING_GPS_IN_USE, BLACK, BACKGROUND);
			LCD_DrawString(STRING_GPS_VISIBLE_X, STRING_GPS_VISIBLE_Y, STRING_GPS_VISIBLE, BLACK, BACKGROUND);
			break;
		
		default:
			break;
	}
}


void UI_Seats_Update ( void )
{
	if (tab_current == 1)
	{
		LCD_DrawSquare(SEAT_1_X, SEAT_1_Y, SEAT_SIZE, seats[0].avail ? GREEN : RED, true);
		LCD_DrawString(SEAT_1_X + 5, SEAT_1_Y + 5, "1", WHITE, seats[0].avail ? GREEN : RED);

		LCD_DrawSquare(SEAT_2_X, SEAT_2_Y, SEAT_SIZE, seats[1].avail ? GREEN : RED, true);
		LCD_DrawString(SEAT_2_X + 5, SEAT_2_Y + 5, "2", WHITE, seats[1].avail ? GREEN : RED);

		LCD_DrawSquare(SEAT_3_X, SEAT_3_Y, SEAT_SIZE, seats[2].avail ? GREEN : RED, true);
		LCD_DrawString(SEAT_3_X + 5, SEAT_3_Y + 5, "3", WHITE, seats[2].avail ? GREEN : RED);
	}
}



/*--------------------------------------------Seats---------------------------------------------*/
Seat* UI_Seat_Get ( uint8_t seatNo )
{
	return &seats[seatNo - 1];
}

void UI_Seat_Set ( uint8_t seatNo, bool avail )
{
	seats[seatNo - 1].avail = avail;
	UI_Seats_Update();
}

uint8_t UI_Seat_Remains ( void )
{
	uint8_t i, count = 0;
	
	for (i = 0; i < SEATS_MAX; ++i)
	{
		if (seats[i].avail)
			count++;
	}
	
	return count;
}



/*--------------------------------Functions-----------------------------------------*/
void UI_Login ( bool valid )
{
	if (valid)
	{
		logState = LOGIN;
		
		LCD_ClearScreen();
		UI_Init_Login();
	}
	
	else
	{
		LCD_DrawString(LOGIN_ERROR_X, LOGIN_ERROR_Y, LOGIN_ERROR, RED, BACKGROUND);
	}
}

void UI_Logout ( void )
{
	logState = LOGOUT;
}

LogState UI_LogState_Get ( void )			
{
	return logState;
}

void UI_Touch ( uint16_t usC , uint16_t usP )
{
	uint8_t tabNo = UI_Tab_Touched(usC, usP);
	UI_Tab_Set(tabNo);
}

void UI_Room_TempHumi_Set ( DHT11_Data* data )
{
	if (tab_current == 0)
	{
		char str[5] = "";

		sprintf(str, "%d.%d", data->temp_int, data->temp_deci);

		LCD_Clear(VALUE_TEMP_X, VALUE_TEMP_Y, 5 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_TEMP_X, VALUE_TEMP_Y, str, RED, BACKGROUND);

		sprintf(str, "%d.%d", data->humi_int, data->humi_deci);
		LCD_Clear(VALUE_HUMIDITY_X, VALUE_HUMIDITY_Y, 5 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_HUMIDITY_X, VALUE_HUMIDITY_Y, str, RED, BACKGROUND);
	}
}

void UI_Room_Light_Set ( uint32_t* data )
{
	if (tab_current == 0)
	{
		char str[5] = "";
		
		sprintf(str, "%u", *data);
		LCD_Clear(VALUE_LIGHT_X, VALUE_LIGHT_Y, 5 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_LIGHT_X, VALUE_LIGHT_Y, str, RED, BACKGROUND);
	}
}


void UI_GPS_Set ( double* deg_lat, double* deg_lon, nmeaINFO* info )
{
	if (tab_current == 2)
	{
		char str[18] = "";
			
		sprintf(str, "%f", *deg_lat);
		LCD_Clear(VALUE_GPS_LATITUDE_X, VALUE_GPS_LATITUDE_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_LATITUDE_X, VALUE_GPS_LATITUDE_Y, str, RED, BACKGROUND);
			
		sprintf(str, "%f", *deg_lon);
		LCD_Clear(VALUE_GPS_LONGITUDE_X, VALUE_GPS_LONGITUDE_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_LONGITUDE_X, VALUE_GPS_LONGITUDE_Y, str, RED, BACKGROUND);
			
		sprintf(str, "%f m", info->elv);
		LCD_Clear(VALUE_GPS_ALTITUDE_X, VALUE_GPS_ALTITUDE_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_ALTITUDE_X, VALUE_GPS_ALTITUDE_Y, str, RED, BACKGROUND);
			
		sprintf(str, "%f km/h", info->speed);
		LCD_Clear(VALUE_GPS_SPEED_X, VALUE_GPS_SPEED_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_SPEED_X, VALUE_GPS_SPEED_Y, str, RED, BACKGROUND);
			
		sprintf(str, "%f deg", info->direction);
		LCD_Clear(VALUE_GPS_DIRECTION_X, VALUE_GPS_DIRECTION_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_DIRECTION_X, VALUE_GPS_DIRECTION_Y, str, RED, BACKGROUND);
			
		sprintf(str, "%d", info->satinfo.inuse);
		LCD_Clear(VALUE_GPS_IN_USE_X, VALUE_GPS_IN_USE_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_IN_USE_X, VALUE_GPS_IN_USE_Y, str, RED, BACKGROUND);
			
		sprintf(str, "%d", info->satinfo.inview);
		LCD_Clear(VALUE_GPS_VISIBLE_X, VALUE_GPS_VISIBLE_Y, 18 * WIDTH_EN_CHAR, HEIGHT_EN_CHAR, BACKGROUND);
		LCD_DrawString(VALUE_GPS_VISIBLE_X, VALUE_GPS_VISIBLE_Y, str, RED, BACKGROUND);
	}
}

