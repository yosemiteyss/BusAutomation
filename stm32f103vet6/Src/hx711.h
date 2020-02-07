#ifndef HX711_H
#define HX711_H

#include "stm32f1xx_hal.h"

typedef struct 
{
	int offset;
	int gain;
	
	GPIO_TypeDef* SCK_PORT;
	uint16_t SCK_PIN;
	
	GPIO_TypeDef* DT_PORT;
	uint16_t DT_PIN;

} HX711_Data;


//#define 			HX711_SCK_PORT                      GPIOB
//#define 			HX711_SCK_PIN                       GPIO_PIN_14
//#define 			HX711_DT_PORT                       GPIOB
//#define 			HX711_DT_PIN                        GPIO_PIN_15

//#define 			HX711_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()


//void 					HX711_Init										( void );
HX711_Data* 	HX711_Tare										( HX711_Data* data, uint8_t times );
int 					HX711_Value										( HX711_Data* data );
int 					HX711_Average_Value						( HX711_Data* data, uint8_t times );


#endif
