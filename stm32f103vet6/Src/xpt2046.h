#ifndef XPT2046_H
#define XPT2046_H

#include "stm32f1xx_hal.h"


// Non-interrupt
#define		XPT2046_PENIRQ_GPIO_PORT						GPIOE
#define		XPT2046_PENIRQ_GPIO_PIN							GPIO_PIN_4
#define   XPT2046_PENIRQ_ActiveLevel          GPIO_PIN_RESET
#define   XPT2046_PENIRQ_Read()               HAL_GPIO_ReadPin ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )


// XPT2046 GPIO
#define   XPT2046_SPI_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE(); \
                                               __HAL_RCC_GPIOE_CLK_ENABLE();

#define   XPT2046_SPI_CS_PIN		              GPIO_PIN_13
#define   XPT2046_SPI_CS_PORT		              GPIOD

#define	  XPT2046_SPI_CLK_PIN	                GPIO_PIN_0
#define		XPT2046_SPI_CLK_PORT	              GPIOE

#define	  XPT2046_SPI_MOSI_PIN	              GPIO_PIN_2
#define	  XPT2046_SPI_MOSI_PORT	              GPIOE

#define	  XPT2046_SPI_MISO_PIN	              GPIO_PIN_3
#define	  XPT2046_SPI_MISO_PORT	              GPIOE

#define   XPT2046_CS_ENABLE()                HAL_GPIO_WritePin ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, GPIO_PIN_SET )
#define   XPT2046_CS_DISABLE()               HAL_GPIO_WritePin ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, GPIO_PIN_RESET )  

#define   XPT2046_CLK_HIGH()                 HAL_GPIO_WritePin ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, GPIO_PIN_SET )    
#define   XPT2046_CLK_LOW()                  HAL_GPIO_WritePin ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, GPIO_PIN_RESET ) 

#define   XPT2046_MOSI_1()                   HAL_GPIO_WritePin ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, GPIO_PIN_SET ) 
#define   XPT2046_MOSI_0()                   HAL_GPIO_WritePin ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, GPIO_PIN_RESET )

#define   XPT2046_MISO()                     HAL_GPIO_ReadPin ( XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN )



#define   XPT2046_THRESHOLD_CalDiff           2               
#define	  XPT2046_CHANNEL_X 	                0x90 	          
#define	  XPT2046_CHANNEL_Y 	                0xd0	         
#define		FLASH_TOUCH_PARA_FLAG_VALUE					0xA5
#define 	FLASH_TOUCH_PARA_FLAG_ADDR					(1*1024)
#define 	FLASH_TOUCH_PARA_ADDR								(2*1024)


#define 	XPT2046_DEBUG_ON         						0

#define 	XPT2046_INFO(fmt,arg...)           	printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define 	XPT2046_ERROR(fmt,arg...)          	printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)
#define 	XPT2046_DEBUG(fmt,arg...)          	do {\
																								if (XPT2046_DEBUG_ON)\
																									printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
																							} while (0)


typedef	struct        
{		
   int16_t x;			
   int16_t y; 
	 int16_t pre_x;		
   int16_t pre_y;
	
} strType_XPT2046_Coordinate;   


typedef struct       
{
	 float An,  		
				 Bn,     
				 Cn,   
				 Dn,    
				 En,    
				 Fn,     
				 Divider;
	 
} strType_XPT2046_Calibration;

typedef struct         
{
	 float dX_X,  			 
				 dX_Y,     
				 dX,   
				 dY_X,    
				 dY_Y,    
				 dY;

} strType_XPT2046_TouchPara;

typedef enum
{
	XPT2046_STATE_RELEASE = 0,	
	XPT2046_STATE_WAITING,			
	XPT2046_STATE_PRESSED,			
	
} enumTouchState	;


#define		TOUCH_PRESSED 								1
#define		TOUCH_NOT_PRESSED							0
#define 	DURIATION_TIME								2


extern volatile uint8_t               ucXPT2046_TouchFlag;
extern strType_XPT2046_TouchPara      strXPT2046_TouchPara[];


void					XPT2046_Init																		( void );
uint8_t				XPT2046_Get_TouchedPoint												( strType_XPT2046_Coordinate* displayPtr, strType_XPT2046_TouchPara* para );
void					XPT2046_TouchDown																( strType_XPT2046_Coordinate* touch );
void					XPT2046_TouchUp																	( strType_XPT2046_Coordinate* touch );
void					XPT2046_TouchEvenHandler												( strType_XPT2046_Coordinate* cinfo );


#endif
