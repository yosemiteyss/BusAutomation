#include "hx711.h"
#include "cmsis_os.h"

/*
void HX711_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HX711_GPIO_CLK_ENABLE();

	// DT
	GPIO_InitStruct.Pin = HX711_DT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(HX711_DT_PORT, &GPIO_InitStruct);

	// SCK 
	GPIO_InitStruct.Pin = HX711_SCK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(HX711_SCK_PORT, &GPIO_InitStruct);
	
}
*/

int HX711_Value(HX711_Data* data)
{
	uint32_t value = 0;
	uint8_t i;

	HAL_GPIO_WritePin(data->SCK_PORT, data->SCK_PIN, GPIO_PIN_RESET);

	//osDelay(2);
	
	while (HAL_GPIO_ReadPin(data->DT_PORT, data->DT_PIN) == GPIO_PIN_SET);
	
	for (i = 0; i < 24; ++i)
	{
			HAL_GPIO_WritePin(data->SCK_PORT, data->SCK_PIN, GPIO_PIN_SET);

			value = value << 1;

			if (HAL_GPIO_ReadPin(data->DT_PORT, data->DT_PIN) == GPIO_PIN_SET)
			{
					value++;
			}

			HAL_GPIO_WritePin(data->SCK_PORT, data->SCK_PIN, GPIO_PIN_RESET);
	}
	
	for (i = 0; i < data->gain; ++i)
	{
			HAL_GPIO_WritePin(data->SCK_PORT, data->SCK_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(data->DT_PORT, data->DT_PIN, GPIO_PIN_RESET);
	}

	value = value ^ 0x800000;
	
	return value;
}

int HX711_Average_Value(HX711_Data* data, uint8_t times)
{
	int sum = 0;
	int i;
	
	for (i = 0; i < times; ++i)
	{
			sum += HX711_Value(data);
	}
	

	return sum / times;
}

HX711_Data* HX711_Tare(HX711_Data* data, uint8_t times)
{
	int sum = HX711_Average_Value(data, times);
	data->offset = sum;

	return data;
}
