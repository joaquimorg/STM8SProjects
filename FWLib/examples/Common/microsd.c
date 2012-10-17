/**
  ******************************************************************************
  * @file microsd.c
  * @brief This file contains basic functions to control the MicroSD card device.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
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
#include "microsd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Write a byte in the MicroSD card the SPI peripheral.
  * @param[in] Data Data to be sent.
  * @retval None
  */
u8 MSD_WriteByte(u8 Data)
{
  /* Wait until the transmit buffer is empty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_SendData(Data);
  /* Wait to receive a byte*/
  while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  /*Return the byte read from the SPI bus */ 
  return SPI_ReceiveData();
}

/**
  * @brief Read a byte from the MicroSD card the SPI peripheral.
  * @par Parameters:
  * None
  * @retval u8 Data read
  */
u8 MSD_ReadByte(void)
{
  vu8 Data = 0;

  /* Wait until the transmit buffer is empty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_SendData(DUMMY);

  /* Wait until a data is received */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  /* Get the received data */
  Data = SPI_ReceiveData();

  /* Return the shifted data */
  return Data;
}

/**
  * @brief Initialize the  MicroSD card the SPI peripheral.
  * The MSD Response:
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed .
  * @par Parameters:
  * None
  * @retval u8 MSD_GoIdleState State of the MSD.
  */
u8 MSD_Init(void)
{
  u32 i;

  /* Set MSD ChipSelect pin in Output push-pull high level */
  GPIO_Init(MSD_CS_PORT, MSD_CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte 0xFF, 10 times with CS high*/
  /* rise CS and MOSI for 80 clocks cycles */
  for (i = 0; i <= 9; i++)
  {
    /* Send dummy byte 0xFF */
    MSD_WriteByte(DUMMY);
  }
  /*------------Put MSD in SPI mode--------------*/
  /* MSD initialized and set to SPI mode properly */
  return (MSD_GoIdleState());
}

/**
  * @brief Enable or Disable the MicroSD card.
  * @param[in] NewState CS pin state
  * @retval None
  */
void MSD_ChipSelect(FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    GPIO_WriteLow(MSD_CS_PORT, MSD_CS_PIN); /* CS pin low: MSD enabled */
  }
  else
  {
    GPIO_WriteHigh(MSD_CS_PORT, MSD_CS_PIN); /* CS pin high: MSD disabled */
  }
}

/**
  * @brief Return the State of the MSD.
  * The MSD Response:
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed.
  * @par Parameters:
  * None
  * @retval u8 State of MSD
  */
u8 MSD_GoIdleState(void)
{
  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD0 (GO_IDLE_STATE) to put MSD in SPI mode */
  MSD_SendCmd(MSD_GO_IDLE_STATE, 0, 0x95);

  /* Wait for In Idle State Response (R1 Format) equal to 0x01 */
  if (MSD_GetResponse(MSD_IN_IDLE_STATE))
  {
    /* No Idle State Response: return response failue */
    return MSD_RESPONSE_FAILURE;
  }
  /*----------Activates the card initialization process-----------*/
  do
  {
    /* MSD chip select high */
    MSD_ChipSelect(DISABLE);
    /* Send Dummy byte 0xFF */
    MSD_WriteByte(DUMMY);

    /* MSD chip select low */
    MSD_ChipSelect(ENABLE);

    /* Send CMD1 (Activates the card process) until response equal to 0x0 */
    MSD_SendCmd(MSD_SEND_OP_COND, 0, 0xFF);
    /* Wait for no error Response (R1 Format) equal to 0x00 */
  }
  while (MSD_GetResponse(MSD_RESPONSE_NO_ERROR));

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte 0xFF */
  MSD_WriteByte(DUMMY);

  return MSD_RESPONSE_NO_ERROR;
}

/**
  * @brief Send 5 bytes command to the MSD.
  * @param[in] Cmd The user expected command to send to MSD card.
  * @param[in] Arg The command argument.
  * @param[in] Crc The CRC.
  * @retval None
  */
void MSD_SendCmd(u8 Cmd, u32 Arg, u8 Crc)
{
  u32 i = 0x00;
  u8 Frame[6];

  /* Construct byte1 */
  Frame[0] = (u8)(Cmd | 0x40);
  /* Construct byte2 */
  Frame[1] = (u8)(Arg >> 24);
  /* Construct byte3 */
  Frame[2] = (u8)(Arg >> 16);
  /* Construct byte4 */
  Frame[3] = (u8)(Arg >> 8);
  /* Construct byte5 */
  Frame[4] = (u8)(Arg);
  /* Construct CRC: byte6 */
  Frame[5] = (Crc);

  /* Send the Cmd bytes */
  for (i = 0; i < 6; i++)
  {
    MSD_WriteByte(Frame[i]);
  }
}

/**
  * @brief Get response from the MSD.
  * The MSD Response:
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed 
  * @param[in] Response Response to get
  * @retval u8 Response value
  */
u8 MSD_GetResponse(u8 Response)
{
  u32 Count = 0xFFF;

  /* Check if response is got or a timeout is happen */
  while ((MSD_ReadByte() != Response) && Count)
  {
    Count--;
  }

  if (Count == 0)
  {
    /* After time out */
    return MSD_RESPONSE_FAILURE;
  }
  else
  {
    /* Right response got */
    return MSD_RESPONSE_NO_ERROR;
  }
}

/**
  * @brief Detect if MSD card is correctly plugged in the memory slot.
  * @par Parameters:
  * None
  * @retval u8 Return if MSD is detected or not
  */
u8 MSD_Detect(void)
{
  vu8 status = MICROSD_PRESENT;
  /* Check GPIO to detect microSD */
  if (GPIO_ReadInputData(GPIOE) & GPIO_PIN_4)
  {
    status = MICROSD_NOT_PRESENT;
  }
  return status;
}

/**
  * @brief Write a block in the MSD card.
	* If the amount of data to write is less than MSD card BLOCK size (512 Byte), DUMMY bytes
  *	will be send to fill the block size.
  * @param[in] pBuffer pointer to the buffer containing the data to be written on the MSD.
  * @param[in] WriteAddr address to write on.
  * @param[in] NumByteToWrite number of data to write
  * @retval u8 MSD response
  */
u8 MSD_WriteBlock(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u32 i = 0;
  vu8 rvalue = MSD_RESPONSE_FAILURE;

  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD24 (MSD_WRITE_BLOCK) to write multiple block */
  MSD_SendCmd(MSD_WRITE_BLOCK, WriteAddr, 0xFF);

  /* Check if the MSD acknowledged the write block command: R1 response (0x00: no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    /* Send a dummy byte */
    MSD_WriteByte(DUMMY);
    /* Send the data token to signify the start of the data */
    MSD_WriteByte(0xFE);
    /* Write the block data to MSD : write count data by block */
    for (i = 0; i < NumByteToWrite; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
		
		/* Send DUMMY bytes when the number of data to be written are lower
		   than the MSD card BLOCK size (512 Byte) */
		for (; i != BLOCK_SIZE; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(DUMMY);
	  }
	
    /* Put CRC bytes (not really needed by us, but required by MSD) */
    MSD_ReadByte();
    MSD_ReadByte();
    /* Read data response */
    if (MSD_GetDataResponse() == MSD_DATA_OK)
    {
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief Get the data MSD card reponse status.
  * @par Parameters:
  * None
  * @retval u8 The MSD response status
  * Read data response xxx0<status>1.
  * - status 010: Data accepted.
  * - status 101: Data rejected due to a crc error.
  * - status 110: Data rejected due to a Write error.
  * - status 111: Data rejected due to other error.
  */
u8 MSD_GetDataResponse(void)
{
  u32 i = 0;
  vu8 response, rvalue;

  while (i <= 64)
  {
    /* Read resonse */
    response = MSD_ReadByte();
    /* Mask unused bits */
    response &= 0x1F;

    switch (response)
    {
      case MSD_DATA_OK:
      {
        rvalue = MSD_DATA_OK;
        break;
      }

      case MSD_DATA_CRC_ERROR:
        return MSD_DATA_CRC_ERROR;

      case MSD_DATA_WRITE_ERROR:
        return MSD_DATA_WRITE_ERROR;

      default:
      {
        rvalue = MSD_DATA_OTHER_ERROR;
        break;
      }
    }
    /* Exit loop in case of data ok */
    if (rvalue == MSD_DATA_OK)
      break;
    /* Increment loop counter */
    i++;
  }
  /* Wait null data */
  while (MSD_ReadByte() == 0);
  /* Return response */
  return response;
}

/**
  * @brief Read a block from the MSD card.
  * @param[in] pBuffer pointer to the buffer that receives the data read from the MSD.
  * @param[in] ReadAddr MSD's internal address to read from.
  * @param[in] NumByteToRead number of bytes to read from the MSD.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed.
  */
u8 MSD_ReadBlock(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  u32 i = 0;
  vu8 rvalue = MSD_RESPONSE_FAILURE;

  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD17 (MSD_READ_SINGLE_BLOCK) to read one block */
  MSD_SendCmd(MSD_READ_SINGLE_BLOCK, ReadAddr, 0xFF);

  /* Check if the MSD acknowledged the read block command: R1 response (0x00: no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    /* Now look for the data token to signify the start of the data */
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Read the MSD block data : read NumByteToRead data */
      for (i = 0; i < NumByteToRead; i++)
      {
        /* Save the received data */
        *pBuffer = MSD_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /* Get CRC bytes (not really needed by us, but required by MSD) */
      MSD_ReadByte();
      MSD_ReadByte();
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief Write a buffer (many blocks) in the MSD card.
	* The amount of data to write should be a multiple of MSD card BLOCK size (512 Byte).  
  * @param[in] pBuffer pointer to the buffer containing the data to be written on the MSD.
  * @param[in] WriteAddr address to write on.
  * @param[in] NumByteToWrite number of data to write.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed
  */
u8 MSD_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  u32 i = 0, NbrOfBlock = 0, Offset = 0;
  vu8 rvalue = MSD_RESPONSE_FAILURE;

  /* Calculate number of blocks to write */
  NbrOfBlock = NumByteToWrite / BLOCK_SIZE;
  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);

  /* Data transfer */
  while (NbrOfBlock--)
  {
    /* Send CMD24 (MSD_WRITE_BLOCK) to write blocks */
    MSD_SendCmd(MSD_WRITE_BLOCK, WriteAddr + Offset, 0xFF);

    /* Check if the MSD acknowledged the write block command: R1 response (0x00: no errors) */
    if (MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
    {
      return MSD_RESPONSE_FAILURE;
    }
    /* Send dummy byte */
    MSD_WriteByte(DUMMY);
    /* Send the data token to signify the start of the data */
    MSD_WriteByte(MSD_START_DATA_SINGLE_BLOCK_WRITE);
    /* Write the block data to MSD : write count data by block */
    for (i = 0; i < BLOCK_SIZE; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /* Set next write address */
    Offset += 512;
    /* Put CRC bytes (not really needed by us, but required by MSD) */
    MSD_ReadByte();
    MSD_ReadByte();
    /* Read data response */
    if (MSD_GetDataResponse() == MSD_DATA_OK)
    {
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_RESPONSE_FAILURE;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief Read a buffer (many blocks) from the MSD card.
  * @param[in] pBuffer pointer to the buffer that receives the data read from the MSD.
  * @param[in] ReadAddr MSD's internal address to read from.
  * @param[in] NumByteToRead number of bytes to read from the MSD.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed
  */
u8 MSD_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
  u32 i = 0, NbrOfBlock = 0, Offset = 0;
  vu8 rvalue = MSD_RESPONSE_FAILURE;

  /* Calculate number of blocks to read */
  NbrOfBlock = NumByteToRead / BLOCK_SIZE;
  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);

  /* Data transfer */
  while (NbrOfBlock --)
  {
    /* Send CMD17 (MSD_READ_SINGLE_BLOCK) to read one block */
    MSD_SendCmd (MSD_READ_SINGLE_BLOCK, ReadAddr + Offset, 0xFF);
    /* Check if the MSD acknowledged the read block command: R1 response (0x00: no errors) */
    if (MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
    {
      return  MSD_RESPONSE_FAILURE;
    }
    /* Now look for the data token to signify the start of the data */
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Read the MSD block data : read NumByteToRead data */
      for (i = 0; i < BLOCK_SIZE; i++)
      {
        /* Read the pointed data */
        *pBuffer = MSD_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /* Set next read address*/
      Offset += 512;
      /* get CRC bytes (not really needed by us, but required by MSD) */
      MSD_ReadByte();
      MSD_ReadByte();
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_RESPONSE_FAILURE;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
