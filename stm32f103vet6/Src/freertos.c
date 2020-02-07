/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "adc.h"
#include <string.h>
#include <stdlib.h>

#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "ui.h"
#include "xpt2046.h"
#include "hx711.h"
#include "dht11.h"
#include "rc522.h"
#include "esp8266.h"
#include "neo6m.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WEIGHT_MAX 					8700000
#define	LIGHT_THRES					60
#define RFID_CARD_ID 				"4A789303"

#define LOCALAP_SSID				"kefi234"
#define LOCALAP_PW					"Wtf67681436"
#define LOCALHOST_IP				"192.168.1.41"
#define LOCALHOST_PORT			"3000"

#define HTTP_MAX_LEN				250																				// 104
#define HTTP_DATA_LEN				140																				// 133
#define HTTP_GET						"GET /data HTTP/1.1\n"
#define HTTP_POST						"POST /data HTTP/1.1\r\n" \
														"Host: %s:%s\r\n" \
														"Content-Type: application/json\r\n" \
														"Content-Length: %d\r\n\r\n" \
														"%s\n"
				
#define HTTP_DATA 					"{\"temp\":\"%d.%d\",\"humidity\":\"%d.%d\"," \
														"\"light\":\"%u\",\"seats\":\"%d\",\"seat1\":\"%d\"," \
														"\"seat2\":\"%d\",\"seat3\":\"%d\",\"lat\":\"%f\",\"lon\":\"%f\"}"


//#define	TASK1_DEBUG					1
//#define	TASK2_DEBUG					1
//#define	TASK3_DEBUG					1
//#define	TASK4_DEBUG					1
//#define	TASK5_DEBUG					1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
DHT11_Data dht11_data;	
uint32_t ldr_data;
double deg_lat;
double deg_lon;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osThreadId myTask05Handle;
osMutexId myMutex01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void MainThread(void const * argument);
void Thread2(void const * argument);
void Thread3(void const * argument);
void Thread4(void const * argument);
void Thread5(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of myMutex01 */
  osMutexDef(myMutex01);
  myMutex01Handle = osMutexCreate(osMutex(myMutex01));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, MainThread, osPriorityNormal, 0, 150);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, Thread2, osPriorityBelowNormal, 0, 150);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, Thread3, osPriorityLow, 0, 150);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, Thread4, osPriorityLow, 0, 250);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* definition and creation of myTask05 */
  osThreadDef(myTask05, Thread5, osPriorityLow, 0, 1000);
  myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_MainThread */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_MainThread */
void MainThread(void const * argument)
{

  /* USER CODE BEGIN MainThread */
	UI_Init();
	
	// RC522		
	RC522_Init();
	RC522_PcdReset();
	RC522_M500PcdConfigISOType('A');
	
	// Waiting for login
	while (UI_LogState_Get() == LOGOUT)
	{
		// RC522
		uint8_t arrayID[4];
		uint8_t statusReturn = RC522_PcdRequest(PICC_REQALL, arrayID);
		
		if (statusReturn == MI_OK)
		{
			// Prevent Card Collusion
			if (RC522_PcdAnticoll(arrayID) == MI_OK) 
			{
				char str[8] = "";
				sprintf(str, "%02X%02X%02X%02X", arrayID[0], arrayID[1], arrayID[2], arrayID[3]);
				
				UI_Login(strcmp(str, RFID_CARD_ID) == 0);
			}
		}
	}

  /* Infinite loop */
  for(;;)
  {
		// Touch Event
		strType_XPT2046_Coordinate coordinate;
		XPT2046_TouchEvenHandler(&coordinate);
		
		if (LCD_Coordinate_Valid(coordinate.x, coordinate.y))
		{
			xSemaphoreTake(myMutex01Handle, portMAX_DELAY);
			UI_Touch(coordinate.x, coordinate.y);
			xSemaphoreGive(myMutex01Handle);
		}
		
		#ifdef TASK1_DEBUG
			UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
			printf("MainThread: %lu\n", uxHighWaterMark);
		#endif
	
		osDelay(1);
  }
	
  /* USER CODE END MainThread */
}

/* USER CODE BEGIN Header_Thread2 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread2 */
void Thread2(void const * argument)
{
  /* USER CODE BEGIN Thread2 */
	while (UI_LogState_Get() == LOGOUT);
	
	// DHT11
	DHT11_Init();
	
  /* Infinite loop */
  for(;;)
  {
		if (DHT11_ReadData(&dht11_data)) 
		{
			xSemaphoreTake(myMutex01Handle, portMAX_DELAY);
			UI_Room_TempHumi_Set(&dht11_data);
			xSemaphoreGive(myMutex01Handle);
		}
		
		#ifdef TASK2_DEBUG
			UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
			printf("Thread2: %lu\n", uxHighWaterMark);
		#endif
		
		osDelay(1);
  }
  /* USER CODE END Thread2 */
}

/* USER CODE BEGIN Header_Thread3 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread3 */
void Thread3(void const * argument)
{
  /* USER CODE BEGIN Thread3 */
	while (UI_LogState_Get() == LOGOUT);
	
	// HX711 Seats
	HX711_Data hx711_data[3] = { {0, 16, GPIOB, GPIO_PIN_14, GPIOB, GPIO_PIN_15}, 
															 {0, 16, GPIOB, GPIO_PIN_12, GPIOB, GPIO_PIN_13}, 
	                             {0, 16, GPIOC, GPIO_PIN_3, GPIOC, GPIO_PIN_4} };
	
	// LDR ADC														 
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start(&hadc1);
	
  /* Infinite loop */
  for(;;)
  {
		// HX711
		uint8_t i;
		
		for (i = 0; i < SEATS_MAX; ++i)
		{
			HX711_Tare(&hx711_data[i], 1);
			
			xSemaphoreTake(myMutex01Handle, portMAX_DELAY);
			UI_Seat_Set(i + 1, !(hx711_data[i].offset > WEIGHT_MAX));
			xSemaphoreGive(myMutex01Handle);
		}
		
		if (hx711_data[0].offset > WEIGHT_MAX && 
			  hx711_data[1].offset > WEIGHT_MAX && 
		    hx711_data[2].offset > WEIGHT_MAX)
		{
			BEEP_On();
		} 
		else
		{
			BEEP_Off();
		}
	
		
		// LDR ADC
		if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
		{
			ldr_data = HAL_ADC_GetValue(&hadc1);
			
			xSemaphoreTake(myMutex01Handle, portMAX_DELAY);
			UI_Room_Light_Set(&ldr_data);
			xSemaphoreGive(myMutex01Handle);
			
			if (ldr_data > LIGHT_THRES)
			{
				LED_Green_On();
			}
			else
			{
				LED_Reset();
			}
		}
		
		
		#ifdef TASK3_DEBUG
			UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
			printf("Thread3: %lu\n", uxHighWaterMark);
		#endif
		
    osDelay(1);
  }
  /* USER CODE END Thread3 */
}

/* USER CODE BEGIN Header_Thread4 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread4 */
void Thread4(void const * argument)
{
  /* USER CODE BEGIN Thread4 */
	while (UI_LogState_Get() == LOGOUT);
	
	// ESP8266
	ESP8266_Init();
	ESP8266_Enable();
	ESP8266_Net_Mode_Choose(STA);
	ESP8266_JoinAP(LOCALAP_SSID, LOCALAP_PW);
	ESP8266_Link_Server(enumTCP, LOCALHOST_IP, LOCALHOST_PORT, Single_ID_0);
	
  /* Infinite loop */
  for(;;)
  {
		// ESP8266 Upload Data
		char* sensor_data = (char*) pvPortMalloc(HTTP_DATA_LEN * sizeof(char));
		char* up_data = (char*) pvPortMalloc(HTTP_MAX_LEN * sizeof(char));
		
		snprintf(sensor_data, 
			HTTP_DATA_LEN, HTTP_DATA, 
			dht11_data.temp_int, dht11_data.temp_deci, dht11_data.humi_int, dht11_data.humi_deci, 
			ldr_data, 
			UI_Seat_Remains(), UI_Seat_Get(1)->avail, UI_Seat_Get(2)->avail, UI_Seat_Get(3)->avail,
			deg_lat, deg_lon);
		
		snprintf(up_data, HTTP_MAX_LEN, HTTP_POST, LOCALHOST_IP, LOCALHOST_PORT, strlen(sensor_data), sensor_data);
		
		//ESP8266_SendString(DISABLE, HTTP_GET, strlen(HTTP_GET), Single_ID_0);
		ESP8266_SendString(DISABLE, up_data, strlen(up_data), Single_ID_0);
		
		vPortFree(sensor_data);
		vPortFree(up_data);
		
		ESP8266_ExitUnvarnishSend();
		
		#ifdef TASK4_DEBUG
			UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
			printf("Thread4: %lu\n", uxHighWaterMark);
		#endif
	
    osDelay(1);
  }
  /* USER CODE END Thread4 */
}

/* USER CODE BEGIN Header_Thread5 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread5 */
void Thread5(void const * argument)
{
  /* USER CODE BEGIN Thread5 */
	while (UI_LogState_Get() == LOGOUT);
	
	// GPS
	GPS_Init();
	
  nmeaINFO info;          
  nmeaPARSER parser;
	nmeaTIME time;
	
  uint8_t new_parse = 0;   
 
	nmea_property()->trace_func = &GPS_Trace;
	nmea_property()->error_func = &GPS_Error;
	nmea_property()->info_func = &GPS_Info;

	nmea_zero_INFO(&info);
	nmea_parser_init(&parser);
	
  /* Infinite loop */
  for(;;)
  {
		GPS_Decode(&deg_lat, &deg_lon, &info, &parser, &time, &new_parse);
		
		xSemaphoreTake(myMutex01Handle, portMAX_DELAY);
		UI_GPS_Set(&deg_lat, &deg_lon, &info);
		xSemaphoreGive(myMutex01Handle);
		
		#ifdef TASK5_DEBUG
			UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
			printf("Thread5: %lu\n", uxHighWaterMark);
		#endif
		
    osDelay(1);
  }
  /* USER CODE END Thread5 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
