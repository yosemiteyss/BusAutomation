#include "xpt2046.h"
#include "lcd.h"

static void         XPT2046_DelayUS                       ( __IO uint32_t ulCount );
static void         XPT2046_WriteCMD                      ( uint8_t ucCmd );
static uint16_t     XPT2046_ReadCMD                       ( void );
static uint16_t     XPT2046_ReadAdc                       ( uint8_t ucChannel );
static void         XPT2046_ReadAdc_XY                    ( int16_t * sX_Ad, int16_t * sY_Ad );
static uint8_t      XPT2046_ReadAdc_Smooth_XY             ( strType_XPT2046_Coordinate * pScreenCoordinate );


strType_XPT2046_TouchPara strXPT2046_TouchPara[] = { 	
 -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,
	0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,
	0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,
	0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,
	0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,
 -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,
 -0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,
};


volatile uint8_t ucXPT2046_TouchFlag = 0;


void XPT2046_Init ( void )
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  XPT2046_SPI_GPIO_CLK_ENABLE();

	// CLK
	GPIO_InitStruct.Pin = XPT2046_SPI_CLK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStruct);
	
	// MOSI
	GPIO_InitStruct.Pin = XPT2046_SPI_MOSI_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStruct);
	
	// MISO
	GPIO_InitStruct.Pin = XPT2046_SPI_MISO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStruct);
	
	// CS
	GPIO_InitStruct.Pin = XPT2046_SPI_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStruct);

  XPT2046_CS_DISABLE();		
	
	// PENIRQ Interrupt
	GPIO_InitStruct.Pin = XPT2046_PENIRQ_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStruct);		
}

static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;

	for ( i = 0; i < ulCount; i++ )
	{
		uint8_t uc = 12;     
	      
		while ( uc -- );     

	}
}

static void XPT2046_WriteCMD ( uint8_t ucCmd ) 
{
	uint8_t i;

	XPT2046_MOSI_0 ();
	
	XPT2046_CLK_LOW ();

	for ( i = 0; i < 8; i ++ ) 
	{
		( ( ucCmd >> ( 7 - i ) ) & 0x01 ) ? XPT2046_MOSI_1() : XPT2046_MOSI_0();
		
	  XPT2046_DelayUS ( 5 );
		
		XPT2046_CLK_HIGH();

	  XPT2046_DelayUS ( 5 );

		XPT2046_CLK_LOW();
	}
	
}

static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;

	XPT2046_MOSI_0();

	XPT2046_CLK_HIGH();

	for ( i=0;i<12;i++ ) 
	{
		XPT2046_CLK_LOW();    
	
		usTemp = XPT2046_MISO();
		
		usBuf |= usTemp << ( 11 - i );
	
		XPT2046_CLK_HIGH();
		
	}
	
	return usBuf;

}

static uint16_t XPT2046_ReadAdc ( uint8_t ucChannel )
{
	XPT2046_WriteCMD ( ucChannel );

  return 	XPT2046_ReadCMD ();
	
}

static void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad )  
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 
	
	sX_Ad_Temp = XPT2046_ReadAdc ( XPT2046_CHANNEL_X );

	XPT2046_DelayUS ( 1 ); 

	sY_Ad_Temp = XPT2046_ReadAdc ( XPT2046_CHANNEL_Y ); 
	
	
	* sX_Ad = sX_Ad_Temp; 
	* sY_Ad = sY_Ad_Temp; 
	
	
}

#if   0			// Precise but slower
static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate )
{
	uint8_t ucCount = 0;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 9 ] = { { 0 }, { 0 } };  

	int32_t lAverage  [ 3 ], lDifference [ 3 ];
	

	do
	{		   
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++; 
			 
	} while ( ( XPT2046_EXTI_Read() == XPT2046_EXTI_ActiveLevel ) && ( ucCount < 9 ) ); 	
	 
	

	if ( XPT2046_EXTI_Read() != XPT2046_EXTI_ActiveLevel )
		ucXPT2046_TouchFlag = 0;				

	
	if ( ucCount == 9 )   								
	{  
		lAverage  [ 0 ] = ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + sBufferArray [ 0 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] ) / 3;
		
		lDifference [ 0 ] = lAverage  [ 0 ]-lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ]-lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ]-lAverage  [ 0 ];
		
		lDifference [ 0 ] = lDifference [ 0 ]>0?lDifference [ 0 ]: ( -lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ]>0?lDifference [ 1 ]: ( -lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ]>0?lDifference [ 2 ]: ( -lDifference [ 2 ] );
		
		
		if (  lDifference [ 0 ] > XPT2046_THRESHOLD_CalDiff  &&  lDifference [ 1 ] > XPT2046_THRESHOLD_CalDiff  &&  lDifference [ 2 ] > XPT2046_THRESHOLD_CalDiff  ) 
			return 0;
		
		
		if ( lDifference [ 0 ] < lDifference [ 1 ] )
		{
			if ( lDifference [ 2 ] < lDifference [ 0 ] ) 
				pScreenCoordinate ->x = ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
			else 
				pScreenCoordinate ->x = ( lAverage  [ 0 ] + lAverage  [ 1 ] ) / 2;	
		}
		
		else if ( lDifference [ 2 ] < lDifference [ 1 ] ) 
			pScreenCoordinate -> x = ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
		
		else 
			pScreenCoordinate ->x = ( lAverage  [ 1 ] + lAverage  [ 2 ] ) / 2;
		
		lAverage  [ 0 ] = ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + sBufferArray [ 1 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] ) / 3;
		
		lDifference [ 0 ] = lAverage  [ 0 ] - lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ] - lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ] - lAverage  [ 0 ];

		lDifference [ 0 ] = lDifference [ 0 ] > 0 ? lDifference [ 0 ] : ( - lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ] > 0 ? lDifference [ 1 ] : ( - lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ] > 0 ? lDifference [ 2 ] : ( - lDifference [ 2 ] );
		
		
		if ( lDifference [ 0 ] > XPT2046_THRESHOLD_CalDiff && lDifference [ 1 ] > XPT2046_THRESHOLD_CalDiff && lDifference [ 2 ] > XPT2046_THRESHOLD_CalDiff ) 
			return 0;
		
		if ( lDifference [ 0 ] < lDifference [ 1 ] )
		{
			if ( lDifference [ 2 ] < lDifference [ 0 ] ) 
				pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
			else 
				pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 1 ] ) / 2;	
		}
		else if ( lDifference [ 2 ] < lDifference [ 1 ] ) 
			pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
		else
			pScreenCoordinate ->y =  ( lAverage  [ 1 ] + lAverage  [ 2 ] ) / 2;
				
		return 1;		
	}
	
	else if ( ucCount > 1 )
	{
		pScreenCoordinate ->x = sBufferArray [ 0 ] [ 0 ];
		pScreenCoordinate ->y = sBufferArray [ 1 ] [ 0 ];	
		return 0;		
	}  	
	return 0; 	
}


#else 
static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate )
{
	uint8_t ucCount = 0, i;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  
	
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;


	do					       				
	{		  
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );  
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++;  
		
	}	while ( ( XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );
	
	
	if ( XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			


	if ( ucCount ==10 )		 					
	{
		lX_Max = lX_Min = sBufferArray [ 0 ] [ 0 ];
		lY_Max = lY_Min = sBufferArray [ 1 ] [ 0 ];       
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray[ 0 ] [ i ] < lX_Min )
				lX_Min = sBufferArray [ 0 ] [ i ];
			
			else if ( sBufferArray [ 0 ] [ i ] > lX_Max )
				lX_Max = sBufferArray [ 0 ] [ i ];

		}
		
		for ( i = 1; i < 10; i ++ )
		{
			if ( sBufferArray [ 1 ] [ i ] < lY_Min )
				lY_Min = sBufferArray [ 1 ] [ i ];
			
			else if ( sBufferArray [ 1 ] [ i ] > lY_Max )
				lY_Max = sBufferArray [ 1 ] [ i ];

		}
		

		pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
		                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;
		
		pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
		                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
		
		
		return 1;		
		
	}   	
	return 0;    	
}

#endif


uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           
	
	strType_XPT2046_Coordinate strScreenCoordinate; 
	

  if ( XPT2046_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
  {    
		pDisplayCoordinate ->x = ( ( pTouchPara[6].dX_X * strScreenCoordinate.x ) + ( pTouchPara[6].dX_Y * strScreenCoordinate.y ) + pTouchPara[6].dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara[6].dY_X * strScreenCoordinate.x ) + ( pTouchPara[6].dY_Y * strScreenCoordinate.y ) + pTouchPara[6].dY );

  }
	 
	else ucRet = 0;          
		
  return ucRet;
} 

uint8_t XPT2046_TouchDetect(void)
{ 
	static enumTouchState touch_state = XPT2046_STATE_RELEASE;
	static uint32_t i;
	uint8_t detectResult = TOUCH_NOT_PRESSED;
	
	switch(touch_state)
	{
		case XPT2046_STATE_RELEASE:	
			if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) 
			{
				touch_state = XPT2046_STATE_WAITING;
				detectResult =TOUCH_NOT_PRESSED;
				}
			else	
			{
				touch_state = XPT2046_STATE_RELEASE;
				detectResult =TOUCH_NOT_PRESSED;
			}
			break;
				
		case XPT2046_STATE_WAITING:
				if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)
				{
					 i++;
					
					if(i > DURIATION_TIME)		
					{
						i=0;
						touch_state = XPT2046_STATE_PRESSED;
						detectResult = TOUCH_PRESSED;
					}
					else												
					{
						touch_state = XPT2046_STATE_WAITING;
						detectResult =	 TOUCH_NOT_PRESSED;					
					}
				}
				else				
				{
				    i = 0;
            touch_state = XPT2046_STATE_RELEASE; 
						detectResult = TOUCH_NOT_PRESSED;
				}
		
			break;
		
		case XPT2046_STATE_PRESSED:	
				if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)		
				{
					touch_state = XPT2046_STATE_PRESSED;
					detectResult = TOUCH_PRESSED;
				}
				else	
				{
					touch_state = XPT2046_STATE_RELEASE;
					detectResult = TOUCH_NOT_PRESSED;
				}
			break;			
		
		default:
				touch_state = XPT2046_STATE_RELEASE;
				detectResult = TOUCH_NOT_PRESSED;
				break;
	
	}		
	
	return detectResult;
}

void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch)
{
	
	if(touch->pre_x == -1 && touch->pre_x == -1)
		return;
	
	
	// TouchDown function

  //Touch_Button_Down(touch->x,touch->y);
  
  
  //Draw_Trail(touch->pre_x,touch->pre_y,touch->x,touch->y,&brush);
	
}

void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch) 
{

	if(touch->pre_x == -1 && touch->pre_x == -1)
		return;
		
	// TouchUp function
	
  //Touch_Button_Up(touch->pre_x,touch->pre_y);	
	
}

void XPT2046_TouchEvenHandler(strType_XPT2046_Coordinate* cinfo)
{
	  //static strType_XPT2046_Coordinate cinfo = {-1,-1,-1,-1};
	
		cinfo->x = cinfo->y = cinfo->pre_x = cinfo->pre_y = -1;
	
		if(XPT2046_TouchDetect() == TOUCH_PRESSED)
		{
			XPT2046_Get_TouchedPoint(cinfo,strXPT2046_TouchPara);
			
			XPT2046_DEBUG("x=%d,y=%d", cinfo->x, cinfo->y);
			
			XPT2046_TouchDown(cinfo);
			
			cinfo->pre_x = cinfo->x; cinfo->pre_y = cinfo->y;  

		}
		else
		{
			XPT2046_TouchUp(cinfo); 
			
			cinfo->x = -1;
			cinfo->y = -1; 
			cinfo->pre_x = -1;
			cinfo->pre_y = -1;
		}

}

