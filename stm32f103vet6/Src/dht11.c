#include "dht11.h"
#include "delay.h"
#include "cmsis_os.h"

static void DHT11_Set_Output (void);
static void DHT11_Set_Input (void);
//static uint8_t DHT11_ReadByte (void);


void DHT11_Init (void) 
{
	//DWT_Delay_Init();
	
	DHT11_Set_Output();
	
	DHT11_GPIO_CLK_ENABLE();
	
	HAL_GPIO_WritePin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN, GPIO_PIN_SET );
}

static void DHT11_Set_Output (void) 
{		
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = DHT11_GPIO_OUT_PIN;											   
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; 
	HAL_GPIO_Init(DHT11_GPIO_OUT_PORT, &GPIO_InitStruct);
}

static void DHT11_Set_Input (void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = DHT11_GPIO_OUT_PIN;											   
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;   
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT11_GPIO_OUT_PORT, &GPIO_InitStruct);
}


bool DHT11_ReadData(DHT11_Data* data)
{
	TickType_t long_time = 0;
	uint32_t time_a = 0, time_b = 0;
	uint8_t i;
	
	DHT11_Set_Output();
	
	DHT11_Out_Reset();
	
	vTaskDelay (20);
	
	DHT11_Out_Set();
		
	DHT11_Set_Input();
	
	long_time = xTaskGetTickCount() + 10;
	
	while (DHT11_Out_Read() == 1) { if (xTaskGetTickCount() > long_time) { return false; } }
	while (DHT11_Out_Read() == 0) { if (xTaskGetTickCount() > long_time) { return false; } }
	while (DHT11_Out_Read() == 1) { if (xTaskGetTickCount() > long_time) { return false; } }
	
	for (i = 0; i < 8; i++)
	{
		while (DHT11_Out_Read() == 0) { time_a++; if (xTaskGetTickCount() > long_time) { return false; } }
		while (DHT11_Out_Read() == 1) { time_b++; if (xTaskGetTickCount() > long_time) { return false; } }
		
		data->humi_int <<= 1;
		
		if (time_b > time_a)
		{
			data->humi_int |= 0x01;
		}
		
		time_a = 0;
		time_b = 0;
	}
	
	for (i = 0; i < 8; i++)
	{
		while (DHT11_Out_Read() == 0) { time_a++; if (xTaskGetTickCount() > long_time) { return false; } }
		while (DHT11_Out_Read() == 1) { time_b++; if (xTaskGetTickCount() > long_time) { return false; } }
		
		data->humi_deci <<= 1;
		
		if (time_b > time_a)
		{
			data->humi_deci |= 0x01;
		}
		
		time_a = 0;
		time_b = 0;
	}
	
	for (i = 0; i < 8; i++)
	{
		while (DHT11_Out_Read() == 0) { time_a++; if (xTaskGetTickCount() > long_time) { return false; } }
		while (DHT11_Out_Read() == 1) { time_b++; if (xTaskGetTickCount() > long_time) { return false; } }
		
		data->temp_int <<= 1;
		
		if (time_b > time_a)
		{
			data->temp_int |= 0x01;
		}
		
		time_a = 0;
		time_b = 0;
	}
	
	for (i = 0; i < 8; i++)
	{
		while (DHT11_Out_Read() == 0) { time_a++; if (xTaskGetTickCount() > long_time) { return false; } }
		while (DHT11_Out_Read() == 1) { time_b++; if (xTaskGetTickCount() > long_time) { return false; } }
		
		data->temp_deci <<= 1;
		
		if (time_b > time_a)
		{
			data->temp_deci |= 0x01;
		}
		
		time_a = 0;
		time_b = 0;
	}
	
	for (i = 0; i < 8; i++)
	{
		while (DHT11_Out_Read() == 0) { time_a++; if (xTaskGetTickCount() > long_time) { return false; } }
		while (DHT11_Out_Read() == 1) { time_b++; if (xTaskGetTickCount() > long_time) { return false; } }
		
		data->check_sum <<= 1;
		
		if (time_b > time_a)
		{
			data->check_sum |= 0x01;
		}
		
		time_a = 0;
		time_b = 0;
	}
	
	if (data->check_sum == 0)
	{
		return false;
	}
	else
	{
		if (data->check_sum == (data->humi_int + data->humi_deci + data->temp_int + data->temp_deci))
		{
			return true;
		}
	}
	

	return false;
	
}


/*
static uint8_t DHT11_ReadByte(void) 
{
	uint8_t i;
	uint8_t temp = 0;

	for (i = 0; i < 8; ++i) {
			while (HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN ) == GPIO_PIN_RESET);

			delay_us(40);

			if (HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN ) == GPIO_PIN_SET) {
					while (HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN ) == GPIO_PIN_SET);

					temp |= (uint8_t) (0x01 << (7 - i));
			} else {
					temp &= (uint8_t) ~ (0x01 << (7 - i));
			}
	}

	return temp;
}

bool DHT11_ReadData(DHT11_Data* data)
{
	DHT11_Set_Output();

	HAL_GPIO_WritePin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN, GPIO_PIN_RESET );
	delay_us(18 * 1000);
	
	HAL_GPIO_WritePin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN, GPIO_PIN_SET );
	delay_us(30);

	DHT11_Set_Input();

	if (HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN ) == GPIO_PIN_RESET) {
		while (HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN ) == GPIO_PIN_RESET);

		while (HAL_GPIO_ReadPin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN ) == GPIO_PIN_SET);

		data->humi_int = DHT11_ReadByte();
		data->humi_deci = DHT11_ReadByte();
		data->temp_int = DHT11_ReadByte();
		data->temp_deci = DHT11_ReadByte();
		data->check_sum = DHT11_ReadByte();

		DHT11_Set_Output();
		
		HAL_GPIO_WritePin ( DHT11_GPIO_OUT_PORT, DHT11_GPIO_OUT_PIN, GPIO_PIN_SET );

		if (data->check_sum == data->humi_int + data->humi_deci + data->temp_int + data->temp_deci) {
				return true;
		}
					
		return false;
	}

	return false;
}
*/
