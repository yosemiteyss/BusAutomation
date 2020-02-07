#include "rc522.h"
#include "delay.h"

void RC522_Init(void)
{
  /* SPI_InitTypeDef  SPI_InitStructure */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	RC522_SPI1_CLK_ENABLE();
  RC522_GPIO_CLK_ENABLE();
  
	/*!< Configure SPI_RC522_SPI pins: CS */
  GPIO_InitStruct.Pin = RC522_GPIO_CS_PIN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Mode = RC522_GPIO_CS_Mode;
  HAL_GPIO_Init(RC522_GPIO_CS_PORT, &GPIO_InitStruct);
	
  /*!< Configure SPI_RC522_SPI pins: SCK */
  GPIO_InitStruct.Pin = RC522_GPIO_SCK_PIN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Mode = RC522_GPIO_SCK_Mode;
  HAL_GPIO_Init(RC522_GPIO_SCK_PORT, &GPIO_InitStruct);
	
  /*!< Configure SPI_RC522_SPI pins: MOSI */
  GPIO_InitStruct.Pin = RC522_GPIO_MOSI_PIN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Mode = RC522_GPIO_MOSI_Mode;
  HAL_GPIO_Init(RC522_GPIO_MOSI_PORT, &GPIO_InitStruct);

  /*!< Configure SPI_RC522_SPI pins: MISO */
  GPIO_InitStruct.Pin = RC522_GPIO_MISO_PIN;
  GPIO_InitStruct.Mode = RC522_GPIO_MISO_Mode;
  HAL_GPIO_Init(RC522_GPIO_MISO_PORT, &GPIO_InitStruct);	
		
  /*!< Configure SPI_RC522_SPI pins: RST */
  GPIO_InitStruct.Pin = RC522_GPIO_RST_PIN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Mode = RC522_GPIO_RST_Mode;
  HAL_GPIO_Init(RC522_GPIO_RST_PORT, &GPIO_InitStruct);
	
	RC522_Reset_Disable();	
	RC522_CS_Disable();
}

void RC522_SPI_SendByte ( uint8_t byte )
{
  uint8_t counter;

  for (counter=0; counter < 8; counter++)
  {     
    if ( byte & 0x80 )
      RC522_MOSI_1 ();
    else 
      RC522_MOSI_0 ();
    
    delay_us(200);
    RC522_SCK_0 ();
    
    delay_us(200);
    RC522_SCK_1();
    
    delay_us(200);
    byte <<= 1; 
  } 	
}

uint8_t RC522_SPI_ReadByte ( void )
{
  uint8_t counter;
  uint8_t SPI_Data;

  for (counter = 0;counter < 8; counter++)
  {
    SPI_Data <<= 1;
    RC522_SCK_0 ();
   
    delay_us(200);
    if ( RC522_MISO_GET() == 1)
     SPI_Data |= 0x01;
    
    delay_us(200);
    RC522_SCK_1 ();
    
    delay_us(200);
  }
	
  return SPI_Data;
}

uint8_t RC522_ReadRawRC ( uint8_t ucAddress )
{
	uint8_t ucAddr, ucReturn;
	
	ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;	
	RC522_CS_Enable();
  
	RC522_SPI_SendByte ( ucAddr );
	ucReturn = RC522_SPI_ReadByte ();
  
	RC522_CS_Disable();
	
	return ucReturn;	
}

void RC522_WriteRawRC ( uint8_t ucAddress, uint8_t ucValue )
{  
	uint8_t ucAddr;
	
	ucAddr = ( ucAddress << 1 ) & 0x7E;	
	RC522_CS_Enable();
	
	RC522_SPI_SendByte ( ucAddr );	
	RC522_SPI_SendByte ( ucValue );
  
	RC522_CS_Disable();		
}

void RC522_SetBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = RC522_ReadRawRC ( ucReg );
  RC522_WriteRawRC ( ucReg, ucTemp | ucMask ); 
}

void RC522_ClearBitMask ( uint8_t ucReg, uint8_t ucMask )  
{
  uint8_t ucTemp;

  ucTemp = RC522_ReadRawRC ( ucReg );
  RC522_WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) ); 
}

void RC522_PcdAntennaOn ( void )
{
  uint8_t uc;

  uc = RC522_ReadRawRC ( TxControlReg );
  if ( ! ( uc & 0x03 ) )
   RC522_SetBitMask(TxControlReg, 0x03);		
}

void RC522_PcdAntennaOff ( void )
{
  RC522_ClearBitMask ( TxControlReg, 0x03 );	
}

void RC522_PcdReset ( void )
{
	RC522_Reset_Disable();
	delay_us ( 1 );
	
	RC522_Reset_Enable();
	delay_us ( 1 );
	
	RC522_Reset_Disable();
	delay_us ( 1 );
	
	RC522_WriteRawRC ( CommandReg, 0x0f );
	
	while ( RC522_ReadRawRC ( CommandReg ) & 0x10 );
	
	delay_us ( 1 );
	
  RC522_WriteRawRC ( ModeReg, 0x3D );        
  RC522_WriteRawRC ( TReloadRegL, 30 );      
	RC522_WriteRawRC ( TReloadRegH, 0 );			 
  RC522_WriteRawRC ( TModeReg, 0x8D );			 
  RC522_WriteRawRC ( TPrescalerReg, 0x3E );	 
	RC522_WriteRawRC ( TxAutoReg, 0x40 );			
}

void RC522_M500PcdConfigISOType ( uint8_t ucType )
{
	if ( ucType == 'A')                     			//ISO14443_A
  {
		RC522_ClearBitMask ( Status2Reg, 0x08 );
		
    RC522_WriteRawRC ( ModeReg, 0x3D );         //3F
		
		RC522_WriteRawRC ( RxSelReg, 0x86 );        //84
		
		RC522_WriteRawRC( RFCfgReg, 0x7F );         //4F
		
		RC522_WriteRawRC( TReloadRegL, 30 );        
		
		RC522_WriteRawRC ( TReloadRegH, 0 );
		
		RC522_WriteRawRC ( TModeReg, 0x8D );
		
		RC522_WriteRawRC ( TPrescalerReg, 0x3E );
		
		delay_us ( 2 );
		
		RC522_PcdAntennaOn ();
  }	 
}

char RC522_PcdComMF522 ( uint8_t ucCommand,
                   uint8_t * pInData, 
                   uint8_t ucInLenByte, 
                   uint8_t * pOutData,
                   uint32_t * pOutLenBit )		
{
  char cStatus = MI_ERR;
  uint8_t ucIrqEn   = 0x00;
  uint8_t ucWaitFor = 0x00;
  uint8_t ucLastBits;
  uint8_t ucN;
  uint32_t ul;

  switch ( ucCommand )
  {
     case PCD_AUTHENT:		  
        ucIrqEn   = 0x12;		
        ucWaitFor = 0x10;		
        break;
     
     case PCD_TRANSCEIVE:		
        ucIrqEn   = 0x77;		
        ucWaitFor = 0x30;		
        break;
     
     default:
       break;     
  }

  RC522_WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 );

  RC522_ClearBitMask ( ComIrqReg, 0x80 );	 

  RC522_WriteRawRC ( CommandReg, PCD_IDLE );		 

  RC522_SetBitMask ( FIFOLevelReg, 0x80 );			

  for ( ul = 0; ul < ucInLenByte; ul ++ )
    RC522_WriteRawRC ( FIFODataReg, pInData [ ul ] ); 
    
  RC522_WriteRawRC ( CommandReg, ucCommand );					

  if ( ucCommand == PCD_TRANSCEIVE )
    RC522_SetBitMask(BitFramingReg,0x80);  				  

  ul = 1000;                             

  do 														         	
  {
       ucN = RC522_ReadRawRC ( ComIrqReg );		 
       ul --;
  } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );	

  RC522_ClearBitMask ( BitFramingReg, 0x80 );	 

  if ( ul != 0 )
  {   
    if ( ! ( RC522_ReadRawRC ( ErrorReg ) & 0x1B ) )	
    {
      cStatus = MI_OK;
      
      if ( ucN & ucIrqEn & 0x01 )				
        cStatus = MI_NOTAGERR;   
        
      if ( ucCommand == PCD_TRANSCEIVE )
      {
        ucN = RC522_ReadRawRC ( FIFOLevelReg );		          
        
        ucLastBits = RC522_ReadRawRC ( ControlReg ) & 0x07;	
        
        if ( ucLastBits )        
          * pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	
        else
          * pOutLenBit = ucN * 8;      
        
        if ( ucN == 0 )		
          ucN = 1;    
        
        if ( ucN > MAXRLEN )
          ucN = MAXRLEN;   
        
        for ( ul = 0; ul < ucN; ul ++ )
          pOutData [ ul ] = RC522_ReadRawRC ( FIFODataReg );   
        
        }        
    }   
    else
      cStatus = MI_ERR;       
  }

  RC522_SetBitMask ( ControlReg, 0x80 );           // stop timer now
  RC522_WriteRawRC ( CommandReg, PCD_IDLE ); 
   
  return cStatus;
}

char RC522_PcdRequest ( uint8_t ucReq_code, uint8_t * pTagType )
{
  char cStatus;  
  uint8_t ucComMF522Buf [ MAXRLEN ]; 
  uint32_t ulLen;

  RC522_ClearBitMask ( Status2Reg, 0x08 );

  RC522_WriteRawRC ( BitFramingReg, 0x07 );

  RC522_SetBitMask ( TxControlReg, 0x03 );	

  ucComMF522Buf [ 0 ] = ucReq_code;		

  cStatus = RC522_PcdComMF522 ( PCD_TRANSCEIVE,	
                          ucComMF522Buf,
                          1, 
                          ucComMF522Buf,
                          & ulLen );	

  if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	
  {    
     * pTagType = ucComMF522Buf [ 0 ];
     * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
  }

  else
   cStatus = MI_ERR;

  return cStatus;	 
}

char RC522_PcdAnticoll ( uint8_t * pSnr )
{
  char cStatus;
  uint8_t uc, ucSnr_check = 0;
  uint8_t ucComMF522Buf [ MAXRLEN ]; 
  uint32_t ulLen;
  
  RC522_ClearBitMask ( Status2Reg, 0x08 );

  RC522_WriteRawRC ( BitFramingReg, 0x00);	

  RC522_ClearBitMask ( CollReg, 0x80 );			  
 
  ucComMF522Buf [ 0 ] = 0x93;	          
  ucComMF522Buf [ 1 ] = 0x20;
 
  cStatus = RC522_PcdComMF522 ( PCD_TRANSCEIVE, 
                          ucComMF522Buf,
                          2, 
                          ucComMF522Buf,
                          & ulLen);     

  if ( cStatus == MI_OK )		           
  {
    for ( uc = 0; uc < 4; uc ++ )
    {
       * ( pSnr + uc )  = ucComMF522Buf [ uc ]; 
       ucSnr_check ^= ucComMF522Buf [ uc ];
    }
    
    if ( ucSnr_check != ucComMF522Buf [ uc ] )
      cStatus = MI_ERR;    				 
  }
  
  RC522_SetBitMask ( CollReg, 0x80 );
      
  return cStatus;		
}

void RC522_CalulateCRC ( uint8_t * pIndata, 
                 uint8_t ucLen, 
                 uint8_t * pOutData )
{
  uint8_t uc, ucN;

  RC522_ClearBitMask(DivIrqReg,0x04);

  RC522_WriteRawRC(CommandReg,PCD_IDLE);

  RC522_SetBitMask(FIFOLevelReg,0x80);

  for ( uc = 0; uc < ucLen; uc ++)
    RC522_WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );   

  RC522_WriteRawRC ( CommandReg, PCD_CALCCRC );

  uc = 0xFF;

  do 
  {
      ucN = RC522_ReadRawRC ( DivIrqReg );
      uc --;
  } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
  
  pOutData [ 0 ] = RC522_ReadRawRC ( CRCResultRegL );
  pOutData [ 1 ] = RC522_ReadRawRC ( CRCResultRegM );		
}

char RC522_PcdSelect ( uint8_t * pSnr )
{
  char ucN;
  uint8_t uc;
  uint8_t ucComMF522Buf [ MAXRLEN ]; 
  uint32_t  ulLen;
  
  ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
  ucComMF522Buf [ 1 ] = 0x70;
  ucComMF522Buf [ 6 ] = 0;

  for ( uc = 0; uc < 4; uc ++ )
  {
    ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
  }
  
  RC522_CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );

  RC522_ClearBitMask ( Status2Reg, 0x08 );

  ucN = RC522_PcdComMF522 ( PCD_TRANSCEIVE,
                     ucComMF522Buf,
                     9,
                     ucComMF522Buf, 
                     & ulLen );
  
  if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
    ucN = MI_OK;  
  else
    ucN = MI_ERR;    
  
  return ucN;		
}

char RC522_PcdAuthState ( uint8_t ucAuth_mode, 
                    uint8_t ucAddr, 
                    uint8_t * pKey,
                    uint8_t * pSnr )
{
  char cStatus;
  uint8_t uc, ucComMF522Buf [ MAXRLEN ];
  uint32_t ulLen;

  ucComMF522Buf [ 0 ] = ucAuth_mode;
  ucComMF522Buf [ 1 ] = ucAddr;

  for ( uc = 0; uc < 6; uc ++ )
    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );   

  for ( uc = 0; uc < 6; uc ++ )
    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );   

  cStatus = RC522_PcdComMF522 ( PCD_AUTHENT,
                          ucComMF522Buf, 
                          12,
                          ucComMF522Buf,
                          & ulLen );

  if ( ( cStatus != MI_OK ) || ( ! ( RC522_ReadRawRC ( Status2Reg ) & 0x08 ) ) )
    cStatus = MI_ERR;   
    
  return cStatus;
}

char RC522_PcdWrite ( uint8_t ucAddr, uint8_t * pData )
{
  char cStatus;
  uint8_t uc, ucComMF522Buf [ MAXRLEN ];
  uint32_t ulLen;
  
  ucComMF522Buf [ 0 ] = PICC_WRITE;
  ucComMF522Buf [ 1 ] = ucAddr;

  RC522_CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );

  cStatus = RC522_PcdComMF522 ( PCD_TRANSCEIVE,
                          ucComMF522Buf,
                          4, 
                          ucComMF522Buf,
                          & ulLen );

  if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || 
         ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
    cStatus = MI_ERR;   
      
  if ( cStatus == MI_OK )
  {
    //memcpy(ucComMF522Buf, pData, 16);
    for ( uc = 0; uc < 16; uc ++ )
      ucComMF522Buf [ uc ] = * ( pData + uc );  
    
    RC522_CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );

    cStatus = RC522_PcdComMF522 ( PCD_TRANSCEIVE,
                           ucComMF522Buf, 
                           18, 
                           ucComMF522Buf,
                           & ulLen );
    
    if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || 
         ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
      cStatus = MI_ERR;   
    
  } 	
  return cStatus;		
}

char RC522_PcdRead ( uint8_t ucAddr, uint8_t * pData )
{
  char cStatus;
  uint8_t uc, ucComMF522Buf [ MAXRLEN ]; 
  uint32_t ulLen;
  
  ucComMF522Buf [ 0 ] = PICC_READ;
  ucComMF522Buf [ 1 ] = ucAddr;

  RC522_CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 
  cStatus = RC522_PcdComMF522 ( PCD_TRANSCEIVE,
                          ucComMF522Buf,
                          4, 
                          ucComMF522Buf,
                          & ulLen );

  if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
  {
    for ( uc = 0; uc < 16; uc ++ )
      * ( pData + uc ) = ucComMF522Buf [ uc ];   
  }
  
  else
    cStatus = MI_ERR;   
   
  return cStatus;		
}

char RC522_PcdHalt( void )
{
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
	uint32_t  ulLen;
  
  ucComMF522Buf [ 0 ] = PICC_HALT;
  ucComMF522Buf [ 1 ] = 0;
	
  RC522_CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 	RC522_PcdComMF522 ( PCD_TRANSCEIVE,
                ucComMF522Buf,
                4, 
                ucComMF522Buf, 
                & ulLen );

  return MI_OK;	
}

void RC522_IC_CMT ( uint8_t * UID,
              uint8_t * KEY,
              uint8_t RW,
              uint8_t * Dat )
{
  uint8_t ucArray_ID [ 4 ] = { 0 }; 
  
	
  RC522_PcdRequest ( 0x52, ucArray_ID ); 

  RC522_PcdAnticoll ( ucArray_ID );      
  
  RC522_PcdSelect ( UID );               
  
  RC522_PcdAuthState ( 0x60, 0x10, KEY, UID );
	

	if ( RW )                        
    RC522_PcdRead ( 0x10, Dat );
   
   else 
     RC522_PcdWrite ( 0x10, Dat );
   	 
   RC522_PcdHalt ();	 
}
