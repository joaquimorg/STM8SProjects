/**
  ******************************************************************************
  * @file i2c_ee.h
  * @brief This file contains all the prototypes for i2c_ee.c module.
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EE_H
#define __I2C_EE_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_EEInit(void);
void I2C_EE_ByteWrite(u8* pBuffer, u16 WriteAddr);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u8 NumByteToRead);

#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


