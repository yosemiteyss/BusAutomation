#ifndef ESP8266_H
#define ESP8266_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

#if defined ( __CC_ARM ) 
#pragma anon_unions 
#endif

typedef enum {
	STA,
	AP,
	STA_AP  
} ENUM_Net_ModeTypeDef;

typedef enum {
	enumTCP,
	enumUDP,
} ENUM_NetPro_TypeDef;

typedef enum {
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

typedef enum {
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


#define RX_BUF_MAX_LEN     																1024

extern struct STRUCT_USARTx_Fram                                                                
{
    char Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
    union {
        __IO uint16_t InfAll;
        struct {
		  __IO uint16_t FramLength       :15;                               
		  __IO uint16_t FramFinishFlag   :1;                                
	    } InfBit;
    }; 
	
} strEsp8266_Fram_Record;

// CH_PD
#define      ESP8266_CH_PD_PORT                            GPIOB
#define      ESP8266_CH_PD_PIN                             GPIO_PIN_8
#define      ESP8226_CH_PD_MODE                            GPIO_MODE_OUTPUT_PP

// RST
#define      ESP8266_RST_PORT                              GPIOB
#define      ESP8266_RST_PIN                               GPIO_PIN_9
#define      ESP8226_RST_MODE                            	 GPIO_MODE_OUTPUT_PP


// USART3
#define      ESP8266_USART_BAUD_RATE                       115200
#define      ESP8266_USARTx                                USART3  

#define      ESP8266_USART_TX_PORT                         GPIOB   
#define      ESP8266_USART_TX_PIN                          GPIO_PIN_10
#define		 	 ESP8226_USART_TX_MODE						  					 GPIO_MODE_AF_PP

#define      ESP8266_USART_RX_PORT                         GPIOB
#define      ESP8266_USART_RX_PIN                          GPIO_PIN_11
#define		   ESP8226_USART_RX_MODE						  					 GPIO_MODE_INPUT
#define		   ESP8226_USART_RX_PULL						  					 GPIO_NOPULL

#define      ESP8266_USART_IRQ                             USART3_IRQn
#define      ESP8266_USART_INT_FUN                      	 USART3_IRQHandler

// Clock
#define      ESP8226_GPIO_CLK_ENABLE()                     __HAL_RCC_GPIOB_CLK_ENABLE()
#define      ESP8226_USART_CLK_ENABLE()                    __HAL_RCC_USART3_CLK_ENABLE()
#define 		 ESP8266_USART_CLK_DISABLE()									 __HAL_RCC_USART3_CLK_DISABLE()

#define      ESP8266_CH_HIGH()                 					   HAL_GPIO_WritePin ( ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN, GPIO_PIN_SET )
#define      ESP8266_CH_LOW()                					     HAL_GPIO_WritePin ( ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN, GPIO_PIN_RESET )

#define      ESP8266_RST_HIGH()            								 HAL_GPIO_WritePin ( ESP8266_RST_PORT, ESP8266_RST_PIN, GPIO_PIN_SET )
#define      ESP8266_RST_LOW()             								 HAL_GPIO_WritePin ( ESP8266_RST_PORT, ESP8266_RST_PIN, GPIO_PIN_RESET )

#define 		 ESP8266_Usart( fmt, ... )                     USART_printf ( ESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
#define 		 PC_Usart( fmt, ... )                          printf ( fmt, ##__VA_ARGS__ )

extern 			 uint8_t esp8266_rxdata;


void                     ESP8266_Init                        ( void );
void 										 ESP8266_Enable											 ( void );
void 										 ESP8266_Stop												 ( void );
void                     ESP8266_Rst                         ( void );
bool                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, uint32_t waittime );
bool                     ESP8266_AT_Test                     ( void );
bool                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bool                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bool                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Get_LinkStatus              ( void );
uint8_t                  ESP8266_Get_IdLinkStatus            ( void );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
bool                     ESP8266_UnvarnishSend               ( void );
void                     ESP8266_ExitUnvarnishSend           ( void );
bool                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId );
char*                    ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );

#endif






