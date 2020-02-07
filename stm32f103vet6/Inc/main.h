/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define XPT2046_MOSI_Pin GPIO_PIN_2
#define XPT2046_MOSI_GPIO_Port GPIOE
#define XPT2046_MISO_Pin GPIO_PIN_3
#define XPT2046_MISO_GPIO_Port GPIOE
#define XPT2049_PENIRQ_Pin GPIO_PIN_4
#define XPT2049_PENIRQ_GPIO_Port GPIOE
#define RC522_RST_Pin GPIO_PIN_5
#define RC522_RST_GPIO_Port GPIOE
#define BUTTON_K2_Pin GPIO_PIN_13
#define BUTTON_K2_GPIO_Port GPIOC
#define BUTTON_K2_EXTI_IRQn EXTI15_10_IRQn
#define LDR_IN_Pin GPIO_PIN_2
#define LDR_IN_GPIO_Port GPIOC
#define HX711_3_SCK_Pin GPIO_PIN_3
#define HX711_3_SCK_GPIO_Port GPIOC
#define BUTTON_K1_Pin GPIO_PIN_0
#define BUTTON_K1_GPIO_Port GPIOA
#define BUTTON_K1_EXTI_IRQn EXTI0_IRQn
#define RC522_SDA_Pin GPIO_PIN_4
#define RC522_SDA_GPIO_Port GPIOA
#define RC522_SCK_Pin GPIO_PIN_5
#define RC522_SCK_GPIO_Port GPIOA
#define RC522_MISO_Pin GPIO_PIN_6
#define RC522_MISO_GPIO_Port GPIOA
#define RC522_MOSI_Pin GPIO_PIN_7
#define RC522_MOSI_GPIO_Port GPIOA
#define HX711_3_DT_Pin GPIO_PIN_4
#define HX711_3_DT_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_1
#define LED_B_GPIO_Port GPIOB
#define HX711_2_SCK_Pin GPIO_PIN_12
#define HX711_2_SCK_GPIO_Port GPIOB
#define HX711_2_DT_Pin GPIO_PIN_13
#define HX711_2_DT_GPIO_Port GPIOB
#define HX711_1_SCK_Pin GPIO_PIN_14
#define HX711_1_SCK_GPIO_Port GPIOB
#define HX711_1_DT_Pin GPIO_PIN_15
#define HX711_1_DT_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_12
#define LCD_BL_GPIO_Port GPIOD
#define XPT2046_CS_Pin GPIO_PIN_13
#define XPT2046_CS_GPIO_Port GPIOD
#define DHT11_OUT_Pin GPIO_PIN_7
#define DHT11_OUT_GPIO_Port GPIOC
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOB
#define ESP8266_CH_PD_Pin GPIO_PIN_8
#define ESP8266_CH_PD_GPIO_Port GPIOB
#define ESP8266_RST_Pin GPIO_PIN_9
#define ESP8266_RST_GPIO_Port GPIOB
#define XPT2046_CLK_Pin GPIO_PIN_0
#define XPT2046_CLK_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
