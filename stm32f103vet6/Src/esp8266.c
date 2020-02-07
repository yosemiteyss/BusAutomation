#include "esp8266.h"
#include "cmsis_os.h"
#include "usart.h"
#include <string.h>
#include <stdarg.h>

uint8_t 									esp8266_rxdata;
struct STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

void 					USART_printf ( USART_TypeDef * USARTx, char * Data, ... );
static char* 	itoa ( int value, char * string, int radix );

/*
void ESP8266_Init ( void )
{
	// GPIO
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	ESP8226_GPIO_CLK_ENABLE();
					
	// CH_PD	
	GPIO_InitStruct.Pin = ESP8266_CH_PD_PIN;	
	GPIO_InitStruct.Mode = ESP8226_CH_PD_MODE;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
	HAL_GPIO_Init(ESP8266_CH_PD_PORT, &GPIO_InitStruct);
	
	// USART3 TX
	GPIO_InitStruct.Pin =  ESP8266_USART_TX_PIN;
	GPIO_InitStruct.Mode = ESP8226_USART_TX_MODE;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP8266_USART_TX_PORT, &GPIO_InitStruct);  
  
	// USART3 RX
	GPIO_InitStruct.Pin = ESP8266_USART_RX_PIN;
	GPIO_InitStruct.Mode = ESP8226_USART_RX_MODE;
	GPIO_InitStruct.Pull = ESP8226_USART_RX_PULL;
	HAL_GPIO_Init(ESP8266_USART_RX_PORT, &GPIO_InitStruct);

	// RST										   
	GPIO_InitStruct.Pin = ESP8266_RST_PIN;	
	GPIO_InitStruct.Mode = ESP8226_RST_MODE;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP8266_RST_PORT, &GPIO_InitStruct);
	
	
	// USART3
	ESP8226_USART_CLK_ENABLE();
	
	huart3.Instance = ESP8266_USARTx;
	huart3.Init.BaudRate = ESP8266_USART_BAUD_RATE;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart3);
	
	// NVIC
	HAL_NVIC_SetPriority(ESP8266_USART_IRQ, 5, 0);
	HAL_NVIC_EnableIRQ(ESP8266_USART_IRQ);
	
	
	HAL_UART_Receive_IT(&huart3, &esp8266_rxdata, 1);
}
*/

void ESP8266_Init ( void )
{
	HAL_UART_Receive_IT(&huart3, &esp8266_rxdata, 1);
}

void ESP8266_Enable ( void )
{
	ESP8266_CH_HIGH();
	ESP8266_RST_HIGH();
	osDelay(1000);
}


void ESP8266_Stop ( void ) {
	ESP8266_RST_LOW();
	
	HAL_NVIC_DisableIRQ(ESP8266_USART_IRQ);
	
	ESP8266_USART_CLK_DISABLE();
}


void ESP8266_Rst ( void )
{
	#if 0
	    ESP8266_Cmd ("AT+RST", "OK", "ready", 2500);   	
	
	#else
	    ESP8266_RST_LOW();
	    osDelay ( 500 ); 
	    ESP8266_RST_HIGH();
	#endif
}


bool ESP8266_Cmd ( char* cmd, char* reply1, char* reply2, uint32_t waittime ) {    
	strEsp8266_Fram_Record.InfBit.FramLength = 0;               

	ESP8266_Usart("%s\r\n", cmd);

	if ( reply1 == 0 && reply2 == 0 )                             
		return true;
	
	osDelay(waittime);                 
	
	strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
	
	
	// Print on USART1
	//PC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);
  
	if ( reply1 != 0 && reply2 != 0 ) {
		return ( (bool) strstr (strEsp8266_Fram_Record.Data_RX_BUF, reply1) || 
             (bool) strstr (strEsp8266_Fram_Record.Data_RX_BUF, reply2) ); 
	}
 	
	else if ( reply1 != 0 ) {
		return ( (bool) strstr (strEsp8266_Fram_Record.Data_RX_BUF, reply1) );
	}
	
	else {
		return ( (bool) strstr (strEsp8266_Fram_Record.Data_RX_BUF, reply2) );
	}
}


bool ESP8266_AT_Test ( void ) {
	ESP8266_RST_HIGH();

	osDelay(1000);

	if (ESP8266_Cmd( "AT", "OK", NULL, 500)) 
			return true;

	ESP8266_Rst();
	
	osDelay(1000);
	
	return false;
}


bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode ) {
	switch (enumMode)
	{
		case STA:
			return ESP8266_Cmd("AT+CWMODE=1", "OK", "no change", 2500); 
		
		case AP:
			return ESP8266_Cmd("AT+CWMODE=2", "OK", "no change", 2500);
		
		case STA_AP:
			return ESP8266_Cmd("AT+CWMODE=3", "OK", "no change", 2500); 
		
		default:
			return false;
	}
}


bool ESP8266_JoinAP ( char* pSSID, char* pPassWord )
{
	char cCmd [120];

	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);
	
	return ESP8266_Cmd(cCmd, "OK", NULL, 5000);
}


bool ESP8266_BuildAP ( char* pSSID, char* pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf(cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode);
	
	return ESP8266_Cmd(cCmd, "OK", 0, 1000);
}


bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));
	
	return ESP8266_Cmd(cStr, "OK", 0, 500);
}


bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

	switch (enumE)
	{
			case enumTCP:
				sprintf(cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum);
				break;
	
			case enumUDP:
				sprintf(cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum);
				break;
			
			default:
				break;
	}

	if (id < 5) {
			sprintf(cCmd, "AT+CIPSTART=%d,%s", id, cStr);
	} else {
			sprintf(cCmd, "AT+CIPSTART=%s", cStr);
	}

	return ESP8266_Cmd(cCmd, "OK", "ALREAY CONNECT", 4000);
}


bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char* pPortNum, char* pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if (enumMode)
	{
		sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum);
		
		sprintf(cCmd2, "AT+CIPSTO=%s", pTimeOver);

		return ( ESP8266_Cmd(cCmd1, "OK", 0, 500) &&
				 ESP8266_Cmd(cCmd2, "OK", 0, 500) );
	} else {
		sprintf (cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum);

		return ESP8266_Cmd(cCmd1, "OK", 0, 500);
	}
}


uint8_t ESP8266_Get_LinkStatus ( void )
{
	if (ESP8266_Cmd("AT+CIPSTATUS", "OK", 0, 500))
	{
		if (strstr(strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:2\r\n"))
			return 2;
		
		else if (strstr(strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:3\r\n"))
			return 3;
		
		else if (strstr(strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:4\r\n"))
			return 4;		
	}
	
	return 0;
}


uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	if (ESP8266_Cmd("AT+CIPSTATUS", "OK", 0, 500))
	{
		if (strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0,"))
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if (strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1,"))
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if (strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2,"))
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if (strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3,"))
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if (strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4,"))
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	
	return ucIdLinkStatus;
}


uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	char* pCh;
	
	ESP8266_Cmd("AT+CIFSR", "OK", 0, 500);
	
	pCh = strstr(strEsp8266_Fram_Record.Data_RX_BUF, "APIP,\"");
	
	if (pCh)
		pCh += 6;
	else
		return 0;
	
	for (uc = 0; uc < ucArrayLength; ++uc)
	{
		pApIp[uc] = *(pCh + uc);
		
		if (pApIp [ uc ] == '\"')
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	
	return 1;
}


bool ESP8266_UnvarnishSend ( void )
{
	if (!ESP8266_Cmd("AT+CIPMODE=1", "OK", 0, 500))
		return false;
	
	return ESP8266_Cmd("AT+CIPSEND", "OK", ">", 500);
}


void ESP8266_ExitUnvarnishSend ( void )
{
	osDelay(1000);
	ESP8266_Usart("+++");
	osDelay(500); 
}


bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if (enumEnUnvarnishTx)
	{
		ESP8266_Usart("%s", pStr);
		bRet = true;
	} 
	else 
	{
		if (ucId < 5)
			sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);

		else
			sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);
		
		ESP8266_Cmd(cStr, "> ", 0, 1000);

		bRet = ESP8266_Cmd(pStr, "SEND OK", 0, 1000);
  }
	
	return bRet;
}


char* ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char* pRecStr = 0;
	
	strEsp8266_Fram_Record.InfBit.FramLength = 0;
	strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
	
	while (!strEsp8266_Fram_Record.InfBit.FramFinishFlag);

	strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
	
	if (enumEnUnvarnishTx) 
	{
		pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;
  }
  else 
	{
		if (strstr(strEsp8266_Fram_Record.Data_RX_BUF, "+IPD"))
			pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;

	}

	return pRecStr;
}

void USART_printf(USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	uint8_t txdata;
   
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0 )    
	{                                                          
		if ( *Data == 0x5c )  
		{                                                                          
			switch ( *++Data )
			{
				case 'r':                                                                  
					txdata = 0x0d;
					HAL_UART_Transmit(&huart3, &txdata, 1, 0xFF);
					Data++;
					break;
				
				case 'n':                                                                 
					txdata = 0x0a;
					HAL_UART_Transmit(&huart3, &txdata, 1, 0xFF);
					Data++;
					break;
				
				default:
					Data++;
					break;
			}                         
		}
		else if ( *Data == '%' )
		{                                                                          
			switch ( *++Data )
			{                                
				case 's':                                                                                  
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						HAL_UART_Transmit(&huart3, (uint8_t *) s, 1, 0xFF);
						while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) == 0);
					}                                
					Data++;                                
					break;
					
				case 'd':                        											
					d = va_arg(ap, int);                                        
					itoa(d, buf, 10);                                        
					for (s = buf; *s; s++) 
					{
						HAL_UART_Transmit(&huart3, (uint8_t*) s, 1, 0xFF);
						while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) == 0);
					}                                        
					Data++;                                
					break;  
					
				default:
					Data++;                                
					break;                                
			}                 
   }                
   else
   {
      HAL_UART_Transmit(&huart3, (uint8_t*) Data, 1, 0xFF);
      Data++;
      while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) == 0);
   }
	}
}

static char * itoa( int value, char *string, int radix )
{
        int     i, d;
        int     flag = 0;
        char    *ptr = string;
        /* This implementation only works for decimal numbers. */
        if (radix != 10)
        {
                *ptr = 0;
                return string;
        }
        if (!value)
        {
                *ptr++ = 0x30;
                *ptr = 0;
                return string;
        }
        /* if this is a negative value insert the minus sign. */
        if (value < 0)
        {
                *ptr++ = '-';
                /* Make the value positive. */
                value *= -1;
        }
        for (i = 10000; i > 0; i /= 10)
        {
                d = value / i;
                if (d || flag)
                {
                        *ptr++ = (char)(d + 0x30);
                        value -= (d * i);
                        flag = 1;
                }
        }
        /* Null terminate the string. */
        *ptr = 0;
        return string;
} /* NCL_Itoa */

