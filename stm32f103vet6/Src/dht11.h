#ifndef DHT11_H
#define DHT11_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
	uint8_t humi_int;
	uint8_t humi_deci;
	uint8_t temp_int;
	uint8_t temp_deci;
	uint8_t check_sum;
	
} DHT11_Data;


#define         DHT11_GPIO_OUT_PORT             								GPIOC
#define         DHT11_GPIO_OUT_PIN              								GPIO_PIN_7
#define 				DHT11_GPIO_CLK_ENABLE()                 	  		__HAL_RCC_GPIOC_CLK_ENABLE()


#define					DHT11_Out_Read()																HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN )
#define					DHT11_Out_Set()																	HAL_GPIO_WritePin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN, GPIO_PIN_SET );
#define					DHT11_Out_Reset()																HAL_GPIO_WritePin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN, GPIO_PIN_RESET );


void            DHT11_Init                      ( void );
bool            DHT11_ReadData                  ( DHT11_Data* data );


#endif
