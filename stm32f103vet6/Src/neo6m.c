#include "neo6m.h"
#include "usart.h"
#include "ui.h"

#include "cmsis_os.h"

uint8_t 			gps_rbuff [GPS_RBUFF_SIZE];
__IO uint8_t 	GPS_TransferEnd = 0;
__IO uint8_t	GPS_HalfTransferEnd = 0;


void GPS_Init ( void )
{
	HAL_UART_Receive_DMA(&huart2, gps_rbuff, GPS_RBUFF_SIZE);
}


void GPS_Trace (const char *str, int str_size)
{
  #ifdef __GPS_DEBUG    
    uint16_t i;
	
    printf("\r\nTrace: ");
	
    for(i = 0; i < str_size; i++)
      printf("%c", *(str + i));
  
    printf("\n");
	
  #endif
}

void GPS_Error (const char *str, int str_size)
{
	#ifdef __GPS_DEBUG 
    uint16_t i;
	
    printf("\r\nError: ");
	
    for (i = 0; i < str_size; i++)
      printf("%c", *(str + i));
	
    printf("\n");
	
  #endif
}

void GPS_Info (const char *str, int str_size)
{
	uint16_t i;

	printf("\r\nInfo: ");

	for(i = 0;i < str_size; i++)
		printf("%c", *(str + i));

	printf("\n");
}

static uint8_t IsLeapYear (uint8_t iYear) 
{ 
	uint16_t Year; 
	Year = 2000 + iYear; 

	if((Year & 3) == 0) 
	{ 
			return ((Year % 400 == 0) || (Year % 100 != 0)); 
	} 
	
	return 0; 
} 

void GPS_GMTconvert (nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t YY,MM,DD,hh,mm,ss;        
     
    if(GMT==0)    return;                
    if(GMT>12)    return;                 

    YY    =    SourceTime->year;               
    MM    =    SourceTime->mon;                
    DD    =    SourceTime->day;                 
    hh    =    SourceTime->hour;                
    mm    =    SourceTime->min;                 
    ss    =    SourceTime->sec;                 

    if(AREA)                        
    { 
        if(hh+GMT<24)    hh    +=    GMT;
        else                       
        { 
            hh    =    hh+GMT-24;        
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //??2?? 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))       
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //??12?? 
            { 
                if(DD<31)    DD++; 
                else        //?????? 
                {               
                    DD    =    1; 
                    MM    =    1; 
                    YY    ++; 
                } 
            } 
        } 
    } 
    else 
    {     
        if(hh>=GMT)    hh    -=    GMT;    
        else                        
        { 
            hh    =    hh+24-GMT;        //????? 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)   
            { 
                if((DD==1) && IsLeapYear(YY)==0)                   
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    
            { 
                if(DD>1)    DD--; 
                else         
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;               
    ConvertTime->mon    =    MM;               
    ConvertTime->day    =    DD;                
    ConvertTime->hour   =    hh;               
    ConvertTime->min    =    mm;               
    ConvertTime->sec    =    ss;               
}  

void GPS_Decode ( double* deg_lat, double* deg_lon, nmeaINFO* info, nmeaPARSER* parser, nmeaTIME* time, uint8_t* new_parse )
{
	/*
		double deg_lat;
		double deg_lon;
	
    nmeaINFO info;          
    nmeaPARSER parser;     
    uint8_t new_parse = 0;   
  
    nmeaTIME beiJingTime;   

    nmea_property()->trace_func = &GPS_Trace;
    nmea_property()->error_func = &GPS_Error;
    nmea_property()->info_func = &GPS_Info;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
	*/

		if (GPS_HalfTransferEnd)    
		{
			nmea_parse(parser, (const char*) &gps_rbuff[0], HALF_GPS_RBUFF_SIZE, info);
			
			GPS_HalfTransferEnd = 0;   
			*new_parse = 1;             
		}
		else if (GPS_TransferEnd)    
		{
			nmea_parse(parser, (const char*) &gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, info);
		 
			GPS_TransferEnd = 0;
			*new_parse = 1;
		}
		
		if (new_parse)                
		{    
			GPS_GMTconvert(&(info->utc), time, 8, 1);
			
			//printf("\r\nTime%d-%02d-%02d,%d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
			
			*deg_lat = nmea_ndeg2degree(info->lat);
			*deg_lon = nmea_ndeg2degree(info->lon);
			
			#ifdef __GPS_PRINT
				printf("latitude: %f, longitude %f\n", *deg_lat, *deg_lon);
				printf("altitude: %f m\n", info->elv);
				printf("Speed: %f km/h\n", info->speed);
				printf("Direction: %f deg\n", info->direction);
				printf("Using GPS: %d, Visible GPS: %d\n", info->satinfo.inuse, info->satinfo.inview);
				printf("Using BD :%d, Visible BD: %d\n", info->BDsatinfo.inuse, info->BDsatinfo.inview);
				printf("PDOP: %f, HDOP: %f, VDOP: %f\n\n", info->PDOP, info->HDOP, info->VDOP);
			#endif
				
			new_parse = 0;
		}
}
