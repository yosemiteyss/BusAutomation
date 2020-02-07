#ifndef NEO6M_H
#define NEO6M_H

#include "stm32f1xx_hal.h"
#include "nmea/nmea.h"
		  
#define 	GPS_RBUFF_SIZE            512                   
#define 	HALF_GPS_RBUFF_SIZE       (GPS_RBUFF_SIZE / 2)    

//#define		__GPS_DEBUG								1
#define		__GPS_PRINT								1


extern uint8_t 					gps_rbuff [GPS_RBUFF_SIZE];
extern __IO uint8_t 		GPS_TransferEnd;
extern __IO uint8_t 		GPS_HalfTransferEnd;


void			GPS_Init						( void );
void			GPS_Trace						( const char *str, int str_size );
void			GPS_Error						( const char *str, int str_size );
void			GPS_Info						( const char *str, int str_size );
void			GPS_GMTconvert			( nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA );


void 			GPS_Decode 					( double* deg_lat, double* deg_lon, nmeaINFO* info, nmeaPARSER* parser, nmeaTIME* time, uint8_t* new_parse );

#endif
