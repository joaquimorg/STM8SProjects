/**
  ******************************************************************************
  * @file i2c_ee.c
  * @brief This file provides a set of functions needed to manage the 
	* communication between I2C peripheral and I2C M24C64 EEPROM.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.0
  * @date 02/27/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "i2c_ee.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0xA0
#define EEPROM_BASE_ADDRESS    0x0000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 EEPROM_ADDRESS = 0xA0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Initializes the I2C peripheral registers to communicate the EEPROM
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * None
  */

void I2C_EEInit(void)
{
   u8 Input_Clock = 0x0;
	 
	/* Get system clock frequency */
	Input_Clock = CLK_GetClockFreq()/1000000;
		
  /* I2C Peripheral Enable */
  I2C_Cmd(ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C_Speed, I2C1_SLAVE_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
}

/**
  * @brief Writes one byte to the I2C EEPROM.
  * @param[in] pBuffer Pointer to the buffer containing the data to be 
	* written to the EEPROM.
  * @param[in] WriteAddr EEPROM's internal address to write to.
  * @retval None
  * @par Required preconditions:
  * None
  */
void I2C_EE_ByteWrite(u8* pBuffer, u16 WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
      
  /* Send Address (on 2 bytes) of first byte to be written & wait event detection */
  I2C_SendData((u8)(WriteAddr >> 8)); /* MSB */
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
  I2C_SendData((u8)(WriteAddr)); /* LSB */
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
 
  /* Send the byte to be written */
  I2C_SendData(*pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(ENABLE);
}

/**
  * @brief Writes more than one byte to the EEPROM with a single WRITE cycle. 
  *  The number of byte can't exceed the EEPROM page size.
  * @param[in] pBuffer Pointer to the buffer containing the data to be 
	* written to the EEPROM.
  * @param[in] WriteAddr EEPROM's internal address to write to.
  * @param[in] NumByteToWrite EEPROM's number of bytes to write to the EEPROM.  
  * @retval None
  * @par Required preconditions:
  * None
  */
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite)
{
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
  I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);  

  /* Send Address (on 2 bytes) of first byte to be written & wait event detection */
  I2C_SendData((u8)(WriteAddr >> 8)); /* MSB */
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
  I2C_SendData((u8)(WriteAddr)); /* LSB */
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  


  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(*pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(ENABLE);
}

/**
  * @brief Reads a block of data from the EEPROM. 
  * @param[in] pBuffer pointer to the buffer that receives the data read
  * from the EEPROM.
  * @param[in] WriteAddr EEPROM's internal address to read from.
  * @param[in] NumByteToWrite EEPROM's number of bytes to read from the EEPROM.  
  * @retval None
  * @par Required preconditions:
  * None
  */
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u8 NumByteToRead)
{  
    /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
  /* Generate start & wait event detection */
    I2C_GenerateSTART(ENABLE);
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
  
  /* Send slave Address in write direction & wait detection event */
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_TX);
   /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
    I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
    
   /* Send Address of first byte to be read & wait event detection */
    I2C_SendData((u8)(ReadAddr >> 8)); /* MSB */
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData((u8)(ReadAddr)); /* LSB */
  /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(ENABLE);
    /* Test on EV5 and clear it */
   while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
  
  /* Send slave Address in read direction & wait event */
    I2C_Send7bitAddress(EEPROM_ADDRESS, I2C_DIRECTION_RX);
   /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
    I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData();

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/