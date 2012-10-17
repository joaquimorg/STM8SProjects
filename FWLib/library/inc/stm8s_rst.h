/**
  ******************************************************************************
  * @file stm8s_rst.h
  * @brief This file contains all functions prototype and macros for the RST peripheral.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_RST_H
#define __STM8S_RST_H

/* Includes ------------------------------------------------------------------*/
/* Contains the description of all STM8 hardware registers */
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/** @addtogroup RST_Exported_Types
  * @{
  */
typedef enum {
  RST_FLAG_EMCF    = (u8)0x10, /*!< EMC reset flag */
  RST_FLAG_SWIMF   = (u8)0x08, /*!< SWIM reset flag */
  RST_FLAG_ILLOPF  = (u8)0x04, /*!< Illigal opcode reset flag */
  RST_FLAG_IWDGF   = (u8)0x02, /*!< Independent watchdog reset flag */
  RST_FLAG_WWDGF   = (u8)0x01  /*!< Window watchdog reset flag */
}RST_Flag_TypeDef;

/**
  * @}
  */



/** @addtogroup RST_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different functions parameters.
  */
/**
  * @brief Macro used by the assert function to check the different RST flags.
  */
#define IS_RST_FLAG_OK(FLAG) (((FLAG) == RST_FLAG_EMCF) || \
                              ((FLAG) == RST_FLAG_SWIMF)  ||\
                              ((FLAG) == RST_FLAG_ILLOPF) ||\
                              ((FLAG) == RST_FLAG_IWDGF)  ||\
                              ((FLAG) == RST_FLAG_WWDGF))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup RST_Exported_functions
  * @{
  */
FlagStatus RST_GetFlagStatus(RST_Flag_TypeDef RST_Flag);
void RST_ClearFlag(RST_Flag_TypeDef RST_Flag);

/**
  * @}
  */
#endif /* __STM8S_RST_H */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
